cmake_minimum_required(VERSION 3.10)

project(whisperer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt automoc, autouic, and autorcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(FLAGS_COMMON -Wall)
set(FLAGS_LINUX -Wextra -Werror -Wconversion -pedantic)
set(FLAGS_WINDOWS /W4 /WX /we)

set(LINUX_SOURCES src/linux_listener.cpp src/file_input_device.cpp src/wl_clipboard_reader.cpp src/translator.cpp src/linux_popup)
set(WINDOWS_SOURCES src/windows_listener.cpp src/windows_clipboard_reader.cpp src/windows_api.cpp src/windows_popup)
set(COMMON_SOURCES src/main.cpp src/translator.cpp src/whisperer.cpp src/lingva_api.cpp)

set(LINUX_TESTS tests/linux_listener_ut.cpp tests/translator_ut.cpp)
set(WINDOWS_TESTS tests/windows_listener_ut.cpp)

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd") # Use dynamic runtime library in Debug mode
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD") # Use dynamic runtime library in Release mode
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)


include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8 # Stable commit
)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

find_package(Qt5 COMPONENTS Widgets QUIET)

if(WIN32)
  # Try to install Qt5 via vcpkg if not found
  if(NOT Qt5_FOUND AND DEFINED ENV{VCPKG_ROOT})
      message(STATUS "Qt5 not found. Attempting to install via vcpkg...")

      if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
          set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
          message(STATUS "Using vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
      endif()

      set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "")

      execute_process(
        COMMAND vcpkg install qt5:${VCPKG_TARGET_TRIPLET}
        WORKING_DIRECTORY "$ENV{VCPKG_ROOT}"
        RESULT_VARIABLE vcpkg_result
      )
      if(NOT vcpkg_result EQUAL 0)
          message(FATAL_ERROR "Failed to install Qt5 via vcpkg.")
      endif()

      if(DEFINED ENV{VCPKG_ROOT})
        set(Qt5_DIR "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/share/cmake/Qt5")
        list(APPEND CMAKE_PREFIX_PATH "$ENV{VCPKG_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/share/cmake/Qt5")
        message(STATUS "Setting Qt5_DIR to: ${Qt5_DIR}")
      endif()

      find_package(Qt5 COMPONENTS Widgets REQUIRED)
  endif()
endif()

if(UNIX)
    set(PLATFORM_SOURCES ${LINUX_SOURCES})
    set(PLATFORM_TESTS ${LINUX_TESTS})
    set(PLATFORM_FLAGS ${FLAGS_COMMON} ${FLAGS_LINUX})
elseif(WIN32)
    set(PLATFORM_SOURCES ${WINDOWS_SOURCES})
    set(PLATFORM_TESTS ${WINDOWS_TESTS})
    set(PLATFORM_FLAGS ${FLAGS_COMMON} ${FLAGS_WINDOWS})
endif()

enable_testing()

add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${PLATFORM_SOURCES})
add_executable(${PROJECT_NAME}-ut ${PLATFORM_TESTS} ${PLATFORM_SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE ${PLATFORM_FLAGS})
target_compile_options(${PROJECT_NAME}-ut PRIVATE ${PLATFORM_FLAGS})

target_link_libraries(${PROJECT_NAME} cpr::cpr nlohmann_json::nlohmann_json Qt5::Widgets)
target_link_libraries(${PROJECT_NAME}-ut gmock gmock_main gtest cpr::cpr nlohmann_json::nlohmann_json Qt5::Widgets)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-ut)
