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
 *    lcd.c
 *
 * Project:
 * --------
 *   Maui_Software
 *
 * Description:
 * ------------
 *   This Module defines the LCD driver.
 *
 * Author:
 * -------
 * -------
 *
 ****************************************************************************/
#ifdef FPGA
#include "lcd_feature.h" 
#include "typedefs.h"
#include "dma_maui_api.h"

#include "hw_reg.h"
#include "lcm_if.h"
#include "lcd_if.h"
#include "lcd_sw_inc.h"
#include "lcd_sw.h"
#include "lcd_hw.h"
#pragma arm section code="SDRAMCODE"

#else
#include "drv_comm.h"
#include "reg_base.h"
#include "lcd_sw_inc.h"
#include "lcd_sw.h"
#include "lcd_hw.h"
/*Serial interface*/
#include "lcd_if_hw.h"
#include "lcm_if.h"
#include "lcd_if.h"
#include "eint.h"
#endif

#if (defined(MAIN_LCD_8BIT_MODE_R61529))
    #define MAIN_LCD_8BIT_MODE
#elif (defined(MAIN_LCD_9BIT_MODE_R61529))
    #define MAIN_LCD_9BIT_MODE
#elif (defined(MAIN_LCD_16BIT_MODE_R61529))
    #define MAIN_LCD_16BIT_MODE
#elif (defined(MAIN_LCD_18BIT_MODE_R61529))
    #define MAIN_LCD_18BIT_MODE
#endif

#ifdef LCD_CMD_DMA_MODE
	#define LCD_delay_R61529()
	
	#if (defined(MAIN_LCD_8BIT_MODE))
		#define LCD_CtrlWrite_R61529(_data) \
		{\
			SET_LCD_CMD_PARAMETER(0,LCD_CMD,(_data));\
			LCD_SEND_DMA_CMD(1);\
		}\

		#define LCD_DataWrite_R61529(_data) \
		{\
			SET_LCD_CMD_PARAMETER(0,LCD_DATA,(_data));\
			LCD_SEND_DMA_CMD(1);\
		}
	#elif (defined(MAIN_LCD_9BIT_MODE))
		
	#elif (defined(MAIN_LCD_16BIT_MODE))
		
	#elif (defined(MAIN_LCD_18BIT_MODE))
		#define LCD_CtrlWrite_R61529(_data) \
		{\
			SET_LCD_CMD_PARAMETER(0,LCD_CMD,(_data));\
			LCD_SEND_DMA_CMD(1);\
		}\

		#define LCD_DataWrite_R61529(_data) \
		{\
			SET_LCD_CMD_PARAMETER(0,LCD_DATA,(_data));\
			LCD_SEND_DMA_CMD(1);\
		}
	#endif
#else
	#define LCD_delay_R61529()

	#if (defined(MAIN_LCD_8BIT_MODE))
	
		#define LCD_CtrlWrite_R61529(_data)  \
		{\
			*(volatile kal_uint8 *) MAIN_LCD_CMD_ADDR= (_data);\
			LCD_delay_R61529();\
		}\
		
		#define LCD_DataWrite_R61529(_data)  \
		{\
			*(volatile kal_uint8 *) MAIN_LCD_DATA_ADDR= (_data);\
			LCD_delay_R61529();\
		}
	#elif (defined(MAIN_LCD_9BIT_MODE))
	
	#elif (defined(MAIN_LCD_16BIT_MODE))
		
	#elif (defined(MAIN_LCD_18BIT_MODE))
		#define LCD_CtrlWrite_R61529(_data)  \
		{\
			*(volatile kal_uint16 *) MAIN_LCD_CMD_ADDR= (_data);\
			LCD_delay_R61529();\
		}\

		#define LCD_DataWrite_R61529(_data)  \
		{\
			*(volatile kal_uint16 *) MAIN_LCD_DATA_ADDR= (_data);\
			LCD_delay_R61529();\
		}
	#endif
#endif /* LCD_CMD_DMA_MODE */

#if (defined(MAIN_LCD_8BIT_MODE))
	#define LCD_DataRead_R61529	(*(volatile kal_uint8 *)MAIN_LCD_DATA_ADDR);
#elif (defined(MAIN_LCD_18BIT_MODE))
	#define LCD_DataRead_R61529 (*(volatile kal_uint32 *)MAIN_LCD_DATA_ADDR);
#endif



extern void LCD_BlockWrite_R61529(kal_uint16 startx,kal_uint16 starty,kal_uint16 endx,kal_uint16 endy);



/* Debug use only. */
//#define __R61529_DEBUG_TRACE__


static void LCD_Delay(kal_uint32 delay_count)
{
	volatile kal_uint32 delay;
	for (delay =0;delay <(delay_count*4);delay++) {}
}

static void Delayms(kal_uint16 data)
{
#if defined(__UBL__)
	kal_uint32 time1=0, diff=0;
	
	time1=drv_get_current_time();

	while(drv_get_duration_ms(time1) < data);
#else
	kal_uint32 i;
	while(data--)
	{
		for(i=0;i<220000;i++){}// 1ms=220000 TIMES	
	}
#endif
}
void LCD_set_reg_R61529(kal_uint16 reg_index, kal_uint16 reg_data)
{

	LCD_CtrlWrite_R61529(reg_index);
	if (reg_data==0xFFFF){
  //no register setting
  }else{
  LCD_DataWrite_R61529(reg_data);
  }
}

void spi_ini_R61529(void)
{
	#if (defined(MT6218B) || defined(MT6219) || defined(MT6217)||defined(MT6226)||defined(MT6227)||defined(MT6228)||defined(MT6229)||defined(MT6230)||defined(MT6235)||defined(MT6238)||defined(MT6268)||defined(MT6236)||defined(MT6236B))
		REG_LCD_SERIAL_CONFIG = (LCD_SERIAL_CONFIG_CLOCK_PHASE_BIT | LCD_SERIAL_CONFIG_CLOCK_POLARITY_BIT);
	#elif (defined(MT6225)||defined(MT6276))
	//format SET_LCD_SERIAL_IF_ATTRB(spo, sph, _3wire, csp0, csp1);
	//SET_LCD_SERIAL_IF_ATTRB(1, 1, 1, 0, 0);
	//format SET_LCD_SERIAL_IF_TIMING(if_num, clk, div, period);
	//SET_LCD_SERIAL_IF_TIMING(0, LCD_SERIAL_CONFIG_104MHZ_CLK, 1, 0);
	#endif	/* MT6218B, MT6219 */
}

