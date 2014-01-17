#include "MMI_features.h"
#include "custresdef.h"
#include "vmswitch.h"

<?xml version="1.0" encoding="UTF-8"?>


<APP id="APP_MRE" name="STR_ID_APP_MRE">

#ifdef __PLUTO_MMI_PACKAGE__
    <MEMORY heap="$(MRE_SYS_ASM_APP_MEMPOOL_SIZE)" inc="vmopt.h"/>
#else
#if defined(__LOW_COST_SUPPORT_COMMON__)
    <MEMORY heap="$(MRE_SYS_ASM_APP_MEMPOOL_SIZE) + 100*1024" inc="vmopt.h"/>
#else
    <MEMORY heap="$(MRE_SYS_ASM_APP_MEMPOOL_SIZE) + 500*1024" inc="vmopt.h"/>
#endif
<APPCONCURRENT concurrent_w_order ="APP_MRE, VAPP_CBM"/>
#endif //__PLUTO_MMI_PACKAGE__


#if (MRE_VERSION >= 2000)
    /* When you use any ID of other module, you need to add
       that header file here, so that Resgen can find the ID */
    <!--Include Area-->
    <INCLUDE file="GlobalResDef.h"/>
#if defined( __PLUTO_MMI_PACKAGE__) && defined(__MMI_FILE_MANAGER__)
    <INCLUDE file="mmi_rp_app_filemanager_def.h"/>
#endif

    <!-----------------------------------------------------String Resource Area----------------------------------------------------->
    <STRING id="STR_MRE_TITLE"/>
    <STRING id="STR_MRE_NETWORK_CMWAP"/>
    <STRING id="STR_MRE_NETWORK_CMNET"/>
    <STRING id="STR_MRE_NETWORK_CMWAP_ONLY"/>
    <STRING id="STR_MRE_NETWORK_CMNET_ONLY"/>
    <STRING id="STR_MRE_NETWORK_CMWAP_DIS_WLAN"/>
    <STRING id="STR_MRE_NETWORK_CMNET_DIS_WLAN"/>
    <STRING id="STR_MRE_NETWORK_CMWAP_ONLY_DIS_WLAN"/>
    <STRING id="STR_MRE_NETWORK_CMNET_ONLY_DIS_WLAN"/>
    <STRING id="STR_MRE_NETWORK_WIFI"/>
    <STRING id="STR_MRE_APP0"/>
    <STRING id="STR_MRE_APP1"/>
    <STRING id="STR_MRE_APP2"/>
    <STRING id="STR_MRE_APP3"/>
    <STRING id="STR_MRE_APP4"/>
    <STRING id="STR_MRE_REACH_MAX_NUM"/>
    <STRING id="STR_MRE_POPUP_CONFLICTS"/>
    <STRING id="STR_MRE_POPUP_FORCE_FREE"/>
#ifndef __MMI_BRIEF_TEXT__    
    <STRING id="STR_MRE_APP5"/>
    <STRING id="STR_MRE_APP6"/>
    <STRING id="STR_MRE_APP7"/>
    <STRING id="STR_MRE_APP8"/>
    <STRING id="STR_MRE_APP9"/>
    <STRING id="STR_MRE_OPERA_MINI"/>
#endif /* __MMI_BRIEF_TEXT__ */

    <!-----------------------------------------------------Image Resource Area------------------------------------------------------>
    /* Image Id and path of you Application , you can use compile option in Path, but need out of "" */
#ifdef __PLUTO_MMI_PACKAGE__
    <IMAGE id="IMAGE_MRE_ICON" >CUST_IMG_PATH"\\\\MainLCD\\\\Submenu\\\\NoSIM\\\\NS_MRE.pbm"</IMAGE>
    <IMAGE id="IMAGE_MRE_LARGE_ICON" >CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_VXP.gif"</IMAGE>
    <IMAGE id="IMAGE_MRE_SMALL_ICON" >CUST_IMG_PATH"\\\\MainLCD\\\\FileManager\\\\FM_SMALL_VXP.bmp"</IMAGE>
    <IMAGE id="IMAGE_MRE_EDITOR_TITLE" >CUST_IMG_PATH"\\\\MainLCD\\\\Titlebar\\\\TB_MRE_Editor.pbm"</IMAGE>
#else
    <IMAGE id="IMAGE_MRE_LARGE_ICON" >RES_IMG_ROOT"\\\\FMGR\\\\ListIcon\\\\MRE.png"</IMAGE>
    <IMAGE id="IMAGE_MRE_SMALL_ICON" >RES_IMG_ROOT"\\\\FMGR\\\\ListIcon\\\\MRE.png"</IMAGE>
    <IMAGE id="IMAGE_MRE_EDITOR_TITLE" >RES_IMG_ROOT"\\\\FMGR\\\\ListIcon\\\\MRE.png"</IMAGE>
#endif

    <AUDIO id= "AUD_ID_MRE_FEEDBACK_SLIENT">CUST_ADO_PATH"\\\\Sound\\\\Silence.imy"</AUDIO>
#ifdef __COSMOS_MMI_PACKAGE__
     #define ROOT_PATH RES_IMG_ROOT"\\\\Mainmenu_Icon\\\\"
     <IMAGE id="IMG_ID_AM_APP_MRE_DEFAULT" >ROOT_PATH"TCard.png"</IMAGE>
#elif defined(__MMI_FTE_SUPPORT__) && defined(__MMI_VUI_LAUNCHER_KEY__)
    <IMAGE id="IMG_ID_AM_APP_MRE_DEFAULT">CUST_IMG_PATH"\\\\MainLCD\\\\VUI\\\\LauncherKey\\\\MainMenu\\\\MRE.png"</IMAGE>
#elif defined(__MMI_FTE_SUPPORT__)&&defined(__MMI_VUI_LAUNCHER__)
   <IMAGE id="IMG_ID_AM_APP_MRE_DEFAULT">CUST_IMG_PATH"\\\\MainLCD\\\\VUI\\\\Launcher\\\\MainMenu\\\\Icons\\\\MRE.png"</IMAGE>
#else
     <IMAGE id="IMG_ID_AM_APP_MRE_DEFAULT">CUST_IMG_PATH"\\\\MainLCD\\\\MREAPP\\\\AM_App_Default.gif"</IMAGE>
#endif
		
    <!------------------------------------------------------Other Resource---------------------------------------------------------->
    <SCREEN id="GRP_ID_MRE_CORE"/>
    <SCREEN id="GRP_ID_MRE0"/>
    <SCREEN id="GRP_ID_MRE1"/>
    <SCREEN id="GRP_ID_MRE2"/>
    <SCREEN id="GRP_ID_MRE3"/>
    <SCREEN id="GRP_ID_MRE4"/>
    <SCREEN id="GRP_ID_MRE5"/>
    <SCREEN id="GRP_ID_MRE6"/>
    <SCREEN id="GRP_ID_MRE7"/>
    <SCREEN id="GRP_ID_MRE8"/>
    <SCREEN id="GRP_ID_MRE9"/>
    <SCREEN id="MRE_SCREEN_ID0"/>
    <SCREEN id="MRE_SCREEN_ID1"/>
    <SCREEN id="MRE_SCREEN_ID2"/>
    <SCREEN id="MRE_SCREEN_ID3"/>
    <SCREEN id="MRE_SCREEN_ID4"/>
    <SCREEN id="MRE_SCREEN_ID5"/>
    <SCREEN id="MRE_SCREEN_ID6"/>
    <SCREEN id="MRE_SCREEN_ID7"/>
    <SCREEN id="MRE_SCREEN_ID8"/>
    <SCREEN id="MRE_SCREEN_ID9"/>
    <SCREEN id="MRE_INPUT_SCREEN_ID"/>

    <!------------------------------------------------------Menu Resource Area------------------------------------------------------>    
