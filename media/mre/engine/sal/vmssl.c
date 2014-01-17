/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2006
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

/*******************************************************************************
 * Filename:
 * ---------
 * vmio.c
 *
 * Project:
 * --------
 *   MAUI
 *
 * Description:
 * ------------
 *   
 *
 * Author:
 * -------
 * 
 *
*==============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!! 
 *------------------------------------------------------------------------------
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * removed!
 *
 * removed!
 * removed!
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!! 
 *============================================================================== 
 *******************************************************************************/
#include "vmswitch.h"
#include "MMI_mre_trc.h"
#include "mmi_trc.h"

#ifdef __MRE_SAL_XYSSL__
#include "soc_consts.h"
#include "vmssl.h"
#include "vmsock.h"
#include "vmio.h"


#include "vmpromng.h"
#include "vmresmng.h"

#include "vmmod.h"



#include "xyssl_interface.h"

#define BUF_LEN 2048

typedef struct _vm_ssl_ctx_t
{
    //VMCHAR proxy_ip[16];
    //VMINT proxy_port;
    VMINT is_proxy;
    VMINT is_connected;
    VMINT is_handshaked;
    VMINT res_id;
    VMINT soc_id;
    VM_P_HANDLE p_hdl;
    vm_ssl_cntx ctx_app;
    xyssl_adp_context_struct ctx_xyssl;
}vm_ssl_ctx_t;


static void _vm_ssl_free_ctx(vm_ssl_ctx_t * ctx_p)
{    
    if (NULL == ctx_p)
    {
        return;
    }

    if (VM_RES_OK == vm_res_release_data(VM_RES_TYPE_SSL, ctx_p->res_id))
    {        
        if (ctx_p->soc_id > 0)
        {
            vm_tcp_close(ctx_p->soc_id);
            ctx_p->soc_id = -1;
        }
        vm_free(ctx_p);
        
    }

}
static void vm_ssl_free_resource(VM_P_HANDLE process_handle)
{
    VMINT resHandle = 0, bufSize = 0;
    vm_ssl_ctx_t* ctx_p = NULL;
    
    while ((resHandle = vm_res_findfirst(process_handle, VM_RES_TYPE_SSL)) 
        != VM_RES_NOT_FIND)
    {
        if (vm_res_get_data(VM_RES_TYPE_SSL, 
            resHandle, (void**)&ctx_p, &bufSize) == 0 && ctx_p != NULL)
        {
            // 
            _vm_ssl_free_ctx(ctx_p);
        }
        else
        {
            // 
        }
        vm_res_release_data(VM_RES_TYPE_SSL, resHandle);
        vm_res_findclose(VM_RES_TYPE_SSL); 
    }
}

static void vm_ssl_notify_process_status(VM_P_HANDLE process_handle, 
    VMINT sys_state)
{
    switch (sys_state)
    {
    case VM_PMNG_UNLOAD:
        vm_ssl_free_resource(process_handle);
        break;
    }
}

static VMINT initialize_ssl_resource(void)
{
    if (vm_res_type_set_notify_callback(VM_RES_TYPE_SSL, 
        vm_ssl_notify_process_status) != 0)
    {
        return -1;
    }

    return 0;
}

static VMINT finialize_ssl_resource(void)
{
    vm_ssl_free_resource(-1);
    
    vm_res_type_set_notify_callback(VM_RES_TYPE_SSL, NULL);

    return 0;
}

static VMINT ssl_mod_evt_proc(MRE_MOD_LIFECIRCLE_EVT event)
{
    switch(event)
    {
    case EVT_MOD_INIT:
        if (initialize_ssl_resource() != 0)
            return -1;
        break;
    case EVT_MOD_RELEASE:
        finialize_ssl_resource();
        break;
    case EVT_MOD_ACTIVE:
        break;
    case EVT_MOD_INACTIVE:
        break;
    default:
        return -2;
    }

    return 0;
}

static vm_ssl_ctx_t * _vm_ssl_malloc_ctx(void)
{
    vm_ssl_ctx_t * ctx_p = NULL;

    if ((ctx_p = vm_malloc_topmost(sizeof(vm_ssl_ctx_t))) != NULL)
    {
        memset(ctx_p, 0, sizeof(vm_ssl_ctx_t));
        if ((ctx_p->res_id = vm_res_save_data(VM_RES_TYPE_SSL, 
            (void*)ctx_p, sizeof(ctx_p), NULL, vm_pmng_get_current_handle())) >= 0)
        {
            ctx_p->p_hdl = vm_pmng_get_current_handle();
            ctx_p->soc_id = -1;
            return ctx_p;
        }
        else
        {
            vm_free(ctx_p);
        }
    }

    return NULL;
}

