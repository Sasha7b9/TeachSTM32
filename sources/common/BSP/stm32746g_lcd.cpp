#include "stm32746g_lcd.h"
#include "../Fonts/fonts.h"
#include "../Fonts/font24.c"
#include "../Fonts/font20.c"
#include "../Fonts/font16.c"
#include "../Fonts/font12.c"
#include "../Fonts/font8.c"
#include "dma2d.h"
#include "ltdc.h"
/** @addtogroup BSP
  * @{
  */
#define XSIZE_PHYS 1024
#define YSIZE_PHYS 600

#define HBP  2
#define VBP  2

#define HSW  10
#define VSW  2

#define HFP  2
#define VFP  2

  /** @addtogroup STM32746G_DISCOVERY
    * @{
    */

    /** @addtogroup STM32746G_DISCOVERY_LCD
      * @{
      */

      /** @defgroup STM32746G_DISCOVERY_LCD_Private_TypesDefinitions STM32746G_DISCOVERY_LCD Private Types Definitions
        * @{
        */
        /**
          * @}
          */

          /** @defgroup STM32746G_DISCOVERY_LCD_Private_Defines STM32746G_DISCOVERY LCD Private Defines
            * @{
            */
#define POLY_X(Z)              ((int32_t)((Points + Z)->X))
#define POLY_Y(Z)              ((int32_t)((Points + Z)->Y))      
            /**
              * @}
              */

              /** @defgroup STM32746G_DISCOVERY_LCD_Private_Macros STM32746G_DISCOVERY_LCD Private Macros
                * @{
                */
#define ABS(X)  ((X) > 0 ? (X) : -(X))      
                /**
                  * @}
                  */

                  /** @defgroup STM32746G_DISCOVERY_LCD_Private_Variables STM32746G_DISCOVERY_LCD Private Variables
                    * @{
                    */
                    //static LTDC_HandleTypeDef  hLtdcHandler;
                    //static DMA2D_HandleTypeDef hDma2dHandler;
#define hLtdcHandler hltdc  
#define hDma2dHandler hdma2d

/* Default LCD configuration with LCD Layer 1 */
static uint32_t            ActiveLayer = 0;
static LCD_DrawPropTypeDef DrawProp[MAX_LAYER_NUMBER];
/**
  * @}
  */

  /** @defgroup STM32746G_DISCOVERY_LCD_Private_FunctionPrototypes STM32746G_DISCOVERY_LCD Private Function Prototypes
    * @{
    */

namespace LCD
{
    static void DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c);
    static void FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3);
    static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);
    static void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode);
}
/**
  * @}
  */

  /** @defgroup STM32746G_DISCOVERY_LCD_Exported_Functions STM32746G_DISCOVERY_LCD Exported Functions
    * @{
    */

    /**
      * @brief  Initializes the LCD.
      * @retval LCD state
      */
uint8_t LCD::Init(void)
{
    /* Select the used LCD */

  //  /* Timing Configuration */
  //  hLtdcHandler.Init.HorizontalSync = HSW - 1;
  //  hLtdcHandler.Init.VerticalSync = VSW - 1;
  //  hLtdcHandler.Init.AccumulatedHBP = HSW + HBP - 1;
  //  hLtdcHandler.Init.AccumulatedVBP = VSW + VBP - 1;
  //  hLtdcHandler.Init.AccumulatedActiveH = VSW + VBP +  YSIZE_PHYS - 1;
  //  hLtdcHandler.Init.AccumulatedActiveW = HSW + HBP + XSIZE_PHYS - 1;
  //  hLtdcHandler.Init.TotalHeigh = VSW + VBP + YSIZE_PHYS + VFP - 1;
  //  hLtdcHandler.Init.TotalWidth = HSW + HBP + XSIZE_PHYS + HFP - 1;
  //  
  //  /* LCD clock configuration */
  //  LCD::ClockConfig(&hLtdcHandler, NULL);

  //  /* Initialize the LCD pixel width and pixel height */
  //  hLtdcHandler.LayerCfg->ImageWidth  = XSIZE_PHYS;
  //  hLtdcHandler.LayerCfg->ImageHeight = YSIZE_PHYS;

  //  /* Background value */
  //  hLtdcHandler.Init.Backcolor.Blue = 0;
  //  hLtdcHandler.Init.Backcolor.Green = 0;
  //  hLtdcHandler.Init.Backcolor.Red = 0;
  //  
  //  /* Polarity */
  //  hLtdcHandler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  //  hLtdcHandler.Init.VSPolarity = LTDC_VSPOLARITY_AL; 
  //  hLtdcHandler.Init.DEPolarity = LTDC_DEPOLARITY_AL;  
  //  hLtdcHandler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  //  hLtdcHandler.Instance = LTDC;

  //  if(HAL_LTDC_GetState(&hLtdcHandler) == HAL_LTDC_STATE_RESET)
  //  {
  //    /* Initialize the LCD Msp: this __weak function can be rewritten by the application */
  //    LCD::MspInit(&hLtdcHandler, NULL);
  //  }
  //  HAL_LTDC_Init(&hLtdcHandler);

  //  /* Assert backlight LCD_BL_CTRL pin */
  //  HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);

    /* Initialize the SDRAM */
    BSP_SDRAM_Init();

    /* Initialize the font */
    SetFont(&LCD_DEFAULT_FONT);


    return LCD_OK;
}

