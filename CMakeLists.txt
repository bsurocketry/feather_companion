cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(feather_companion)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPICO_BOARD=adafruit_feather_rp2040")

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