void init_lcd_interface_R61529(void)
{	
kal_uint32 backup_val;
#ifndef FPGA
  /// Turn on LCD clock
 #if defined(__OLD_PDN_ARCH__)     
   {
    #if defined(__OLD_PDN_DEFINE__)
    DRV_Reg(DRVPDN_CON1_CLR) = DRVPDN_CON1_LCD;
    #elif defined(__CLKG_DEFINE__)
    //modifed for new clock gating
    DRV_Reg(MMCG_CLR0) = MMCG_CON0_LCD;
    #endif   //defined(__OLD_PDN_DEFINE__)
   }	   
 #else //defined(__OLD_PDN_ARCH__) 
      PDN_CLR(PDN_LCD);
 #endif //defined(__OLD_PDN_ARCH__) 
#endif //FPGA

	  REG_LCD_ROI_CTRL=0;
#if (defined(MT6226)||defined(MT6227)||defined(MT6228)||defined(MT6229)||defined(MT6230)||defined(MT6235)||defined(MT6238)||defined(MT6268) || defined(MT6236) || defined(MT6236B)) 
   	SET_LCD_PARALLEL_CE2WR_SETUP_TIME((kal_uint32)1);//111
   	SET_LCD_PARALLEL_CE2WR_HOLD_TIME(1);//1111
   	SET_LCD_PARALLEL_CE2RD_SETUP_TIME(3);//3333
   	SET_LCD_PARALLEL_WRITE_WAIT_STATE(2);//5555
   	SET_LCD_PARALLEL_READ_LATENCY_TIME(15);//1515
   	SET_LCD_ROI_CTRL_CMD_LATENCY(0);//000

	  #if (defined(MAIN_LCD_8BIT_MODE))
   		SET_LCD_PARALLEL_8BIT_DATA_BUS;
   	#elif (defined(MAIN_LCD_9BIT_MODE))
   	  SET_LCD_PARALLEL_9BIT_DATA_BUS;
   	#elif (defined(MAIN_LCD_16BIT_MODE))
   	  SET_LCD_PARALLEL_16BIT_DATA_BUS;
   	#elif (defined(MAIN_LCD_18BIT_MODE))
			SET_LCD_PARALLEL_18BIT_DATA_BUS;
					#if (defined(MT6235)||defined(MT6238))
			    //GPIO_ModeSetup(20, 1);
			    //GPIO_ModeSetup(21, 1);
			    #elif (defined(MT6268))
			    //GPIO_ModeSetup(68, 1);
			    //GPIO_ModeSetup(69, 1);
			    #elif (defined(MT6236) || defined(MT6236B)) 
            #ifndef FPGA	
			      //GPIO_ModeSetup(35, 1);
			      //GPIO_ModeSetup(36, 1);
			      //GPIO_ModeSetup(37, 1);
			      //GPIO_ModeSetup(38, 1);
            //Set Driving current here:
            *(volatile kal_uint32 *)0x80000500 = 0x90888888;
            *(volatile kal_uint32 *)0x80000504 = 0x88888880;
            #endif //FPGA	
	 		    #endif //(defined(MT6236)) 
		#endif   	
   	
 #if (defined(MT6236) || defined(MT6236B)) 
      SET_LCD_PARALLEL0_CLOCK_104M;
 #else
      SET_LCD_PARALLEL_CLOCK_26M;
 #endif //(defined(MT6236))    

	#if (defined(MT6226)||defined(MT6227)||defined(MT6229)||defined(MT6230)||defined(MT6235)||defined(MT6238)||defined(MT6268)||defined(MT6236))
			SET_LCD_PARALLEL_GAMMA_R_TABLE(LCD_PARALLEL_GAMMA_DISABLE);
			SET_LCD_PARALLEL_GAMMA_G_TABLE(LCD_PARALLEL_GAMMA_DISABLE);
			SET_LCD_PARALLEL_GAMMA_B_TABLE(LCD_PARALLEL_GAMMA_DISABLE);
	#endif   	
      set_lcd_driving_current(LCD_DRIVING_8MA);//haiyong add
    	
#elif (defined(MT6225)||defined(MT6276))
    //format SET_LCD_PARALLEL_IF_TIMING(if_num, clk, Whold, Wsetup, Wwait_state, Rhold, Rsetup, Rlatency, period)
  //SET_LCD_PARALLEL_IF_TIMING(0,0,2,2,12,2,2,6,0);//care write cycle(write register) 100ns
  //SET_LCD_PARALLEL_IF_TIMING(0,0,1,2,3,2,2,6,0); //don't care write cycle(write register) 100ns
  	SET_LCD_PARALLEL_IF_TIMING(0,0,0,1,2,2,2,6,0);	 //don't care write cycle(write register) 100ns
    #if (defined(MAIN_LCD_8BIT_MODE))
    SET_LCD_PARALLEL_DATA_BUS(0, LCD_PARALLEL_BUS_WIDTH_8BIT);//SET_LCD_PARALLEL_8BIT_DATA_BUS;
    #elif (defined(MAIN_LCD_9BIT_MODE))
    SET_LCD_PARALLEL_DATA_BUS(0, LCD_PARALLEL_BUS_WIDTH_9BIT);//SET_LCD_PARALLEL_9BIT_DATA_BUS;
    #elif (defined(MAIN_LCD_16BIT_MODE))
    SET_LCD_PARALLEL_DATA_BUS(0, LCD_PARALLEL_BUS_WIDTH_16BIT);//SET_LCD_PARALLEL_16BIT_DATA_BUS;            
    #elif (defined(MAIN_LCD_18BIT_MODE))
    SET_LCD_PARALLEL_DATA_BUS(0, LCD_PARALLEL_BUS_WIDTH_18BIT);//SET_LCD_PARALLEL_18BIT_DATA_BUS;
    #endif	
    
    DISABLE_LCD_PARALLEL0_GAMMA(backup_val);
	
    #if (defined(MT6225))
      set_lcd_driving_current(LCD_DRIVING_4MA);//Sophie modified for MT6225 boot-up
    #endif    

#elif (defined(MT6256)||defined(MT6255)||defined(MT6250)||defined(MT6260))
    //format SET_LCD_PARALLEL_IF_TIMING(if_num, clk, Whold, Wsetup, Wwait_state, Rhold, Rsetup, Rlatency, period)
  	SET_LCD_PARALLEL_IF_TIMING(0,0,3,0,3,15,1,18,0);	 //temp for MT6256E2 bring up
  	//SET_LCD_PARALLEL_IF_TIMING(0,0,0,0,1,15,1,18,2);	 //temp for MT6256E2 bring up
    #if (defined(MAIN_LCD_8BIT_MODE))
    SET_LCD_PARALLEL_DATA_BUS(0, LCD_PARALLEL_BUS_WIDTH_8BIT);//SET_LCD_PARALLEL_8BIT_DATA_BUS;
    #elif (defined(MAIN_LCD_9BIT_MODE))
    SET_LCD_PARALLEL_DATA_BUS(0, LCD_PARALLEL_BUS_WIDTH_9BIT);//SET_LCD_PARALLEL_9BIT_DATA_BUS;
    #elif (defined(MAIN_LCD_16BIT_MODE))
    SET_LCD_PARALLEL_DATA_BUS(0, LCD_PARALLEL_BUS_WIDTH_16BIT);//SET_LCD_PARALLEL_16BIT_DATA_BUS;            
    #elif (defined(MAIN_LCD_18BIT_MODE))
    SET_LCD_PARALLEL_DATA_BUS(0, LCD_PARALLEL_BUS_WIDTH_18BIT);//SET_LCD_PARALLEL_18BIT_DATA_BUS;
    #endif	
    set_lcd_driving_current(LCD_DRIVING_8MA);
    DISABLE_LCD_PARALLEL0_GAMMA(backup_val);      
    	   
#endif//#if (defined(MT6226)||defined(MT6227)||defined(MT6228)||defined(MT6229)||defined(MT6230)||defined(MT6235)||defined(MT6238)||defined(MT6268) || defined(MT6236) || defined(MT6236B)) 

}	/* init_lcd_interface() */


