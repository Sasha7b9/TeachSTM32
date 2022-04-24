#pragma once
#include "stm32f7xx_hal.h"

#define GT811_CMD_WR        0XBA    /* GT811 Write address  */
#define GT811_CMD_RD        0XBB    /* GT811 Read  address  */


/* Max detectable simultaneous touches */
#define TS_MAX_NB_TOUCH    5

/**
 * @brief : Definitions for GT811 I2C register addresses on 8 bit
 **/

#define GT811_EXCHG_XY		 1		    /* Swap XY  */

#define GT811_READ_XY_REG  0x721  	/* GT811 touch message start address  */
#define GT811_CONFIG_REG   0x6A2	  /* GT811 configuration start address  */

#define GT811_WAKEUP       0x0FFF   /* GT811 wakeup command  */
#define GT811_SLEEP        0x8000   /* GT811 sleep  command  */

#define GT811_VERSION      0x717    /* GT811 version register addresses */

#define GT811_VERSION_VALUE      0x2010   /* GT811 firmware version */

typedef struct
{
    uint8_t  touchDetected;                /*!< Total number of active touches detected at last scan */
    uint16_t touchX[TS_MAX_NB_TOUCH];      /*!< Touch X[0], X[1] coordinates on 12 bits */
    uint16_t touchY[TS_MAX_NB_TOUCH];      /*!< Touch Y[0], Y[1] coordinates on 12 bits */
    uint8_t  touchWeight[TS_MAX_NB_TOUCH]; /*!< Touch_Weight[0], Touch_Weight[1] : weight property of touches */
    uint32_t SensorId; /*!< type of gesture detected : take value of type @ref TS_GestureIdTypeDef */
} TS_StateTypeDef;


namespace GT811
{
    uint8_t  Init(void);
    uint16_t ReadID(void);
    void     GetState(TS_StateTypeDef *TS_State);
}