static vm_ssl_ctx_t * _vm_ssl_get_ctx_by_res_id(VMINT res_id)
{
    VMINT resHandle = 0, bufSize = 0;
    vm_ssl_ctx_t * ctx_p = NULL;

    if ((resHandle = vm_res_findfirst(-1, VM_RES_TYPE_SSL)) != VM_RES_NOT_FIND)
    {
        if (vm_res_get_data(VM_RES_TYPE_SSL, 
            resHandle, (void**)&ctx_p, &bufSize) == 0 && ctx_p != NULL)
        {
            if (ctx_p->res_id == res_id)
            {
                vm_res_findclose(VM_RES_TYPE_SSL);
                return ctx_p;
            }
        }

        while ((resHandle = vm_res_findnext(-1, VM_RES_TYPE_SSL)) != VM_RES_NOT_FIND)
        {
            if (vm_res_get_data(VM_RES_TYPE_SSL, 
            resHandle, (void**)&ctx_p, &bufSize) == 0 && ctx_p != NULL)
            {
                if (ctx_p->res_id == res_id)
                {
                    vm_res_findclose(VM_RES_TYPE_SSL);
                    return ctx_p;
                }
            }
        }
        vm_res_findclose(VM_RES_TYPE_SSL);
    }	
    return NULL;
}

static vm_ssl_ctx_t * _vm_ssl_get_ctx_by_soc_id(VMINT soc_id)
{
    VMINT resHandle = 0, bufSize = 0;
    vm_ssl_ctx_t * ctx_p = NULL;

    if ((resHandle = vm_res_findfirst(-1, VM_RES_TYPE_SSL)) != VM_RES_NOT_FIND)
    {
        if (vm_res_get_data(VM_RES_TYPE_SSL, 
            resHandle, (void**)&ctx_p, &bufSize) == 0 && ctx_p != NULL)
        {
            if (ctx_p->soc_id == soc_id)
            {
                vm_res_findclose(VM_RES_TYPE_SSL);
                return ctx_p;
            }
        }

        while ((resHandle = vm_res_findnext(-1, VM_RES_TYPE_SSL)) != VM_RES_NOT_FIND)
        {
            if (vm_res_get_data(VM_RES_TYPE_SSL, 
            resHandle, (void**)&ctx_p, &bufSize) == 0 && ctx_p != NULL)
            {
                if (ctx_p->soc_id == soc_id)
                {
                    vm_res_findclose(VM_RES_TYPE_SSL);
                    return ctx_p;
                }
            }
        }
        vm_res_findclose(VM_RES_TYPE_SSL);
    }	
    return NULL;
}

static int vm_ssl_int_read(void *p, unsigned char * data, int len)
{
    VMINT handle = *(VMINT*)p;
    VMINT ret = 0;

    ret = vm_tcp_read(handle, (void *)data, len);
    if (ret == 0)
        ret = SOC_WOULDBLOCK;

    return ret;
}

static int vm_ssl_int_write(void *p, unsigned char * data, int len)
{
    VMINT handle = *(VMINT*)p;
    VMINT ret = 0;
    
    ret = vm_tcp_write(handle, (void *)data, len);
    if (ret == 0)
        ret = SOC_WOULDBLOCK;

    return ret;
}

static void *vm_ssl_int_malloc(int size, void *user_data)
{
    VM_P_HANDLE phdl;
    void *p = NULL;
    vm_ssl_ctx_t *ctx_p = (vm_ssl_ctx_t *)user_data;
    if (NULL == ctx_p)
        return NULL;
        
    phdl = vm_res_get_process_handle(VM_RES_TYPE_SSL, ctx_p->res_id);
    if (vm_pmng_set_ctx(phdl) == VM_PMNG_OP_OK)
    {
        p = vm_malloc_topmost(size);
        memset(p, 0, size);
        vm_pmng_reset_ctx(); 
    }

    
    return p;
}

