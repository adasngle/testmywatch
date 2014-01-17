/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2001
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *    lcd_sw.h
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   This file is intends for LCD driver.
 *
 * Author:
 * -------
 * -------
 *
 ****************************************************************************/
#include "lcd_if_hw.h"
#include "lcm_if.h"
#include "lcd_if.h"


#ifndef _LCD_SW_H
#define _LCD_SW_H
//#if (defined(MT6226)||defined(MT6227)||defined(MT6228)||defined(MT6229)||defined(MT6230)||defined(MT6235)||defined(MT6238)||defined(MT6268)||defined(MT6236) || defined(MT6236B))

#ifdef LQT_SUPPORT/*Do not remove LQT code segment*/ 
#include "lcd_lqt.h"
extern kal_uint8 lcd_at_mode;
extern kal_bool lcd_update_permission;
#endif/*LQT_SUPPORT*/

extern kal_bool  lcd_assert_fail;
extern kal_uint16 lcd_driver_rotation;
extern kal_uint32 back_potch ;

#ifdef DUAL_LCD
kal_uint16 lcd_power_ctrl_shadow1;
#endif

#if (defined(MT6260)||defined(MT6250))
#define COMBO_LCM_ID_AUTO_DETECT
#endif

#define LCD_CMD_DMA_MODE

#if (defined(MT6256)||defined(MT6255)||defined(MT6250)||defined(MT6260))
#undef LCD_CMD_DMA_MODE
#endif

#define LCD_DUMMYADDR   				0x90000000

#ifdef COMBO_LCM_ID_AUTO_DETECT
  // define ILI9486 interface width
  #define MAIN_LCD_9BIT_MODE_ILI9486  
  
  // define R61529 interface width
  #if ((defined(SERIAL_FLASH_SUPPORT)&&(defined(MT6255)))||defined(MT6250)||defined(MT6260))
  #define MAIN_LCD_8BIT_MODE_R61529  //for serial flash booting up .must use 8bit lcm
  #else
  #define MAIN_LCD_18BIT_MODE_R61529
  #endif
#else
  #if ((defined(SERIAL_FLASH_SUPPORT)&&(defined(MT6255)))||defined(MT6250)||defined(MT6260))
  #define MAIN_LCD_9BIT_MODE		//for serial flash booting up .must use 8bit lcm
  #else
  #define MAIN_LCD_18BIT_MODE
  #endif
#endif

#define MAIN_LCD_CMD_ADDR               LCD_PARALLEL0_A0_LOW_ADDR
#define MAIN_LCD_DATA_ADDR              LCD_PARALLEL0_A0_HIGH_ADDR

#ifdef COMBO_LCM_ID_AUTO_DETECT
  // define ILI9486 output color format
  #if (defined(MMI_MAIN_LCD_DEFAULT_FORMAT) && (MMI_MAIN_LCD_DEFAULT_FORMAT == 24))
      #if (defined(MAIN_LCD_8BIT_MODE_ILI9486))
          #define MAIN_LCD_OUTPUT_FORMAT_ILI9486  LCM_8BIT_24_BPP_RGB888_1
      #elif (defined(MAIN_LCD_9BIT_MODE_ILI9486))
          #define MAIN_LCD_OUTPUT_FORMAT_ILI9486  LCM_9BIT_18_BPP_RGB666_1
      #elif (defined(MAIN_LCD_16BIT_MODE_ILI9486))
          #define MAIN_LCD_OUTPUT_FORMAT_ILI9486  LCM_16BIT_24_BPP_RGB888_1
      #elif (defined(MAIN_LCD_18BIT_MODE_ILI9486))
          #define MAIN_LCD_OUTPUT_FORMAT_ILI9486  LCM_18BIT_18_BPP_RGB666_1
      #endif    
  #else   // if MMI_MAIN_LCD_DEFAULT_FORMAT == DEFAULT
      #if (defined(MAIN_LCD_8BIT_MODE_ILI9486))
          #define MAIN_LCD_OUTPUT_FORMAT_ILI9486  LCM_8BIT_18_BPP_RGB666_1
      #elif (defined(MAIN_LCD_9BIT_MODE_ILI9486))
          #define MAIN_LCD_OUTPUT_FORMAT_ILI9486  LCM_9BIT_18_BPP_RGB666_1
      #elif (defined(MAIN_LCD_16BIT_MODE_ILI9486))
          #define MAIN_LCD_OUTPUT_FORMAT_ILI9486  LCM_16BIT_16_BPP_RGB565_1
      #elif (defined(MAIN_LCD_18BIT_MODE_ILI9486))
          #define MAIN_LCD_OUTPUT_FORMAT_ILI9486  LCM_18BIT_18_BPP_RGB666_1//LCM_18BIT_24_BPP_RGB888_1//////
      #endif
  #endif
  
  // define R61529 output color format
  #if (defined(MMI_MAIN_LCD_DEFAULT_FORMAT) && (MMI_MAIN_LCD_DEFAULT_FORMAT == 24))
      #if (defined(MAIN_LCD_8BIT_MODE_R61529))
          #define MAIN_LCD_OUTPUT_FORMAT_R61529  LCM_8BIT_24_BPP_RGB888_1
      #elif (defined(MAIN_LCD_9BIT_MODE_R61529))
          #define MAIN_LCD_OUTPUT_FORMAT_R61529  LCM_9BIT_18_BPP_RGB666_1
      #elif (defined(MAIN_LCD_16BIT_MODE_R61529))
          #define MAIN_LCD_OUTPUT_FORMAT_R61529  LCM_16BIT_24_BPP_RGB888_1
      #elif (defined(MAIN_LCD_18BIT_MODE_R61529))
          #define MAIN_LCD_OUTPUT_FORMAT_R61529  LCM_18BIT_18_BPP_RGB666_1
      #endif    
  #else   // if MMI_MAIN_LCD_DEFAULT_FORMAT == DEFAULT
      #if (defined(MAIN_LCD_8BIT_MODE_R61529))
          #define MAIN_LCD_OUTPUT_FORMAT_R61529  LCM_8BIT_18_BPP_RGB666_1
      #elif (defined(MAIN_LCD_9BIT_MODE_R61529))
          #define MAIN_LCD_OUTPUT_FORMAT_R61529  LCM_9BIT_18_BPP_RGB666_1
      #elif (defined(MAIN_LCD_16BIT_MODE_R61529))
          #define MAIN_LCD_OUTPUT_FORMAT_R61529  LCM_16BIT_16_BPP_RGB565_1
      #elif (defined(MAIN_LCD_18BIT_MODE_R61529))
          #define MAIN_LCD_OUTPUT_FORMAT_R61529  LCM_18BIT_18_BPP_RGB666_1//LCM_18BIT_24_BPP_RGB888_1//////
      #endif
  #endif
