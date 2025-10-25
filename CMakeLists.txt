cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

project(virtual-midi-foot VERSION 1.0.0)

FetchContent_Declare(JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG        7.0.9
  SOURCE_DIR     ${CMAKE_SOURCE_DIR}/deps/JUCE
)

set (JUCE_COPY_PLUGIN_AFTER_BUILD ON)
set (JUCE_ENABLE_MODULE_SOURCE_GROUPS ON)

FetchContent_MakeAvailable(JUCE)

juce_add_plugin(virtual-midi-foot
    BUNDLE_ID nl.izmar.virtual-midi-foot
    COMPANY_NAME Izmar
    IS_SYNTH TRUE
    MICROPHONE_PERMISSION_ENABLED FALSE
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    PLUGIN_MANUFACTURER_CODE Izmr
    PLUGIN_CODE Vmif 
    FORMATS LV2 Standalone
    PRODUCT_NAME "virtual-midi-foot")

set(_src_root_path "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(
  GLOB_RECURSE _source_list
  LIST_DIRECTORIES false
  RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
  "${_src_root_path}/*.c*"
  "${_src_root_path}/*.h*"
)

target_sources(virtual-midi-foot PRIVATE ${_source_list})

target_compile_definitions(virtual-midi-foot PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_DISPLAY_SPLASH_SCREEN=0)

if (PROJECT_IS_TOP_LEVEL)
    # Create symlink to compile_commands.json for IDE to pick it up
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()

target_link_libraries(virtual-midi-foot PRIVATE
    juce::juce_audio_utils
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