#ifndef __MMI_MRE_DISABLE_FMGR__    
    <MENUITEM id="MENU_ID_FMGR_MRE_FILE_OPEN" str="STR_GLOBAL_OPEN"></MENUITEM>
    <MENUITEM id="MENU_ID_FMGR_MRE_OPTION_INSTALL" str="STR_GLOBAL_INSTALL"></MENUITEM>
     <MENU id="MENU_ID_FMGR_MRE_TYPE_OPTIONS" type="OPTION" str="STR_GLOBAL_OPTIONS">
        <MENUITEM_ID>MENU_ID_FMGR_MRE_OPTION_INSTALL</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_MRE_FILE_OPEN</MENUITEM_ID>
#ifdef __PLUTO_MMI_PACKAGE__
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_FORWARD_SEND</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_FORWARD_USE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_FOLDER_CREATE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MORE_RITS</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_RENAME</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_DELETE</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_SORT</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MARK_SEVERAL</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_COPY</MENUITEM_ID>
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_MOVE</MENUITEM_ID>
        #ifndef __MMI_SLIM_FILE_MANAGER__
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_DELETE_ALL</MENUITEM_ID>
        #endif
        <MENUITEM_ID>MENU_ID_FMGR_GEN_OPTION_DETAIL</MENUITEM_ID>
#endif
     </MENU> 

	<RECEIVER id="EVT_ID_SRV_FMGR_NOTIFICATION_DEV_PLUG_IN" proc="mmi_mre_fmgr_notify_hdlr"/>
	<RECEIVER id="EVT_ID_SRV_FMGR_NOTIFICATION_DEV_PLUG_OUT" proc="mmi_mre_fmgr_notify_hdlr"/>
	<RECEIVER id="EVT_ID_SRV_FMGR_NOTIFICATION_FORMAT" proc="mmi_mre_fmgr_notify_hdlr"/>
    <RECEIVER id="EVT_ID_USB_ENTER_MS_MODE" proc="mmi_mre_enter_usb_mode"/>
    <RECEIVER id="EVT_ID_SRV_NEW_MMS_MSG" proc="mmi_mre_mms_interrupt_hdlr"/>
        
    <RECEIVER id="EVT_ID_GPIO_LCD_SLEEP_IN" proc="vm_frm_backlight_notify_hdlr"/>
    <RECEIVER id="EVT_ID_GPIO_LCD_SLEEP_OUT" proc="vm_frm_backlight_notify_hdlr"/>
#endif
    
#endif    
#ifdef __MMI_MRE_SUB_MENU__
    <!------------------------------------------------------Menu Resource Area------------------------------------------------------>
    <MENUITEM id="MENU_ID_MRE_SUB_MENU" str="STR_MRE_TITLE" img="IMAGE_MRE_ICON" highlight="mmi_mre_sub_menu_highlight"></MENUITEM>
#endif

    /***MRE Service***/
    <CACHEDATA type="double" id="NVRAM_MRE_SRV_DA_SIM1">
        <DEFAULT_VALUE>[0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF]</DEFAULT_VALUE>
        <DESCRIPTION>MRE Dataaccount SIM1</DESCRIPTION>
    </CACHEDATA>
    
    <CACHEDATA type="double" id="NVRAM_MRE_SRV_DA_SIM1_PX">
        <DEFAULT_VALUE>[0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF]</DEFAULT_VALUE>
        <DESCRIPTION>MRE Dataaccount SIM1 with proxy</DESCRIPTION>
    </CACHEDATA>
    
    <CACHEDATA type="double" id="NVRAM_MRE_SRV_DA_SIM2">
        <DEFAULT_VALUE>[0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF]</DEFAULT_VALUE>
        <DESCRIPTION>MRE Dataaccount SIM2</DESCRIPTION>
    </CACHEDATA>
    
    <CACHEDATA type="double" id="NVRAM_MRE_SRV_DA_SIM2_PX">
        <DEFAULT_VALUE>[0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF]</DEFAULT_VALUE>
        <DESCRIPTION>MRE Dataaccount SIM2 with proxy</DESCRIPTION>
    </CACHEDATA>      
    
    
    <CACHEDATA type="double" id="NVRAM_MRE_SRV_DA_SIM3">
        <DEFAULT_VALUE>[0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF]</DEFAULT_VALUE>
        <DESCRIPTION>MRE Dataaccount SIM3</DESCRIPTION>
    </CACHEDATA>
    
    <CACHEDATA type="double" id="NVRAM_MRE_SRV_DA_SIM3_PX">
        <DEFAULT_VALUE>[0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF]</DEFAULT_VALUE>
        <DESCRIPTION>MRE Dataaccount SIM3 with proxy</DESCRIPTION>
    </CACHEDATA>

    
    <CACHEDATA type="double" id="NVRAM_MRE_SRV_DA_SIM4">
        <DEFAULT_VALUE>[0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF]</DEFAULT_VALUE>
        <DESCRIPTION>MRE Dataaccount SIM4</DESCRIPTION>
    </CACHEDATA>
    
    <CACHEDATA type="double" id="NVRAM_MRE_SRV_DA_SIM4_PX">
        <DEFAULT_VALUE>[0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF]</DEFAULT_VALUE>
        <DESCRIPTION>MRE Dataaccount SIM4 with proxy</DESCRIPTION>
    </CACHEDATA>

    
    <CACHEDATA type="byte" id="NVRAM_MRE_SRV_DA_IS_WIFI_ONLY">
        <DEFAULT_VALUE>[0x0]</DEFAULT_VALUE>
        <DESCRIPTION>MRE Dataaccount only use wifi</DESCRIPTION>
    </CACHEDATA>     

     <!--------------------------------------------------Update app area----------------------------------------------------->
     <STRING id="STR_MRE_UPDATE_TITLE"/>
     <STRING id="STR_MRE_UPDATE_QUERY"/>
     <STRING id="STR_MRE_UPDATE_UPDATING"/>
     <STRING id="STR_MRE_UPDATE_SUCCESS"/>
     <STRING id="STR_MRE_UPDATE_FAIL"/>
     
#ifndef __COSMOS_MMI_PACKAGE__
     <SCREEN id="SCR_ID_MRE_UPDATE_QUERY"/>
     <SCREEN id="SCR_ID_MRE_UPDATE_WAIT"/>
     <SCREEN id="SCR_ID_MRE_UPDATE_RESULT"/>