static void vm_ssl_int_free(void *p, void *user_data)
{
    VM_P_HANDLE phdl;
    vm_ssl_ctx_t *ctx_p = (vm_ssl_ctx_t *)user_data;
    if (NULL == ctx_p)
        return;
    
    phdl = vm_res_get_process_handle(VM_RES_TYPE_SSL, ctx_p->res_id);
    if (vm_pmng_set_ctx(phdl) == VM_PMNG_OP_OK)
    {
        vm_free(p);
        vm_pmng_reset_ctx(); 
    }
}
static VMINT vm_ssl_init(vm_ssl_ctx_t *ctx, const vm_ssl_cntx *ctx_out)
{
    int ret;
    //x509_cert * ca_chain;
    //VM_P_HANDLE phdl; 
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 21, __LINE__);            

    if (NULL == ctx_out->connection_callback ||
        NULL == ctx_out->host || 0 == ctx_out->port)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E1, 21, __LINE__);            
        return -2;
    }

    ctx->ctx_app.host = vm_malloc_topmost(strlen(ctx_out->host) + 1);
    ctx->ctx_app.host[strlen(ctx_out->host)] = 0;
    strncpy(ctx->ctx_app.host, ctx_out->host, strlen(ctx_out->host));
    if (ctx_out->ua)
    {
        ctx->ctx_app.ua = vm_malloc_topmost(strlen(ctx_out->ua) + 1);
        ctx->ctx_app.ua[strlen(ctx_out->ua)] = 0;
        strncpy(ctx->ctx_app.ua, ctx_out->ua, strlen(ctx_out->ua));
    }
    ctx->ctx_app.port = ctx_out->port;
    ctx->ctx_app.connection_callback = ctx_out->connection_callback;
    ctx->ctx_app.authmod = ctx_out->authmod;
    //ctx->ctx_app.ca_chain_load_callback = ctx_out->ca_chain_load_callback;
    
    ret = xyssl_adp_ssl_init(&(ctx->ctx_xyssl.xyssl_adp_cntx), vm_ssl_int_malloc, vm_ssl_int_free, (void *)ctx);
    if (0 != ret)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E2, 21, ret);            
        return ret;
    }
    
    //phdl = vm_res_get_process_handle(VM_RES_TYPE_SSL, ctx->res_id);
    //if (vm_pmng_set_ctx(phdl) == VM_PMNG_OP_OK)
    //{
    //    ctx->ctx_app.ca_chain_load_callback();
    //    vm_pmng_reset_ctx(); 
    //}

    
    
    xyssl_adp_ssl_set_ca_chain(&(ctx->ctx_xyssl.xyssl_adp_cntx), &(ctx->ctx_xyssl.xyssl_ca_chain), (char *)ctx->ctx_app.host); // of application to load trusted ca certificates
    xyssl_adp_ssl_set_endpoint(&(ctx->ctx_xyssl.xyssl_adp_cntx), XYSSL_ADP_SSL_IS_CLIENT);
    xyssl_adp_ssl_set_authmode(&(ctx->ctx_xyssl.xyssl_adp_cntx), ctx->ctx_app.authmod);
    xyssl_adp_ssl_set_session(&(ctx->ctx_xyssl.xyssl_adp_cntx), 0, 0, &(ctx->ctx_xyssl.xyssl_adp_ssn)); // TODO:
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 21, __LINE__);            
    return 0;
}

static VMINT vm_int_tcp_proxy_connected_cb(vm_ssl_ctx_t *ctx_p)
{

    VMINT request_buff_len, ret;
    void *request_buff;
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 91,  __LINE__);            

    request_buff = vm_malloc_topmost(1024);    
    memset(request_buff, 0, 1024);
    //use HTTP 1.0 as some proxy servers do not support HTTP 1.1
    _snprintf(request_buff, 1024, "CONNECT %s:%d HTTP/1.0\r\nHost: %s:%d\r\nUser-Agent: %s\r\n\r\n", 
        ctx_p->ctx_app.host, ctx_p->ctx_app.port, 
        ctx_p->ctx_app.host, ctx_p->ctx_app.port, ctx_p->ctx_app.ua ? ctx_p->ctx_app.ua : "");

    request_buff_len = strlen(request_buff);

    ret = vm_tcp_write(ctx_p->soc_id, request_buff, request_buff_len);
    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_LOG, 
        ctx_p->soc_id,
        ret,
        request_buff_len,
        ctx_p->p_hdl,
        0, 
        0,
        91, __LINE__);          
        
    if (ret == request_buff_len)
        ret = 0;
    //if proxy command write fails; wait for timer to expire by default.
    //if proxy command write succeeds; wait for command response.
    vm_free(request_buff);
    // TODO: ret => evt
    //return ret;
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 91,  __LINE__);            
    return 0;
}

