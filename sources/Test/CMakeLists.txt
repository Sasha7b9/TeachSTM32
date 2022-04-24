project (Test)

include_directories(../common/MDK_KEIL/CMSIS/
                    src/Hardware
                    ../common/BSP
                    ../common/stm32CubeF7/Drivers/STM32F7xx_HAL_Driver/Inc/
                    ../common/stm32CubeF7/Drivers/CMSIS/Device/ST/STM32F7xx/Include/
                    ../common/stm32CubeF7/Drivers/CMSIS/Include/
)

add_definitions(-DSTM32F746xx -DDEVICE -DVS_KEIL)

add_compile_options(/wd4100)
add_compile_options(/wd4242)
add_compile_options(/wd4244)
add_compile_options(/wd4365)
add_compile_options(/wd4459)
add_compile_options(/wd4464)
add_compile_options(/wd4820)
add_compile_options(/wd5045)
add_compile_options(/wd5219)
add_compile_options(/wd5220)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

file(GLOB BSP       ../common/BSP/*.*)
file(GLOB SRC       src/*.*)
file(GLOB HARDWARE  src/Hardware/*.*)

source_group(BSP      FILES ${BSP})
source_group(src      FILES ${SRC})
source_group(Hardware FILES ${HARDWARE})

add_library(${PROJECT_NAME} ${BSP} ${SRC} ${HARDWARE}
)