#else
  #if (defined(MMI_MAIN_LCD_DEFAULT_FORMAT) && (MMI_MAIN_LCD_DEFAULT_FORMAT == 24))
      #if (defined(MAIN_LCD_8BIT_MODE))
          #define MAIN_LCD_OUTPUT_FORMAT  LCM_8BIT_24_BPP_RGB888_1
      #elif (defined(MAIN_LCD_9BIT_MODE))
          #define MAIN_LCD_OUTPUT_FORMAT  LCM_9BIT_18_BPP_RGB666_1
      #elif (defined(MAIN_LCD_16BIT_MODE))
          #define MAIN_LCD_OUTPUT_FORMAT  LCM_16BIT_24_BPP_RGB888_1
      #elif (defined(MAIN_LCD_18BIT_MODE))
          #define MAIN_LCD_OUTPUT_FORMAT  LCM_18BIT_18_BPP_RGB666_1
      #endif    
  #else   // if MMI_MAIN_LCD_DEFAULT_FORMAT == DEFAULT
      #if (defined(MAIN_LCD_8BIT_MODE))
          #define MAIN_LCD_OUTPUT_FORMAT  LCM_8BIT_18_BPP_RGB666_1        
      #elif (defined(MAIN_LCD_9BIT_MODE))
          #define MAIN_LCD_OUTPUT_FORMAT  LCM_9BIT_18_BPP_RGB666_1
      #elif (defined(MAIN_LCD_16BIT_MODE))
          #define MAIN_LCD_OUTPUT_FORMAT  LCM_16BIT_16_BPP_RGB565_1
      #elif (defined(MAIN_LCD_18BIT_MODE))
          #define MAIN_LCD_OUTPUT_FORMAT  LCM_18BIT_18_BPP_RGB666_1//LCM_18BIT_24_BPP_RGB888_1//////
      #endif
  #endif
#endif

    // define main supported combo lcm ID
    typedef enum
    {
       ILI9486  = 0,
       R61529  = 1,
       MAIN_LCM_MAX_NUM  //please add main lcd ID above this item, this "MAIN_LCM_MAX_NUM" will be used as the number of LCMs.
    }  main_lcm_enum;

#ifdef DUAL_LCD
    // define sub supported combo lcm ID
    typedef enum
    {
       SUB_EXAMPLE_LCM_A  = 0,
       SUB_EXAMPLE_LCM_B  = 1,
       SUB_EXAMPLE_LCM_C  = 2,
       SUB_LCM_MAX_NUM  //please add sub lcd ID above this item, this "SUB_LCM_MAX_NUM" will be used as the number of LCMs.
    }  sub_lcm_enum;    //notice, the enum value should not be conflict with main lcd enum.
#endif

#ifdef COMBO_LCM_ID_AUTO_DETECT
    kal_uint32 LCM_ID_Get(void);

    // MAIN_LCD_OUTPUT_FORMAT will be chosen as current used LCM's output color format
    #define MAIN_LCD_OUTPUT_FORMAT  ((LCM_ID_Get() == 0)? MAIN_LCD_OUTPUT_FORMAT_ILI9486 : MAIN_LCD_OUTPUT_FORMAT_R61529)
    #define DEFAULT_MAIN_LCD_OUTPUT_FORMAT MAIN_LCD_OUTPUT_FORMAT_ILI9486
#endif

//#endif
/***********************TA7601********************************/
/*RENESAS, 262K Color LCD, Parallel Interface,*/
#ifdef LCD_CMD_DMA_MODE
#define LCD_SEND_DMA_CMD(n) \
	{\
    volatile kal_int32 i;\
		while (LCD_IS_RUNNING) {};\
		DISABLE_LCD_TRANSFER_COMPLETE_INT;\
		DISABLE_ALL_LCD_LAYER_WINDOW;\
		SET_LCD_ROI_CTRL_NUMBER_OF_CMD(n);\
		ENABLE_LCD_ROI_CTRL_CMD_FIRST;\
		SET_LCD_ROI_WINDOW_SIZE(0,0);\
		START_LCD_TRANSFER;\
    	for(i=0;i<50;i++){};\
		while (LCD_IS_RUNNING) {};\
	}
#endif


/***********************HD66791********************************/
/*RENESAS, 262K Color LCD, Parallel Interface,*/
#endif  /*_LCD_SW_H*/
