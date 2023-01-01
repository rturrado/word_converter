# Packages
include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG "61720589cbef9707b3c5247d02141a7c31e2876f"
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(
    googletest
)


# Test sources
set(test_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/command_line_parser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/converter.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/input_reader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/output_writer.cpp"
)
set(app_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)
list(APPEND app_sources ${test_sources})


# Test executable
add_executable(${PROJECT_NAME}_test ${app_sources})
target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME}_test PRIVATE
    lib_${PROJECT_NAME}
    gmock
    gtest
)

# Target compile options
if(MSVC)
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        /W3 /WX /w34996
        /D_CONSOLE /DCONSOLE
        /D_UNICODE /DUNICODE
        /diagnostics:column /EHsc /FC /fp:precise /Gd /GS /MP /sdl /utf-8 /Zc:inline
    )
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME}_test PRIVATE
        -pedantic-errors -Werror -Wall -Wextra
        -Wl,-z,defs
        -Wno-deprecated
        # -Wconversion -Wsign-conversion -Wno-sign-conversion  # do not sort flags; order matters
    )
endif()
if(CODE_COVERAGE)
    if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
        target_compile_options(${PROJECT_NAME}_test PRIVATE -fprofile-arcs -ftest-coverage)
        target_link_options(${PROJECT_NAME}_test PRIVATE -fprofile-arcs -ftest-coverage)
    endif()
endif()


# Resource dir
# Copy <source dir>/res to <binary dir>/res
add_custom_target(resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res
    DEPENDS ${PROJECT_NAME}_test
    COMMENT "copying resources folder from ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res"
)


include(GoogleTest)
gtest_add_tests(TARGET ${PROJECT_NAME}_test
    SOURCES ${test_sources}
    EXTRA_ARGS ${PROJECT_SOURCE_DIR}/res
)