static VMINT vm_int_tcp_proxy_read_cb(vm_ssl_ctx_t *ctx_p)
{
    VMBYTE *buf;
    VMINT ret, len = 0;

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 92,  __LINE__);            
    buf = vm_malloc_topmost(BUF_LEN);
    memset(buf, 0, BUF_LEN);
    while ((ret = vm_tcp_read (ctx_p->soc_id, buf + len, BUF_LEN - len)) > 0)
    {
        len += ret;
    }

    if (len <= 0)
    {
        vm_free(buf);        
        // TODO: ret => evt
        //return len;
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E1, 92,  __LINE__);            
        return 0;
    }
    
    if (!(strncmp(buf, "HTTP/1.0 200", 12/*strlen ("HTTP/1.0 200")*/)
        //for handling response of proxies which support HTTP/1.1
        && strncmp(buf, "HTTP/1.1 200", 12/*strlen ("HTTP/1.1 200")*/)))
    {
        ctx_p->is_connected = 1;
    }

    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_LOG, 
        ctx_p->soc_id,
        ctx_p->is_connected,
        ret,
        len,
        0, 
        0,
        92, __LINE__);          

    vm_free(buf);

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 92,  __LINE__);            
    return 0;
    
}

static VMINT vm_ssl_int_handshake(vm_ssl_ctx_t *ctx_p)
{
    int ret = 0;

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 93,  __LINE__);            
    
    if (0 != (ret = xyssl_adp_ssl_handshake(&(ctx_p->ctx_xyssl.xyssl_adp_cntx))))
    {
        
        switch (ret)
        {
        case SOC_WOULDBLOCK:
            ret = 0;
            break;
        case XYSSL_ERR_X509_CERT_VERIFY_FAILED:
            {
                ret = VM_SSL_EVT_CERTIFICATE_VALIDATION_FAILED;
            }break;
        default:
            {
                ret = VM_SSL_EVT_HANDSHAKE_FAILED;
            }break;
        }
        
        return ret;
    }

    if (SSL_HANDSHAKE_OVER == ctx_p->ctx_xyssl.xyssl_adp_cntx.state)
    {
        ctx_p->is_handshaked = 1;
        //ret = VM_SSL_EVT_CONNECTED;
    }

    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_LOG, 
        ret,
        ctx_p->is_connected,
        ctx_p->is_handshaked,
        ctx_p->ctx_xyssl.xyssl_adp_cntx.state,
        0, 
        0,
        93, __LINE__);          

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 93,  __LINE__);            
    return 0;
}