#endif  
    <TIMER id="MRE_SYS_TIMER_0"/>
    <TIMER id="MRE_SYS_TIMER_1"/>
    <TIMER id="MRE_SYS_TIMER_2"/>
    <TIMER id="MRE_SYS_TIMER_3"/>
    <TIMER id="MRE_SYS_TIMER_4"/>
    <TIMER id="MRE_SYS_TIMER_5"/>
    <TIMER id="MRE_SYS_TIMER_6"/>
    <TIMER id="MRE_SYS_TIMER_7"/>
    <TIMER id="MRE_SYS_TIMER_8"/>
    <TIMER id="MRE_SYS_TIMER_9"/>
    <TIMER id="MRE_SYS_TIMER_10"/>
    <TIMER id="MRE_SYS_TIMER_11"/>
    <TIMER id="MRE_SYS_TIMER_12"/>
    <TIMER id="MRE_SYS_TIMER_13"/>
    <TIMER id="MRE_SYS_TIMER_14"/>
    <TIMER id="MRE_SYS_TIMER_15"/>
    <TIMER id="MRE_SYS_TIMER_16"/>
    <TIMER id="MRE_SYS_TIMER_17"/>
    <TIMER id="MRE_SYS_TIMER_18"/>
    <TIMER id="MRE_SYS_TIMER_19"/>
    <TIMER id="MRE_SYS_TIMER_20"/>
    <TIMER id="MRE_SYS_TIMER_21"/>
    <TIMER id="MRE_SYS_TIMER_22"/>
    <TIMER id="MRE_SYS_TIMER_23"/>
    <TIMER id="MRE_SYS_TIMER_24"/>
    <TIMER id="MRE_SYS_TIMER_25"/>
    <TIMER id="MRE_SYS_TIMER_26"/>
    <TIMER id="MRE_SYS_TIMER_27"/>
    <TIMER id="MRE_SYS_TIMER_28"/>
    <TIMER id="MRE_SYS_TIMER_29"/>
    <TIMER id="MRE_SYS_TIMER_30"/>
    <TIMER id="MRE_SYS_TIMER_31"/>
    <TIMER id="MRE_SYS_TIMER_32"/>
    <TIMER id="MRE_SYS_TIMER_33"/>
    <TIMER id="MRE_SYS_TIMER_34"/>
    <TIMER id="MRE_SYS_TIMER_35"/>
    <TIMER id="MRE_SYS_TIMER_36"/>
    <TIMER id="MRE_SYS_TIMER_37"/>
    <TIMER id="MRE_SYS_TIMER_38"/>
    <TIMER id="MRE_SYS_TIMER_39"/>
    <TIMER id="MRE_SYS_TIMER_40"/>
    <TIMER id="MRE_SYS_TIMER_41"/>
    <TIMER id="MRE_SYS_TIMER_42"/>
    <TIMER id="MRE_SYS_TIMER_43"/>
    <TIMER id="MRE_SYS_TIMER_44"/>
    <TIMER id="MRE_SYS_TIMER_45"/>
    <TIMER id="MRE_SYS_TIMER_46"/>
    <TIMER id="MRE_SYS_TIMER_47"/>
    <TIMER id="MRE_SYS_TIMER_48"/>
    <TIMER id="MRE_SYS_TIMER_49"/>
    <TIMER id="MRE_SYS_TIMER_50"/>
    <TIMER id="MRE_SYS_TIMER_51"/>
    <TIMER id="MRE_SYS_TIMER_52"/>
    <TIMER id="MRE_SYS_TIMER_53"/>
    <TIMER id="MRE_SYS_TIMER_54"/>
    <TIMER id="MRE_SYS_TIMER_55"/>
    <TIMER id="MRE_SYS_TIMER_56"/>
    <TIMER id="MRE_SYS_TIMER_57"/>
    <TIMER id="MRE_SYS_TIMER_58"/>
    <TIMER id="MRE_SYS_TIMER_59"/>
    <TIMER id="MRE_SYS_TIMER_60"/>
    <TIMER id="MRE_SYS_TIMER_61"/>
    <TIMER id="MRE_SYS_TIMER_62"/>
    <TIMER id="MRE_SYS_TIMER_63"/>
    <TIMER id="MRE_SYS_TIMER_64"/>
    <TIMER id="MRE_SYS_TIMER_65"/>
    <TIMER id="MRE_SYS_TIMER_66"/>
    <TIMER id="MRE_SYS_TIMER_67"/>
    <TIMER id="MRE_SYS_TIMER_68"/>
    <TIMER id="MRE_SYS_TIMER_69"/>
    <TIMER id="MRE_SYS_TIMER_70"/>
    <TIMER id="MRE_SYS_TIMER_71"/>
    <TIMER id="MRE_SYS_TIMER_72"/>
    <TIMER id="MRE_SYS_TIMER_73"/>
    <TIMER id="MRE_SYS_TIMER_74"/>
    <TIMER id="MRE_SYS_TIMER_75"/>
    <TIMER id="MRE_SYS_TIMER_76"/>
    <TIMER id="MRE_SYS_TIMER_77"/>
    <TIMER id="MRE_SYS_TIMER_78"/>
    <TIMER id="MRE_SYS_TIMER_79"/>
    <TIMER id="MRE_SYS_TIMER_80"/>
    <TIMER id="MRE_SYS_TIMER_81"/>
    <TIMER id="MRE_SYS_TIMER_82"/>
    <TIMER id="MRE_SYS_TIMER_83"/>
    <TIMER id="MRE_SYS_TIMER_84"/>
    <TIMER id="MRE_SYS_TIMER_85"/>
    <TIMER id="MRE_SYS_TIMER_86"/>
    <TIMER id="MRE_SYS_TIMER_87"/>
    <TIMER id="MRE_SYS_TIMER_88"/>
    <TIMER id="MRE_SYS_TIMER_89"/>
    <TIMER id="MRE_SYS_TIMER_90"/>
    <TIMER id="MRE_SYS_TIMER_91"/>
    <TIMER id="MRE_SYS_TIMER_92"/>
    <TIMER id="MRE_SYS_TIMER_93"/>
    <TIMER id="MRE_SYS_TIMER_94"/>
    <TIMER id="MRE_SYS_TIMER_95"/>
    <TIMER id="MRE_SYS_TIMER_96"/>
    <TIMER id="MRE_SYS_TIMER_97"/>
    <TIMER id="MRE_SYS_TIMER_98"/>
    <TIMER id="MRE_SYS_TIMER_99"/>

#ifdef __MRE_SAL_PN__
	<RECEIVER id="EVT_ID_SRV_PNS_START_REGISTER"	   proc="srv_mre_push_receiver_broadcast_receiver" />
#endif

#if (MRE_VERSION >= 2000)
#ifdef __MMI_NCENTER_SUPPORT__
    <SENDER id="EVT_ID_MRE_NOTIFICATION_MESSAGE" hfile="mredef.h"/>

    <RECEIVER id="EVT_ID_MRE_NOTIFICATION_MESSAGE"   proc="vm_ncenter_event_handler"/>

#endif
#ifndef __MMI_MRE_DISABLE_FMGR__
<FILE_TYPES>
    <FILE_OPTION_HDLR option="MENU_ID_FMGR_MRE_FILE_OPEN" enabler="mmi_fmgr_vxp_option_enabler" handler="mmi_fmgr_vxp_option_hdlr" />
    <FILE_OPTION_HDLR option="MENU_ID_FMGR_MRE_OPTION_INSTALL" enabler="mmi_fmgr_vxp_option_enabler" handler="mmi_fmgr_vxp_option_hdlr"/>

