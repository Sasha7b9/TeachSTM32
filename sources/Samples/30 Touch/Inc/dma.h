#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f7xx_hal.h"

    extern DMA_HandleTypeDef hdma_memtomem_dma2_stream0;

    void MX_DMA_Init(void);

#ifdef __cplusplus
}
#endif
