cmake_minimum_required(VERSION 3.10)
project(ex4_tests CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR})
add_executable(tests test_main.cpp matrix_tests.cpp catch.hpp activation_tests.cpp util.h)

if(MINGW)
    find_program(HAS_LLD lld)
    if (HAS_LLD)
        # Use 'lld' linker, "mingw-w64-x86_64-lld", as otherwise linking Catch2 is very slow.
        # see https://github.com/catchorg/Catch2/issues/1205
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    else()
        message(WARNING "You're compiling for Windows via MinGW, but don't have the LLD linker, linking the tests is going to"
                "be very slow. If you're using MSYS2+MinGW, try doing 'pacman -S mingw-w64-x86_64-lld' at the 'MSYS2 MinGW 64-bit' shell")
    endif()
endif()

target_link_libraries(tests PRIVATE mlpnetwork)