
#include "lcd_sw_inc.h"
#include "lcd_sw.h"
#include "lcd_hw.h"


/*****************************************************************************
 * COMBO LCM ID and string name *
 *============================================================================*/
 
#pragma arm section code = "PRIMARY_ROCODE", rwdata = "PRIMARY_RW", rodata = "PRIMARY_RODATA"

typedef struct
{
    kal_uint32 pattern_head;
    kal_uint32 main_lcd_id;
    kal_uint32 pattern_tail;
}main_lcd_block;

char* (main_lcm_list[]) =
{
	"ILI9486",
	"R61529",
	"EXAMPLE_LCM" //please add main lcd names above this item, the order must corresponding to main_lcm_enum.	
	              //notice, the name should be less than 50 characters.
}; 

kal_uint32 main_lcm_number = MAIN_LCM_MAX_NUM;

#if defined(__MTK_TARGET__)
__align(4) main_lcd_block MainLCDBlock = 
#else
main_lcd_block MainLCDBlock = 
#endif
{
    0x44434C4D,
    0,   //modify this value to adapt the current attached LCM
    0x4C494154
};

#ifdef DUAL_LCD

typedef struct
{
	kal_uint32 pattern_head;
	kal_uint32 sub_lcd_id;
	kal_uint32 pattern_tail;
}sub_lcd_block;


char* (sub_lcm_list[]) =
{
	"SUB_EXAMPLE_LCM_A",
	"SUB_EXAMPLE_LCM_B",
	"SUB_EXAMPLE_LCM_C",
	"EXAMPLE_LCM" //please add sub lcd names above this item, the order must corresponding to sub_lcm_enum.	
	              //notice, the name should be less than 50 characters.
}; 
kal_uint32 sub_lcm_number = SUB_LCM_MAX_NUM;

#if defined(__MTK_TARGET__)
__align(4) sub_lcd_block SubLCDBlock = 
#else
sub_lcd_block SubLCDBlock = 
#endif
{
	0x44434C53,
	0,   //modify this value to adapt the current attached LCM
	0x4C494154
};

#endif


/*****************************************************************************
 *wrap function for combo lcm interface *
 *============================================================================*/

void spi_ini(void)
{
	switch (MainLCDBlock.main_lcd_id)
	{
		case ILI9486:
			spi_ini_ILI9486();
			break;				
		case R61529:
			spi_ini_R61529();
			break;	
		default:
			ASSERT(0);			
	}
}
void init_lcd_interface(void)
{
	switch (MainLCDBlock.main_lcd_id)
	{
		case ILI9486:
			init_lcd_interface_ILI9486();
			break;				
		case R61529:
			init_lcd_interface_R61529();
			break;	
		default:
			ASSERT(0);			
	}
}

void LCD_FunConfig(void)
{
	switch (MainLCDBlock.main_lcd_id)
	{
		case ILI9486:
			LCD_FunConfig_ILI9486();
			break;				
		case R61529:
			LCD_FunConfig_R61529();
			break;	
	
		default:
			ASSERT(0);			
	}
}

#ifdef LQT_SUPPORT
void LCD_gamma_test(kal_uint8 level, kal_uint8 color)
{
	switch (MainLCDBlock.main_lcd_id)
	{
		case ILI9486:
			LCD_gamma_test_ILI9486(level, color);
			break;				
		case R61529:
			LCD_gamma_test_R61529(level, color);
			break;	
			
		default:
			ASSERT(0);			
	}
}
void LCD_flicker_test(kal_uint8 level)
{
	switch (MainLCDBlock.main_lcd_id)
	{
		case ILI9486:
			LCD_flicker_test_ILI9486(level);
			break;				
		case R61529:
			LCD_flicker_test_R61529(level);
			break;	
	
		default:
			ASSERT(0);			
	}
}
#endif

#ifdef COMBO_LCM_ID_AUTO_DETECT
kal_bool LCM_ID_Validation(void)
{
  switch (MainLCDBlock.main_lcd_id)
  {
      case ILI9486:
          return LCM_ID_check_ILI9486();
      case R61529:
          return LCM_ID_check_R61529();
  
      default:
          ASSERT(0);          
  }
}

kal_uint32 LCM_ID_Get(void)
{
  return MainLCDBlock.main_lcd_id;
}

void LCM_ID_Alter(void)
{
  MainLCDBlock.main_lcd_id++;
}
#endif

/*****************************************************************************
 *global varible*
 *============================================================================*/
 
kal_bool  lcd_assert_fail=KAL_FALSE;
kal_uint16 lcd_driver_rotation=LCD_LAYER_ROTATE_NORMAL;
kal_uint32 back_potch = 1;