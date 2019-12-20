cmake_minimum_required(VERSION 3.10)
project(ex4_tests CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})


add_library(mlpnetwork-lib ../Matrix.cpp ../Matrix.h ../Activation.cpp ../Activation.h
                           ../Dense.cpp ../Dense.h ../MlpNetwork.h ../Digit.h ../MlpNetwork.cpp)
target_compile_options(mlpnetwork-lib PRIVATE -Wall -Wextra -Wvla -g -Werror)

add_executable(mlpnetwork-executable ../main.cpp)
target_link_libraries(mlpnetwork-executable PRIVATE mlpnetwork-lib)
add_executable(tests test_main.cpp matrix_tests.cpp catch.hpp activation_tests.cpp util.h)

if(MINGW)
    find_program(HAS_LLD lld)
    if (HAS_LLD)
        # see https://github.com/catchorg/Catch2/issues/1205
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    endif()
endif()

target_link_libraries(tests PRIVATE mlpnetwork-lib)