void CABC_function_R61529(kal_uint16 mode) 
{
return;
	switch(mode){
	case 0:
		//CABC Mode off Setting 
		LCD_CtrlWrite_R61529(0x55); //Set CABC Mode off 
		LCD_DataWrite_R61529(0x00); 
		break;
		
	case 1:
		//CABC UI Mode Setting 
		LCD_CtrlWrite_R61529(0x51); //DBV[7:0]=0xFF 
		LCD_DataWrite_R61529(0xFF); 
		LCD_CtrlWrite_R61529(0x53); //BCTRL=1, BL=1 
		LCD_DataWrite_R61529(0x24); 
		LCD_CtrlWrite_R61529(0x55); //Set CABC UI Mode 
		LCD_DataWrite_R61529(0x01); 	
		break;
		
	case 2:
		//CABC Still_mode Setting 
		LCD_CtrlWrite_R61529(0x51); //DBV[7:0]=0xFF 
		LCD_DataWrite_R61529(0xFF); 
		LCD_CtrlWrite_R61529(0x53); //BCTRL=1, BL=1 
		LCD_DataWrite_R61529(0x24); 
		LCD_CtrlWrite_R61529(0x55); //Set CABC Still Mode 
		LCD_DataWrite_R61529(0x02); 	
		break;
		
	case 3:
		//CABC Moving_mode Setting 
		LCD_CtrlWrite_R61529(0x51); //DBV[7:0]=0xFF 
		LCD_DataWrite_R61529(0xFF); 
		LCD_CtrlWrite_R61529(0x53); //BCTRL=1, BL=1 
		LCD_DataWrite_R61529(0x24); 
		LCD_CtrlWrite_R61529(0x55); //Set CABC Moving Mode 
		LCD_DataWrite_R61529(0x03); 	
		break;
	}

}


kal_uint32 test_potch = 0;

void LCD_EnterSleep_R61529(void)
{
	#if defined(LQT_SUPPORT)/*Please don't remove LQT code segments*/

    if(!(lcd_at_mode==LCD_AT_RELEASE_MODE))
    {
    	return;
    }	
    #endif /*defined(LQT_SUPPORT))*/	

	LCD_CtrlWrite_R61529(0x0010); 
    Delayms(90);
#ifdef __R61529_DEBUG_TRACE__
	kal_prompt_trace(MOD_ENG,"EnterSleep");
#endif
}


void LCD_ExitSleep_R61529(void)
{
	#if defined(LQT_SUPPORT)/*Please don't remove LQT code segments*/

    if(!(lcd_at_mode==LCD_AT_RELEASE_MODE))
    {
    	return;
    }	
  	#endif /*defined(LQT_SUPPORT))*/	
	LCD_CtrlWrite_R61529(0x0011);
	Delayms(120);

#ifdef __R61529_DEBUG_TRACE__
	kal_prompt_trace(MOD_ENG,"ExitSleep");
#endif

}

void LCD_Partial_On_R61529(kal_uint16 start_page,kal_uint16 end_page)
{

}

void LCD_Partial_Off_R61529(void)
{

}

kal_uint8 LCD_Partial_line_R61529(void)
{
	return 1;		/* partial display in 1 line alignment */
}

void LCD_blockClear_R61529(kal_uint16 x1,kal_uint16 y1,kal_uint16 x2,kal_uint16 y2,kal_uint16 data)
{
	kal_uint16 LCD_x;
	kal_uint16 LCD_y;
	kal_uint8 r_color,g_color,b_color;

	r_color=(data&0xF800)>>10;	/* transfer to RGB666 */
	g_color=(data>>5)&0x3F;
	b_color=((data)&0x1F)<<1;
	
	LCD_CtrlWrite_R61529(0x002a);
	LCD_DataWrite_R61529((x1&0xff00)>>8);//start x
	LCD_DataWrite_R61529(x1&0x00ff);
	
	LCD_DataWrite_R61529((x2&0xff00)>>8);//end x
	LCD_DataWrite_R61529(x2&0x00ff);

	LCD_CtrlWrite_R61529(0x002b);
	LCD_DataWrite_R61529((y1&0xff00)>>8);//start x
	LCD_DataWrite_R61529(y1&0x00ff);
	
	LCD_DataWrite_R61529((y2&0xff00)>>8);//end x
	LCD_DataWrite_R61529(y2&0x00ff);
	

	LCD_CtrlWrite_R61529(0x002c);//transfor begin
	for(LCD_y=y1;LCD_y<=y2;LCD_y++)
	{
		for(LCD_x=x1;LCD_x<=x2;LCD_x++)
		{
		#if (defined(MAIN_LCD_8BIT_MODE))
			*((volatile unsigned char *) MAIN_LCD_DATA_ADDR)=(kal_uint8)((data&0xFF00)>>8);
			*((volatile unsigned char *) MAIN_LCD_DATA_ADDR)=(kal_uint8)(data&0xFF);			
		#elif (defined(MAIN_LCD_9BIT_MODE))
			*((volatile unsigned short *) MAIN_LCD_DATA_ADDR)=((r_color<<6)|(g_color>>3)<<9);
			*((volatile unsigned short *) MAIN_LCD_DATA_ADDR)=((((g_color&0x07)<<6)|b_color)<<9);		
		#elif (defined(MAIN_LCD_16BIT_MODE) || defined(MAIN_LCD_18BIT_MODE))
			*((volatile unsigned int *) MAIN_LCD_DATA_ADDR)=(r_color<<12)|(g_color<<6)|b_color;
		#endif
		}
	}
}

void LCD_ClearAll_R61529(kal_uint16 data)
{
   LCD_blockClear_R61529(0,0,LCD_WIDTH-1,LCD_HEIGHT-1,data);
   //LCD_blockClear_R61529(0,0,360-1,640-1,data);
}
//////////////haiyong add for lcd porting
void LCD_Read_GRAM_R61529(kal_uint16 startx,kal_uint16 starty,kal_uint16 endx,kal_uint16 endy)
{
	kal_uint32 R61529_GRAM_DATA;
	kal_uint16 LCD_x,LCD_y;


	LCD_CtrlWrite_R61529(0x002a);
	LCD_DataWrite_R61529((startx&0xff00)>>8);//start x
	LCD_DataWrite_R61529(startx&0x00ff);
	
	LCD_DataWrite_R61529((endx&0xff00)>>8);//end x
	LCD_DataWrite_R61529(endx&0x00ff);
	
	LCD_CtrlWrite_R61529(0x002b);
	LCD_DataWrite_R61529((starty&0xff00)>>8);//start x
	LCD_DataWrite_R61529(starty&0x00ff);
	
	LCD_DataWrite_R61529((endy&0xff00)>>8);//end x
	LCD_DataWrite_R61529(endy&0x00ff);

#ifdef __R61529_DEBUG_TRACE__
	kal_prompt_trace(MOD_ENG,"startx starty endx endy= %d, %d, %d, %d",startx,starty,endx,endy);
#endif
	LCD_CtrlWrite_R61529(0x002E);
	R61529_GRAM_DATA=LCD_DataRead_R61529;
	Delayms(20);

	for(LCD_y=starty;LCD_y<=endy;LCD_y++)
	{
		for(LCD_x=startx;LCD_x<=endx;LCD_x++)
		{
			R61529_GRAM_DATA=LCD_DataRead_R61529;
#ifdef __R61529_DEBUG_TRACE__
			kal_prompt_trace(MOD_ENG,"R61529_GRAM_DATA = %x",R61529_GRAM_DATA);
#endif
		}
	}
}
void LCD_Read_ID_R61529(void)
{
	kal_uint32 R61529_ID2;
	LCD_CtrlWrite_R61529(0x00DB);
	//LCD_DataWrite_R61529(0x0000);//dummy data
	//LCD_DataWrite_R61529(0x0088);//read ID2
	R61529_ID2=LCD_DataRead_R61529;
	R61529_ID2=LCD_DataRead_R61529;
#ifdef __R61529_DEBUG_TRACE__
	kal_prompt_trace(MOD_ENG,"R61529_ID2 = %d",R61529_ID2);
#endif
}