/**
  * @brief  DeInitializes the LCD.
  * @retval LCD state
  */
uint8_t LCD::DeInit(void)
{
    /* Initialize the hLtdcHandler Instance parameter */
    hLtdcHandler.Instance = LTDC;

    /* Disable LTDC block */
    __HAL_LTDC_DISABLE(&hLtdcHandler);

    /* DeInit the LTDC */
    HAL_LTDC_DeInit(&hLtdcHandler);

    /* DeInit the LTDC MSP : this __weak function can be rewritten by the application */
    LCD::MspDeInit(&hLtdcHandler, NULL);

    return LCD_OK;
}

/**
  * @brief  Gets the LCD X size.
  * @retval Used LCD X size
  */
uint32_t LCD::GetXSize(void)
{
    return hLtdcHandler.LayerCfg[ActiveLayer].ImageWidth;
}

/**
  * @brief  Gets the LCD Y size.
  * @retval Used LCD Y size
  */
uint32_t LCD::GetYSize(void)
{
    return hLtdcHandler.LayerCfg[ActiveLayer].ImageHeight;
}

/**
  * @brief  Set the LCD X size.
  * @param  imageWidthPixels : image width in pixels unit
  * @retval None
  */
void LCD::SetXSize(uint32_t imageWidthPixels)
{
    hLtdcHandler.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

/**
  * @brief  Set the LCD Y size.
  * @param  imageHeightPixels : image height in lines unit
  * @retval None
  */
void LCD::SetYSize(uint32_t imageHeightPixels)
{
    hLtdcHandler.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}

/**
  * @brief  Initializes the LCD layer in ARGB8888 format (32 bits per pixel).
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
void LCD::LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{
    LCD_LayerCfgTypeDef  layer_cfg;

    /* Layer Init */
    layer_cfg.WindowX0 = 0;
    layer_cfg.WindowX1 = LCD::GetXSize();
    layer_cfg.WindowY0 = 0;
    layer_cfg.WindowY1 = LCD::GetYSize();
    layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
    layer_cfg.FBStartAdress = FB_Address;
    layer_cfg.Alpha = 255;
    layer_cfg.Alpha0 = 0;
    layer_cfg.Backcolor.Blue = 0;
    layer_cfg.Backcolor.Green = 0;
    layer_cfg.Backcolor.Red = 0;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    layer_cfg.ImageWidth = LCD::GetXSize();
    layer_cfg.ImageHeight = LCD::GetYSize();

    HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LayerIndex);

    DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
    DrawProp[LayerIndex].pFont = &Font24;
    DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;
}

/**
  * @brief  Initializes the LCD layer in RGB565 format (16 bits per pixel).
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
void LCD::LayerRgb565Init(uint16_t LayerIndex, uint32_t FB_Address)
{
    LCD_LayerCfgTypeDef  layer_cfg;

    /* Layer Init */
    layer_cfg.WindowX0 = 0;
    layer_cfg.WindowX1 = LCD::GetXSize();
    layer_cfg.WindowY0 = 0;
    layer_cfg.WindowY1 = LCD::GetYSize();
    layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
    layer_cfg.FBStartAdress = FB_Address;
    layer_cfg.Alpha = 255;
    layer_cfg.Alpha0 = 0;
    layer_cfg.Backcolor.Blue = 0;
    layer_cfg.Backcolor.Green = 0;
    layer_cfg.Backcolor.Red = 0;
    layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
    layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
    layer_cfg.ImageWidth = LCD::GetXSize();
    layer_cfg.ImageHeight = LCD::GetYSize();

    HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LayerIndex);

    DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
    DrawProp[LayerIndex].pFont = &Font24;
    DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;
}

/**
  * @brief  Selects the LCD Layer.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
void LCD::SelectLayer(uint32_t LayerIndex)
{
    ActiveLayer = LayerIndex;
}

/**
  * @brief  Sets an LCD Layer visible
  * @param  LayerIndex: Visible Layer
  * @param  State: New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE
  * @retval None
  */
void LCD::SetLayerVisible(uint32_t LayerIndex, FunctionalState State)
{
    if (State == ENABLE)
    {
        __HAL_LTDC_LAYER_ENABLE(&hLtdcHandler, LayerIndex);
    }
    else
    {
        __HAL_LTDC_LAYER_DISABLE(&hLtdcHandler, LayerIndex);
    }
    __HAL_LTDC_RELOAD_CONFIG(&hLtdcHandler);
}

/**
  * @brief  Configures the transparency.
  * @param  LayerIndex: Layer foreground or background.
  * @param  Transparency: Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
  * @retval None
  */
void LCD::SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{
    HAL_LTDC_SetAlpha(&hLtdcHandler, Transparency, LayerIndex);
}

/**
  * @brief  Sets an LCD layer frame buffer address.
  * @param  LayerIndex: Layer foreground or background
  * @param  Address: New LCD frame buffer value
  * @retval None
  */
void LCD::SetLayerAddress(uint32_t LayerIndex, uint32_t Address)
{
    HAL_LTDC_SetAddress(&hLtdcHandler, Address, LayerIndex);
}

/**
  * @brief  Sets display window.
  * @param  LayerIndex: Layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height
  * @retval None
  */
