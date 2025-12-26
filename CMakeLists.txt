cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23) 
project(Sequence_Runner LANGUAGES CXX)

find_package(Slint QUIET)
if (NOT Slint_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    Slint
    GIT_REPOSITORY https://github.com/slint-ui/slint.git
    GIT_TAG release/1
    SOURCE_SUBDIR api/cpp
  )
  FetchContent_MakeAvailable(Slint)
endif (NOT Slint_FOUND)

add_executable(Sequence_Runner
    src/main.cpp
    src/external/tinyfiledialogs.cpp
    src/ui/filedialog.cpp
    src/ui/uiutils.hpp
    src/external/json.hpp
    src/files/datahandle.cpp
    src/files/data.hpp 
    src/other/cfg.hpp
    src/other/exceptionhandler.hpp
    src/runner/sequence_runner.hpp
    ui/app.rc
)

set_target_properties(Sequence_Runner PROPERTIES
    OUTPUT_NAME "Sequence_Runner"
)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
set_target_properties(Sequence_Runner PROPERTIES WIN32_EXECUTABLE TRUE )
set_target_properties(Sequence_Runner PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif()

target_link_libraries(Sequence_Runner PRIVATE Slint::Slint)
slint_target_sources(Sequence_Runner ui/app_window.slint)

target_include_directories(Sequence_Runner PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/files
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/src/external
    ${CMAKE_CURRENT_SOURCE_DIR}/src/other
    ${CMAKE_CURRENT_SOURCE_DIR}/src/runner
)

add_custom_command(TARGET Sequence_Runner POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy 
    $<TARGET_RUNTIME_DLLS:Sequence_Runner> 
    $<TARGET_FILE_DIR:Sequence_Runner> 
    COMMAND_EXPAND_LISTS
)