void LCD_Init_R61529(kal_uint32 bkground, void **buf_addr)
{


 SET_LCD_CTRL_RESET_PIN;//set reset pin high
 Delayms(10);
 CLEAR_LCD_CTRL_RESET_PIN;//set reset pin low
 Delayms(5);
 SET_LCD_CTRL_RESET_PIN;//set reset pin high
 Delayms(10);


 LCD_CtrlWrite_R61529(0x01);  //SOFT_RESET
 LCD_CtrlWrite_R61529(0x00); 
 Delayms(2);

 LCD_CtrlWrite_R61529(0xb0);   
 LCD_DataWrite_R61529(0x04);
 
 LCD_CtrlWrite_R61529(0x13);  // nomal mode

 LCD_CtrlWrite_R61529(0xB3);   //Frame Memory Access and Interface Setting
 LCD_DataWrite_R61529(0x02);  //WEMODE  0002
 LCD_DataWrite_R61529(0x00);  //TEON  TEI[2:0]
 LCD_DataWrite_R61529(0x00);  //
 LCD_DataWrite_R61529(0x00);  //EPF[1:0], DFM    ( fixel format option choice)

 LCD_CtrlWrite_R61529(0xB4);   //Display Mode and Frame Memory Write Mode Setting
 LCD_DataWrite_R61529(0x00);
 
//////////////////////Power Set/////////////////////////////
 LCD_CtrlWrite_R61529(0x11);   //Exit_Sleep_mode
 Delayms(2);
 
 LCD_CtrlWrite_R61529(0x0A);   
 LCD_DataWrite_R61529(0x0c);  // get_power mode
 
 LCD_CtrlWrite_R61529(0xC0);   //Panel Driving Setting
 LCD_DataWrite_R61529(0x03);//13  //REV, SM, GS, BGR, SS
 LCD_DataWrite_R61529(0xdf);//DF  //NL[7:0],63h:400Lines Type A, E3h:400Lines Type B
 LCD_DataWrite_R61529(0x40);  //NL[8], SCN[6:0]
 LCD_DataWrite_R61529(0x00);  //BLV, PTV => DOT / Column Inv Selection
 LCD_DataWrite_R61529(0x10);  //NDL PTDC   ??
 LCD_DataWrite_R61529(0x13);  //PTG,ISC[3:0]
 LCD_DataWrite_R61529(0x00);  //BLS
 LCD_DataWrite_R61529(0x44);  //PCDIVH[3:0], PCDIVL[3:0]

 LCD_CtrlWrite_R61529(0xC1);   //Display Timing Setting for Normal  Mode
 LCD_DataWrite_R61529(0x07);  //DIV[3:0]  
 LCD_DataWrite_R61529(0x24);  //RTN[5:0]   
 LCD_DataWrite_R61529(0x04);  //BP[7:0]  03
 LCD_DataWrite_R61529(0x04);  //FP[7:0]  03
 LCD_DataWrite_R61529(0x10);  //2-Dot inversion
 
 LCD_CtrlWrite_R61529(0xC4);   //Source/Gate Driving Timing Setting
 LCD_DataWrite_R61529(0x70);  //NOWB[2:0], NOW[2:0]   0020
 LCD_DataWrite_R61529(0x00);  //
 LCD_DataWrite_R61529(0x03);  //SEQGND[3:0]   03
 LCD_DataWrite_R61529(0x01);  //SEQVCIL[3:0] 

 LCD_CtrlWrite_R61529(0xC8);   //Gamma Set A
 LCD_DataWrite_R61529(0x00);  
 LCD_DataWrite_R61529(0x0E);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x25);
 LCD_DataWrite_R61529(0x31);
 LCD_DataWrite_R61529(0x4a);
 LCD_DataWrite_R61529(0x3c);
 LCD_DataWrite_R61529(0x23);
 LCD_DataWrite_R61529(0x19);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x0d);  
 LCD_DataWrite_R61529(0x05);
 LCD_DataWrite_R61529(0x00);  
 LCD_DataWrite_R61529(0x0E);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x25);
 LCD_DataWrite_R61529(0x31);
 LCD_DataWrite_R61529(0x4A);
 LCD_DataWrite_R61529(0x3C);
 LCD_DataWrite_R61529(0x23);
 LCD_DataWrite_R61529(0x19);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x0D); 
 LCD_DataWrite_R61529(0x05);
  
 LCD_CtrlWrite_R61529(0xC9);   //Gamma Set B
 LCD_DataWrite_R61529(0x00);  
 LCD_DataWrite_R61529(0x0E);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x25);
 LCD_DataWrite_R61529(0x31);
 LCD_DataWrite_R61529(0x4a);
 LCD_DataWrite_R61529(0x3c);
 LCD_DataWrite_R61529(0x23);
 LCD_DataWrite_R61529(0x19);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x0d);  
 LCD_DataWrite_R61529(0x05);
 LCD_DataWrite_R61529(0x00);  
 LCD_DataWrite_R61529(0x0E);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x25);
 LCD_DataWrite_R61529(0x31);
 LCD_DataWrite_R61529(0x4A);
 LCD_DataWrite_R61529(0x3C);
 LCD_DataWrite_R61529(0x23);
 LCD_DataWrite_R61529(0x19);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x0D); 
 LCD_DataWrite_R61529(0x05);
 
 LCD_CtrlWrite_R61529(0xCA);   //Gamma Set C
 LCD_DataWrite_R61529(0x00);  
 LCD_DataWrite_R61529(0x0e);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x25);
 LCD_DataWrite_R61529(0x31);
 LCD_DataWrite_R61529(0x4a);
 LCD_DataWrite_R61529(0x3c);
 LCD_DataWrite_R61529(0x23);
 LCD_DataWrite_R61529(0x19);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x0d);  
 LCD_DataWrite_R61529(0x05);
 LCD_DataWrite_R61529(0x00);  
 LCD_DataWrite_R61529(0x0E);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x25);
 LCD_DataWrite_R61529(0x31);
 LCD_DataWrite_R61529(0x4A);
 LCD_DataWrite_R61529(0x3C);
 LCD_DataWrite_R61529(0x23);
 LCD_DataWrite_R61529(0x19);
 LCD_DataWrite_R61529(0x14);
 LCD_DataWrite_R61529(0x0D); 
 LCD_DataWrite_R61529(0x05);
 
 LCD_CtrlWrite_R61529(0xD0);   //Power Setting(charge Pump Setting)
 //LCD_DataWrite_R61529(0x95);//A6//DC4(8+VCL)  DC1(VSP)  DC2(VGH/L) ----nomal
 //LCD_DataWrite_R61529(0x97);//A6//DC4(8+VCL) ----- Long time
 LCD_DataWrite_R61529(0xa9);//A6//DC4(8+VCL) ----- Short time
 LCD_DataWrite_R61529(0x06);//06 max,  0e min //BT
 LCD_DataWrite_R61529(0x08); 
 LCD_DataWrite_R61529(0x20);//10 //DC1M(VSN)  //long 10   short 20
 LCD_DataWrite_R61529(0x31);//1E //VC1[2:0]2.82V  VC2[2:0]4V
 LCD_DataWrite_R61529(0x04); 
 LCD_DataWrite_R61529(0x01); 
 LCD_DataWrite_R61529(0x00); 
 LCD_DataWrite_R61529(0x08); 
 LCD_DataWrite_R61529(0x01); 
 LCD_DataWrite_R61529(0x00); 
 LCD_DataWrite_R61529(0x06); 
 LCD_DataWrite_R61529(0x01);  
 LCD_DataWrite_R61529(0x00); 
 LCD_DataWrite_R61529(0x00);   
 LCD_DataWrite_R61529(0x20); 
  
 LCD_CtrlWrite_R61529(0xD1); //VCOM Setting
 LCD_DataWrite_R61529(0x02);//  WCVDC
 LCD_DataWrite_R61529(0x2A);//   PVH (PVLVL) [6:0]
 LCD_DataWrite_R61529(0x1F);//   NVH (NVLVL) [6:0]
 LCD_DataWrite_R61529(0x57); //0x5E ≮ VCOM for 9k1323 for 52ver

 LCD_CtrlWrite_R61529(0x2A);   // set cloumn adrdress
 LCD_DataWrite_R61529(0x00); 
 LCD_DataWrite_R61529(0x00);  
 LCD_DataWrite_R61529(0x01);  
 LCD_DataWrite_R61529(0x3f); 
 LCD_CtrlWrite_R61529(0x2B);   // set page adrdress
 LCD_DataWrite_R61529(0x00); 
 LCD_DataWrite_R61529(0x00);  
 LCD_DataWrite_R61529(0x01);  
 LCD_DataWrite_R61529(0xe0); 
 
 LCD_CtrlWrite_R61529(0x35); 
 LCD_DataWrite_R61529(0x00);// Fmark Output ON

 LCD_CtrlWrite_R61529(0x36); 
 LCD_DataWrite_R61529(0x00);//0008
 
 LCD_CtrlWrite_R61529(0x3A); //fixel format ( DPI, MDDI, DSI) 
 LCD_DataWrite_R61529(0x66);//0x55//5-16bit   6-18    7-24
 
 LCD_ClearAll_R61529(bkground);
 Delayms(1); //100
 LCD_CtrlWrite_R61529(0x2C); //write memory start
 Delayms(10);
 LCD_CtrlWrite_R61529(0x29); //set_display_on 
 //Delayms(100);
}