void LCD::SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    /* Reconfigure the layer size */
    HAL_LTDC_SetWindowSize(&hLtdcHandler, Width, Height, LayerIndex);

    /* Reconfigure the layer position */
    HAL_LTDC_SetWindowPosition(&hLtdcHandler, Xpos, Ypos, LayerIndex);
}

/**
  * @brief  Configures and sets the color keying.
  * @param  LayerIndex: Layer foreground or background
  * @param  RGBValue: Color reference
  * @retval None
  */
void LCD::SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue)
{
    /* Configure and Enable the color Keying for LCD Layer */
    HAL_LTDC_ConfigColorKeying(&hLtdcHandler, RGBValue, LayerIndex);
    HAL_LTDC_EnableColorKeying(&hLtdcHandler, LayerIndex);
}

/**
  * @brief  Disables the color keying.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
void LCD::ResetColorKeying(uint32_t LayerIndex)
{
    /* Disable the color Keying for LCD Layer */
    HAL_LTDC_DisableColorKeying(&hLtdcHandler, LayerIndex);
}

/**
  * @brief  Sets the LCD text color.
  * @param  Color: Text color code ARGB(8-8-8-8)
  * @retval None
  */
void LCD::SetTextColor(uint32_t Color)
{
    DrawProp[ActiveLayer].TextColor = Color;
}

/**
  * @brief  Gets the LCD text color.
  * @retval Used text color.
  */
uint32_t LCD::GetTextColor(void)
{
    return DrawProp[ActiveLayer].TextColor;
}

/**
  * @brief  Sets the LCD background color.
  * @param  Color: Layer background color code ARGB(8-8-8-8)
  * @retval None
  */
void LCD::SetBackColor(uint32_t Color)
{
    DrawProp[ActiveLayer].BackColor = Color;
}

/**
  * @brief  Gets the LCD background color.
  * @retval Used background colour
  */
uint32_t LCD::GetBackColor(void)
{
    return DrawProp[ActiveLayer].BackColor;
}

/**
  * @brief  Sets the LCD text font.
  * @param  fonts: Layer font to be used
  * @retval None
  */
void LCD::SetFont(sFONT *fonts)
{
    DrawProp[ActiveLayer].pFont = fonts;
}

/**
  * @brief  Gets the LCD text font.
  * @retval Used layer font
  */
sFONT *LCD::GetFont(void)
{
    return DrawProp[ActiveLayer].pFont;
}

/**
  * @brief  Reads an LCD pixel.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @retval RGB pixel color
  */
uint32_t LCD::ReadPixel(uint16_t Xpos, uint16_t Ypos)
{
    uint32_t ret = 0;

    if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
    {
        /* Read data value from SDRAM memory */
        ret = *(__IO uint32_t *) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (2 * (Ypos * LCD::GetXSize() + Xpos)));
    }
    else if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
    {
        /* Read data value from SDRAM memory */
        ret = (*(__IO uint32_t *) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (2 * (Ypos * LCD::GetXSize() + Xpos))) & 0x00FFFFFF);
    }
    else if ((hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || \
        (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || \
        (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))
    {
        /* Read data value from SDRAM memory */
        ret = *(__IO uint16_t *) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (2 * (Ypos * LCD::GetXSize() + Xpos)));
    }
    else
    {
        /* Read data value from SDRAM memory */
        ret = *(__IO uint8_t *) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (2 * (Ypos * LCD::GetXSize() + Xpos)));
    }

    return ret;
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: Color of the background
  * @retval None
  */
void LCD::Clear(uint32_t Color)
{
    /* Clear the LCD */
    LL_FillBuffer(ActiveLayer, (uint32_t *)(hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress), LCD::GetXSize(), LCD::GetYSize(), 0, Color);
}

/**
  * @brief  Clears the selected line.
  * @param  Line: Line to be cleared
  * @retval None
  */