static void vm_int_tcp_cb(VMINT handle, VMINT event)
{

    vm_ssl_ctx_t *ctx_p = _vm_ssl_get_ctx_by_soc_id(handle);
    VMINT ret = 0;
    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 9,  __LINE__);  
    
    if (NULL == ctx_p)
    {
        return;
    }

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_LOG, 
        handle,
        event,
        ctx_p->is_handshaked,
        ctx_p->is_connected,
        0, 
        0,
        9, __LINE__);          
    
    switch(event) {

        case VM_TCP_EVT_CONNECTED: 
            {
                if (ctx_p->is_proxy)
                {
                    if (0 != (ret = vm_int_tcp_proxy_connected_cb(ctx_p)))
                        break;
                }
                else
                {
                    ctx_p->is_connected = 1;
                    if (0 != (ret = vm_ssl_int_handshake(ctx_p)))
                        break;
                    else if (ctx_p->is_handshaked)
                        event = VM_SSL_EVT_CONNECTED;
                }
                /*if proxy connection
                    create a secure tunnel
                    start ssl
                else
                    start ssl*/            
                    
            }
            break;
        case VM_TCP_EVT_CAN_WRITE:
            {
                if (!ctx_p->is_handshaked)
                {
                    if (0 != (ret = vm_ssl_int_handshake(ctx_p)))
                        break;
                    else if (ctx_p->is_handshaked)
                        event = VM_SSL_EVT_CONNECTED;
                }
                else
                {
                    //event = VM_SSL_EVT_CAN_WRITE;
                }
                /*if handshake is not over
                    then call ssl_handshake to continue handshaking
                else 
                    call ssl_write to write application data*/
            }
            break;
            /* This message means you can receive data from server */
        case VM_TCP_EVT_CAN_READ:
            {
                if (ctx_p->is_proxy)
                {
                    if (!ctx_p->is_connected)
                    {
                        if (0 != (ret = vm_int_tcp_proxy_read_cb(ctx_p)))
                            break;
                    }
                    if (!ctx_p->is_connected) 
                    {// step here, means read ok, but response failed.
                        ret = VM_SSL_EVT_PIPE_BROKEN;
                        break;
                    }
                }
                
                if (!ctx_p->is_handshaked)
                {
                    if (0 != (ret = vm_ssl_int_handshake(ctx_p)))
                        break;
                    else if (ctx_p->is_handshaked)
                        event = VM_SSL_EVT_CONNECTED;
                }
                else
                {
                    //event = VM_SSL_EVT_CAN_READ;
                    //ctx_p->ctx_app.connection_callback(ctx_p->res_id, VM_SSL_EVT_CAN_READ);
                }

                /*if handshake is not over
                    then call ssl_handshake to continue handshake
                else 
                    call ssl_read to read application data*/
            }
            break;
        case VM_TCP_EVT_PIPE_BROKEN:
        case VM_TCP_EVT_HOST_NOT_FOUND:
            ret = event;
            break;
        default:
            break;
    }

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_LOG, 
        handle,
        event,
        ctx_p->is_handshaked,
        ctx_p->is_connected,
        ctx_p->ctx_app.connection_callback, 
        ret,
        9, __LINE__);      

    if (0 != ret) // exception
    {
        if (vm_pmng_set_ctx(ctx_p->p_hdl) == VM_PMNG_OP_OK)
        {
            ctx_p->ctx_app.connection_callback(ctx_p->res_id, ret);
            vm_pmng_reset_ctx();
        }
        vm_ssl_close(ctx_p->res_id);
    }
    
    if (ctx_p->is_connected && ctx_p->is_handshaked) // normally
    {
        if (vm_pmng_set_ctx(ctx_p->p_hdl) == VM_PMNG_OP_OK)
        {
            ctx_p->ctx_app.connection_callback(ctx_p->res_id, event);
            vm_pmng_reset_ctx();
        }
    }
    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 9,  __LINE__);  
}

static int vm_ssl_int_conn(vm_ssl_ctx_t *ctx_p) /* if return < 0, tcp failed */
{
    VMINT retval;
    VMINT apn;
    vm_apn_info_ext apn_info = {0};

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 22, __LINE__);            

    retval = vm_get_default_apn_info(&apn_info);

    if (vm_is_support_wifi() && vm_wifi_is_connected()) {
        apn = VM_TCP_APN_WIFI;
    }
    else if ((0 == retval && 0 == apn_info.apn_info_id)
        || 0 != retval  /*means error in getting apn_info, assume proxy not present*/) {
            apn = VM_TCP_APN_CMNET;
    }
    else {
        //proxy present.
        apn = VM_TCP_APN_CMWAP;
    }

    if (apn == VM_TCP_APN_CMWAP) {
        //tcp_backup_and_update_parameters(server, port);
        //ctx_p->proxy_port = apn_info.proxy_port;
        //memcpy(ctx_p->proxy_ip, &apn_info.proxy_ip, sizeof(ctx_p->proxy_ip));
        ctx_p->is_proxy = 1;
        retval = vm_tcp_connect(apn_info.proxy_ip, apn_info.proxy_port, apn, vm_int_tcp_cb); 
    }
    else {
        retval = vm_tcp_connect (ctx_p->ctx_app.host, ctx_p->ctx_app.port, apn, vm_int_tcp_cb);
    }

    if (retval > 0)
        ctx_p->soc_id = retval;

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_LOG, 
        apn,
        retval,
        ctx_p->res_id,
        ctx_p->ctx_app.port,
        0, 
        0,
        22, __LINE__);          
    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 22, __LINE__);            
    return ctx_p->soc_id;
}


