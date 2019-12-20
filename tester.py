from pathlib import Path
import pytest
import sys
import subprocess
import re

IMAGES_DIR = Path('images')
PARAMETERS_DIR = Path('parameters')
EXECUTABLE_PATH = Path('../cmake-build-debug/tests/mlpnetwork-executable')
SCHOOL_SOLUTION = Path('school_solution')
SCHOOL_OUT = Path("school-out")

ERROR_REGEX = re.compile(r"^Error: .+\n")

if sys.platform == "win32" or sys.platform == "msys":
    EXECUTABLE_PATH = EXECUTABLE_PATH.with_suffix(".exe")
if sys.platform == "cygwin":
    print("Cygwin toolchain isn't supported, try using MSYS2 MingGW, or better - use an Aquarium PC", file=sys.stderr)
    exit(1)

PARAMS = [PARAMETERS_DIR / p for p in ["w1", "w2", "w3", "w4", "b1", "b2", "b3", "b4"]]
RESULT_REGEX = re.compile(r"Mlp result: (.+) at probability: (.+)")

image_to_result = {
    'im0': (5, 0.99821),
    'im1': (0, 0.999917),
    'im2': (4, 0.996058),
    'im3': (1, 0.99999),
    'im4': (9, 0.999897),
    'im5': (2, 0.997467),
    'im6': (1, 0.99997),
    'im7': (3, 1),
    'im8': (1, 0.99957),
    'im9': (4, 0.999974)
}

ERROR_FILES = ["random_bytes", "too_long", "too_short"]

def test_configured_correctly():
    assert EXECUTABLE_PATH.exists(), f"Executable should exist at {EXECUTABLE_PATH}"
    for param in PARAMS:
        assert param.exists(), f"Param file {param} should exist"
    for im in image_to_result.keys():
        assert (IMAGES_DIR/im).exists(), f"Image file {IMAGES_DIR/im} should exist"


def run_on_image(image: str, executable=None):
    image_file = IMAGES_DIR / image
    args = [str(arg.resolve()) for arg in [executable or EXECUTABLE_PATH] + PARAMS]
    input = f"{image_file}\nq\n"
    proc = subprocess.run(args, input=input, universal_newlines=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                           )
    return proc.stdout, proc.stderr

def gen_school_output():
    for image in list(image_to_result.keys()) + ERROR_FILES:
        out, err = run_on_image(image, executable=SCHOOL_SOLUTION)
        with open(SCHOOL_OUT / image, 'w', encoding='utf-8') as sout:
            sout.write(out)


@pytest.mark.parametrize("image", image_to_result.keys())
def test_on_image(image: str):
    expected_digit, expected_prob = image_to_result[image]
    out, err = run_on_image(image)

    print(f"STDOUT: {out}\nSTDERR: {err}\n")
    assert "Image processed:" in out, "There was an error in your program - no digit was output"
    match = RESULT_REGEX.search(out)
    gotten_digit, gotten_prob = match.group(1), match.group(2)

    assert expected_digit == int(gotten_digit), "Your program identified the wrong digit"
    assert expected_prob == float(gotten_prob), "Your program got the right digit, but the probability is wrong"

    with open(SCHOOL_OUT / image, 'r', encoding='utf-8') as exp_out_file:
        expected_out = exp_out_file.read()
        assert expected_out == out, "Your program got the right digit and probability, but output format is wrong"


@pytest.mark.parametrize("err_img", ERROR_FILES)
def test_on_error(err_img: str):
    out, err = run_on_image(err_img)

    print(f"STDOUT: {out}\nSTDERR: {err}\n")
    match = ERROR_REGEX.findall(err)
    assert match is not None, f"Could not find error string in STDERR for invalid image file {err_img}"
    shouldnt_match = RESULT_REGEX.search(out)
    assert shouldnt_match is None, f"Program should've terminated after printing error for image file {err_img}"


if __name__ == '__main__':
    if "genschool" in sys.argv:
        gen_school_output()
        print("Generated output via school binary")
        exit(0)
    pytest.main(sys.argv + ["-vvs"])
