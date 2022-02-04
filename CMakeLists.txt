cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_SDK_PATH "/mnt/c/Users/fores/pico/pico-sdk")


include(pico_sdk_import.cmake)
project(oled)
pico_sdk_init()

add_executable(oled
	oled.c
	ssd1306.c
)

pico_set_program_name(oled "oled")
pico_set_program_version(oled "0.1")

pico_enable_stdio_uart(oled 1)
pico_enable_stdio_usb(oled 1)

target_link_libraries(oled pico_stdlib hardware_i2c)

pico_add_extra_outputs(oled)