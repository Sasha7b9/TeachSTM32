#pragma once
#include "stm32746g_sdram.h"
#include "../Fonts/fonts.h"

typedef struct 
{ 
  uint32_t TextColor; 
  uint32_t BackColor;  
  sFONT    *pFont;
}LCD_DrawPropTypeDef;   
   
typedef struct 
{
  int16_t X;
  int16_t Y;
}Point, * pPoint; 
   
/** 
  * @brief  Line mode structures definition  
  */ 
typedef enum
{
  CENTER_MODE             = 0x01,    /* Center mode */
  RIGHT_MODE              = 0x02,    /* Right mode  */
  LEFT_MODE               = 0x03     /* Left mode   */
}Text_AlignModeTypdef;

/**
  * @}
  */ 

/** @defgroup STM32746G_DISCOVERY_LCD_Exported_Constants STM32746G_DISCOVERY_LCD Exported Constants
  * @{
  */ 
#define MAX_LAYER_NUMBER       ((uint32_t)2)

#define LCD_LayerCfgTypeDef    LTDC_LayerCfgTypeDef

#define LTDC_ACTIVE_LAYER	     ((uint32_t)1) /* Layer 1 */
/** 
  * @brief  LCD status structure definition  
  */     
#define LCD_OK                 ((uint8_t)0x00)
#define LCD_ERROR              ((uint8_t)0x01)
#define LCD_TIMEOUT            ((uint8_t)0x02)

/** 
  * @brief  LCD FB_StartAddress  
  */
//#define LCD_FB_START_ADDRESS       ((uint32_t)0xD0000000)

/** 
  * @brief  LCD color  
  */ 
/* --------------------------------ARGB888 -------------------------------------*/
//#define LCD_COLOR_BLUE          ((uint32_t)0xFF0000FF)
//#define LCD_COLOR_GREEN         ((uint32_t)0xFF00FF00)
//#define LCD_COLOR_RED           ((uint32_t)0xFFFF0000)
//#define LCD_COLOR_CYAN          ((uint32_t)0xFF00FFFF)
//#define LCD_COLOR_MAGENTA       ((uint32_t)0xFFFF00FF)
//#define LCD_COLOR_YELLOW        ((uint32_t)0xFFFFFF00)
//#define LCD_COLOR_LIGHTBLUE     ((uint32_t)0xFF8080FF)
//#define LCD_COLOR_LIGHTGREEN    ((uint32_t)0xFF80FF80)
//#define LCD_COLOR_LIGHTRED      ((uint32_t)0xFFFF8080)
//#define LCD_COLOR_LIGHTCYAN     ((uint32_t)0xFF80FFFF)
//#define LCD_COLOR_LIGHTMAGENTA  ((uint32_t)0xFFFF80FF)
//#define LCD_COLOR_LIGHTYELLOW   ((uint32_t)0xFFFFFF80)
//#define LCD_COLOR_DARKBLUE      ((uint32_t)0xFF000080)
//#define LCD_COLOR_DARKGREEN     ((uint32_t)0xFF008000)
//#define LCD_COLOR_DARKRED       ((uint32_t)0xFF800000)
//#define LCD_COLOR_DARKCYAN      ((uint32_t)0xFF008080)
//#define LCD_COLOR_DARKMAGENTA   ((uint32_t)0xFF800080)
//#define LCD_COLOR_DARKYELLOW    ((uint32_t)0xFF808000)
//#define LCD_COLOR_WHITE         ((uint32_t)0xFFFFFFFF)
//#define LCD_COLOR_LIGHTGRAY     ((uint32_t)0xFFD3D3D3)
//#define LCD_COLOR_GRAY          ((uint32_t)0xFF808080)
//#define LCD_COLOR_DARKGRAY      ((uint32_t)0xFF404040)
//#define LCD_COLOR_BLACK         ((uint32_t)0xFF000000)
//#define LCD_COLOR_BROWN         ((uint32_t)0xFFA52A2A)
//#define LCD_COLOR_ORANGE        ((uint32_t)0xFFFFA500)
//#define LCD_COLOR_TRANSPARENT   ((uint32_t)0xFF000000)