void LCD_PWRON_R61529(kal_bool on)
{
   	if(on)
   	{
	   	//kal_prompt_trace(MOD_ENG,"sleep out");
      	LCD_ExitSleep_R61529();
   	}
   	else
	{
	   	//kal_prompt_trace(MOD_ENG,"sleep in");
      	LCD_EnterSleep_R61529();
   	}
}


void LCD_SetContrast_R61529(kal_uint8 level)
{
}

void LCD_ON_R61529(kal_bool on)
{
	if (on)
   	{
   		//kal_prompt_trace(MOD_ENG,"screen on");
      	LCD_ExitSleep_R61529();
   	}
   	else
   	{
	   	//kal_prompt_trace(MOD_ENG,"screen off");
      	LCD_EnterSleep_R61529();
   	}
}
void LCD_BlockWrite_R61529(kal_uint16 startx,kal_uint16 starty,kal_uint16 endx,kal_uint16 endy)
{

#ifdef __R61529_DEBUG_TRACE__
	//kal_prompt_trace(MOD_ENG,"LCD block write");
#endif
	#if defined(__RF_DESENSE_TEST__) && defined(__FM_DESENSE_COPY_NO_LCM_UPDATE_)
		// under this test, we will not write any command to LCM.
		return;
	#endif
	ASSERT(lcd_assert_fail==KAL_FALSE);
	lcd_assert_fail = KAL_TRUE;
	#ifdef LQT_SUPPORT	
		if(!lcd_update_permission&&!(lcd_at_mode==LCD_AT_RELEASE_MODE))
		{
			lcd_assert_fail = KAL_FALSE;
			return;//in LQT mode but not update permitted
		}
		if(lcd_update_permission&&!(lcd_at_mode==LCD_AT_RELEASE_MODE))
		{
			startx = 0;
			starty = 0;
			endx = LCD_WIDTH -1;
			endy = LCD_HEIGHT -1; //in LQT mode and update permitted
		}
	#endif /*LQT_SUPPORT*/


#if (defined(LCD_CMD_DMA_MODE))
	#if(defined(MAIN_LCD_18BIT_MODE)||defined(MAIN_LCD_8BIT_MODE))
	{
//SET_LCD_PARALLEL_IF_TIMING(0,0,2,2,12,2,2,6,0);//care write cycle(write register) 100ns

		SET_LCD_CMD_PARAMETER(0,LCD_CMD,0x002A);			  //Set_column_address(2Ah)
		SET_LCD_CMD_PARAMETER(1,LCD_DATA,(startx&0xFF00)>>8);
		SET_LCD_CMD_PARAMETER(2,LCD_DATA,startx&0x00FF);
		SET_LCD_CMD_PARAMETER(3,LCD_DATA,(endx&0xFF00)>>8);
		SET_LCD_CMD_PARAMETER(4,LCD_DATA,endx&0x00FF);
			
		SET_LCD_CMD_PARAMETER(5,LCD_CMD,0x002B);			  //Set_page_address(2Bh)
		SET_LCD_CMD_PARAMETER(6,LCD_DATA,(starty&0xFF00)>>8);
		SET_LCD_CMD_PARAMETER(7,LCD_DATA,starty&0x00FF);
		SET_LCD_CMD_PARAMETER(8,LCD_DATA,(endy&0xFF00)>>8);
		SET_LCD_CMD_PARAMETER(9,LCD_DATA,endy&0x00FF);
			  
		SET_LCD_CMD_PARAMETER(10,LCD_CMD,0x002C); 		  //Write_memory_start(2Ch)

//SET_LCD_PARALLEL_IF_TIMING(0,0,0,1,2,2,2,6,0);	 //don't care write cycle(write register) 100ns

		//SET_LCD_CMD_PARAMETER(11,LCD_CMD,0x0029); 		  //Write_memory_start(2Ch)
		SET_LCD_ROI_CTRL_NUMBER_OF_CMD(11);		
		ENABLE_LCD_TRANSFER_COMPLETE_INT;
		ENABLE_LCD_ROI_CTRL_CMD_FIRST;//ENABLE_LCD_ROI_CTRL_CMD_FIRST;
		//while(LCD_IS_RUNNING);
		START_LCD_TRANSFER;
	
	}
	#endif
#else	
	#if(defined(MAIN_LCD_18BIT_MODE)||defined(MAIN_LCD_8BIT_MODE))
	{
//SET_LCD_PARALLEL_IF_TIMING(0,0,2,2,12,2,2,6,0);//care write cycle(write register) 100ns

		LCD_CtrlWrite_R61529(0x002a);
		LCD_DataWrite_R61529((startx&0xff00)>>8);//start x
		LCD_DataWrite_R61529(startx&0x00ff);

		LCD_DataWrite_R61529((endx&0xff00)>>8);//end x
		LCD_DataWrite_R61529(endx&0x00ff);

		LCD_CtrlWrite_R61529(0x002b);
		LCD_DataWrite_R61529((starty&0xff00)>>8);//start x
		LCD_DataWrite_R61529(starty&0x00ff);

		LCD_DataWrite_R61529((endy&0xff00)>>8);//end x
		LCD_DataWrite_R61529(endy&0x00ff);

		LCD_CtrlWrite_R61529(0x002c);//transfor begin

//SET_LCD_PARALLEL_IF_TIMING(0,0,0,1,2,2,2,6,0);	 //don't care write cycle(write register) 100ns

		ENABLE_LCD_TRANSFER_COMPLETE_INT;
		DISABLE_LCD_ROI_CTRL_CMD_FIRST;//ENABLE_LCD_ROI_CTRL_CMD_FIRST;
		//while(LCD_IS_RUNNING);
		START_LCD_TRANSFER;
	}
	#endif
#endif

		lcd_assert_fail = KAL_FALSE;

}