#if (defined (__FLAVOR_MRE_SDK_VENDOR__) || defined (__FLAVOR_MRE_SDK_VENDOR_EXPRESS__) || \
    defined (__FLAVOR_MRE_SDK_QVGA__) || \
    defined (__FLAVOR_MRE_SDK_WQVGA__) || \
    defined (__FLAVOR_MRE_SDK_HVGA__) || \
    defined (__FLAVOR_MRE_SDK_WVGA__) || \
    defined (__FLAVOR_MRE_SDK_LQVGA__) || \
    defined (__FLAVOR_MRE_SDK_QQVGA__) || \
    defined (__FLAVOR_MRE_SDK_QCIF__))
    <FILE_TYPE type_id="FMGR_TYPE_DLL" extension="DLL" small_icon="IMAGE_MRE_SMALL_ICON" large_icon="IMAGE_MRE_LARGE_ICON" option="MENU_ID_FMGR_MRE_TYPE_OPTIONS"/>
#endif
    <FILE_TYPE type_id="FMGR_TYPE_VXP" extension="VXP" small_icon="IMAGE_MRE_SMALL_ICON" large_icon="IMAGE_MRE_LARGE_ICON" option="MENU_ID_FMGR_MRE_TYPE_OPTIONS"/>
    <FILE_TYPE type_id="FMGR_TYPE_VPP" extension="VPP" small_icon="IMAGE_MRE_SMALL_ICON" large_icon="IMAGE_MRE_LARGE_ICON" option="MENU_ID_FMGR_MRE_TYPE_OPTIONS"/>
</FILE_TYPES>
#endif


#ifndef __MMI_MRE_DISABLE_ALARM__
    <RECEIVER id="EVT_ID_SRV_REMINDER_NOTIFY" proc="vm_alarm_notify_hdlr"/>
#endif

#ifdef __MMI_MRE_3RD_ROM_PRELOAD__

#ifdef __MMI_MRE_APP_CALCULATOR__
    #ifdef __MMI_MAINLCD_176X220__
	      <ThirdROMBIN id="MREAPPCALCx">"mre\custom\romapp\ToolApps\MREAppCalc_QCIF"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPCALCx">"mre\custom\romapp\ToolApps\MREAppCalc_WQVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPCALCx">"mre\custom\romapp\ToolApps\MREAppCalc_QVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X240__
	      <ThirdROMBIN id="MREAPPCALCx">"mre\custom\romapp\ToolApps\MREAppCalc_QVGA_LS"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPCALCx">"mre\custom\romapp\ToolApps\MREAppCalc_HVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPCALCx">"mre\custom\romapp\ToolApps\MREAppCalc_WVGA"</ThirdROMBIN>
    #endif
#endif

#ifdef __MMI_MRE_APP_CONVERTER__	
    #ifdef __MMI_MAINLCD_176X220__
	      <ThirdROMBIN id="MREAPPCONVx">"mre\custom\romapp\ToolApps\MREAppConvt_QCIF"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPCONVx">"mre\custom\romapp\ToolApps\MREAppConvt_WQVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPCONVx">"mre\custom\romapp\ToolApps\MREAppConvt_QVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X240__
	      <ThirdROMBIN id="MREAPPCONVx">"mre\custom\romapp\ToolApps\MREAppConvt_QVGA_LS"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPCONVx">"mre\custom\romapp\ToolApps\MREAppConvt_HVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPCONVx">"mre\custom\romapp\ToolApps\MREAppConvt_WVGA"</ThirdROMBIN>
    #endif
#endif

#ifdef __MMI_MRE_APP_HEALTH__	
    #ifdef __MMI_MAINLCD_176X220__
	      <ThirdROMBIN id="MREAPPHEALT">"mre\custom\romapp\ToolApps\MREAppHealth_QCIF"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPHEALT">"mre\custom\romapp\ToolApps\MREAppHealth_WQVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPHEALT">"mre\custom\romapp\ToolApps\MREAppHealth_QVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X240__
	      <ThirdROMBIN id="MREAPPHEALT">"mre\custom\romapp\ToolApps\MREAppHealth_QVGA_LS"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPHEALT">"mre\custom\romapp\ToolApps\MREAppHealth_HVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPHEALT">"mre\custom\romapp\ToolApps\MREAppHealth_WVGA"</ThirdROMBIN>
    #endif
#endif

#ifdef __MMI_MRE_APP_STOPWATCH__
	  #ifdef __MMI_MAINLCD_176X220__
	      <ThirdROMBIN id="MREAPPSTOPW">"mre\custom\romapp\ToolApps\MREAppStopWatch_QCIF"</ThirdROMBIN>
	  #endif
	  #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPSTOPW">"mre\custom\romapp\ToolApps\MREAppStopWatch_WQVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPSTOPW">"mre\custom\romapp\ToolApps\MREAppStopWatch_QVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X240__
	      <ThirdROMBIN id="MREAPPSTOPW">"mre\custom\romapp\ToolApps\MREAppStopWatch_QVGA_LS"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPSTOPW">"mre\custom\romapp\ToolApps\MREAppStopWatch_HVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPSTOPW">"mre\custom\romapp\ToolApps\MREAppStopWatch_WVGA"</ThirdROMBIN>
    #endif
#endif

#ifdef __MMI_MRE_APP_OPERA__	
	<ThirdROMBIN id="MREAPPOPERA">"mre\custom\romapp\MREAppOpera"</ThirdROMBIN>
#endif


#ifdef __MMI_MRE_APP_OPERA_MINI_V6__
	<ThirdROMBIN id="MREAPPOPAV6">"mre\custom\romapp\OperaMini-6.1-RC2"</ThirdROMBIN>
#endif


#ifdef __MMI_MRE_APP_3GT_GAME_BALANCE_BALL_CHS__

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_240X320_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X240__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_320X240_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_240X400_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_320X480_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_480X800_SC.vxp"</ThirdROMBIN>
    #endif



#elif defined (__MMI_MRE_APP_3GT_GAME_BALANCE_BALL_CHT__)

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_240X320_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X240__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_320X240_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_240X400_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_320X480_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_480X800_TC.vxp"</ThirdROMBIN>
    #endif

#elif defined (__MMI_MRE_APP_3GT_GAME_BALANCE_BALL_ENG__)

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_240X320_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X240__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_320X240_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_240X400_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_320X480_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTBA">"mre\custom\romapp\3gt\BALANCEBALL_480X800_EN.vxp"</ThirdROMBIN>
    #endif

#endif /* __MMI_MRE_APP_3GT_GAME_BALANCE_BALL_ENG__ */


#ifdef __MMI_MRE_APP_3GT_GAME_RASH_MOTOR_CHS__

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_240X320_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X240__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_320X240_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_240X400_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_320X480_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_480X800_SC.vxp"</ThirdROMBIN>
    #endif



#elif defined (__MMI_MRE_APP_3GT_GAME_RASH_MOTOR_CHT__)

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_240X320_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X240__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_320X240_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_240X400_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_320X480_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_480X800_TC.vxp"</ThirdROMBIN>
    #endif