void LCD::ClearStringLine(uint32_t Line)
{
    uint32_t color_backup = DrawProp[ActiveLayer].TextColor;
    DrawProp[ActiveLayer].TextColor = DrawProp[ActiveLayer].BackColor;

    /* Draw rectangle with background color */
    FillRect(0, (Line * DrawProp[ActiveLayer].pFont->Height), LCD::GetXSize(), DrawProp[ActiveLayer].pFont->Height);

    DrawProp[ActiveLayer].TextColor = color_backup;
    SetTextColor(DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  Displays one character.
  * @param  Xpos: Start column address
  * @param  Ypos: Line where to display the character shape.
  * @param  Ascii: Character ascii code
  *           This parameter must be a number between Min_Data = 0x20 and Max_Data = 0x7E
  * @retval None
  */
void LCD::DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii)
{
    DrawChar(Xpos, Ypos, &DrawProp[ActiveLayer].pFont->table[(Ascii - ' ') * \
        DrawProp[ActiveLayer].pFont->Height * ((DrawProp[ActiveLayer].pFont->Width + 7) / 8)]);
}

/**
  * @brief  Displays characters on the LCD.
  * @param  Xpos: X position (in pixel)
  * @param  Ypos: Y position (in pixel)
  * @param  Text: Pointer to string to display on LCD
  * @param  Mode: Display mode
  *          This parameter can be one of the following values:
  *            @arg  CENTER_MODE
  *            @arg  RIGHT_MODE
  *            @arg  LEFT_MODE
  * @retval None
  */
void LCD::DisplayStringAt(uint16_t Xpos, uint16_t Ypos, uint8_t *Text, Text_AlignModeTypdef Mode)
{
    uint16_t ref_column = 1, i = 0;
    uint32_t size = 0, xsize = 0;
    uint8_t *ptr = Text;

    /* Get the text size */
    while (*ptr++) size++;

    /* Characters number per line */
    xsize = (LCD::GetXSize() / DrawProp[ActiveLayer].pFont->Width);

    switch (Mode)
    {
    case CENTER_MODE:
    {
        ref_column = Xpos + ((xsize - size) * DrawProp[ActiveLayer].pFont->Width) / 2;
        break;
    }
    case LEFT_MODE:
    {
        ref_column = Xpos;
        break;
    }
    case RIGHT_MODE:
    {
        ref_column = -Xpos + ((xsize - size) * DrawProp[ActiveLayer].pFont->Width);
        break;
    }
    default:
    {
        ref_column = Xpos;
        break;
    }
    }

    /* Check that the Start column is located in the screen */
    if ((ref_column < 1) || (ref_column >= 0x8000))
    {
        ref_column = 1;
    }

    /* Send the string character by character on LCD */
    while ((*Text != 0) & (((LCD::GetXSize() - (i * DrawProp[ActiveLayer].pFont->Width)) & 0xFFFF) >= DrawProp[ActiveLayer].pFont->Width))
    {
        /* Display one character on LCD */
        DisplayChar(ref_column, Ypos, *Text);
        /* Decrement the column position by 16 */
        ref_column += DrawProp[ActiveLayer].pFont->Width;
        /* Point on the next character */
        Text++;
        i++;
    }
}

/**
  * @brief  Displays a maximum of 60 characters on the LCD.
  * @param  Line: Line where to display the character shape
  * @param  ptr: Pointer to string to display on LCD
  * @retval None
  */
void LCD::DisplayStringAtLine(uint16_t Line, uint8_t *ptr)
{
    DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}

/**
  * @brief  Draws an horizontal line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void LCD::DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    uint32_t  Xaddress = 0;

    /* Get the line address */
    if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
    { /* RGB565 format */
        Xaddress = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 2 * (LCD::GetXSize() * Ypos + Xpos);
    }
    else
    { /* ARGB8888 format */
        Xaddress = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 4 * (LCD::GetXSize() * Ypos + Xpos);
    }

    /* Write line */
    LL_FillBuffer(ActiveLayer, (uint32_t *)Xaddress, Length, 1, 0, DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  Draws a vertical line.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Length: Line length
  * @retval None
  */
void LCD::DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length)
{
    uint32_t  Xaddress = 0;

    /* Get the line address */
    if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
    { /* RGB565 format */
        Xaddress = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 2 * (LCD::GetXSize() * Ypos + Xpos);
    }
    else
    { /* ARGB8888 format */
        Xaddress = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 4 * (LCD::GetXSize() * Ypos + Xpos);
    }

    /* Write line */
    LL_FillBuffer(ActiveLayer, (uint32_t *)Xaddress, 1, Length, (LCD::GetXSize() - 1), DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  Draws an uni-line (between two points).
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @retval None
  */
void LCD::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
        yinc1 = 0, yinc2 = 0, den = 0, num = 0, num_add = 0, num_pixels = 0,
        curpixel = 0;

    deltax = ABS(x2 - x1);        /* The difference between the x's */
    deltay = ABS(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */

    if (x2 >= x1)                 /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else                          /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1)                 /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else                          /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay)         /* There is at least one x-value for every y-value */
    {
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        num_add = deltay;
        num_pixels = deltax;         /* There are more x-values than y-values */
    }
    else                          /* There is at least one y-value for every x-value */
    {
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        num_add = deltax;
        num_pixels = deltay;         /* There are more y-values than x-values */
    }

    for (curpixel = 0; curpixel <= num_pixels; curpixel++)
    {
        LCD::DrawPixel(x, y, DrawProp[ActiveLayer].TextColor);   /* Draw the current pixel */
        num += num_add;                            /* Increase the numerator by the top of the fraction */
        if (num >= den)                           /* Check if numerator >= denominator */
        {
            num -= den;                             /* Calculate the new numerator value */
            x += xinc1;                             /* Change the x as appropriate */
            y += yinc1;                             /* Change the y as appropriate */
        }
        x += xinc2;                               /* Change the x as appropriate */
        y += yinc2;                               /* Change the y as appropriate */
    }
}

/**
  * @brief  Draws a rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width
  * @param  Height: Rectangle height
  * @retval None
  */
void LCD::DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    /* Draw horizontal lines */
    LCD::DrawHLine(Xpos, Ypos, Width);
    LCD::DrawHLine(Xpos, (Ypos + Height), Width);

    /* Draw vertical lines */
    LCD::DrawVLine(Xpos, Ypos, Height);
    LCD::DrawVLine((Xpos + Width), Ypos, Height);
}

/**
  * @brief  Draws a circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  * @retval None
  */
