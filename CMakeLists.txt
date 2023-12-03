cmake_minimum_required(VERSION 3.13)

include(FetchContent)

project(plugin VERSION 0.0.1)

FetchContent_Declare(JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG        7.0.5
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/deps/JUCE
)

set (JUCE_COPY_PLUGIN_AFTER_BUILD ON)
set (JUCE_ENABLE_MODULE_SOURCE_GROUPS ON)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(plugin
    BUNDLE_ID org.example.plugin
    COMPANY_NAME Izmar
    IS_SYNTH TRUE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    PLUGIN_MANUFACTURER_CODE Izmr
    PLUGIN_CODE PluG
    FORMATS VST3 Standalone
    PRODUCT_NAME "plugin")

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(
  GLOB_RECURSE _source_list
  LIST_DIRECTORIES false
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${_src_root_path}/*.c*"
  "${_src_root_path}/*.h*"
)

target_sources(plugin PRIVATE ${_source_list})

target_compile_definitions(plugin PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries(plugin PRIVATE
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