#elif defined (__MMI_MRE_APP_3GT_GAME_RASH_MOTOR_ENG__)

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_240X320_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X240__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_320X240_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_240X400_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_320X480_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTMO">"mre\custom\romapp\3gt\RASHMOTOR_480X800_EN.vxp"</ThirdROMBIN>
    #endif

#endif /* __MMI_MRE_APP_3GT_GAME_RASH_MOTOR_ENG__ */
#ifdef __MMI_MRE_APP_3GT_GAME_BOWLING_CHS__

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_240X320_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_240X400_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_320X480_SC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_480X800_SC.vxp"</ThirdROMBIN>
    #endif



#elif defined (__MMI_MRE_APP_3GT_GAME_BOWLING_CHT__)

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_240X320_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_240X400_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_320X480_TC.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_480X800_TC.vxp"</ThirdROMBIN>
    #endif

#elif defined (__MMI_MRE_APP_3GT_GAME_BOWLING_ENG__)

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_240X320_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_240X400_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_320X480_EN.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTBO">"mre\custom\romapp\3gt\blq_480X800_EN.vxp"</ThirdROMBIN>
    #endif

#endif /* __MMI_MRE_APP_3GT_GAME_BOWLING_ENG__ */

#ifdef __MMI_MRE_APP_3GT_GAME_DANDELION__

    #if defined (__MMI_MAINLCD_240X320__)
    <ThirdROMBIN id="MREAPP3GTDA">"mre\custom\romapp\3gt\pgy_240X320.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPP3GTDA">"mre\custom\romapp\3gt\pgy_240X400.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPP3GTDA">"mre\custom\romapp\3gt\pgy_320X480.vxp"</ThirdROMBIN>
    #elif defined (__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPP3GTDA">"mre\custom\romapp\3gt\pgy_480X800.vxp"</ThirdROMBIN>
    #endif
    