void LCD::DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int32_t   decision;    /* Decision Variable */
    uint32_t  current_x;   /* Current X Value */
    uint32_t  current_y;   /* Current Y Value */

    decision = 3 - (Radius << 1);
    current_x = 0;
    current_y = Radius;

    while (current_x <= current_y)
    {
        LCD::DrawPixel((Xpos + current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);

        LCD::DrawPixel((Xpos - current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);

        LCD::DrawPixel((Xpos + current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);

        LCD::DrawPixel((Xpos - current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);

        LCD::DrawPixel((Xpos + current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);

        LCD::DrawPixel((Xpos - current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);

        LCD::DrawPixel((Xpos + current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);

        LCD::DrawPixel((Xpos - current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);

        if (decision < 0)
        {
            decision += (current_x << 2) + 6;
        }
        else
        {
            decision += ((current_x - current_y) << 2) + 10;
            current_y--;
        }
        current_x++;
    }
}

/**
  * @brief  Draws an poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void LCD::DrawPolygon(pPoint Points, uint16_t PointCount)
{
    int16_t x = 0, y = 0;

    if (PointCount < 2)
    {
        return;
    }

    DrawLine(Points->X, Points->Y, (Points + PointCount - 1)->X, (Points + PointCount - 1)->Y);

    while (--PointCount)
    {
        x = Points->X;
        y = Points->Y;
        Points++;
        DrawLine(x, y, Points->X, Points->Y);
    }
}

/**
  * @brief  Draws an ellipse on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  * @retval None
  */
void LCD::DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
    int x = 0, y = -YRadius, err = 2 - 2 * XRadius, e2;
    float k = 0, rad1 = 0, rad2 = 0;

    rad1 = XRadius;
    rad2 = YRadius;

    k = (float)(rad2 / rad1);

    do {
        LCD::DrawPixel((Xpos - (uint16_t)(x / k)), (Ypos + y), DrawProp[ActiveLayer].TextColor);
        LCD::DrawPixel((Xpos + (uint16_t)(x / k)), (Ypos + y), DrawProp[ActiveLayer].TextColor);
        LCD::DrawPixel((Xpos + (uint16_t)(x / k)), (Ypos - y), DrawProp[ActiveLayer].TextColor);
        LCD::DrawPixel((Xpos - (uint16_t)(x / k)), (Ypos - y), DrawProp[ActiveLayer].TextColor);

        e2 = err;
        if (e2 <= x) {
            err += ++x * 2 + 1;
            if (-y == x && e2 <= y) e2 = 0;
        }
        if (e2 > y) err += ++y * 2 + 1;
    } while (y <= 0);
}

/**
  * @brief  Draws a pixel on LCD.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  RGB_Code: Pixel color in ARGB mode (8-8-8-8)
  * @retval None
  */
void LCD::DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t RGB_Code)
{
    /* Write data value to all SDRAM memory */
    if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
    { /* RGB565 format */
        *(__IO uint16_t *) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (2 * (Ypos * LCD::GetXSize() + Xpos))) = (uint16_t)RGB_Code;
    }
    else
    { /* ARGB8888 format */
        *(__IO uint32_t *) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (4 * (Ypos * LCD::GetXSize() + Xpos))) = RGB_Code;
    }
}

/**
  * @brief  Draws a bitmap picture loaded in the internal Flash in ARGB888 format (32 bits per pixel).
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD
  * @param  pbmp: Pointer to Bmp picture address in the internal Flash
  * @retval None
  */
void LCD::DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp)
{
    uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;
    uint32_t address;
    uint32_t input_color_mode = 0;

    /* Get bitmap data address offset */
    index = *(__IO uint16_t *) (pbmp + 10);
    index |= (*(__IO uint16_t *) (pbmp + 12)) << 16;

    /* Read bitmap width */
    width = *(uint16_t *)(pbmp + 18);
    width |= (*(uint16_t *)(pbmp + 20)) << 16;

    /* Read bitmap height */
    height = *(uint16_t *)(pbmp + 22);
    height |= (*(uint16_t *)(pbmp + 24)) << 16;

    /* Read bit/pixel */
    bit_pixel = *(uint16_t *)(pbmp + 28);

    /* Set the address */
    address = hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (((LCD::GetXSize() * Ypos) + Xpos) * (4));

    /* Get the layer pixel format */
    if ((bit_pixel / 8) == 4)
    {
        input_color_mode = CM_ARGB8888;
    }
    else if ((bit_pixel / 8) == 2)
    {
        input_color_mode = CM_RGB565;
    }
    else
    {
        input_color_mode = CM_RGB888;
    }

    /* Bypass the bitmap header */
    pbmp += (index + (width * (height - 1) * (bit_pixel / 8)));

    /* Convert picture to ARGB8888 pixel format */
    for (index = 0; index < height; index++)
    {
        /* Pixel format conversion */
        LL_ConvertLineToARGB8888((uint32_t *)pbmp, (uint32_t *)address, width, input_color_mode);

        /* Increment the source and destination buffers */
        address += (LCD::GetXSize() * 4);
        pbmp -= width * (bit_pixel / 8);
    }
}

/**
  * @brief  Draws a full rectangle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Width: Rectangle width
  * @param  Height: Rectangle height
  * @retval None
  */
void LCD::FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
    uint32_t  x_address = 0;

    /* Set the text color */
    LCD::SetTextColor(DrawProp[ActiveLayer].TextColor);

    /* Get the rectangle start address */
    if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
    { /* RGB565 format */
        x_address = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 2 * (LCD::GetXSize() * Ypos + Xpos);
    }
    else
    { /* ARGB8888 format */
        x_address = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 4 * (LCD::GetXSize() * Ypos + Xpos);
    }
    /* Fill the rectangle */
    LL_FillBuffer(ActiveLayer, (uint32_t *)x_address, Width, Height, (LCD::GetXSize() - Width), DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  Draws a full circle.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  Radius: Circle radius
  * @retval None
  */
void LCD::FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
    int32_t  decision;     /* Decision Variable */
    uint32_t  current_x;   /* Current X Value */
    uint32_t  current_y;   /* Current Y Value */

    decision = 3 - (Radius << 1);

    current_x = 0;
    current_y = Radius;

    LCD::SetTextColor(DrawProp[ActiveLayer].TextColor);

    while (current_x <= current_y)
    {
        if (current_y > 0)
        {
            LCD::DrawHLine(Xpos - current_y, Ypos + current_x, 2 * current_y);
            LCD::DrawHLine(Xpos - current_y, Ypos - current_x, 2 * current_y);
        }

        if (current_x > 0)
        {
            LCD::DrawHLine(Xpos - current_x, Ypos - current_y, 2 * current_x);
            LCD::DrawHLine(Xpos - current_x, Ypos + current_y, 2 * current_x);
        }
        if (decision < 0)
        {
            decision += (current_x << 2) + 6;
        }
        else
        {
            decision += ((current_x - current_y) << 2) + 10;
            current_y--;
        }
        current_x++;
    }

    LCD::SetTextColor(DrawProp[ActiveLayer].TextColor);
    DrawCircle(Xpos, Ypos, Radius);
}

/**
  * @brief  Draws a full poly-line (between many points).
  * @param  Points: Pointer to the points array
  * @param  PointCount: Number of points
  * @retval None
  */
void LCD::FillPolygon(pPoint Points, uint16_t PointCount)
{
    int16_t X = 0, Y = 0, X2 = 0, Y2 = 0, X_center = 0, Y_center = 0, X_first = 0, Y_first = 0, pixelX = 0, pixelY = 0, counter = 0;
    uint16_t  image_left = 0, image_right = 0, image_top = 0, image_bottom = 0;

    image_left = image_right = Points->X;
    image_top = image_bottom = Points->Y;

    for (counter = 1; counter < PointCount; counter++)
    {
        pixelX = POLY_X(counter);
        if (pixelX < image_left)
        {
            image_left = pixelX;
        }
        if (pixelX > image_right)
        {
            image_right = pixelX;
        }

        pixelY = POLY_Y(counter);
        if (pixelY < image_top)
        {
            image_top = pixelY;
        }
        if (pixelY > image_bottom)
        {
            image_bottom = pixelY;
        }
    }

    if (PointCount < 2)
    {
        return;
    }

    X_center = (image_left + image_right) / 2;
    Y_center = (image_bottom + image_top) / 2;

    X_first = Points->X;
    Y_first = Points->Y;

    while (--PointCount)
    {
        X = Points->X;
        Y = Points->Y;
        Points++;
        X2 = Points->X;
        Y2 = Points->Y;

        FillTriangle(X, X2, X_center, Y, Y2, Y_center);
        FillTriangle(X, X_center, X2, Y, Y_center, Y2);
        FillTriangle(X_center, X2, X, Y_center, Y2, Y);
    }

    FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
    FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
    FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);
}

/**
  * @brief  Draws a full ellipse.
  * @param  Xpos: X position
  * @param  Ypos: Y position
  * @param  XRadius: Ellipse X radius
  * @param  YRadius: Ellipse Y radius
  * @retval None
  */
void LCD::FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius)
{
    int x = 0, y = -YRadius, err = 2 - 2 * XRadius, e2;
    float k = 0, rad1 = 0, rad2 = 0;

    rad1 = XRadius;
    rad2 = YRadius;

    k = (float)(rad2 / rad1);

    do
    {
        LCD::DrawHLine((Xpos - (uint16_t)(x / k)), (Ypos + y), (2 * (uint16_t)(x / k) + 1));
        LCD::DrawHLine((Xpos - (uint16_t)(x / k)), (Ypos - y), (2 * (uint16_t)(x / k) + 1));

        e2 = err;
        if (e2 <= x)
        {
            err += ++x * 2 + 1;
            if (-y == x && e2 <= y) e2 = 0;
        }
        if (e2 > y) err += ++y * 2 + 1;
    } while (y <= 0);
}

