from pathlib import Path
import pytest
import sys
import subprocess
import re

IMAGES_DIR = Path('images')
PARAMETERS_DIR = Path('parameters')
EXECUTABLE_PATH = Path('../cmake-build-debug/mlpnetwork')

if sys.platform == "win32" or sys.platform == "msys":
    EXECUTABLE_PATH = EXECUTABLE_PATH.with_suffix(".exe")
if sys.platform == "cygwin":
    print("Cygwin toolchain isn't supported, try using WSL or MSYS2 MingGW, or better - use an Aquarium PC", file=sys.stderr)
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


def test_configured_correctly():
    assert EXECUTABLE_PATH.exists(), f"Executable should exist at {EXECUTABLE_PATH}"
    for param in PARAMS:
        assert param.exists(), f"Param file {param} should exist"
    for im in image_to_result.keys():
        assert (IMAGES_DIR/im).exists(), f"Image file {IMAGES_DIR/im} should exist"


def run_on_image(image: str):
    image_file = IMAGES_DIR / image
    args = [EXECUTABLE_PATH, *PARAMS]
    input = f"{image_file.absolute()}\n"
    proc = subprocess.run(args, input=input, universal_newlines=True,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                           )
    return proc.stdout, proc.stderr


@pytest.mark.parametrize("image", image_to_result.keys())
def test_on_image(image: str):
    expected_digit, expected_prob = image_to_result[image]
    out, err = run_on_image(image)
    print(f"Output of running with image '{image}':\n{out}")

    assert "Image processed:" in out, "There was an error in your program - no digit was output"
    match = RESULT_REGEX.search(out)
    gotten_digit, gotten_prob = match.group(1), match.group(2)

    assert expected_digit == int(gotten_digit), "Your program identified the wrong digit"
    assert expected_prob == float(gotten_prob), "Your program got the right digit, but the probability is wrong"