VMINT _vm_reg_ssl_module(void)
{
    int res_code = REG_MRE_MODULE_SUCCESS;
    
    if ((res_code = _vm_reg_module("SSL", 
        (MOD_EVT_PROCESS)ssl_mod_evt_proc)) != REG_MRE_MODULE_SUCCESS)
    {
        //MMI_TRACE(MMI_MRE_TRC_ERROR, TRC_MRE_VMSOCK_672 , res_code);
    }

    return res_code;
}

VMINT vm_ssl_is_support(void)
{    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 1, __LINE__);            
    return 1;
}

VMINT vm_ssl_connect(const vm_ssl_cntx *ctx)
{
    //VMINT ret;
    vm_ssl_ctx_t *ctx_p = NULL;

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 2, __LINE__);            

    if (NULL == ctx)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E1, 2, __LINE__);            
        return -2;
    }

    ctx_p = _vm_ssl_malloc_ctx();
    
    if (NULL == ctx_p)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E2, 2, __LINE__);            
        return -3;
    }



    if (0 != vm_ssl_init(ctx_p, ctx))
    {
        _vm_ssl_free_ctx(ctx_p);
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E3, 2, __LINE__);            
        return -4;
    }

    if (0 > vm_ssl_int_conn(ctx_p))
    {
        _vm_ssl_free_ctx(ctx_p);
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E4, 2, __LINE__);            
        return -5;
    }
    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_LOG, 
        ctx->authmod,
        ctx->port,
        ctx_p->res_id,
        ctx->host,
        ctx->connection_callback, 
        0,
        2, __LINE__);    
        
    xyssl_adp_ssl_set_bio(&(ctx_p->ctx_xyssl.xyssl_adp_cntx), vm_ssl_int_read, &(ctx_p->soc_id), vm_ssl_int_write, &(ctx_p->soc_id));
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 2, __LINE__);            
    return ctx_p->res_id;
}

VMINT vm_ssl_load_ca_chain_cert(VMINT handle, const void *cert_buf, VMINT buf_len)
{
    //ssl_set_ca_chain(ssl_context * ssl, (x509_cert*)cert_buf,char * peer_cn)
    int ret;
    vm_ssl_ctx_t *ctx_p = _vm_ssl_get_ctx_by_res_id(handle);
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 3, handle);            
    if (NULL == ctx_p)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E1, 3, __LINE__);            
        return -2;
    }
    
    ret = xyssl_adp_x509parse_crt(&(ctx_p->ctx_xyssl.xyssl_adp_cntx), (x509_cert*)&(ctx_p->ctx_xyssl.xyssl_ca_chain), (unsigned char *)cert_buf, buf_len);
    if (0 != ret)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E2, 3, ret);            
        return ret;
    }
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 3, __LINE__);            
    return 0;
}

/*
VMINT vm_ssl_handshake(VMINT handle)
{
    int ret;
    vm_ssl_ctx_t * ctx_p = _vm_ssl_get_ctx_by_res_id(handle);
    if (NULL == ctx_p)
    {
        return -2;
    }
    

    ret = xyssl_adp_ssl_handshake(&(ctx_p->ctx_xyssl.xyssl_adp_cntx));
    if (0 != ret)
    {
        return ret;
    }
    return 0;
}
*/

VMINT vm_ssl_read(VMINT handle, VMUINT8 *buf, VMINT len)
{
    int ret;
    vm_ssl_ctx_t * ctx_p = _vm_ssl_get_ctx_by_res_id(handle);
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 4, handle);            
    if (NULL == ctx_p)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E1, 4, __LINE__);            
        return -2;
    }
    
    ret = xyssl_adp_ssl_read(&(ctx_p->ctx_xyssl.xyssl_adp_cntx), (unsigned char *)buf, len);
    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 4, ret);            

    if (ret == SOC_WOULDBLOCK)
        ret = 0;

    if (ret == XYSSL_ERR_SSL_PEER_CLOSE_NOTIFY)
        ret = VM_TCP_READ_EOF;

    return ret;
}

VMINT vm_ssl_write(VMINT handle, const VMUINT8 *buf, VMINT len)
{
    int ret;
    vm_ssl_ctx_t * ctx_p = _vm_ssl_get_ctx_by_res_id(handle);
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 5, handle);            
    if (NULL == ctx_p)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E1, 5, __LINE__);            
        return -2;
    }
    
    ret = xyssl_adp_ssl_write(&(ctx_p->ctx_xyssl.xyssl_adp_cntx), (unsigned char *)buf, len);

    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 5, ret);            

    if (ret == SOC_WOULDBLOCK)
        ret = 0;

    return ret;
}