void LCD_Size_R61529(kal_uint16 *out_LCD_width,kal_uint16 *out_LCD_height)
{
   *out_LCD_width = LCD_WIDTH;
   *out_LCD_height = LCD_HEIGHT;
}

/*Engineering mode*/
kal_uint8 LCD_GetParm_R61529(lcd_func_type type)
{
   switch(type)
   {
      case lcd_Bais_func:
         return 1;
      case lcd_Contrast_func:
         return 1;
      case lcd_LineRate_func:
         return 1;
      case lcd_Temperature_Compensation_func:
         return 1;
      default:
         ASSERT(0);
      return 100;
   }
}

void LCD_SetBias_R61529(kal_uint8 *bias)
{
}

void LCD_Contrast_R61529(kal_uint8 *contrast)
{
}

void LCD_LineRate_R61529(kal_uint8 *linerate)
{
}

void LCD_Temp_Compensate_R61529(kal_uint8 *compensate)
{
}

void LCD_Set_Scan_Direction_R61529(kal_uint8 rotate_value)
{

}	/* LCD_Set_Scan_Direction_TA7601() */


#ifdef LQT_SUPPORT/*Please don't remove LQT code segments*/
/*************************************************************************
* FUNCTION
*   LCD_gamma_test() and LCD_flicker_test()
*
* DESCRIPTION
*   Generating test pattern by AT commands.
*
* PARAMETERS
*   level, color
*
* RETURNS
*   None
*
* GLOBALS AFFECTED
*
*************************************************************************/
void LCD_gamma_test_R61529(kal_uint8 level, kal_uint8 color)
{
	kal_uint16 LCD_x;
	kal_uint16 LCD_y;
	kal_uint16 r_color,g_color,b_color,w_color;
    kal_uint32 lcd_layer0_buffer;	


  		  		
	  lcd_layer0_buffer=REG_LCD_LAYER0_BUFF_ADDR;
	  r_color=(level>>1)<<11;	/* transfer to RGB565 */
	  g_color=level<<5;
	  b_color=level>>1;
	  w_color=(r_color|g_color|b_color);
		for(LCD_y=0;LCD_y<LCD_HEIGHT;LCD_y++)
		{
			for(LCD_x=0;LCD_x<LCD_WIDTH/2;LCD_x++)
			{
				switch(color)
				{
		      case 0:
						*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)= ((w_color<<16)|w_color);
						break;					
					case 1:
						*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)= ((r_color<<16)|r_color);
						break;
		      case 2:
						*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)= ((g_color<<16)|g_color);
						break;
		      case 3:
						*((kal_uint32 *)lcd_layer0_buffer+LCD_y*(LCD_WIDTH/2)+LCD_x)= ((b_color<<16)|b_color);
						break;

		      default:
		         ASSERT(0);
				}
			}
		}		
}

void LCD_flicker_test_R61529(kal_uint8 level)
{
  	kal_uint16 LCD_x,LCD_y;

	LCD_CtrlWrite_R61529(0x002a);
	LCD_DataWrite_R61529((0&0xff00)>>8);//start x
	LCD_DataWrite_R61529(0&0x00ff);
	LCD_DataWrite_R61529(((480-1)&0xff00)>>8);//end x
	LCD_DataWrite_R61529((480-1)&0x00ff);
	
	LCD_CtrlWrite_R61529(0x002b);
	LCD_DataWrite_R61529((0&0xff00)>>8);//start y
	LCD_DataWrite_R61529(0&0x00ff);
	LCD_DataWrite_R61529(((800-1)&0xff00)>>8);//end y
	LCD_DataWrite_R61529((800-1)&0x00ff);
	
	LCD_CtrlWrite_R61529(0x002c);//transfor begin


	for(LCD_y=0;LCD_y<=800-1;LCD_y++)
	{
		
		if(LCD_y&0x2)
		{
			for(LCD_x=0;LCD_x<=480-1;LCD_x+=1)
			{
				*(volatile kal_uint32 *) MAIN_LCD_DATA_ADDR= (0x0003FFFF);
				//Delayms(1);
			}
		}
		else
		{
			for(LCD_x=0;LCD_x<=480-1;LCD_x+=1)
			{
				*(volatile kal_uint32 *) MAIN_LCD_DATA_ADDR= (0x00000000);
				//Delayms(1);
			}
		}
	}
	//Delayms(5000);
}

void LCD_Frame_Pixel_Test_R61529(void)
{

  	kal_uint16 LCD_x,LCD_y;

	LCD_CtrlWrite_R61529(0x002a);
	LCD_DataWrite_R61529((0&0xff00)>>8);//start x
	LCD_DataWrite_R61529(0&0x00ff);
	LCD_DataWrite_R61529(((480-1)&0xff00)>>8);//end x
	LCD_DataWrite_R61529((480-1)&0x00ff);
	
	LCD_CtrlWrite_R61529(0x002b);
	LCD_DataWrite_R61529((0&0xff00)>>8);//start y
	LCD_DataWrite_R61529(0&0x00ff);
	LCD_DataWrite_R61529(((800-1)&0xff00)>>8);//end y
	LCD_DataWrite_R61529((800-1)&0x00ff);
	
	LCD_CtrlWrite_R61529(0x002c);//transfor begin

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#else

	for(LCD_y=0;LCD_y<=800-1;LCD_y++)
	{
		for(LCD_x=0;LCD_x<=480-1;LCD_x+=1)
		{

			if(0==LCD_y ||(800-1)==LCD_y)
			{
				*(volatile kal_uint32 *) MAIN_LCD_DATA_ADDR= (0x0003FFFF);

			}
			else if(0== LCD_x)
			{
				*(volatile kal_uint32 *) MAIN_LCD_DATA_ADDR= (0x0003FFFF);
			}
			else if(480-2==LCD_x)
			{
				*(volatile kal_uint32 *) MAIN_LCD_DATA_ADDR= (0x0003FFFF);
			}
			else
			{
				*(volatile kal_uint32 *) MAIN_LCD_DATA_ADDR= (0x00000000);
			}
		}
	}
#endif    
}

