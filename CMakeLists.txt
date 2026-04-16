cmake_minimum_required(VERSION 2.4...4.3)
set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/cmake
)
include(LoadOptions)

set_project_options()
project(${PROJECT} LANGUAGES CXX)
set_compiler_options()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(HEADERS
)

set(SOURCES
    src/core/file.cc
    src/core/format_determiner.cc
    src/core/format.cc
)

set(TEST_SOURCES
    tests/test_argparse.cc
)

set(INSTALL_GTEST OFF)

include(EnableGoogleTest)
include(CTest)

add_executable(
    formatof
    src/cli/main.cc
    ${SOURCES}
)


if (GTest_FOUND)
add_executable(
      unit_tests
      tests/main.cc
      ${SOURCES}
      ${TEST_SOURCES}
  )
  target_compile_definitions(unit_tests PRIVATE FORMATOF_RESOURCE_DIR="${CMAKE_SOURCE_DIR}/tests/resources")
  target_link_libraries(
      unit_tests
      GTest::gtest_main
  )
  gtest_discover_tests(unit_tests)
endif()
