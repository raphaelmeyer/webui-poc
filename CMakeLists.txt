cmake_minimum_required(VERSION 3.14)

project(WebUi)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

add_subdirectory(vendor EXCLUDE_FROM_ALL)
add_subdirectory(frontend)

# Settings
add_library(settings INTERFACE)
target_compile_features(settings INTERFACE cxx_std_17)
target_compile_options(settings INTERFACE -pedantic -Wall -Werror)

add_library(machine
  source/machine.cc
  source/machine_api.cc
)
target_link_libraries(machine
  PUBLIC
    settings
    nlohmann_json::nlohmann_json
    pistache::pistache
)
target_include_directories(machine PUBLIC source)

add_executable(test
  source/test-machine.cc
)
target_link_libraries(test PRIVATE machine doctest::doctest)
add_custom_target(run-tests ALL test ARGS -fc)

add_executable(webui webui/main.cc)
target_link_libraries(webui PRIVATE settings machine)

install(TARGETS webui)
install(DIRECTORY frontend/build/ DESTINATION ui)