void LCD_CS_Test_R61529(kal_uint16 cs,kal_uint16 bgr,kal_uint16 mx)
{
	
#ifdef __R61529_DEBUG_TRACE__
	kal_prompt_trace(MOD_ENG,"LCD_CS_Test  cs=%d,bgr=%d,mx=%d",cs,bgr,mx);
#endif

	if(0==cs)
	{
		if(0==bgr && 0==mx)
		{
			LCD_CtrlWrite_R61529(0x0036);
			LCD_DataWrite_R61529(0x00);
		}
		else if(1==bgr && 0==mx)
		{
			LCD_CtrlWrite_R61529(0x0036);
			LCD_DataWrite_R61529(0x08);
		}
		else if(1==bgr && 1==mx)
		{
			LCD_CtrlWrite_R61529(0x0036);
			LCD_DataWrite_R61529(0x48);
		}
	}
	else
	{	//cs1
		//re define
		#define MAIN_LCD_CMD1_ADDR			LCD_PARALLEL1_A0_LOW_ADDR
		#define MAIN_LCD_DATA1_ADDR			LCD_PARALLEL1_A0_HIGH_ADDR


		#define LCD_CtrlWrite1_R61529(_data)  \
		{\
			*(volatile kal_uint16 *) MAIN_LCD_CMD1_ADDR= (_data);\
		}\

		#define LCD_DataWrite1_R61529(_data)  \
		{\
			*(volatile kal_uint16 *) MAIN_LCD_DATA1_ADDR= (_data);\
		}
		// code
		if(0==bgr && 0==mx)
		{
			LCD_CtrlWrite1_R61529(0x0036);
			LCD_DataWrite1_R61529(0x00);
		}
		else if(1==bgr && 1==mx)
		{
			LCD_CtrlWrite1_R61529(0x0036);
			LCD_DataWrite1_R61529(0x48);
		}

	}
}


static void display_RGBbar_R61529(void)
{
  	kal_uint16 LCD_x,LCD_y,n,r;
#ifdef __R61529_DEBUG_TRACE__
	kal_prompt_trace(MOD_ENG,"R61529  display_RGBbar");
#endif
	#define R61529_LCD_HEIGHT	800
	#define R61529_LCD_WIDTH	480
	

	LCD_CtrlWrite_R61529(0x002a);
	LCD_DataWrite_R61529((0&0xff00)>>8);//start x
	LCD_DataWrite_R61529(0&0x00ff);
	LCD_DataWrite_R61529(((R61529_LCD_WIDTH-1)&0xff00)>>8);//end x
	LCD_DataWrite_R61529((R61529_LCD_WIDTH-1)&0x00ff);
	
	LCD_CtrlWrite_R61529(0x002b);
	LCD_DataWrite_R61529((0&0xff00)>>8);//start y
	LCD_DataWrite_R61529(0&0x00ff);
	LCD_DataWrite_R61529(((R61529_LCD_HEIGHT-1)&0xff00)>>8);//end y
	LCD_DataWrite_R61529((R61529_LCD_HEIGHT-1)&0x00ff);
	
	LCD_CtrlWrite_R61529(0x002c);//transfor begin

#if 0
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
/* under construction !*/
#endif
	for(LCD_y=0;LCD_y<=R61529_LCD_HEIGHT-1;LCD_y++)
	{
		for(LCD_x=0;LCD_x<=R61529_LCD_WIDTH-1;LCD_x+=1)
		{
			if(LCD_y<=((R61529_LCD_HEIGHT-1)/3))//R
			{
				*(volatile kal_uint32 *) MAIN_LCD_DATA_ADDR= (0x0003F000);
				//Delayms(1);
			}
			else if(LCD_y>=((R61529_LCD_HEIGHT-1)/3*2))//B
			{
				*(volatile kal_uint32 *) MAIN_LCD_DATA_ADDR= (0x0000003F) ;
				//Delayms(1);
			}
			else//G
			{
				*(volatile kal_uint32 *) MAIN_LCD_DATA_ADDR= (0x00000FC0);
				//Delayms(1);
			}
		}
	}
	//Delayms(500);

}

#endif/*LQT_SUPPORT*/
LCM_IOCTRL_STATUS_ENUM 
LCD_IOCTRL_R61529(LCM_IOCTRL_ID_ENUM ID, void* Parameters)
{
   switch (ID)
   {
       case LCM_IOCTRL_QUERY__FRAME_RATE:
            *((unsigned int *)Parameters) = 66.38;
            return LCM_IOCTRL_OK;

       case LCM_IOCTRL_SET__FRAME_RATE:

            return LCM_IOCTRL_NOT_SUPPORT;


       case LCM_IOCTRL_QUERY__FRAME_MARKER:

            return LCM_IOCTRL_NOT_SUPPORT;


       case LCM_IOCTRL_SET__FRAME_MARKER:

			return LCM_IOCTRL_NOT_SUPPORT;
			
		   //Fine Tue the FRM_Marker to meet all scenarios tearing free. MUST set command immediately		   
		   //LCD_CtrlWrite_R61529(0x0044);//frame rate control
		   //LCD_DataWrite_R61529(((*((unsigned int *)Parameters)+ back_potch)&0xFF00)>>8);//scanline high byte
		   //LCD_DataWrite_R61529((*((unsigned int *)Parameters)+ back_potch)&0xFF);//scanline low byte

		   /*
			//make sure it is the same as LCD_Init_R61529
		   LCD_CtrlWrite_R61529(0x0044); //set_tear_scanline
		   LCD_DataWrite_R61529(((*((unsigned int *)Parameters)+ back_potch)&0xFF00)>>8);////TEP[8]
		   LCD_DataWrite_R61529((*((unsigned int *)Parameters)+ back_potch)&0xFF);//TEP[7:0] 		   

		   return LCM_IOCTRL_OK;
			*/
		   
		   break;


       case LCM_IOCTRL_QUERY__SUPPORT_H_V_SIGNAL_FUNC:
       case LCM_IOCTRL_QUERY__SUPPORT_V_PULSE_WIDTH:
       case LCM_IOCTRL_QUERY__SUPPORT_H_PULSE_WIDTH:
            return LCM_IOCTRL_NOT_SUPPORT;

       case LCM_IOCTRL_QUERY__BACK_PORCH:
            *((unsigned int *)Parameters) = 4;
            return LCM_IOCTRL_OK;

       case LCM_IOCTRL_QUERY__FRONT_PORCH:
            *((unsigned int *)Parameters) = 4;
            return LCM_IOCTRL_OK;

       case LCM_IOCTRL_SET__BACK_PORCH:
            return LCM_IOCTRL_NOT_SUPPORT;

       case LCM_IOCTRL_SET__FRONT_PORCH:

            return LCM_IOCTRL_NOT_SUPPORT;

       case LCM_IOCTRL_QUERY__TE_EDGE_ATTRIB:
		 	   //LCM_TE_ATTRIB_ENUM
		 	   *((unsigned int *)Parameters) = LCM_TE_FAILING_EDGE;
            return LCM_IOCTRL_OK;

       case LCM_IOCTRL_QUERY__SUPPORT_READBACK_FUNC:
            return LCM_IOCTRL_NOT_SUPPORT;

       case LCM_IOCTRL_QUERY__SCANLINE_REG:
            return LCM_IOCTRL_NOT_SUPPORT;

       case LCM_IOCTRL_QUERY__IF_CS_NUMBER:
		   //Return the LCM is ocupied which CS.
		   *(kal_uint32*) (Parameters) =  LCD_IF_PARALLEL_0;
		   return LCM_IOCTRL_OK;
			

		
		case LCM_IOCTRL_QUERY__SYNC_MODE:
		
			 //Return correct value based on the mode you set in lcd_init_r61509u(~)
		
			 //If you set TE to be VH-Sync mode in lcd_init_r61509u(~)<LCD_CtrlWrite_ILI9481(0x35); LCD_DataWrite_ILI9481(0x01);>
			 //*(kal_uint32*) (Parameters) = LCM_TE_VHSYNC_MODE;
			 //return LCM_IOCTRL_OK;
		
			 //If you set TE to be V-Sync mode in lcd_init_r61509u(~)<LCD_CtrlWrite_ILI9481(0x35); LCD_DataWrite_ILI9481(0x00);>
			 *(kal_uint32*) (Parameters) = LCM_TE_VSYNC_MODE;
			 return LCM_IOCTRL_OK;
		
		case LCM_IOCTRL_QUERY__FLIP_MIRROR:
			 return LCM_IOCTRL_NOT_SUPPORT;
		
		case LCM_IOCTRL_QUERY__ROTATION:
			 return LCM_IOCTRL_OK;
      case LCM_IOCTRL_QUERY__LCM_WIDTH:
			 *(kal_uint32*) (Parameters) = 320;
			 return LCM_IOCTRL_OK;

      case LCM_IOCTRL_QUERY__LCM_HEIGHT:
			 *(kal_uint32*) (Parameters) = 480;
			 return LCM_IOCTRL_OK;

    case LCM_IOCTRL_QUERY__LCD_PPI: 
       *((kal_uint32*)Parameters) = LCM_DPI;
       return LCM_IOCTRL_OK;
       
       default:
          return LCM_IOCTRL_NOT_SUPPORT;
   }
}

