cmake_minimum_required(VERSION 3.0...3.5)
project(cpp-project LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
)

set(SOURCES
)

set(TEST_SOURCES
    tests/phony_test.cc
)

set(CMAKE_MODULE_PATH 
    ${CMAKE_SOURCE_DIR}/cmake
)

set(INSTALL_GTEST OFF)

include(EnableGoogleTest)
include(CTest)
include(SetGlobalDefines)
include(SetPlatformFlags)

install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/)

add_executable(
    cpp-project
    src/main.cc
    ${SOURCES}
)
target_include_directories(cpp-project PRIVATE include)
target_link_libraries(
    cpp-project
)


if (GTest_FOUND)
  add_executable(
      unit_tests
      tests/main.cc
      ${TEST_SOURCES}
  )
  target_include_directories(unit_tests PRIVATE include)
  target_link_libraries(
      unit_tests
      GTest::gtest_main
  )
  gtest_discover_tests(unit_tests)
endif()
