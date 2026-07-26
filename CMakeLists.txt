# Mandatory line, sets the minimum version of CMake that should be used with this repository.
# 3.22 is the minimum CMake version required by JUCE.
# 3.25 is required for the MSVC_DEBUG_INFORMATION_FORMAT feature to work.
# To verify your version run
# $ cmake --version
cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# Enable setting CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
set(CMAKE_POLICY_DEFAULT_CMP0141        NEW      CACHE STRING "" FORCE)

# Sets the minimum deployment target
# macOS 10.11 is the minimum required by JUCE
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "" FORCE)
# Builds a binary that works on Apple Silicon (arm64) and Intel-based Macs
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "" FORCE)

# Sets a few variables, like PROJECT_NAME.
project(SkyDronePlugin)

# Set the C++ standard only if this project is standalone;
# otherwise, let clients decide.
if(PROJECT_IS_TOP_LEVEL)
  if (NOT CMAKE_CXX_STANDARD)
    # Always use the newest C++ standard on green-field projects if possible.
    set(CMAKE_CXX_STANDARD 23)
  endif()

  # Links to C++ runtime statically
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  # Embeds debug information
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:Embedded>" CACHE STRING "" FORCE)
endif()

# I like to download the dependencies to the same folder as the project.
# If you want to install them system-wide, set CPM_SOURCE_CACHE with a path to the dependencies
# either as an environment variable or pass it with -DCPM_SOURCE_CACHE=<path> when invoking cmake.
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Downloads CPM if not already downloaded. CPM is an easy-to-use package manager nicely integrated with CMake.
include(cmake/cpm.cmake)

# This commands downloads AND configures JUCE. It sets up some variables, like JUCE_SOURCE_DIR.
cpmaddpackage(
  NAME
    JUCE
  GIT_TAG
    8.0.12
  GITHUB_REPOSITORY
    juce-framework/JUCE
  SOURCE_DIR
    ${LIB_DIR}/juce
)
# https://github.com/adamstark/AudioFile trying for audio file gen for testing
cpmaddpackage(
    NAME
      AUDIOFILE
    GIT_TAG
      1.1.4
    GITHUB_REPOSITORY
      adamstark/AudioFile
    SOURCE_DIR
      ${LIB_DIR}/audiofile
)

# Adds compiler warning utilities & other.
include(cmake/CompilerWarnings.cmake)
include(cmake/Util.cmake)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  # Sets the Windows VST3 installation location to one
  # that doesn't require elevated permissions
  set_directory_properties(PROPERTIES JUCE_VST3_COPY_DIR "$ENV{LOCALAPPDATA}/Programs/Common/VST3")
endif()

# Adds a plugin target.
set(PLUGIN_NAME "SkyDrone")
juce_add_plugin(
  SkyDronePlugin
  PRODUCT_NAME
    ${PLUGIN_NAME}
  VERSION
    0.0.0
  COMPANY_NAME
    "CloudMapMusic"
  PLUGIN_MANUFACTURER_CODE
    Clmm
  PLUGIN_CODE
    Cmdr
  FORMATS
    Standalone
    AU
    VST3
  IS_SYNTH
    FALSE
  NEEDS_MIDI_INPUT
    FALSE
  NEEDS_MIDI_OUTPUT
    FALSE
  VST3_AUTO_MANIFEST
    FALSE
  COPY_PLUGIN_AFTER_BUILD
    TRUE
)

# Configures the JUCE sources included in the plugin target.
target_compile_definitions(SkyDronePlugin
  PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

juce_add_binary_data(sky_drone_plugin_assets
  HEADER_NAME
    "SkyDronePluginAssets.h"
  NAMESPACE
    sky_drone::assets
  SOURCES
    assets/Background.png
    assets/Logo.png
    # TODO: Add custom font files
)

add_library(sky_drone::sky_drone_plugin_assets ALIAS sky_drone_plugin_assets)

juce_add_module(sky_drone_plugin ALIAS_NAMESPACE sky_drone)

# Additional definitions required by all users of this module.
target_compile_definitions(sky_drone_plugin
  INTERFACE
    PLUGIN_NAME="${PLUGIN_NAME}"
)

# Enables strict C++ warnings.
target_compile_options(sky_drone_plugin INTERFACE "${PROJECT_WARNINGS_CXX}")

target_link_libraries(sky_drone_plugin INTERFACE sky_drone::sky_drone_plugin_assets)

target_link_libraries(
  SkyDronePlugin
  PRIVATE
    sky_drone::sky_drone_plugin
  PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# In Visual Studio this command provides a nice grouping of source files in "filters".
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..)

option(BUILD_TESTS "When on, will download the GoogleTest framework and add the automated tests project" OFF)
if(BUILD_TESTS)
  # This command allows running tests from the "build" folder (the one where CMake generates the project to).
  enable_testing()

  # Adds all the targets configured in the "test" folder.
  add_subdirectory(test)
endif()