#ifdef COMBO_LCM_ID_AUTO_DETECT
kal_bool LCM_ID_check_R61529()
{
    kal_uint8 dummy, R61529_1, R61529_2, R61529_3, R61529_4;

    LCD_CtrlWrite_R61529(0xB0);  //command to open Manufacturer command access right
    LCD_DataWrite_R61529(0x0);

    LCD_CtrlWrite_R61529(0xBF);  //Device Code Read
    dummy = LCD_DataRead_R61529;
    R61529_1 = LCD_DataRead_R61529;
    R61529_2 = LCD_DataRead_R61529;
    R61529_3 = LCD_DataRead_R61529;
    R61529_4 = LCD_DataRead_R61529;
    dummy = LCD_DataRead_R61529;

    if (1 == R61529_1 && 34 == R61529_2 &&
        21 == R61529_3 && 41 == R61529_4 && 1 == dummy)
      return KAL_TRUE;
    else
      return KAL_FALSE;
}
#endif

LCD_Funcs LCD_func_R61529 = {
   LCD_Init_R61529,
   LCD_PWRON_R61529,
   LCD_SetContrast_R61529,
   LCD_ON_R61529,
   LCD_BlockWrite_R61529,
   LCD_Size_R61529,
   LCD_EnterSleep_R61529,
   LCD_ExitSleep_R61529,
   LCD_Partial_On_R61529,
   LCD_Partial_Off_R61529,
   LCD_Partial_line_R61529,
   /*Engineering mode*/
   LCD_GetParm_R61529,
   LCD_SetBias_R61529,
   LCD_Contrast_R61529,
   LCD_LineRate_R61529,
   LCD_Temp_Compensate_R61529
#ifdef LCM_ROTATE_SUPPORT
   ,LCD_Set_Scan_Direction_R61529
#endif
#ifdef LQT_SUPPORT/*Please don't remove LQT code segments*/
    ,LCD_gamma_test_R61529
    ,LCD_flicker_test_R61529
#endif
   ,0 // NULL function for SetFrmMarker Function.
   ,LCD_IOCTRL_R61529
};

void LCD_FunConfig_R61529(void)
{
	MainLCD = &LCD_func_R61529;
}

#ifdef __LCM_AT_TEST__ // Add by hesong 0225
void at_command_test_lcm(kal_uint32 *d)
{
#ifdef __R61529_DEBUG_TRACE__
	kal_prompt_trace(MOD_ENG,"at_command_test_lcm  %d",d[1]);
#endif
	switch(d[1]){
	case 2:
		/*LCD-test-002
		   No fricker
		   确保开机过程和IDLE界面屏不会闪, 可尋求IC vendor協助或
		   自行調整frame rate/COM_H and COM_L, 參考page "Flicker Test Code"
		*/
		LCD_flicker_test_R61529(d[2]);//255/200/160
		break;

	case 3:
		/*LCD-test-003
		   WR toggle with CS1(CS0不toggle)
		   在Init後,特意去用CS1 Port去更改LCD IC Register(例如Scan direction, Color format...), 
		   確定對CS0上的LCM無影?
		*/
		LCD_CS_Test_R61529(d[2],d[3],d[4]);
		
		break;
	case 4:
		/*LCD-test-004
		   LCD 1-pixel Frame test
		   最外圍1-pixel?度?的特殊檢?pattern, 
		   檢查可能的IC起始pixel/line傳?方式錯誤, 
		   檢查TFT上的layout?題,可參考page"LCD 1-pixel Frame test coding"
		*/
		LCD_Frame_Pixel_Test_R61529();
		break;

	case 7:
		/*LCD-test-007
		   sleep in status OK.
		   能正确进入sleep状态
		   (若有?流?的?,應?能確認?IC耗??流達到spec.需求)
		*/
		LCD_EnterSleep_R61529();
		break;
	case 8:
		/*LCD-test-008
		   Sleep-in with CS/WR toggle 
		   lcd 进入睡眠后，用AT Cmd给LCM发命令或数据(非睡眠序列)，
		   不会导致LCM死机, ,某些LCD IC在进入Sleep-in时只是将Power关掉,
		   内部逻辑电压依旧可以起作用,所以在sleep-in時设置Mode Register
		   有可能有作用。因此這?測?可以敘述IC在Sleep-in時是否能接收Command
		*/
		display_RGBbar_R61529();
		break;
	case 9:
		/*LCD-test-009
		   sleep out success
		   能正确退出睡眠状态
		*/
		LCD_ExitSleep_R61529();

		break;
	case 15:
		/*LCD-test-015
		   Read GRAM function OK
		   能正常用AT Command去读GRAM的数据
		*/
		
		LCD_Read_GRAM_R61529(d[2],d[3],d[4],d[5]);
		break;
		
	case 16:
		/*LCD-test-016
		   Read Register function OK
		   能讀回正常的Chip ID
		*/
		LCD_Read_ID_R61529();
		break;

	case 0xFE:
		LCD_CtrlWrite_R61529(0x0028);//display off
#ifdef __R61529_DEBUG_TRACE__
		kal_prompt_trace(MOD_ENG,"display off");
#endif
		break;
	case 0xFF:
		LCD_CtrlWrite_R61529(0x0029);//display on
#ifdef __R61529_DEBUG_TRACE__
		kal_prompt_trace(MOD_ENG,"display on");
#endif
		break;
	default:
#ifdef __R61529_DEBUG_TRACE__
		kal_prompt_trace(MOD_ENG,"woo...default!");
#endif
		break;
	}
}
#endif
