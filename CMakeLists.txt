# THIS FILE IS AUTO GENERATED FROM THE TEMPLATE! DO NOT CHANGE!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.27)

# generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(fsae-2024 C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# Uncomment for hardware floating point
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

# Uncomment for software floating point add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common
                    -fmessage-length=0)

# uncomment to mitigate c++17 absolute addresses warnings set(CMAKE_CXX_FLAGS
# "${CMAKE_CXX_FLAGS} -Wno-register")

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
  message(STATUS "Maximum optimization for speed")
  add_compile_options(-Ofast)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
  message(STATUS "Maximum optimization for speed, debug info included")
  add_compile_options(-Ofast -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
  message(STATUS "Maximum optimization for size")
  add_compile_options(-Os)
else()
  message(STATUS "Minimal optimization, debug info included")
  add_compile_options(-Og -g)
endif()

include_directories(
  Core/Inc
  Drivers/STM32F4xx_HAL_Driver/Inc
  Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
  Drivers/CMSIS/Device/ST/STM32F4xx/Include
  Drivers/CMSIS/Include
  Core/ThreadSafe
  AZURE_RTOS/App
  Middlewares/ST/filex/common/inc/
  Middlewares/ST/filex/ports/generic/inc/
  Middlewares/ST/threadx/common/inc/
  Middlewares/ST/threadx/ports/cortex_m4/gnu/inc/
  Middlewares/ST/threadx/utility/low_power/
  FileX/App
  Drivers/BSP/Components/lan8742/
  Drivers/BSP/Components/n25q128a/
  Drivers/BSP/Components/n25q512a/
  Drivers/BSP/Components/n25q256a/
  Middlewares/ST/levelx/common/inc/
  FileX/Target
  Middlewares/ST/netxduo/common/inc/
  Middlewares/ST/netxduo/ports/cortex_m4/gnu/inc/
  Middlewares/ST/netxduo/addons/cloud/
  Middlewares/ST/usbx/common/usbx_host_classes/inc/
  Middlewares/ST/usbx/common/core/inc/
  Middlewares/ST/usbx/common/usbx_device_classes/inc/
  Middlewares/ST/usbx/ports/generic/inc/
  Middlewares/ST/usbx/common/usbx_network/inc/
  Middlewares/ST/usbx/common/usbx_stm32_device_controllers/
  Middlewares/ST/usbx/common/usbx_stm32_host_controllers/
  Core/SEGGER_RTT/RTT
  Core/SEGGER_RTT/Config
  Drivers/Sensors/ADC/Inc
  Drivers/Sensors/IMU/Inc
  Drivers/Sensors/GNSS/Inc
  Drivers/Configs/Inc
  Tasks/Inc
  USB_DEVICE/App
  USB_DEVICE/Target
  STM32_USB_Device_Library/Core/Inc
  STM32_USB_Device_Library/Class/CDC/Inc
  Drivers/Sensors/Wheel/Inc
  Drivers/Traction/Inverter/Inc
  Drivers/Sensors/Steering/Inc
  Drivers/Sensors/WheelSpeed/Inc)

add_definitions(
  -DDEBUG
  -DUSE_HAL_DRIVER
  -DSTM32F407xx
  -DSTM32_THREAD_SAFE_STRATEGY=2
  -DTX_INCLUDE_USER_DEFINE_FILE
  -DFX_INCLUDE_USER_DEFINE_FILE
  -DJSMN_STATIC)

file(
  GLOB_RECURSE
  SOURCES
  "AZURE_RTOS/*.*"
  "FileX/*.*"
  "ThreadX/*.*"
  "Core/*.*"
  "Middlewares/*.*"
  "Drivers/*.*"
  "Tasks/*.*"
  "USB_DEVICE/*.*"
  "STM32_USB_Device_Library/*.*")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VGTX_FLASH.ld)

add_link_options(
  -Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map
)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(
  TARGET ${PROJECT_NAME}.elf
  POST_BUILD
  COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
  COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf>
          ${BIN_FILE}
  COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")