VMINT vm_ssl_close(VMINT handle)
{
    vm_ssl_ctx_t * ctx_p = _vm_ssl_get_ctx_by_res_id(handle);
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 6, handle);            
    if (NULL == ctx_p)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E1, 6, __LINE__);            
        return -2;
    }

    xyssl_adp_ssl_close_notify(&(ctx_p->ctx_xyssl.xyssl_adp_cntx));
    
    xyssl_adp_ssl_deinit(&(ctx_p->ctx_xyssl.xyssl_adp_cntx));
    xyssl_adp_x509_free(&(ctx_p->ctx_xyssl.xyssl_adp_cntx), &(ctx_p->ctx_xyssl.xyssl_ca_chain));
    if (ctx_p->ctx_app.host)
        vm_free(ctx_p->ctx_app.host);
    if (ctx_p->ctx_app.ua)
        vm_free(ctx_p->ctx_app.ua);
    _vm_ssl_free_ctx(ctx_p);
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 6, __LINE__);            
    return 0;
}

VMINT vm_ssl_get_verify_result(VMINT handle)
{
    int ret;
    vm_ssl_ctx_t * ctx_p = _vm_ssl_get_ctx_by_res_id(handle);
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 7, handle);            
    if (NULL == ctx_p)
    {
        MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E1, 7, __LINE__);            
        return -2;
    }

    ret = xyssl_adp_ssl_get_verify_result(&(ctx_p->ctx_xyssl.xyssl_adp_cntx));
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 7, ret);            

    return ret;
}

VMINT vm_ssl_base64_encode(VMBYTE *dst, VMINT *dlen, VMBYTE *src, VMINT slen)
{
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 10, __LINE__);            
    return (VMINT)xyssl_adp_base64_encode((unsigned char *)dst, (int *)dlen, (unsigned char *)src, (int)slen);
}

VMINT vm_ssl_base64_decode(VMBYTE *dst, VMINT *dlen, VMBYTE *src, VMINT  slen)
{
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 11, __LINE__);            
    return (VMINT)xyxxl_adp_base64_decode((unsigned char *)dst, (int *)dlen, (unsigned char *)src, (int)slen);
}

VMINT vm_ssl_sha1_hmac(VMBYTE *key, VMINT keylen, VMBYTE *input, VMINT ilen, VMBYTE output[20])
{
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 12, __LINE__);            
    xyssl_adp_sha1_hmac((unsigned char *)key, (int)keylen, (unsigned char *)input, (int)ilen, (unsigned char *)output);
    return 0;
}

VMINT vm_get_ssl_sym(VMCHAR* func_name)
{
    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_S, 8, __LINE__);       
    
    if(strcmp(func_name,"vm_ssl_is_support") == 0)
        return (VMINT)vm_ssl_is_support;
    if(strcmp(func_name,"vm_ssl_connect") == 0)
        return (VMINT)vm_ssl_connect;        
    if(strcmp(func_name,"vm_ssl_load_ca_chain_cert") == 0)
        return (VMINT)vm_ssl_load_ca_chain_cert;
    if(strcmp(func_name,"vm_ssl_read") == 0)
        return (VMINT)vm_ssl_read;        
    if(strcmp(func_name,"vm_ssl_write") == 0)
        return (VMINT)vm_ssl_write;
    if(strcmp(func_name,"vm_ssl_close") == 0)
        return (VMINT)vm_ssl_close;        
    if(strcmp(func_name,"vm_ssl_get_verify_result") == 0)
        return (VMINT)vm_ssl_get_verify_result;   
    if(strcmp(func_name,"vm_ssl_base64_encode") == 0)
        return (VMINT)vm_ssl_base64_encode;   
    if(strcmp(func_name,"vm_ssl_base64_decode") == 0)
        return (VMINT)vm_ssl_base64_decode;   
    if(strcmp(func_name,"vm_ssl_sha1_hmac") == 0)
        return (VMINT)vm_ssl_sha1_hmac;   

    
    MMI_TRACE(MMI_MRE_TRC_MOD_VMSOCK, TRC_MRE_XYSSL_E, 8, func_name);            
    return 0;

    
}




#endif

