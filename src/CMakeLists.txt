set(include_dir ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME})


# Packages
include(FetchContent)
FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG "a33701196adfad74917046096bf5a2aa0ab0bb50"
)
FetchContent_Declare(rtc
    GIT_REPOSITORY https://github.com/rturrado/rtc.git
    GIT_TAG "35eb8fdf5c3382c2ddeb887b962db0b080cec1a8"
)
FetchContent_MakeAvailable(
    fmt
    rtc
)


# Sources
set(app_sources
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)


# Main executable
add_executable(${PROJECT_NAME} ${app_sources})
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${include_dir}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PUBLIC
    fmt
    rtc
)


# Target compile options
if(MSVC)
    set(msvc_compile_options
        /W3 /WX /w34996
        /D_CONSOLE /DCONSOLE
        /D_UNICODE /DUNICODE
        /diagnostics:column /EHsc /FC /fp:precise /Gd /GS /MP /sdl /utf-8 /Zc:inline
    )
    target_compile_options(${PROJECT_NAME} PRIVATE ${msvc_compile_options})
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    set(unixlike_compile_options
        -pedantic-errors -Werror -Wall -Wextra
        -Wl,-z,defs
        -Wno-deprecated
        # -Wconversion -Wsign-conversion -Wno-sign-conversion  # do not sort flags; order matters
    )
    target_compile_options(${PROJECT_NAME} PRIVATE ${unixlike_compile_options})
endif()
