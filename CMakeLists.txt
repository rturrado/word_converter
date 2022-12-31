cmake_minimum_required(VERSION 3.22)


# CMake policies
if (POLICY CMP0102)
    cmake_policy(SET CMP0102 NEW)
endif()
if (POLICY CMP0126)
    cmake_policy(SET CMP0126 NEW)
endif()
cmake_policy(SET CMP0128 NEW)
if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()


project("word_converter"
    VERSION 1.0
    DESCRIPTION "Converts all numbers English written into digits of the provided text. "
    HOMEPAGE_URL https://github.com/rturrado/word_converter.git
    LANGUAGES C CXX
)


# Address sanitizer
if(ASAN_ENABLED)
    string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")
    if(MSVC)
        add_compile_options(-fsanitize=address)
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()


# Subdirectories
# src and test
add_subdirectory(src)

if(WORD_CONVERTER_BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(test)
endif()