#endif //__MMI_MRE_APP_3GT_GAME_BOWLING_CHS__   

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE1__) && (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\Assassin_Creed"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE1__) && (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\Assassin_Creed"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE1__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\Assassin_Creed"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE1__) && (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\Assassin_Creed"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE1__) && (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\Assassin_Creed"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE1__) && (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\Assassin_Creed"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE1__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\Assassin_Creed"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE1__) && (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\Assassin_Creed"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE2__) && (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE2__) && (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE2__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE2__) && (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE2__) && (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE2__) && (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE2__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE2__) && (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\BrainChallenge3"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE3__) && (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE3__) && (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE3__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE3__) && (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE3__) && (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE3__) && (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE3__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE3__) && (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBBR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\BlockBreaker3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE4__) && (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\BrainChallenge3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\QVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE4__) && (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\BrainChallenge3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WQVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE4__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\BrainChallenge3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\HVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_CN_PACKAGE4__) && (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\BrainChallenge3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\CN\WVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE4__) && (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\BrainChallenge3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\QVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE4__) && (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\BrainChallenge3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WQVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE4__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\BrainChallenge3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\HVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_GB_PACKAGE4__) && (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLBCL">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\BrainChallenge3"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLACR">"mre\custom\romapp\gameloft\thirdrom\first_package\GB\WVGA\Assassin_Creed"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_2ND_PACKAGE__) && (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPGLMC2">"mre\custom\romapp\gameloft\thirdrom\2nd_wave\QVGA\ModernCombat2"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_2ND_PACKAGE__) && (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPGLMC2">"mre\custom\romapp\gameloft\thirdrom\2nd_wave\WQVGA\ModernCombat2"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_2ND_PACKAGE__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLMC2">"mre\custom\romapp\gameloft\thirdrom\2nd_wave\HVGA\ModernCombat2"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_2ND_PACKAGE__) && (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPGLMC2">"mre\custom\romapp\gameloft\thirdrom\2nd_wave\WVGA\ModernCombat2"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_AS6_CN_MT6250__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\gllive_odcv2\CN\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLLV2">"mre\custom\romapp\gameloft\thirdrom\gllive_odcv2\gllive_odc_v2"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_GAMELOFT_AS6_GB_MT6250__) && (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPGLAS6">"mre\custom\romapp\gameloft\thirdrom\gllive_odcv2\GB\Asphalt6"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPGLLV2">"mre\custom\romapp\gameloft\thirdrom\gllive_odcv2\gllive_odc_v2"</ThirdROMBIN>
#endif

#if defined (__MMI_MRE_APP_YAHOOMV2__)
#if defined (__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_220X176__) || defined (__MMI_MAINLCD_240X320__) || \
	defined (__MMI_MAINLCD_320X240__) || defined (__MMI_MAINLCD_240X400__) || defined (__MMI_MAINLCD_320X480__)

<ThirdROMBIN id="MREAPPYM2XP">"mre\custom\romapp\yahoo\ThirdROM\yahoomessengerv2_normal_800"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPYM2UI">"mre\custom\romapp\yahoo\ThirdROM\yahoomessengerv2_normal_ui_vsm"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPYM2SL">"mre\custom\romapp\yahoo\ThirdROM\yahoomessengerv2_normal_SSL_vsm"</ThirdROMBIN>

#endif /* normal resolutions */

#if defined (__MMI_MAINLCD_480X800__)

<ThirdROMBIN id="MREAPPYM2XP">"mre\custom\romapp\yahoo\ThirdROM\yahoomessengerv2_wvga_800"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPYM2UI">"mre\custom\romapp\yahoo\ThirdROM\yahoomessengerv2_wvga_ui_vsm"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPYM2SL">"mre\custom\romapp\yahoo\ThirdROM\yahoomessengerv2_wvga_SSL_vsm"</ThirdROMBIN>

#endif /* WVGA resolution */

#if defined (__MMI_MAINLCD_128X160__)

<ThirdROMBIN id="MREAPPYM2XP">"mre\custom\romapp\yahoo\ThirdROM\yahoomessengerv2_qqvga_800"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPYM2UI">"mre\custom\romapp\yahoo\ThirdROM\yahoomessengerv2_qqvga_ui_vsm"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPYM2SL">"mre\custom\romapp\yahoo\ThirdROM\yahoomessengerv2_qqvga_SSL_vsm"</ThirdROMBIN>

#endif /* QQVGA resolution */
#endif

#if defined(__MMI_MRE_APP_TWITTER__)

    #if defined (__MRE_SAL_XYSSL__)

        #if defined (__MMI_MAINLCD_128X160__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QQVGA_Key_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_QQVGA_Key_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_176X220__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QCIF_Key_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_QCIF_Key_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_220X176__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QCIF_LS_Key_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_QCIF_LS_Key_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_240X320__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QVGA_Key_Touch_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_QVGA_Key_Touch_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_320X240__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QVGA_LS_Key_Touch_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_QVGA_LS_Key_Touch_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_240X400__)

            #if defined (__MMI_MRE_APP_TWITTER_KEY_SUPPORT__)
                <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_WQVGA_Key_Touch_1200K"</ThirdROMBIN>
                <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_WQVGA_Key_Touch_1200K_VSM"</ThirdROMBIN>
            #else
                <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_WQVGA_FullTouch_1200K"</ThirdROMBIN>
                <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_WQVGA_FullTouch_1200K_VSM"</ThirdROMBIN>
            #endif

        #endif

        #if defined (__MMI_MAINLCD_320X480__)

            #if defined (__MMI_MRE_APP_TWITTER_KEY_SUPPORT__)
                <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_HVGA_Key_Touch_2M"</ThirdROMBIN>
                <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_HVGA_Key_Touch_2M_VSM"</ThirdROMBIN>
            #else
                <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_HVGA_FullTouch_3M"</ThirdROMBIN>
                <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_HVGA_FullTouch_3M_VSM"</ThirdROMBIN>
            #endif

        #endif

        #if defined (__MMI_MAINLCD_480X800__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_WVGA_FullTouch_4.5M"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\native_SSL\Twitter_WVGA_FullTouch_4.5M_VSM"</ThirdROMBIN>
        #endif

    #else /*__MRE_SAL_XYSSL__*/

        #if defined (__MMI_MAINLCD_128X160__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QQVGA_Key_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_QQVGA_Key_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_176X220__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QCIF_Key_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_QCIF_Key_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_220X176__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QCIF_LS_Key_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_QCIF_LS_Key_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_240X320__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QVGA_Key_Touch_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_QVGA_Key_Touch_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_320X240__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_QVGA_LS_Key_Touch_800K"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_QVGA_LS_Key_Touch_800K_VSM"</ThirdROMBIN>
        #endif

        #if defined (__MMI_MAINLCD_240X400__)

            #if defined (__MMI_MRE_APP_TWITTER_KEY_SUPPORT__)
                <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_WQVGA_Key_Touch_1200K"</ThirdROMBIN>
                <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_WQVGA_Key_Touch_1200K_VSM"</ThirdROMBIN>
            #else
                <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_WQVGA_FullTouch_1200K"</ThirdROMBIN>
                <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_WQVGA_FullTouch_1200K_VSM"</ThirdROMBIN>
            #endif

        #endif

        #if defined (__MMI_MAINLCD_320X480__)

            #if defined (__MMI_MRE_APP_TWITTER_KEY_SUPPORT__)
                <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_HVGA_Key_Touch_2M"</ThirdROMBIN>
                <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_HVGA_Key_Touch_2M_VSM"</ThirdROMBIN>
            #else
                <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_HVGA_FullTouch_3M"</ThirdROMBIN>
                <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_HVGA_FullTouch_3M_VSM"</ThirdROMBIN>
            #endif

        #endif

        #if defined (__MMI_MAINLCD_480X800__)
            <ThirdROMBIN id="MREAPPTWITR">"mre\custom\romapp\twitter\third_ROM\Twitter_WVGA_FullTouch_4.5M"</ThirdROMBIN>
            <ThirdROMBIN id="MREAPPTWVSM">"mre\custom\romapp\twitter\third_ROM\VSM\lib_SSL\Twitter_WVGA_FullTouch_4.5M_VSM"</ThirdROMBIN>
        #endif

    #endif

#endif /*__MMI_MRE_APP_TWITTER__*/


#if defined __MMI_MRE_APP_FACEBOOK_V2__
/************************************************* normal FBV2 vxp*********************************************************/
#if defined (__MMI_MAINLCD_320X480__)

/*
 * Dump of Facebook_v2_1000K.vxp.
 */
<ThirdROMBIN id="MREAPPFBV20">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Normal_HVGA_1000"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFBMSG">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Messenger"</ThirdROMBIN>


#elif defined (__MMI_MAINLCD_480X800__)

/*
 * Dump of Facebook_v2_1500K.vxp.
 */
<ThirdROMBIN id="MREAPPFBV20">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Normal_WVGA_1500"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFBMSG">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Messenger"</ThirdROMBIN>

#else
/*
 * Dump of Facebook_v2_800K.vxp
 */
<ThirdROMBIN id="MREAPPFBV20">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Normal_QVGA_800"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFBMSG">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Messenger"</ThirdROMBIN>

#endif /* __MMI_MAINLCD_320X480__ */
#endif
/********************************************END: normal FBV2 vxp**************************************************************/

#if defined __MMI_MRE_APP_FACEBOOK_V2_STANDARD__
/************************************************* standard FBV2 vxp*********************************************************/
#if defined (__MMI_MAINLCD_320X480__)

/*
 * Dump of Facebook_v2_1000K.vxp.
 */
<ThirdROMBIN id="MREAPPFBV20">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Standard_HVGA_1000"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFB2SM">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBV2_Camera_VSM_1000"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFBMSG">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Messenger"</ThirdROMBIN>


#elif defined (__MMI_MAINLCD_480X800__)

/*
 * Dump of Facebook_v2_1500K.vxp.
 * For WVGA
 */
<ThirdROMBIN id="MREAPPFBV20">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Standard_WVGA_1500"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFB2SM">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBV2_Camera_VSM_1500"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFBMSG">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Messenger"</ThirdROMBIN>

#elif defined (__MMI_MAINLCD_320X400__)

/*
 * Dump of Facebook_v2_800K.vxp.
 * For WQVGA
 */
<ThirdROMBIN id="MREAPPFBV20">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Standard_QVGA_800"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFB2SM">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBV2_Camera_VSM_450"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFBMSG">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Messenger"</ThirdROMBIN>

#else
/*
 * Dump of Facebook_v2_800K.vxp
 * For QVGA and lesser
 */
<ThirdROMBIN id="MREAPPFBV20">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Standard_QVGA_800"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFB2SM">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBV2_Camera_VSM_600"</ThirdROMBIN>
<ThirdROMBIN id="MREAPPFBMSG">"mre\custom\romapp\facebook\facebooklite\third_ROM\MTKFBv2_Messenger"</ThirdROMBIN>

#endif /* __MMI_MAINLCD_320X480__ */


#endif
/********************************************END: standard FBV2 vxp**************************************************************/

#ifdef __MMI_MRE_APP_YAHOOWEATHER__

#if defined (__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_220X176__) || defined (__MMI_MAINLCD_240X320__) || \
	defined (__MMI_MAINLCD_320X240__) || defined (__MMI_MAINLCD_240X400__) || defined (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPYWAPP">"mre\custom\romapp\yahoo\ThirdROM\MTKYWv10_Normal_350"</ThirdROMBIN>
#endif	/* normal resolutions */

#if defined (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPYWAPP">"mre\custom\romapp\yahoo\ThirdROM\MTKYWv10_WVGA_500"</ThirdROMBIN>
#endif /* WVGA resolution */

#endif


#ifdef __MMI_MRE_APP_YAHOOFINANCE__

#if defined (__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_220X176__) || defined (__MMI_MAINLCD_240X320__) || \
	defined (__MMI_MAINLCD_320X240__) || defined (__MMI_MAINLCD_240X400__) || defined (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPYFAPP">"mre\custom\romapp\yahoo\ThirdROM\MTKYFv10_Normal_600"</ThirdROMBIN>
#endif	/* normal resolutions */

#if defined (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPYFAPP">"mre\custom\romapp\yahoo\ThirdROM\MTKYFv10_WVGA_750"</ThirdROMBIN>
#endif /* WVGA resolution */

#endif


#ifdef __MMI_MRE_APP_YAHOONEWS__

#if defined (__MMI_MAINLCD_176X220__) || defined (__MMI_MAINLCD_220X176__) || defined (__MMI_MAINLCD_240X320__) || \
	defined (__MMI_MAINLCD_320X240__) || defined (__MMI_MAINLCD_240X400__) || defined (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPYNAPP">"mre\custom\romapp\yahoo\ThirdROM\MTKYNv10_Normal_350"</ThirdROMBIN>
#endif	/* normal resolutions */

#if defined (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPYNAPP">"mre\custom\romapp\yahoo\ThirdROM\MTKYNv10_WVGA_500"</ThirdROMBIN>
#endif /* WVGA resolution */

#endif

#if defined(__MMI_MRE_APP_KAIXIN__)
#if defined(__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPPKAIXI">"mre\custom\romapp\kaixin\Kaixin_WVGA"</ThirdROMBIN>
    <ThirdROMBIN id="MREAPPKXVSM">"mre\custom\romapp\kaixin\kaixin_WVGA.vsm"</ThirdROMBIN>
#elif defined(__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPPKAIXI">"mre\custom\romapp\kaixin\Kaixin_HVGA"</ThirdROMBIN>
    <ThirdROMBIN id="MREAPPKXVSM">"mre\custom\romapp\kaixin\kaixin_HVGA.vsm"</ThirdROMBIN>
#elif defined(__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPPKAIXI">"mre\custom\romapp\kaixin\Kaixin_WQVGA"</ThirdROMBIN>
    <ThirdROMBIN id="MREAPPKXVSM">"mre\custom\romapp\kaixin\kaixin_WQVGA.vsm"</ThirdROMBIN>
#else
    <ThirdROMBIN id="MREAPPKAIXI">"mre\custom\romapp\kaixin\Kaixin_QVGA"</ThirdROMBIN>
#endif /* __MMI_MAINLCD_480X800__ */
#endif /* __MMI_MRE_APP_KAIXIN__ */

#if defined(__MMI_MRE_APP_YOUKU__)
#if defined(__MMI_MAINLCD_480X800__)
    <ThirdROMBIN id="MREAPPYOUKU">"mre\custom\romapp\youku\Youku_WVGA"</ThirdROMBIN>
#elif defined(__MMI_MAINLCD_320X480__)
    <ThirdROMBIN id="MREAPPYOUKU">"mre\custom\romapp\youku\Youku_HVGA"</ThirdROMBIN>
#elif defined(__MMI_MAINLCD_240X400__)
    <ThirdROMBIN id="MREAPPYOUKU">"mre\custom\romapp\youku\Youku_WQVGA"</ThirdROMBIN>
#else
    <ThirdROMBIN id="MREAPPYOUKU">"mre\custom\romapp\youku\Youku_QVGA"</ThirdROMBIN>
#endif /* __MMI_MAINLCD_480X800__ */
#endif /* __MMI_MRE_APP_YOUKU__ */
#ifdef __MMI_MRE_APP_WOCHACHA__
	
#if defined (__MMI_MAINLCD_240X400__)
<ThirdROMBIN id="MREAPPWCHCH">"mre\custom\romapp\wochacha\wcc_240X400.vxp"</ThirdROMBIN>
#endif

#if defined (__MMI_MAINLCD_240X320__)
<ThirdROMBIN id="MREAPPWCHCH">"mre\custom\romapp\wochacha\wcc_240X320.vxp"</ThirdROMBIN>
#endif

#if defined (__MMI_MAINLCD_320X480__)
<ThirdROMBIN id="MREAPPWCHCH">"mre\custom\romapp\wochacha\wcc_320X480.vxp"</ThirdROMBIN>
#endif

#if defined (__MMI_MAINLCD_480X800__)
<ThirdROMBIN id="MREAPPWCHCH">"mre\custom\romapp\wochacha\wcc_480X800.vxp"</ThirdROMBIN>
#endif

#endif

#if defined(__MMI_MRE_APP_SINA_WEIBO__) && (__MMI_MRE_APP_SINA_WEIBO__ == __MRE_APP_DEFAULT__)

#if defined (__MMI_MAINLCD_240X400__)
	<ThirdROMBIN id="MREAPPWEIBO">"mre\custom\romapp\Sina\weibo_WQVGA.vxp"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPWBVSM">"mre\custom\romapp\Sina\weibo_WQVGA.vsm"</ThirdROMBIN>
#endif

#if defined (__MMI_MAINLCD_240X320__)
	<ThirdROMBIN id="MREAPPWEIBO">"mre\custom\romapp\Sina\weibo_QVGA.vxp"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPWBVSM">"mre\custom\romapp\Sina\weibo_QVGA.vsm"</ThirdROMBIN>
#endif

#if defined (__MMI_MAINLCD_320X480__)
	<ThirdROMBIN id="MREAPPWEIBO">"mre\custom\romapp\Sina\weibo_HVGA.vxp"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPWBVSM">"mre\custom\romapp\Sina\weibo_HVGA.vsm"</ThirdROMBIN>
#endif

#if defined (__MMI_MAINLCD_480X800__)
	<ThirdROMBIN id="MREAPPWEIBO">"mre\custom\romapp\Sina\weibo_WVGA.vxp"</ThirdROMBIN>
	<ThirdROMBIN id="MREAPPWBVSM">"mre\custom\romapp\Sina\weibo_WVGA.vsm"</ThirdROMBIN>
#endif
	
#endif

#ifdef __MMI_MRE_APP_ZAKAT__	
    #ifdef __MMI_MAINLCD_176X220__
	      <ThirdROMBIN id="MREAPPZAKAT">"mre\custom\romapp\ToolApps\MREAppZakat_QCIF"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPZAKAT">"mre\custom\romapp\ToolApps\MREAppZakat_WQVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPZAKAT">"mre\custom\romapp\ToolApps\MREAppZakat_QVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X240__
	      <ThirdROMBIN id="MREAPPZAKAT">"mre\custom\romapp\ToolApps\MREAppZakat_QVGA_LS"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPZAKAT">"mre\custom\romapp\ToolApps\MREAppZakat_HVGA"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPZAKAT">"mre\custom\romapp\ToolApps\MREAppZakat_WVGA"</ThirdROMBIN>
    #endif
#endif

#ifdef __MMI_MRE_APP_ANGRYBIRDS_EN__	
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPAGYBD">"mre\custom\romapp\kkfun\MREAppAngrybird_QVGA_EN"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPAGYBD">"mre\custom\romapp\kkfun\MREAppAngrybird_WQVGA_EN"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPAGYBD">"mre\custom\romapp\kkfun\MREAppAngrybird_HVGA_EN"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPAGYBD">"mre\custom\romapp\kkfun\MREAppAngrybird_WVGA_EN"</ThirdROMBIN>
    #endif
#else 
#ifdef __MMI_MRE_APP_ANGRYBIRDS_SC__
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPAGYBD">"mre\custom\romapp\kkfun\MREAppAngrybird_QVGA_SC"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPAGYBD">"mre\custom\romapp\kkfun\MREAppAngrybird_WQVGA_SC"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPAGYBD">"mre\custom\romapp\kkfun\MREAppAngrybird_HVGA_SC"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPAGYBD">"mre\custom\romapp\kkfun\MREAppAngrybird_WVGA_SC"</ThirdROMBIN>
    #endif
#endif
#endif

#ifdef __MMI_MRE_APP_FISHINGFUN_EN__
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPFISHF">"mre\custom\romapp\kkfun\MREAppFishingFun_QVGA_EN"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPFISHF">"mre\custom\romapp\kkfun\MREAppFishingFun_WQVGA_EN"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPFISHF">"mre\custom\romapp\kkfun\MREAppFishingFun_HVGA_EN"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPFISHF">"mre\custom\romapp\kkfun\MREAppFishingFun_WVGA_EN"</ThirdROMBIN>
    #endif
#else 
#ifdef __MMI_MRE_APP_FISHINGFUN_SC__
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPFISHF">"mre\custom\romapp\kkfun\MREAppFishingFun_QVGA_SC"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPFISHF">"mre\custom\romapp\kkfun\MREAppFishingFun_WQVGA_SC"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPFISHF">"mre\custom\romapp\kkfun\MREAppFishingFun_HVGA_SC"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPFISHF">"mre\custom\romapp\kkfun\MREAppFishingFun_WVGA_SC"</ThirdROMBIN>
    #endif
#endif
#endif

#ifdef __MMI_MRE_APP_FRUITNINJA_EN__
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPFRUIT">"mre\custom\romapp\kkfun\MREAppFruitNinja_QVGA_EN"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPFRUIT">"mre\custom\romapp\kkfun\MREAppFruitNinja_WQVGA_EN"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPFRUIT">"mre\custom\romapp\kkfun\MREAppFruitNinja_HVGA_EN"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPFRUIT">"mre\custom\romapp\kkfun\MREAppFruitNinja_WVGA_EN"</ThirdROMBIN>
    #endif
#else 
#ifdef __MMI_MRE_APP_FRUITNINJA_SC__
    #ifdef __MMI_MAINLCD_240X320__
	      <ThirdROMBIN id="MREAPPFRUIT">"mre\custom\romapp\kkfun\MREAppFruitNinja_QVGA_SC"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_240X400__
	      <ThirdROMBIN id="MREAPPFRUIT">"mre\custom\romapp\kkfun\MREAppFruitNinja_WQVGA_SC"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_320X480__
	      <ThirdROMBIN id="MREAPPFRUIT">"mre\custom\romapp\kkfun\MREAppFruitNinja_HVGA_SC"</ThirdROMBIN>
    #endif
    #ifdef __MMI_MAINLCD_480X800__
	      <ThirdROMBIN id="MREAPPFRUIT">"mre\custom\romapp\kkfun\MREAppFruitNinja_WVGA_SC"</ThirdROMBIN>
    #endif
#endif
#endif
#if defined(__MMI_MRE_GAME_EGYPTCODE_EN__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_WVGA_EN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_EGYPTCODE_CHS__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPEGYPT">"mre\custom\romapp\mtone_games\ThirdROM\EGYPT_WVGA_CHS"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_GIRLSLOT_EN__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_WVGA_EN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_GIRLSLOT_CHS__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPGISLT">"mre\custom\romapp\mtone_games\ThirdROM\GIRLSLOT_WVGA_CHS"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_GOLDMINER_EN__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_WVGA_EN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X240__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_LQVGA_CHEN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_GOLDMINER_CHS__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_WVGA_CHS"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X240__)
  <ThirdROMBIN id="MREAPPGDMNR">"mre\custom\romapp\mtone_games\ThirdROM\GOLDENMINER_LQVGA_CHEN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_HAPPYEGG_EN__)
#if defined(__MMI_MAINLCD_176X220__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_WVGA_EN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_128X160__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_128160_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X240__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_LQVGA_CHEN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_HAPPYEGG_CHS__)
#if defined(__MMI_MAINLCD_176X220__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_WVGA_CHS"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_128X160__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_128160_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X240__)
 <ThirdROMBIN id="MREAPPHPEGG">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYEGG_LQVGA_CHEN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_HAPPYWAR_EN__)
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPHPWAR">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYWAR_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPHPWAR">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYWAR_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPHPWAR">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYWAR_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPHPWAR">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYWAR_WVGA_EN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_HAPPYWAR_CHS__)
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPHPWAR">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYWAR_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPHPWAR">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYWAR_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPHPWAR">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYWAR_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPHPWAR">"mre\custom\romapp\mtone_games\ThirdROM\HAPPYWAR_WVGA_CHS"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_KINGCOMBAT_EN__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_WVGA_EN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_KINGCOMBAT_CHS__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPKCBAT">"mre\custom\romapp\mtone_games\ThirdROM\KINGCOMBAT_WVGA_CHS"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_MOTO_EN__)
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPMOTOO">"mre\custom\romapp\mtone_games\ThirdROM\MOTO_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPMOTOO">"mre\custom\romapp\mtone_games\ThirdROM\MOTO_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPMOTOO">"mre\custom\romapp\mtone_games\ThirdROM\MOTO_WVGA_EN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_MOTO_CHS__)
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPMOTOO">"mre\custom\romapp\mtone_games\ThirdROM\MOTO_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPMOTOO">"mre\custom\romapp\mtone_games\ThirdROM\MOTO_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPMOTOO">"mre\custom\romapp\mtone_games\ThirdROM\MOTO_WVGA_CHS"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_PARKOUR_EN__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_WVGA_EN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_PARKOUR_CHS__)
#if defined(__MMI_MAINLCD_176X220__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_QCIF_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_QVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_WQVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_HVGA_CHEN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPPARKU">"mre\custom\romapp\mtone_games\ThirdROM\PARKOUR_WVGA_CHS"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_TEARDRESS_EN__)
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPTEAED">"mre\custom\romapp\mtone_games\ThirdROM\TEARDRESS_QVGA_EN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPTEAED">"mre\custom\romapp\mtone_games\ThirdROM\TEARDRESS_WQVGA_EN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPTEAED">"mre\custom\romapp\mtone_games\ThirdROM\TEARDRESS_HVGA_EN"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPTEAED">"mre\custom\romapp\mtone_games\ThirdROM\TEARDRESS_WVGA_EN"</ThirdROMBIN>
#endif
#endif

#if defined(__MMI_MRE_GAME_TEARDRESS_CHS__)
#if defined(__MMI_MAINLCD_240X320__)
  <ThirdROMBIN id="MREAPPTEAED">"mre\custom\romapp\mtone_games\ThirdROM\TEARDRESS_QVGA_CHS"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_240X400__)
  <ThirdROMBIN id="MREAPPTEAED">"mre\custom\romapp\mtone_games\ThirdROM\TEARDRESS_WQVGA_CHS"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_320X480__)
  <ThirdROMBIN id="MREAPPTEAED">"mre\custom\romapp\mtone_games\ThirdROM\TEARDRESS_HVGA_CHS"</ThirdROMBIN>
#endif
#if defined(__MMI_MAINLCD_480X800__)
  <ThirdROMBIN id="MREAPPTEAED">"mre\custom\romapp\mtone_games\ThirdROM\TEARDRESS_WVGA_CHS"</ThirdROMBIN>
#endif
#endif


#endif

#endif /* __MRE__ */ 
</APP>