/**
  * @brief  Enables the display.
  * @retval None
  */
void LCD::DisplayOn(void)
{
    /* Display On */
    __HAL_LTDC_ENABLE(&hLtdcHandler);
    HAL_GPIO_WritePin(LCD_DISP_GPIO_PORT, LCD_DISP_PIN, GPIO_PIN_SET);        /* Assert LCD_DISP pin */
    HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);  /* Assert LCD_BL_CTRL pin */
}

/**
  * @brief  Disables the display.
  * @retval None
  */
void LCD::DisplayOff(void)
{
    /* Display Off */
    __HAL_LTDC_DISABLE(&hLtdcHandler);
    HAL_GPIO_WritePin(LCD_DISP_GPIO_PORT, LCD_DISP_PIN, GPIO_PIN_RESET);      /* De-assert LCD_DISP pin */
    HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_RESET);/* De-assert LCD_BL_CTRL pin */
}

/**
  * @brief  Initializes the LTDC MSP.
  * @param  hltdc: LTDC handle
  * @param  Params
  * @retval None
  */
__weak void LCD::MspInit(LTDC_HandleTypeDef *hltdc, void *Params)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable the LTDC and DMA2D clocks */
    __HAL_RCC_LTDC_CLK_ENABLE();
    __HAL_RCC_DMA2D_CLK_ENABLE();

    /* Enable GPIOs clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    /**LTDC GPIO Configuration
    PE4     ------> LTDC_B0
    PE5     ------> LTDC_G0
    PE6     ------> LTDC_G1
    PI9     ------> LTDC_VSYNC
    PI10     ------> LTDC_HSYNC
    PF10     ------> LTDC_DE
    PH2     ------> LTDC_R0
    PH3     ------> LTDC_R1
    PH8     ------> LTDC_R2
    PH9     ------> LTDC_R3
    PH10     ------> LTDC_R4
    PH11     ------> LTDC_R5
    PH12     ------> LTDC_R6
    PG6     ------> LTDC_R7
    PG7     ------> LTDC_CLK
    PH13     ------> LTDC_G2
    PH14     ------> LTDC_G3
    PH15     ------> LTDC_G4
    PI0     ------> LTDC_G5
    PI1     ------> LTDC_G6
    PI2     ------> LTDC_G7
    PD6     ------> LTDC_B2
    PG11     ------> LTDC_B3
    PG12     ------> LTDC_B1
    PB8     ------> LTDC_B6
    PB9     ------> LTDC_B7
    PI4     ------> LTDC_B4
    PI5     ------> LTDC_B5
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_1
        | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9
        | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13
        | GPIO_PIN_14 | GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_LTDC;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* LCD_BL_CTRL GPIO configuration */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
}

