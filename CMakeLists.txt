cmake_minimum_required(VERSION 3.10)

project(whisperer)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FLAGS_COMMON -Wall)
set(FLAGS_LINUX -Wextra -Werror -Wconversion -pedantic)
set(FLAGS_WINDOWS /W4 /WX /we)

set(SOURCES src/linux_listener.cpp src/file_input_device.cpp src/wl_clipboard_reader.cpp src/translator.cpp src/whisperer.cpp src/lingva_api.cpp)
set(TESTS tests/listener_ut.cpp tests/translator_ut.cpp)

# Set output directory for all executables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

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

# Check for Qt installation
find_package(Qt5 COMPONENTS Widgets QUIET)

# On Windows, if Qt5 is not found, attempt to install it via vcpkg
if(WIN32)
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

enable_testing()

add_executable(${PROJECT_NAME} src/main.cpp ${SOURCES})
add_executable(${PROJECT_NAME}-ut ${TESTS} ${SOURCES})

if(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS_COMMON} ${FLAGS_LINUX})
elseif(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS_COMMON} ${FLAGS_WINDOWS})
endif()

target_link_libraries(${PROJECT_NAME}-ut gtest gmock_main cpr::cpr nlohmann_json::nlohmann_json Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} cpr::cpr nlohmann_json::nlohmann_json Qt5::Widgets)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}-ut)
