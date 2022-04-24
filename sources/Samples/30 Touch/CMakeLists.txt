project (Sample30)

include_directories(Inc)
                    
add_definitions(-DSTM32F746xx -DDEVICE -DVS_KEIL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

file(GLOB SRC       Src/*.* Inc/*.*)

source_group(src    FILES ${SRC})

add_library(${PROJECT_NAME} ${SRC}
)
