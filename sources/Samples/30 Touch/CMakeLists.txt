project (Sample30)

include_directories(../../common/MDK_KEIL/CMSIS/
                    Inc
                    ../../common/stm32CubeF7/Drivers/STM32F7xx_HAL_Driver/Inc/
                    ../../common/stm32CubeF7/Drivers/CMSIS/Device/ST/STM32F7xx/Include/
                    ../../common/stm32CubeF7/Drivers/CMSIS/Include/
)

add_definitions(-DSTM32F746xx -DDEVICE -DVS_KEIL -D__CC_ARM)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

file(GLOB SRC       Src/*.* Inc/*.*)

source_group(src    FILES ${SRC})

add_library(${PROJECT_NAME} ${SRC}
)