/**
  * @brief  DeInitializes BSP_LCD MSP.
  * @param  hltdc: LTDC handle
  * @param  Params
  * @retval None
  */
__weak void LCD::MspDeInit(LTDC_HandleTypeDef *hltdc, void *Params)
{

    /* Disable LTDC block */
    __HAL_LTDC_DISABLE(hltdc);

    /**LTDC GPIO Configuration
  PE4     ------> LTDC_B0
  PE5     ------> LTDC_G0
  PE6     ------> LTDC_G1
  PI9     ------> LTDC_VSYNC
  PI10     ------> LTDC_HSYNC
  PF10     ------> LTDC_DE
  PH2     ------> LTDC_R0
  PH3     ------> LTDC_R1
  PH8     ------> LTDC_R2
  PH9     ------> LTDC_R3
  PH10     ------> LTDC_R4
  PH11     ------> LTDC_R5
  PH12     ------> LTDC_R6
  PG6     ------> LTDC_R7
  PG7     ------> LTDC_CLK
  PH13     ------> LTDC_G2
  PH14     ------> LTDC_G3
  PH15     ------> LTDC_G4
  PI0     ------> LTDC_G5
  PI1     ------> LTDC_G6
  PI2     ------> LTDC_G7
  PD6     ------> LTDC_B2
  PG11     ------> LTDC_B3
  PG12     ------> LTDC_B1
  PB8     ------> LTDC_B6
  PB9     ------> LTDC_B7
  PI4     ------> LTDC_B4
  PI5     ------> LTDC_B5
  */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOI, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_0 | GPIO_PIN_1
        | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5);

    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_10);

    HAL_GPIO_DeInit(GPIOH, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9
        | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13
        | GPIO_PIN_14 | GPIO_PIN_15);

    HAL_GPIO_DeInit(GPIOG, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);

    /* Disable LTDC clock */
    __HAL_RCC_LTDC_CLK_DISABLE();

    /* GPIO pins clock can be shut down in the application
       by surcharging this __weak function */
}

/**
  * @brief  Clock Config.
  * @param  hltdc: LTDC handle
  * @param  Params
  * @note   This API is called by LCD::Init()
  *         Being __weak it can be overwritten by the application
  * @retval None
  */
__weak void LCD::ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params)
{
    static RCC_PeriphCLKInitTypeDef  periph_clk_init_struct;

    /* RK043FN48H LCD clock configuration */
    /* PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz */
    /* PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz */
    /* PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz */
    /* LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz */
    periph_clk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    periph_clk_init_struct.PLLSAI.PLLSAIN = 192;
    periph_clk_init_struct.PLLSAI.PLLSAIR = 5;
    periph_clk_init_struct.PLLSAIDivR = RCC_PLLSAIDIVR_4;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
}


/*******************************************************************************
                            Static Functions
*******************************************************************************/

/**
  * @brief  Draws a character on LCD.
  * @param  Xpos: Line where to display the character shape
  * @param  Ypos: Start column address
  * @param  c: Pointer to the character data
  * @retval None
  */
void LCD::DrawChar(uint16_t Xpos, uint16_t Ypos, const uint8_t *c)
{
    uint32_t i = 0, j = 0;
    uint16_t height, width;
    uint8_t  offset;
    uint8_t *pchar;
    uint32_t line;

    height = DrawProp[ActiveLayer].pFont->Height;
    width = DrawProp[ActiveLayer].pFont->Width;

    offset = 8 * ((width + 7) / 8) - width;

    for (i = 0; i < height; i++)
    {
        pchar = ((uint8_t *)c + (width + 7) / 8 * i);

        switch (((width + 7) / 8))
        {

        case 1:
            line = pchar[0];
            break;

        case 2:
            line = (pchar[0] << 8) | pchar[1];
            break;

        case 3:
        default:
            line = (pchar[0] << 16) | (pchar[1] << 8) | pchar[2];
            break;
        }

        for (j = 0; j < width; j++)
        {
            if (line & (1 << (width - j + offset - 1)))
            {
                LCD::DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].TextColor);
            }
            else
            {
                LCD::DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].BackColor);
            }
        }
        Ypos++;
    }
}