/* --------------------------------RGB565 -------------------------------------*/
#define LCD_COLOR_BLUE          0x001F
#define LCD_COLOR_GREEN         0x07E0
#define LCD_COLOR_RED           0xF800
#define LCD_COLOR_CYAN          0x07FF
#define LCD_COLOR_MAGENTA       0xF81F
#define LCD_COLOR_YELLOW        0xFFE0
#define LCD_COLOR_LIGHTBLUE     0x841F
#define LCD_COLOR_LIGHTGREEN    0x87F0
#define LCD_COLOR_LIGHTRED      0xFC10
#define LCD_COLOR_LIGHTCYAN     0x87FF
#define LCD_COLOR_LIGHTMAGENTA  0xFC1F
#define LCD_COLOR_LIGHTYELLOW   0xFFF0
#define LCD_COLOR_DARKBLUE      0x0010
#define LCD_COLOR_DARKGREEN     0x0400
#define LCD_COLOR_DARKRED       0x8000
#define LCD_COLOR_DARKCYAN      0x0410
#define LCD_COLOR_DARKMAGENTA   0x8010
#define LCD_COLOR_DARKYELLOW    0x8400
#define LCD_COLOR_WHITE         0xFFFF
#define LCD_COLOR_LIGHTGRAY     0xD69A
#define LCD_COLOR_GRAY          0x8410
#define LCD_COLOR_DARKGRAY      0x4208
#define LCD_COLOR_BLACK         0x0000
#define LCD_COLOR_BROWN         0xA145
#define LCD_COLOR_ORANGE        0xFD20

/** 
  * @brief LCD default font 
  */ 
#define LCD_DEFAULT_FONT        Font24     

/**
  * @brief LCD special pins
  */
/* Display enable pin */
#define LCD_DISP_PIN                    GPIO_PIN_12
#define LCD_DISP_GPIO_PORT              GPIOI
#define LCD_DISP_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOI_CLK_ENABLE()
#define LCD_DISP_GPIO_CLK_DISABLE()     __HAL_RCC_GPIOI_CLK_DISABLE()

/* Backlight control pin */
#define LCD_BL_CTRL_PIN                  GPIO_PIN_3
#define LCD_BL_CTRL_GPIO_PORT            GPIOK
#define LCD_BL_CTRL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOK_CLK_ENABLE()
#define LCD_BL_CTRL_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOK_CLK_DISABLE()

namespace LCD   
{
    uint8_t  Init(void);
    uint8_t  DeInit(void);
    uint32_t GetXSize(void);
    uint32_t GetYSize(void);
    void     SetXSize(uint32_t imageWidthPixels);
    void     SetYSize(uint32_t imageHeightPixels);

    /* Functions using the LTDC controller */
    void     LayerDefaultInit(uint16_t LayerIndex, uint32_t FrameBuffer);
    void     LayerRgb565Init(uint16_t LayerIndex, uint32_t FB_Address);
    void     SetTransparency(uint32_t LayerIndex, uint8_t Transparency);
    void     SetLayerAddress(uint32_t LayerIndex, uint32_t Address);
    void     SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue);
    void     ResetColorKeying(uint32_t LayerIndex);
    void     SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

    void     SelectLayer(uint32_t LayerIndex);
    void     SetLayerVisible(uint32_t LayerIndex, FunctionalState State);

    void     SetTextColor(uint32_t Color);
    uint32_t GetTextColor(void);
    void     SetBackColor(uint32_t Color);
    uint32_t GetBackColor(void);
    void     SetFont(sFONT *fonts);
    sFONT   *GetFont(void);

    uint32_t ReadPixel(uint16_t Xpos, uint16_t Ypos);
    void     DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t pixel);
    void     Clear(uint32_t Color);
    void     ClearStringLine(uint32_t Line);
    void     DisplayStringAtLine(uint16_t Line, uint8_t *ptr);
    void     DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode);
    void     DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

    void     DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
    void     DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
    void     DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void     DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
    void     DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
    void     DrawPolygon(pPoint Points, uint16_t PointCount);
    void     DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
    void     DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp);

    void     FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
    void     FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
    void     FillPolygon(pPoint Points, uint16_t PointCount);
    void     FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

    void     DisplayOff(void);
    void     DisplayOn(void);

    /* These functions can be modified in case the current settings
       need to be changed for specific application needs */
    void     MspInit(LTDC_HandleTypeDef *hltdc, void *Params);
    void     MspDeInit(LTDC_HandleTypeDef *hltdc, void *Params);
    void     ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params);
}
