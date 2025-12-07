cmake_minimum_required(VERSION 3.0...3.5)
project(formatof LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
)

set(SOURCES
    src/core/file.cc
    src/core/format.cc
    src/core/format_list.cc
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
    formatof
    src/cli/main.cc
    ${SOURCES}
)

find_package(Qt6 CONFIG COMPONENTS Widgets PrintSupport)

if (Qt6_FOUND)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    qt_add_executable(formatof-gui
    MANUAL_FINALIZATION
    src/main.cc
    ${SOURCES}
    #    resources/icon.png
    )

    target_link_libraries(formatof-gui PRIVATE Qt6::Widgets Qt6::PrintSupport)

    set_target_properties(formatof-gui PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME formatof
        MACOSX_BUNDLE_BUNDLE_VERSION 0.1
        MACOSX_BUNDLE_GUI_IDENTIFIER formatof
        #    MACOSX_BUNDLE_ICON_FILE ../Resources/icon.png
        #    RESOURCE resources/icon.png
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )

    qt_finalize_executable(formatof-gui)
endif()

if (GTest_FOUND)
  add_executable(
      tests
      tests/main.cc
      ${SOURCES}
      ${TEST_SOURCES}
  )
  target_compile_definitions(tests PRIVATE FORMATOF_RESOURCE_DIR="${CMAKE_SOURCE_DIR}/tests/resources")
  target_include_directories(tests PRIVATE src/core)
  target_link_libraries(
      tests
      GTest::gtest_main
  )
  gtest_discover_tests(tests)
endif()
