/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   mtk_gps_storage.c
 *
 * Project:
 * --------
 * 
 *
 * Description:
 * ------------
 *   This file implements porting layer file/NV functions
 *
 * Author:
 * -------
 * 
 *
 ****************************************************************************/
#include "fs_errcode.h"
#include "fs_type.h"
#include "fs_func.h"


#include "mtk_gps.h"
#include "kal_trace.h"

#include "mtk_gps_type.h"
#include "kal_general_types.h"
#include "kal_public_api.h"

#include "mnl_trc.h"



static FS_HANDLE hNonVolHandle = -1;  /* nonvol file handle */
#ifdef __GPS_MT3326__
static FS_HANDLE hEPOHandle = -1;     /* EPO file handle removed 20110824  changed*/
#endif

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_storage_open
 * DESCRIPTION
 *
 * RETURNS
 *  success(0)
 *****************************************************************************/

mtk_int32
mtk_gps_sys_storage_open (void)
{
  int iDrvLetter;
  WCHAR wszFilename[16];
  
  iDrvLetter = FS_GetDrive(FS_DRIVE_V_NORMAL, 1, 0);
  if (iDrvLetter > 0)
  {    
    kal_wsprintf(wszFilename, "%c:\\MTKGPS.dat", iDrvLetter);
    hNonVolHandle = FS_Open(wszFilename, FS_READ_WRITE | FS_CREATE);
    kal_print_string_trace(MOD_MNL,TRACE_FUNC ,"[file open]hNonVolHandle:%d. file:%s ",hNonVolHandle,wszFilename);

    MNL_TRACE((TRACE_INFO, MNL_TRC_INF_FILE_OPEN_NVFILE, 
        iDrvLetter, hNonVolHandle, FS_READ_WRITE | FS_CREATE));
    
    if (hNonVolHandle >= 0)
    {
      return  MTK_GPS_SUCCESS;
    }
  }
  else
  {
    MNL_TRACE((TRACE_WARNING, MNL_TRC_WRN_FILE_GET_DIRVE_FAIL));
  }
  
  return  MTK_GPS_ERROR;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_storage_close
 * DESCRIPTION
 *
 * RETURNS
 *  void
 *****************************************************************************/
void
mtk_gps_sys_storage_close (void)
{

  MNL_TRACE((TRACE_INFO, MNL_TRC_INF_FILE_CLOSE_NVFILE, hNonVolHandle));  

  if (hNonVolHandle >= 0)
  {  

    FS_Close(hNonVolHandle);
	hNonVolHandle = -1;
  }
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_storage_delete
 * DESCRIPTION
 *
 * RETURNS
 *  void
 *****************************************************************************/
void
mtk_gps_sys_storage_delete (void)
{
    int iDrvLetter;
    WCHAR wszFilename[32];

    iDrvLetter = FS_GetDrive(FS_DRIVE_V_NORMAL, 1, 0);

    if (iDrvLetter > 0)
    {
      MNL_TRACE((TRACE_INFO, MNL_TRC_INF_FILE_DEL_NVFILE, iDrvLetter));    
      kal_wsprintf(wszFilename, "%c:\\MTKGPS.dat", iDrvLetter);
      if (FS_Delete(wszFilename) == FS_NO_ERROR)
      {
		return;
      }
    }
    else
    {
      MNL_TRACE((TRACE_WARNING, MNL_TRC_WRN_FILE_GET_DIRVE_FAIL));
    }    
    return;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_storage_read
 * DESCRIPTION
 *
 *  blocking read until reaching 'length' or EOF
 * PARAMETERS
 *  buffer      [OUT]
 *  offset      [IN]
 *  length      [IN]
 *  p_nRead     [OUT]
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32
mtk_gps_sys_storage_read (void* buffer, mtk_uint32 offset, mtk_uint32 length,
                      mtk_uint32* p_nRead)
{
  UINT nRead;
  int  result;
  

  if (hNonVolHandle < 0)
  {
    return MTK_GPS_ERROR;
  }

  result = FS_Read(hNonVolHandle, buffer, length, &nRead);
  if (p_nRead)
  {
    *p_nRead = nRead;
  }
  return  result ? MTK_GPS_ERROR : MTK_GPS_SUCCESS;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_storage_write
 * DESCRIPTION
 *
 * PARAMETERS
 *  buffer      [IN]
 *  offset      [IN]
 *  length      [IN]
 *  p_nWritten  [OUT]
 * RETURNS
 *  success(0)
 *****************************************************************************/
mtk_int32
mtk_gps_sys_storage_write (const void* buffer, mtk_uint32 offset, mtk_uint32 length,
                       mtk_uint32* p_nWritten)
{
  UINT nWrite;
  int  result;

  
  if (hNonVolHandle < 0)
  {
    return MTK_GPS_ERROR;
  }

  result = FS_Write(hNonVolHandle, (void*) buffer, length, &nWrite);
  kal_print_string_trace(MOD_MNL,TRACE_FUNC ,"[file write]result:%d",result);
  if(0 != result)
  {
    MNL_TRACE((TRACE_WARNING, MNL_TRC_WRN_FILE_WRITE_NVFILE_FAIL));
  }
  if (p_nWritten)
  {
    *p_nWritten = nWrite;
  }
  return  result ? MTK_GPS_ERROR : MTK_GPS_SUCCESS;
}

#ifdef __GPS_MT3326__

/*****************************************************************************
 * FUNCTION
 *  mtk_gps_sys_epo_open
 * DESCRIPTION
 *  Open EPO file
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *****************************************************************************/
mtk_int32
mtk_gps_sys_epo_open (void)
{
    int iDrvLetter;
    WCHAR wszFilename[32];

    iDrvLetter = FS_GetDrive(FS_DRIVE_V_NORMAL, 1, 0);
    if (iDrvLetter > 0)
    {
        kal_wsprintf(wszFilename, "%c:\\MTK14.epo", iDrvLetter);
        hEPOHandle = FS_Open(wszFilename, FS_READ_WRITE);
        if (hEPOHandle >= 0)
        {
        return  MTK_GPS_SUCCESS;
        }
    }

    return  MTK_GPS_ERROR;
}

/*****************************************************************************
 * FUNCTION
 *  mtk_sys_epo_close
 * DESCRIPTION
 *  Close EPO file
 * RETURNS
 *  void
 *****************************************************************************/
void
mtk_gps_sys_epo_close (void)
{
    if (hEPOHandle >= 0)
    {
    FS_Close(hEPOHandle);
        hEPOHandle = -1;
    }
}

/*****************************************************************************
 * FUNCTION
 *  mtk_sys_epo_read
 * DESCRIPTION
 *  Read EPO file
 *  (blocking read until reaching 'length' or EOF)
 * PARAMETERS
 *  buffer      [OUT]
 *  offset      [IN]
 *  length      [IN]
 *  p_nRead     [OUT]
 * RETURNS
 *  success(MTK_GPS_SUCCESS)
 *****************************************************************************/
mtk_int32
mtk_gps_sys_epo_read (void* buffer, mtk_uint32 offset, mtk_uint32 length,
                      mtk_uint32* p_nRead)
{
    UINT nRead;
    int  result;

    if (hEPOHandle < 0)
    {
        return MTK_GPS_ERROR;
    }

    // Since MNL now read the EPO file sequentially,
    // we can ignore the offset parameter from MNL's request
    // otherwise we may need add FS_Seek() here
    result = FS_Read(hEPOHandle, buffer, length, &nRead);
    if (p_nRead)
    {
        *p_nRead = nRead;
    }
    return  result ? MTK_GPS_ERROR : MTK_GPS_SUCCESS;
}
#endif