/**
  * @brief  Fills a triangle (between 3 points).
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @param  x3: Point 3 X position
  * @param  y3: Point 3 Y position
  * @retval None
  */
void LCD::FillTriangle(uint16_t x1, uint16_t x2, uint16_t x3, uint16_t y1, uint16_t y2, uint16_t y3)
{
    int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
        yinc1 = 0, yinc2 = 0, den = 0, num = 0, num_add = 0, num_pixels = 0,
        curpixel = 0;

    deltax = ABS(x2 - x1);        /* The difference between the x's */
    deltay = ABS(y2 - y1);        /* The difference between the y's */
    x = x1;                       /* Start x off at the first pixel */
    y = y1;                       /* Start y off at the first pixel */

    if (x2 >= x1)                 /* The x-values are increasing */
    {
        xinc1 = 1;
        xinc2 = 1;
    }
    else                          /* The x-values are decreasing */
    {
        xinc1 = -1;
        xinc2 = -1;
    }

    if (y2 >= y1)                 /* The y-values are increasing */
    {
        yinc1 = 1;
        yinc2 = 1;
    }
    else                          /* The y-values are decreasing */
    {
        yinc1 = -1;
        yinc2 = -1;
    }

    if (deltax >= deltay)         /* There is at least one x-value for every y-value */
    {
        xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
        yinc2 = 0;                  /* Don't change the y for every iteration */
        den = deltax;
        num = deltax / 2;
        num_add = deltay;
        num_pixels = deltax;         /* There are more x-values than y-values */
    }
    else                          /* There is at least one y-value for every x-value */
    {
        xinc2 = 0;                  /* Don't change the x for every iteration */
        yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
        den = deltay;
        num = deltay / 2;
        num_add = deltax;
        num_pixels = deltay;         /* There are more y-values than x-values */
    }

    for (curpixel = 0; curpixel <= num_pixels; curpixel++)
    {
        LCD::DrawLine(x, y, x3, y3);

        num += num_add;              /* Increase the numerator by the top of the fraction */
        if (num >= den)             /* Check if numerator >= denominator */
        {
            num -= den;               /* Calculate the new numerator value */
            x += xinc1;               /* Change the x as appropriate */
            y += yinc1;               /* Change the y as appropriate */
        }
        x += xinc2;                 /* Change the x as appropriate */
        y += yinc2;                 /* Change the y as appropriate */
    }
}

/**
  * @brief  Fills a buffer.
  * @param  LayerIndex: Layer index
  * @param  pDst: Pointer to destination buffer
  * @param  xSize: Buffer width
  * @param  ySize: Buffer height
  * @param  OffLine: Offset
  * @param  ColorIndex: Color index
  * @retval None
  */
void LCD::LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
    /* Register to memory mode with ARGB8888 as color Mode */
    hDma2dHandler.Init.Mode = DMA2D_R2M;
    if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
    { /* RGB565 format */
        hDma2dHandler.Init.ColorMode = DMA2D_RGB565;
        ColorIndex = ((ColorIndex & LCD_COLOR_RED) << 8) | ((ColorIndex & LCD_COLOR_GREEN) << 5) | ((ColorIndex & LCD_COLOR_BLUE) << 3);
    }
    else
    { /* ARGB8888 format */
        hDma2dHandler.Init.ColorMode = DMA2D_ARGB8888;
    }
    hDma2dHandler.Init.OutputOffset = OffLine;

    hDma2dHandler.Instance = DMA2D;

    /* DMA2D Initialization */
    if (HAL_DMA2D_Init(&hDma2dHandler) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDma2dHandler, LayerIndex) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDma2dHandler, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
            {
                /* Polling For DMA transfer */
                HAL_DMA2D_PollForTransfer(&hDma2dHandler, 3770);
            }
        }
    }
}

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode
  * @retval None
  */
void LCD::LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode)
{
    /* Configure the DMA2D Mode, Color Mode and output offset */
    hDma2dHandler.Init.Mode = DMA2D_M2M_PFC;
    hDma2dHandler.Init.ColorMode = DMA2D_ARGB8888;
    hDma2dHandler.Init.OutputOffset = 0;

    /* Foreground Configuration */
    hDma2dHandler.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDma2dHandler.LayerCfg[1].InputAlpha = 0xFF;
    hDma2dHandler.LayerCfg[1].InputColorMode = ColorMode;
    hDma2dHandler.LayerCfg[1].InputOffset = 0;

    hDma2dHandler.Instance = DMA2D;

    /* DMA2D Initialization */
    if (HAL_DMA2D_Init(&hDma2dHandler) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDma2dHandler, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDma2dHandler, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
            {
                /* Polling For DMA transfer */
                HAL_DMA2D_PollForTransfer(&hDma2dHandler, 10);
            }
        }
    }
}

/**
  * @}
  */

  /**
    * @}
    */

    /**
      * @}
      */

      /**
        * @}
        */

        /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
