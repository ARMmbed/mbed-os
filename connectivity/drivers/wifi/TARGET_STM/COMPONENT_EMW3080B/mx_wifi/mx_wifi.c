/**
  ******************************************************************************
  * @file    mx_wifi.c
  * @author  MCD Application Team
  * @brief   Host driver API of MXCHIP Wi-Fi component.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "mx_wifi.h"
#include "core/mx_wifi_ipc.h"

/* Private defines -----------------------------------------------------------*/
/* debug log function */
#ifdef MX_WIFI_API_DEBUG
#define DEBUG_LOG(M, ...)  (void) printf((M), ##__VA_ARGS__)
#else
#define DEBUG_LOG(M, ...)
#endif /* MX_WIFI_API_DEBUG */

#define MX_HTONL(A)   ((((uint32_t)(A) & 0xff000000U) >> 24) | \
                       (((uint32_t)(A) & 0x00ff0000U) >>  8) | \
                       (((uint32_t)(A) & 0x0000ff00U) <<  8) | \
                       (((uint32_t)(A) & 0x000000ffU) << 24))

#define MX_NTOHL       MX_HTONL


#define MX_IN_RANGE(c, lo, up)  (((c) >= (lo)) && ((c) <= (up)))
#define MX_IS_PRINT(c)          (MX_IN_RANGE((c), 0x20, 0x7f))
#define MX_ISDIGIT(c)           (MX_IN_RANGE((c), '0', '9'))
#define MX_ISXDIGIT(c)          (MX_ISDIGIT(c) || MX_IN_RANGE((c), 'a', 'f') || MX_IN_RANGE((c), 'A', 'F'))
#define MX_ISLOWER(c)           (MX_IN_RANGE((c), 'a', 'z'))
#define MX_ISSPACE(c)           (((c) == ' ')\
                                 || ((c) == '\f') || ((c) == '\n') || ((c) == '\r') || ((c) == '\t') || ((c) == '\v'))

#if 0
/* select function file descriptions */
fd_set mc_select_rfds;
fd_set mc_select_wfds;
fd_set mc_select_efds;
int32_t mc_select_ret_value;
#endif /* 0 */

MX_STAT_DECLARE();
void _MX_WIFI_RecvThread(void* );

/* Private data types -----------------------------------------------*/
typedef struct mx_in_addr
{
  uint32_t s_addr;
} mx_in_addr_t;

typedef struct
{
  uint32_t addr;
} mx_ip4_addr_t;

typedef mx_ip4_addr_t mx_ip_addr_t;



#define MX_SIN_ZERO_LEN    8
typedef struct mx_sockaddr_in
{
  uint8_t       sin_len;
  uint8_t       sin_family;
  uint16_t      sin_port;
  mx_in_addr_t  sin_addr;
  int8_t        sin_zero[MX_SIN_ZERO_LEN];
} mx_sockaddr_in_t;


/* Private functions ---------------------------------------------------------*/
static int32_t mx_aton(const int8_t *ptr, mx_ip_addr_t *addr);
static int32_t mx_aton_r(const int8_t *cp);
static int8_t *mx_ntoa(const mx_ip4_addr_t *addr);

/**
  * @brief  Function description
  * @param  Params
  * @retval socket status
  */

static int32_t mx_aton(const int8_t *ptr, mx_ip_addr_t *addr)
{
  uint32_t val = 0;
  uint32_t base;
  int8_t c0;
  const int8_t *cp = ptr;
  uint32_t parts[4];
  uint32_t *pp = parts;
  int32_t ret = 1;
  int32_t done;

  c0 = *cp;
  done = 0;
  for (;;)
  {
    /*
     * Collect number up to ``.''.
     * Values are specified as for C:
     * 0x=hex, 0=octal, 1-9=decimal.
     */
    if (done == 1)
    {
      break;
    }

    if (!MX_ISDIGIT(c0))
    {
      ret = 0;
      done = 1;
    }
    else
    {
      val = 0;
      base = 10;
      if (c0 == '0')
      {
        ++cp;
        c0 = (int8_t) * cp;
        if ((c0 == (int8_t) 'x') || (c0 == (int8_t) 'X'))
        {
          base = 16;
          ++cp;
          c0 = (int8_t) * cp;
        }
        else
        {
          base = 8;
        }
      }

      for (;;)
      {
        if (MX_ISDIGIT(c0))
        {
          val = (val * base) + (uint32_t)c0 - (uint32_t) '0';
          ++cp;
          c0 = (int8_t) * cp;
        }
        else if ((base == 16U) && MX_ISXDIGIT(c0))
        {
          val = (val << 4) | ((uint32_t)c0 + 10U - (uint32_t)(MX_ISLOWER(c0) ? 'a' : 'A'));
          ++cp;
          c0 = (int8_t) * cp;
        }
        else
        {
          break;
        }
      }

      if (c0 == '.')
      {
        /*
         * Internet format:
         *  a.b.c.d
         *  a.b.c   (with c treated as 16 bits)
         *  a.b (with b treated as 24 bits)
         */
        if (pp >= (parts + 3))
        {
          ret = 0;
          done = 1;
        }
        else
        {
          *pp = val;
          pp++;
          ++cp;
          c0 = (int8_t) * cp;
        }
      }
      else
      {
        done = 1;
      }
    }
  }
  /*
   * Check for trailing characters.
   */
  if ((c0 != (int8_t)'\0') && (MX_ISSPACE((c0)) == false))
  {
    ret = 0;
  }
  else

    /*
     * Concoct the address according to
     * the number of parts specified.
     */
  {
    switch (pp - parts + 1)
    {
      case 0:
        ret = 0;          /* initial nondigit */
        break;

      case 1:             /* a -- 32 bits */
        break;

      case 2:             /* a.b -- 8.24 bits */
        if (val > 0xffffffUL)
        {
          ret = 0;
        }
        val |= parts[0] << 24;
        break;

      case 3:             /* a.b.c -- 8.8.16 bits */
        if (val > 0xffffU)
        {
          ret = 0;
          break;
        }
        val |= (parts[0] << 24) | (parts[1] << 16);
        break;

      case 4:             /* a.b.c.d -- 8.8.8.8 bits */
        if (val > 0xffU)
        {
          ret = 0;
          break;
        }
        val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
        break;

      default:
        ret = 0;
        break;
    }
  }

  if (ret == 1)
  {
    if (addr != NULL)
    {
      addr->addr = MX_HTONL(val);
    }
  }
  return ret;
}




static int32_t mx_aton_r(const int8_t *cp)
{
  mx_ip_addr_t val;
  int32_t       ret;
  val.addr = 0;
  if (mx_aton(cp, &val) != 0)
  {
    ret = (int32_t) val.addr;
  }
  else
  {
    ret = 0;
  }
  return (ret);
}



/**
  * @brief                   Register physical interface IO operation function
  * @param  Obj              mxchip wifi object
  * @param  IO_Init          IO init function
  * @param  IO_DeInit        IO deinit function
  * @param  IO_Delay         IO delay function
  * @param  IO_Send          IO send function
  * @param  IO_Receive       IO recv function
  * @return MX_WIFI_STATUS_T status code
  */
MX_WIFI_STATUS_T MX_WIFI_RegisterBusIO(MX_WIFIObject_t *Obj,
                                       IO_Init_Func IO_Init,
                                       IO_DeInit_Func IO_DeInit,
                                       IO_Delay_Func IO_Delay,
                                       IO_Send_Func IO_Send,
                                       IO_Receive_Func IO_Receive)
{
  MX_WIFI_STATUS_T rc;

  if ((NULL == Obj) || (NULL == IO_Init) || (NULL == IO_DeInit) || (NULL == IO_Send) || \
      (NULL == IO_Receive) || (NULL == IO_Delay))
  {
    rc = MX_WIFI_STATUS_ERROR;
  }
  else
  {
    Obj->fops.IO_Init = IO_Init;
    Obj->fops.IO_DeInit = IO_DeInit;
    Obj->fops.IO_Send = IO_Send;
    Obj->fops.IO_Receive = IO_Receive;
    Obj->fops.IO_Delay = IO_Delay;
    rc = MX_WIFI_STATUS_OK;
  }
  return rc;
}

/**
  * @brief                   reset wifi module by hardware
  * @param  Obj              wifi object
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_HardResetModule(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T rc;
  int32_t ret;


  MX_STAT_INIT();

  if (NULL == Obj)
  {
    rc = MX_WIFI_STATUS_ERROR;
  }
  else
  {
    /* reset Wi-Fi by reset pin */
    ret = Obj->fops.IO_Init(MX_WIFI_RESET);
    if ((int32_t)0 == ret)
    {
      rc = MX_WIFI_STATUS_OK;
    }
    else
    {
      rc = MX_WIFI_STATUS_ERROR;
    }
  }

  return rc;
}

/**
  * @brief                   set timeout for mxchip wifi API
  * @param  Obj              wifi object
  * @param  Timeout          timeout in ms
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_SetTimeout(MX_WIFIObject_t *Obj, uint32_t Timeout)
{
  MX_WIFI_STATUS_T rc;

  if (NULL == Obj)
  {
    rc = MX_WIFI_STATUS_ERROR;
  }
  else
  {
    Obj->Runtime.Timeout = Timeout;
    rc = MX_WIFI_STATUS_OK;
  }
  return rc;
}

/*
 * run with RTOS
 */
THREAD_DECLARE(MX_WIFI_RecvThreadId);


/**
  * @brief                   wifi stat recv thread for RTOS
  * @param  argument         thread arguments
  */
void _MX_WIFI_RecvThread(void* arg)
{
  MX_WIFIObject_t *Obj = wifi_obj_get();

  if (NULL != Obj)
  {
    while (true)
    {
      (void) mipc_poll(500);

    }
  }
  /* Delete the Thread */
  THREAD_TERMINATE(MX_WIFI_RecvThreadId);
}



/**
  * @brief                   wifi init
  * @param  Obj              wifi object
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_Init(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;
  uint16_t rparams_size;
  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    if (Obj->Runtime.interfaces == 0u)
    {
      LOCK_INIT(Obj->lockcmd);

      /* 0. set cmd timeout */
      Obj->Runtime.Timeout = MX_WIFI_CMD_TIMEOUT;

      /* 1. init wifi low level IO(UART/SPI) */
      (void)(Obj->fops.IO_Init(MX_WIFI_INIT));
      {
        /* 2. init wifi ipc */
        if (MIPC_CODE_SUCCESS == mipc_init(Obj->fops.IO_Send))
        {

          /* 2a start thread for RTOS implementation  */

          if (THREAD_OK != THREAD_INIT(MX_WIFI_RecvThreadId, _MX_WIFI_RecvThread, NULL,
                                       MX_WIFI_RECEIVED_THREAD_STACK_SIZE,
                                       MX_WIFI_RECEIVED_THREAD_PRIORITY))
          {
            ret = MX_WIFI_STATUS_ERROR;
          }
          else
          {
            /* 3. get version */
            (void)memset(&(Obj->SysInfo.FW_Rev[0]), 0, MX_WIFI_FW_REV_SIZE);
            rparams_size = MX_WIFI_FW_REV_SIZE;
            if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SYS_VERSION_CMD,
                                                  NULL, 0,
                                                  &(Obj->SysInfo.FW_Rev[0]),
                                                  &rparams_size,
                                                  MX_WIFI_CMD_TIMEOUT))
            {
              (void)strncpy((char *)(Obj->SysInfo.Product_Name),
                            MX_WIFI_PRODUCT_NAME, MX_WIFI_PRODUCT_NAME_SIZE);
              (void)strncpy((char *)(Obj->SysInfo.Product_ID),
                            MX_WIFI_PRODUCT_ID, MX_WIFI_PRODUCT_ID_SIZE);

              /* 4. get MAC */
              (void)memset(&(Obj->SysInfo.MAC[0]), 0, MX_WIFI_MAC_SIZE);
              rparams_size = MX_WIFI_MAC_SIZE;
              if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_GET_MAC_CMD,
                                                    NULL, 0,
                                                    &(Obj->SysInfo.MAC[0]),
                                                    &rparams_size,
                                                    MX_WIFI_CMD_TIMEOUT))
              {
                ret = MX_WIFI_STATUS_OK;
                Obj->Runtime.interfaces++;
              } /* get MAC */
            } /* get version */
          }
        }
      }
    }
    else
    {
      Obj->Runtime.interfaces++;
      ret = MX_WIFI_STATUS_OK;
    }
  }

  return ret;
}

/**
  * @brief                   wifi deinit
  * @param  Obj              wifi object
  * @return MX_WIFI_STATUS_T
  */
MX_WIFI_STATUS_T MX_WIFI_DeInit(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T ret;

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    if (Obj->Runtime.interfaces == 1u)
    {
      (void) THREAD_DEINIT(MX_WIFI_RecvThreadId);
      (void)mipc_deinit();
      Obj->fops.IO_DeInit();
      ret = MX_WIFI_STATUS_OK;
      Obj->Runtime.interfaces--;
    }
    else
    {
      ret = MX_WIFI_STATUS_OK;
      if (Obj->Runtime.interfaces > 0u)
      {
        Obj->Runtime.interfaces--;
      }
    }
  }

  return ret;
}

/**
  * @brief                   wifi data yield
  * @param  Obj              wifi object
  * @param  timeout          timetou in ms
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_IO_YIELD(MX_WIFIObject_t *Obj, uint32_t timeout)
{

  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_OK;
  if (NULL != Obj)
  {
#if MX_WIFI_BARE_OS
    mipc_poll(timeout);
#endif
  }
  return ret;
}

/**
  * @brief                   wifi software reboot
  * @param  Obj              wifi object
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_ResetModule(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SYS_REBOOT_CMD, NULL, 0,
                                          NULL, 0, MX_WIFI_CMD_TIMEOUT))
    {
      ret = MX_WIFI_STATUS_OK;
    }
  }
  return ret;
}

/**
  * @brief                   wifi factory reset
  * @param  Obj              wifi object
  * @return MX_WIFI_STATUS_T
  */
MX_WIFI_STATUS_T MX_WIFI_ResetToFactoryDefault(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SYS_RESET_CMD, NULL, 0,
                                          NULL, 0, MX_WIFI_CMD_TIMEOUT))
    {
      ret = MX_WIFI_STATUS_OK;
    }
  }
  return ret;
}

/**
  * @brief                   wifi get firmware version
  * @param  Obj              wifi object
  * @param  version          buffer to get version
  * @param  size             buffer size
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_GetVersion(MX_WIFIObject_t *Obj, uint8_t *version, uint32_t size)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;
  uint16_t rparams_size;

  if ((NULL == Obj) || (NULL == version) || (size > (uint32_t)MX_WIFI_FW_REV_SIZE) \
      || ((uint32_t)0 == size))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    (void)memset(&(Obj->SysInfo.FW_Rev[0]), 0, MX_WIFI_FW_REV_SIZE);
    rparams_size = MX_WIFI_FW_REV_SIZE;
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SYS_VERSION_CMD, NULL, 0,
                                          &(Obj->SysInfo.FW_Rev[0]),
                                          &rparams_size, MX_WIFI_CMD_TIMEOUT))
    {
      (void)memcpy(version, &(Obj->SysInfo.FW_Rev[0]), MX_WIFI_FW_REV_SIZE);
      ret = MX_WIFI_STATUS_OK;
    }

  }
  return ret;
}

/**
  * @brief                   wifi get MAC address
  * @param  Obj              wifi object
  * @param  mac              buffer to get MAC address (6 bytes)
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_GetMACAddress(MX_WIFIObject_t *Obj, uint8_t *mac)
{
  MX_WIFI_STATUS_T ret;

  if ((NULL == Obj) || (NULL == mac))
  {
    ret = MX_WIFI_STATUS_ERROR;
  }
  else
  {
    (void)memcpy(mac, Obj->SysInfo.MAC, MX_WIFI_MAC_SIZE);
    ret = MX_WIFI_STATUS_OK;
  }

  return ret;
}

/**
  * @brief                   wifi scan start
  * @param  Obj              wifi object
  * @param  scan_mode        scan mode
  * @param  ssid             ssid to scan
  * @param  len              max number to scan
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_Scan(MX_WIFIObject_t *Obj, mc_wifi_scan_mode_t scan_mode,
                              char *ssid, int32_t len)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;
  wifi_scan_cparams_t cparams;
  wifi_scan_rparams_t *rparams_p = (wifi_scan_rparams_t *)(&(Obj->Runtime.scan_result[0]));
  uint16_t rparams_size;

  if ((NULL == Obj) || (((mc_wifi_scan_mode_t)MC_SCAN_ACTIVE == scan_mode) &&
                        ((NULL == ssid) || (len <= 0) || (len > 32))))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    memset(&cparams, 0, sizeof(cparams));
    memcpy(&(cparams.ssid[0]), ssid, len);
    rparams_size = MX_WIFI_SCAN_BUF_SIZE;
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_SCAN_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)rparams_p, &rparams_size,
                                          MX_WIFI_SCAN_TIMEOUT))
    {
      Obj->Runtime.scan_number = rparams_p->num;
      memcpy(&(Obj->Runtime.scan_result[0]), &(rparams_p->ap[0]),
             Obj->Runtime.scan_number * sizeof(mwifi_ap_info_t));
      ret = MX_WIFI_STATUS_OK;
    }
  }
  return ret;
}

/**
  * @brief                   get wifi scan result
  * @param  Obj              wifi object
  * @param  results          buffer to get scan result
  * @param  number           max ap number to get
  * @return int8_t           ap number got
  */
int8_t MX_WIFI_Get_scan_result(MX_WIFIObject_t *Obj, uint8_t *results, uint8_t number)
{
  int8_t copy_number;

  if ((NULL == Obj) || (NULL == results) || ((uint8_t)0 == number))
  {
    copy_number = 0;
  }
  else
  {
    copy_number = (int8_t)(MIN(Obj->Runtime.scan_number, number));
    (void)memcpy(results, Obj->Runtime.scan_result, (size_t)copy_number * sizeof(mwifi_ap_info_t));
  }
  return copy_number;
}

/**
  * @brief                   register wifi station status event callback
  * @param  Obj              wifi object
  * @param  cb               callback function
  * @param  arg              callback argument
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_RegisterStatusCallback(MX_WIFIObject_t *Obj,
                                                mx_wifi_status_callback_t cb,
                                                void *arg)
{
  MX_WIFI_STATUS_T rc;

  if (NULL == Obj)
  {
    rc = MX_WIFI_STATUS_ERROR;
  }
  else
  {
    Obj->Runtime.status_cb[0] = cb;
    Obj->Runtime.callback_arg[0] = arg;
    rc = MX_WIFI_STATUS_OK;
  }
  return rc;
}

/**
  * @brief                   unregister wifi station event callback function
  * @param  Obj              wifi object
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_UnRegisterStatusCallback(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T rc;

  if (NULL == Obj)
  {
    rc = MX_WIFI_STATUS_ERROR;
  }
  else
  {
    Obj->Runtime.status_cb[0] = NULL;
    Obj->Runtime.callback_arg[0] = NULL;
    rc = MX_WIFI_STATUS_OK;
  }
  return rc;
}

/**
  * @brief                   register wifi event callback for station or AP mode
  * @param  Obj              wifi object
  * @param  cb               callback function
  * @param  arg              callback argument
  * @param  interface        wifi mode
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_RegisterStatusCallback_if(MX_WIFIObject_t *Obj,
                                                   mx_wifi_status_callback_t cb,
                                                   void *arg,
                                                   mwifi_if_t interface)
{
  MX_WIFI_STATUS_T rc;

  if (NULL == Obj)
  {
    rc = MX_WIFI_STATUS_ERROR;
  }
  else
  {
    if ((mwifi_if_t)MC_SOFTAP == interface)
    {
      Obj->Runtime.status_cb[1] = cb;
      Obj->Runtime.callback_arg[1] = arg;
    }
    else
    {
      Obj->Runtime.status_cb[0] = cb;
      Obj->Runtime.callback_arg[0] = arg;
    }
    rc = MX_WIFI_STATUS_OK;

  }
  return rc;
}

/**
  * @brief                   register wifi event callback for station or AP mode
  * @param  Obj              wifi object
  * @param  interface        wifi mode
  * @return MX_WIFI_STATUS_T
  */
MX_WIFI_STATUS_T MX_WIFI_UnRegisterStatusCallback_if(MX_WIFIObject_t *Obj, mwifi_if_t interface)
{
  MX_WIFI_STATUS_T rc;

  if (NULL == Obj)
  {
    rc = MX_WIFI_STATUS_ERROR;
  }
  else
  {
    if ((mwifi_if_t)MC_SOFTAP == interface)
    {
      Obj->Runtime.status_cb[1] = NULL;
      Obj->Runtime.callback_arg[1] = NULL;
    }
    else
    {
      Obj->Runtime.status_cb[0] = NULL;
      Obj->Runtime.callback_arg[0] = NULL;
    }
    rc = MX_WIFI_STATUS_OK;
  }
  return rc;
}


static int8_t *mx_ntoa(const mx_ip4_addr_t *addr)
{
  uint32_t MX_S_ADDR;
  uint8_t val;
  int8_t inv[3];
  uint8_t *ap;
  uint8_t rem;
  uint8_t i;
  int32_t len = 0;
  int8_t *buf_ret;
  static int8_t buf[16];

  MX_S_ADDR = addr->addr;

  ap = (uint8_t *)&MX_S_ADDR;
  for (uint8_t n = 0; n < (uint8_t) 4; n++)
  {
    i = 0;
    val = ap[n];
    do
    {
      rem = val % 10U;
      val /=  10U;
      inv[i] = (int8_t)'0' + rem;
      i++;
    } while (val != 0U);

    while (i != 0U)
    {
      i--;
      if (len < 16)
      {
        buf[len] = inv[i];
        len++;
      }
    }

    if ((n < 3U) && (len < 16))
    {
      buf[len] = (int8_t) '.';
      len++;
    }
  }

  if (len < 16)
  {
    buf[len] = (int8_t) '\0';
    buf_ret = buf;

  }
  else
  {
    buf_ret = (int8_t *) "000.000.000.000";
  }

  return buf_ret;
}


/**
  * @brief                   wifi connect to an AP
  * @param  Obj              wifi object
  * @param  SSID             ssid of AP
  * @param  Password         password of AP
  * @param  SecType          security type (not used, auto)
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_Connect(MX_WIFIObject_t *Obj, const char *SSID,
                                 const char *Password, MX_WIFI_SecurityType_t SecType)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;
  wifi_connect_cparams_t cparams;
  int32_t status =  MIPC_CODE_ERROR;
  uint16_t rparams_size;
  mwifi_ip_attr_t ip_attr;
  mx_ip4_addr_t net_ipaddr;

  (void)SecType;

  if ((NULL == Obj) || (NULL == SSID) || (strlen(SSID) > (uint32_t)MX_MAX_SSID_LEN)
      || (NULL == Password) || (strlen(Password) > (uint32_t)MX_MAX_KEY_LEN))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    strncpy((char *)cparams.ssid, SSID, MX_MAX_SSID_LEN);
    strncpy((char *)cparams.key, Password, MX_MAX_KEY_LEN);
    cparams.key_len = strlen(Password);
    cparams.use_attr = 0;
    cparams.use_ip = 0;

    if ((uint8_t)0 == Obj->NetSettings.DHCP_IsEnabled)
    {
      (void)memset(&ip_attr, 0, sizeof(mwifi_ip_attr_t));
      (void)memcpy(&net_ipaddr, Obj->NetSettings.IP_Addr, 4);
      (void)memcpy(ip_attr.localip, mx_ntoa(&net_ipaddr), MX_MAX_IP_LEN);
      (void)memcpy(&net_ipaddr, Obj->NetSettings.IP_Mask, 4);
      (void)memcpy(ip_attr.netmask, mx_ntoa(&net_ipaddr), MX_MAX_IP_LEN);
      (void)memcpy(&net_ipaddr, Obj->NetSettings.Gateway_Addr, 4);
      (void)memcpy(ip_attr.gateway, mx_ntoa(&net_ipaddr), MX_MAX_IP_LEN);
      (void)memcpy(&net_ipaddr, Obj->NetSettings.DNS1, 4);
      (void)memcpy(ip_attr.dnserver, mx_ntoa(&net_ipaddr), MX_MAX_IP_LEN);
      cparams.use_ip = 1;
      memcpy(&(cparams.ip), &ip_attr, sizeof(cparams.ip));
    }

    rparams_size = sizeof(status);
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_CONNECT_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &rparams_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }

  }
  return ret;
}

/**
  * @brief                   connect to an AP with WAP-E
  * @param  Obj              wifi object
  * @param  SSID             ssid of AP
  * @param  Identity         identity of client
  * @param  Password         password of client
  * @param  attr             EAP attribute
  * @param  ip               ip attribute
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_EAP_Connect(MX_WIFIObject_t *Obj, const char *SSID,
                                     const char *Identity, const char *Password,
                                     mwifi_eap_attr_t *attr, mwifi_ip_attr_t *ip)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_OK;

#if 0
  lib_ipc_eap_attr_t ipc_eap_attr;

  if ((NULL == Obj) || (NULL == SSID) || (strlen(SSID) > (uint32_t)MX_MAX_SSID_LEN) || \
      (NULL == Identity) || (strlen(Identity) > (uint32_t)MX_MAX_IDENTITY_LEN) || \
      (NULL == Password) || (strlen(Password) > (uint32_t)MX_MAX_KEY_LEN))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    if (!MX_WIFI_LOCK((Obj)))
    {
      ret = MX_WIFI_STATUS_TIMEOUT;
    }
    else
    {
      (void)memset(&ipc_eap_attr, 0, sizeof(ipc_eap_attr));

      if (NULL != attr)
      {
        /* just set eap type here, rootca/cert/key set by specifical API */
        if (((uint8_t)EAP_TYPE_TLS != attr->eap_type) && ((uint8_t)EAP_TYPE_TTLS != attr->eap_type) &&
            ((uint8_t)EAP_TYPE_PEAP != attr->eap_type))
        {
          ret = MX_WIFI_STATUS_PARAM_ERROR;
        }
        else
        {
          ipc_eap_attr.eap_type = attr->eap_type;

          if (NULL != attr->rootca)
          {
            if (MIPC_CODE_SUCCESS != wifi_eap_set_rootca_cmd(attr->rootca, strlen(attr->rootca)))
            {
              ret = MX_WIFI_STATUS_ERROR;
            }
          }

          if ((MX_WIFI_STATUS_OK == ret) && (NULL != attr->client_cert))
          {
            if (MIPC_CODE_SUCCESS != wifi_eap_set_client_cert_cmd(attr->client_cert, strlen(attr->client_cert)))
            {
              ret = MX_WIFI_STATUS_ERROR;
            }
          }

          if ((MX_WIFI_STATUS_OK == ret) && (NULL != attr->client_key))
          {
            if (MIPC_CODE_SUCCESS != wifi_eap_set_client_key_cmd(attr->client_key, strlen(attr->client_key)))
            {
              ret = MX_WIFI_STATUS_ERROR;
            }
          }
        }
      }
      else
      {
        ipc_eap_attr.eap_type = (uint8_t)EAP_TYPE_PEAP;  /* default eap type */
      }

      if (MX_WIFI_STATUS_OK == ret)
      {
        if (MIPC_CODE_SUCCESS != wifi_eap_connect_cmd(SSID, (int32_t)strlen(SSID),
                                                      Identity, (int32_t)strlen(Identity),
                                                      Password, (int32_t)strlen(Password),
                                                      &ipc_eap_attr, sizeof(ipc_eap_attr),
                                                      ip, (NULL != ip) ? sizeof(mwifi_ip_attr_t) : 0u))
        {
          ret = MX_WIFI_STATUS_ERROR;
        }
      }

      (void)MX_WIFI_UNLOCK((Obj));
    }
  }
#endif /* 0 */

  return ret;
}

/**
  * @brief                   wifi station disconnect
  * @param  Obj              wifi object
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_Disconnect(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;
  int32_t status =  MIPC_CODE_ERROR;
  uint16_t rparams_size;

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    rparams_size = sizeof(status);
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_DISCONNECT_CMD, NULL, 0,
                                          (uint8_t *)&status, &rparams_size,
                                          15000))  /* disconnect max timeout 15s */
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief                   connect AP with WPS
  * @param  Obj              wifi object
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_WPS_Connect(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;

#if 0
  int32_t ret_flag = MIPC_RET_FLAG_CLR;
  int32_t addr_ret_val;
  int32_t addr_ret_flag;
  int32_t *p_addr_ret_val;
  int32_t *p_addr_ret_flag;

  if (NULL != Obj)
  {
    p_addr_ret_val = &ret;
    p_addr_ret_flag = &ret_flag;
    (void)memcpy(&addr_ret_val, &p_addr_ret_val, sizeof(addr_ret_val));
    (void)memcpy(&addr_ret_flag, &p_addr_ret_flag, sizeof(addr_ret_flag));
    if (MIPC_CODE_SUCCESS == wifi_wps_connect_cmd(addr_ret_val, addr_ret_flag))
    {
      if (MIPC_CODE_SUCCESS == mipc_poll(&ret_flag, &ret, Obj->Runtime.Timeout))
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
#endif /* 0 */

  return ret;
}

/**
  * @brief                   stop connecting to an AP with WPS
  * @param  Obj              wifi object
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_WPS_Stop(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;

#if 0
  int32_t ret_flag = MIPC_RET_FLAG_CLR;
  int32_t addr_ret_val;
  int32_t addr_ret_flag;
  int32_t *p_addr_ret_val;
  int32_t *p_addr_ret_flag;

  if (NULL != Obj)
  {
    p_addr_ret_val = &ret;
    p_addr_ret_flag = &ret_flag;
    (void)memcpy(&addr_ret_val, &p_addr_ret_val, sizeof(addr_ret_val));
    (void)memcpy(&addr_ret_flag, &p_addr_ret_flag, sizeof(addr_ret_flag));
    if (MIPC_CODE_SUCCESS == wifi_wps_stop_cmd(addr_ret_val, addr_ret_flag))
    {
      if (MIPC_CODE_SUCCESS == mipc_poll(&ret_flag, &ret, Obj->Runtime.Timeout))
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
#endif /* 0 */

  return ret;
}

/**
  * @brief                   get wifi station connect status
  * @param  Obj              wifi object
  * @return int8_t           1 -- connected, 0 -- not connected
  */
int8_t MX_WIFI_IsConnected(MX_WIFIObject_t *Obj)
{
  int8_t ret = 0;
  wifi_get_linkinof_rparams_t rparams;
  uint16_t rparams_size = sizeof(rparams);
  rparams.status = MIPC_CODE_ERROR;
  if (NULL != Obj)
  {
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_GET_LINKINFO_CMD, NULL, 0,
                                          (uint8_t *)&rparams, &rparams_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == rparams.status)
      {
        Obj->NetSettings.IsConnected = rparams.info.is_connected;
        ret = Obj->NetSettings.IsConnected;
      }
    }
  }
  return ret;
}

/**
  * @brief                   get wifi IPv4 address
  * @param  Obj              wifi object
  * @param  ipaddr           address buffer
  * @param  wifi_if          wifi interface
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_GetIPAddress(MX_WIFIObject_t *Obj, uint8_t *ipaddr, mwifi_if_t wifi_if)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;
  wifi_get_ip_rparams_t rparams;
  uint16_t rparams_size = sizeof(rparams);
  int32_t ip;
  int32_t netmask;
  int32_t gw;
  int32_t dns;

  rparams.status = MIPC_CODE_ERROR;

  if (NULL != Obj)
  {
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_GET_IP_CMD,
                                          (uint8_t *)&wifi_if, sizeof(wifi_if),
                                          (uint8_t *)&rparams, &rparams_size,
                                          1000))
    {
      if (MIPC_CODE_SUCCESS == rparams.status)
      {
        ip = mx_aton_r((int8_t const *) & (rparams.ip.localip[0]));
        memcpy(&(Obj->NetSettings.IP_Addr[0]), &ip, 4);

        netmask = mx_aton_r((int8_t const *) & (rparams.ip.netmask[0]));
        memcpy(&(Obj->NetSettings.IP_Mask[0]), &netmask, 4);

        gw = mx_aton_r((int8_t const *) & (rparams.ip.gateway[0]));
        memcpy(&(Obj->NetSettings.Gateway_Addr[0]), &gw, 4);

        dns = mx_aton_r((int8_t const *) & (rparams.ip.dnserver[0]));
        memcpy(&(Obj->NetSettings.DNS1[0]), &dns, 4);

        (void)memcpy(ipaddr, Obj->NetSettings.IP_Addr, 4);
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief                   get wifi IPv6 address info
  * @param  Obj              wifi object
  * @param  ipaddr6          ipv6 address buffer
  * @param  addr_num         ipv6 address index (0,1,2)
  * @param  wifi_if          wifi interface
  * @return MX_WIFI_STATUS_T status
  */
MX_WIFI_STATUS_T MX_WIFI_GetIP6Address(MX_WIFIObject_t *Obj, uint8_t *ipaddr6, int32_t addr_num, mwifi_if_t wifi_if)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;

#if 0
  int32_t ret_flag = MIPC_RET_FLAG_CLR;
  int32_t addr_ret_flag;
  int32_t *p_addr_ret_flag;

  if ((NULL != Obj) && (NULL != ipaddr6) && (addr_num < 3) && (addr_num >= 0))
  {
    p_addr_ret_flag = &ret_flag;
    (void)memcpy(&addr_ret_flag, &p_addr_ret_flag, sizeof(addr_ret_flag));
    if (MIPC_CODE_SUCCESS == wifi_get_ip6_addr_cmd((int32_t)addr_num, (int32_t)wifi_if, addr_ret_flag))
    {
      if (MIPC_CODE_SUCCESS == mipc_poll(&ret_flag, NULL, Obj->Runtime.Timeout))
      {
        (void)memcpy(ipaddr6, Obj->NetSettings.IP6_Addr[addr_num], 16);
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
#endif /* 0 */

  return ret;
}

/**
  * @brief  Get the local IPv6 address state of the wifi module.
  * @param  Obj: pointer to module handle
  * @param  addr_num: index of the IPv6 address (index: 0/1/2).
  * @param  wifi_if: wifi mode(station or softap).
  * @retval IPV6 address State, error if < 0
  */
int32_t MX_WIFI_GetIP6AddressState(MX_WIFIObject_t *Obj, int32_t addr_num, mwifi_if_t wifi_if)
{
  int32_t state = MX_WIFI_STATUS_ERROR;

#if 0
  int32_t ret_flag = MIPC_RET_FLAG_CLR;
  int32_t addr_ret_flag;
  int32_t *p_addr_ret_flag;

  if ((NULL != Obj) && (addr_num < 3) && (addr_num >= 0))
  {
    p_addr_ret_flag = &ret_flag;
    (void)memcpy(&addr_ret_flag, &p_addr_ret_flag, sizeof(addr_ret_flag));
    if (MIPC_CODE_SUCCESS == wifi_get_ip6_state_cmd((int32_t)addr_num, (int32_t)wifi_if, addr_ret_flag))
    {
      if (MIPC_CODE_SUCCESS == mipc_poll(&ret_flag, NULL, Obj->Runtime.Timeout))
      {
        state = Obj->NetSettings.IP6_state[addr_num];
      }
    }
  }
#endif /* 0 */

  return state;
}

/*
* SoftAP
 */

/**
  * @brief  Start softAP(miniAP) mode
  * @param  Obj: pointer to module handle
  * @param  ap_settings: softAP settings.
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_StartAP(MX_WIFIObject_t *Obj, MX_WIFI_APSettings_t *ap_settings)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;
  wifi_softap_start_cparams_t cparams;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t rparams_size = sizeof(status);

  if ((NULL == Obj) || (NULL == ap_settings))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    memcpy((char *)cparams.ssid, ap_settings->SSID, sizeof(cparams.ssid));
    memcpy((char *)cparams.key, ap_settings->pswd, sizeof(cparams.key));
    cparams.channel = ap_settings->channel;
    memcpy(&(cparams.ip), &(ap_settings->ip), sizeof(cparams.ip));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_SOFTAP_START_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &rparams_size,
                                          3000))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Stop softAP(miniAP) mode
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_StopAP(MX_WIFIObject_t *Obj)
{
  MX_WIFI_STATUS_T ret = MX_WIFI_STATUS_ERROR;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t out_size = sizeof(status);

  if (NULL != Obj)
  {
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_SOFTAP_STOP_CMD, NULL, 0,
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

#if MX_WIFI_NETWORK_BYPASS_MODE == 0
/**
  * @brief  Create a socket.
  * @param  Obj: pointer to module handle
  * @param  domain: socket domain
  * @param  type: socket type
  * @param  protocol: socket protocol
  * @retval Socket file descriptor, return < 1 if failed.
  */
int32_t MX_WIFI_Socket_create(MX_WIFIObject_t *Obj, int32_t domain, int32_t type, int32_t protocol)
{
  int32_t ret_fd = -1;
  socket_create_cparams_t cp;
  socket_create_rparams_t rp;
  uint16_t out_size = sizeof(rp);

  if (NULL != Obj)
  {
    cp.domain = domain;
    cp.type = type;
    cp.protocol = protocol;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_CREATE_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      ret_fd = rp.fd;
    }
  }

  return ret_fd;
}

/**
  * @brief  Set option for a socket
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  level: option level
  * @param  optname: option to set
  * @param  optvalue: value buffer for the option
  * @param  optlen: length of the option value
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_setsockopt(MX_WIFIObject_t *Obj, int32_t sockfd, int32_t level,
                                  int32_t optname, const void *optvalue, int32_t optlen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  socket_setsockopt_cparams_t cp;
  socket_setsockopt_rparams_t rp;
  uint16_t out_size = sizeof(rp);

  rp.status =  MIPC_CODE_ERROR;
  if ((NULL == Obj) || (sockfd < 0) || (NULL == optvalue) || (optlen <= 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.socket = sockfd;
    cp.level = level;
    cp.optname = optname;
    cp.optlen = optlen > sizeof(cp.optval) ? sizeof(cp.optval) : optlen;
    memcpy(&(cp.optval[0]), optvalue, cp.optlen);
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_SETSOCKOPT_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.status == MIPC_CODE_SUCCESS)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Get option of a socket.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  level: option level
  * @param  optname: option to set
  * @param  optvalue: buffer pointer of value of the option
  * @param  optlen: buffer pointer of length of the option value
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_getsockopt(MX_WIFIObject_t *Obj, int32_t sockfd, int32_t level,
                                  int32_t optname, void *optvalue, uint32_t *optlen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  socket_getsockopt_cparams_t cp;
  socket_getsockopt_rparams_t rp;
  uint16_t out_size = sizeof(rp);
  rp.status =  MIPC_CODE_ERROR;

  if ((NULL == Obj) || (sockfd < 0) || (NULL == optvalue) || (NULL == optlen))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.socket = sockfd;
    cp.level = level;
    cp.optname = optname;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_GETSOCKOPT_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.status == MIPC_CODE_SUCCESS)
      {
        *optlen = (rp.optlen > (*optlen)) ? (*optlen) : rp.optlen;
        memcpy(optvalue, &(rp.optval[0]), *optlen);
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Bind a socket.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: socket address
  * @param  addrlen: address length
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_bind(MX_WIFIObject_t *Obj, int32_t sockfd,
                            const struct sockaddr *addr, int32_t addrlen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  socket_bind_cparams_t cp;
  socket_bind_rparams_t rp;
  uint16_t out_size = sizeof(rp);
  rp.status =  MIPC_CODE_ERROR;

  if ((NULL == Obj) || (sockfd < 0) || (NULL == addr) || (addrlen <= 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.socket = sockfd;
    memcpy(&(cp.addr), addr, sizeof(cp.addr));
    cp.length = addrlen;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_BIND_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.status == MIPC_CODE_SUCCESS)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Listen a socket.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  backlog: max number to queued.
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_listen(MX_WIFIObject_t *Obj, int32_t sockfd, int32_t backlog)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  socket_listen_cparams_t cp;
  socket_listen_rparams_t rp;
  uint16_t rp_size = sizeof(rp);
  rp.status =  MIPC_CODE_ERROR;

  if ((NULL == Obj) || (sockfd < 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.socket = sockfd;
    cp.n = backlog;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_LISTEN_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &rp_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.status == MIPC_CODE_SUCCESS)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Accept a socket.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: client address
  * @param  addrlen: length of client address
  * @retval Accepted client socket fd, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_accept(MX_WIFIObject_t *Obj, int32_t sockfd,
                              struct sockaddr *addr, uint32_t *addrlen)
{
  int32_t ret_fd = -1;
  socket_accept_cparams_t cp;
  socket_accept_rparams_t rp;
  uint16_t rp_size = sizeof(rp);
  rp.socket =  -1;

  if ((NULL == Obj) || (sockfd < 0) || (NULL == addr) || (NULL == addrlen))
  {
    ret_fd = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.socket = sockfd;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_ACCEPT_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &rp_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.socket >= 0)
      {
        memcpy(addr, &(rp.addr), sizeof(rp.addr));
        *addrlen = rp.length;
        ret_fd = rp.socket;
      }
    }
  }
  return ret_fd;
}

/**
  * @brief  Socket connect.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: client address
  * @param  addrlen: length of client address
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_connect(MX_WIFIObject_t *Obj, int32_t sockfd,
                               const struct sockaddr *addr, int32_t addrlen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  socket_connect_cparams_t cp;
  socket_connect_rparams_t rp;
  uint16_t out_size = sizeof(rp);
  rp.status =  MIPC_CODE_ERROR;

  if ((NULL == Obj) || (sockfd < 0) || (NULL == addr) || (addrlen <= 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.socket = sockfd;
    memcpy(&(cp.addr), addr, sizeof(cp.addr));
    cp.length = addrlen;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_CONNECT_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.status == MIPC_CODE_SUCCESS)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Socket shutdown.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  mode: shutdown mode:
  *        0    Stop receiving data for this socket;
  *        1    Stop trying to transmit data from this socket
  *        2    Stop all transmit from this socket
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_shutdown(MX_WIFIObject_t *Obj, int32_t sockfd, int32_t mode)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  socket_shutdown_cparams_t cp;
  socket_shutdown_rparams_t rp;
  uint16_t out_size = sizeof(rp);
  rp.status =  MIPC_CODE_ERROR;

  if ((NULL == Obj) || (sockfd < 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.filedes = sockfd;
    cp.how = mode;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_SHUTDOWN_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.status == MIPC_CODE_SUCCESS)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Socket close.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_close(MX_WIFIObject_t *Obj, int32_t sockfd)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  socket_close_cparams_t cp;
  socket_close_rparams_t rp;
  uint16_t out_size = sizeof(rp);
  rp.status =  MIPC_CODE_ERROR;

  if ((NULL == Obj) || (sockfd < 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.filedes = sockfd;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_CLOSE_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.status == MIPC_CODE_SUCCESS)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Socket send.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  buf: send data buffer
  * @param  len: length of send data
  * @param  flags: zero for MXOS
  * @retval Number of bytes sent, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_send(MX_WIFIObject_t *Obj, int32_t sockfd, uint8_t *buf,
                            int32_t len, int32_t flags)
{
  int32_t ret = -1;
  socket_send_cparams_t *cp;
  uint16_t cp_size;
  socket_send_rparams_t rp;
  uint16_t rp_size = sizeof(rp);
  int32_t datalen;

  if ((NULL == Obj) || (sockfd < 0) || (NULL == buf) || (len <= 0))
  {
    ret = -1;
  }
  else
  {
    if ((len + sizeof(socket_send_cparams_t) - 1) > MX_WIFI_IPC_PAYLOAD_SIZE)
    {
      datalen = (MX_WIFI_IPC_PAYLOAD_SIZE - (sizeof(socket_send_cparams_t) - 1));
    }
    else
    {
      datalen = len;
    }
    rp.sent = 0;
    cp_size = (sizeof(socket_send_cparams_t) - 1 + datalen);
    cp = (socket_send_cparams_t *)MX_WIFI_MALLOC(cp_size);
    if (NULL != cp)
    {
      cp->socket = sockfd;
      memcpy(&(cp->buffer[0]), buf, datalen);
      cp->size = datalen;
      cp->flags = flags;
      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_SEND_CMD,
                                            (uint8_t *)cp, cp_size,
                                            (uint8_t *)&rp, &rp_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        ret = rp.sent;
      }
      MX_WIFI_FREE(cp);
    }
  }
  return ret;
}

/**
  * @brief  Socket sendto.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  buf: send data buffer
  * @param  len: length of send data
  * @param  flags: zero for MXOS
  * @param  toaddr: address to send
  * @param  toaddrlen: length of address to send
  * @retval Number of bytes sent, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_sendto(MX_WIFIObject_t *Obj, int32_t sockfd, uint8_t *buf,
                              int32_t len, int32_t flags,
                              struct sockaddr *toaddr, int32_t toaddrlen)
{
  int32_t ret = -1;
  socket_sendto_cparams_t *cp;
  uint16_t cp_size;
  socket_sendto_rparams_t rp;
  uint16_t rp_size = sizeof(rp);
  int32_t datalen;

  if ((NULL == Obj) || (sockfd < 0) || (NULL == buf) || (len <= 0) || (NULL == toaddr) || (toaddrlen <= 0))
  {
    ret = -1;
  }
  else
  {
    if ((len + sizeof(socket_sendto_cparams_t) - 1) > MX_WIFI_IPC_PAYLOAD_SIZE)
    {
      datalen = (MX_WIFI_IPC_PAYLOAD_SIZE - (sizeof(socket_sendto_cparams_t) - 1));
    }
    else
    {
      datalen = len;
    }
    cp_size = (sizeof(socket_sendto_cparams_t) - 1 + datalen);
    cp = (socket_sendto_cparams_t *)MX_WIFI_MALLOC(cp_size);
    if (NULL != cp)
    {
      rp.sent = 0;
      cp->socket = sockfd;
      memcpy(&(cp->buffer[0]), buf, datalen);
      cp->size = datalen;
      cp->flags = flags;
      memcpy(&(cp->addr), toaddr, sizeof(cp->addr));
      cp->length = toaddrlen;
      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_SENDTO_CMD,
                                            (uint8_t *)cp, cp_size,
                                            (uint8_t *)&rp, &rp_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        ret = rp.sent;
      }
      MX_WIFI_FREE(cp);
    }
  }
  return ret;
}

/**
  * @brief  Socket recv.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  buf: recv buffer
  * @param  len: length of recv buffer
  * @param  flags: zero for MXOS
  * @retval Number of bytes received, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_recv(MX_WIFIObject_t *Obj, int32_t sockfd, uint8_t *buf,
                            int32_t len, int32_t flags)
{
  int32_t ret = -1;
  socket_recv_cparams_t cp;
  socket_recv_rparams_t *rp;
  uint16_t rp_size;
  int32_t datalen;

  if ((NULL == Obj) || (sockfd < 0) || (NULL == buf) || (len <= 0))
  {
    ret = -1;
  }
  else
  {
    ret = 0;
    if ((len + sizeof(socket_recv_rparams_t) - 1) > MX_WIFI_IPC_PAYLOAD_SIZE)
    {
      datalen = (MX_WIFI_IPC_PAYLOAD_SIZE - (sizeof(socket_recv_rparams_t) - 1));
    }
    else
    {
      datalen = len;
    }
    rp_size = (sizeof(socket_recv_rparams_t) - 1 + datalen);
    rp = (socket_recv_rparams_t *)MX_WIFI_MALLOC(rp_size);
    if (NULL != rp)
    {
      rp->received = 0;
      cp.socket = sockfd;
      cp.size = datalen;
      cp.flags = flags;
      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_RECV_CMD,
                                            (uint8_t *)&cp, sizeof(cp),
                                            (uint8_t *)rp, &rp_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        if ((rp->received <= datalen) && (rp->received > 0))
        {
          memcpy(buf, &(rp->buffer[0]), rp->received);
        }
        ret = rp->received;
      }
      MX_WIFI_FREE(rp);
    }
  }
  return ret;
}

/**
  * @brief  Socket recvfrom.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  buf: recv buffer
  * @param  len: length of recv buffer
  * @param  fromaddr: address of the data source
  * @param  fromaddrlen: length of address of the data source
  * @param  flags: zero for MXOS
  * @retval Number of bytes received, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_recvfrom(MX_WIFIObject_t *Obj, int32_t sockfd, uint8_t *buf,
                                int32_t len, int32_t flags,
                                struct sockaddr *fromaddr, uint32_t *fromaddrlen)
{
  int32_t ret = -1;
  socket_recvfrom_cparams_t cp;
  socket_recvfrom_rparams_t *rp;
  uint16_t rp_size;
  int32_t datalen;

  if ((NULL == Obj) || (sockfd < 0) || (NULL == buf) || (len <= 0) || (NULL == fromaddr) \
      || (NULL == fromaddrlen))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    ret = 0;
    if ((len + sizeof(socket_recvfrom_rparams_t) - 1) > MX_WIFI_IPC_PAYLOAD_SIZE)
    {
      datalen = (MX_WIFI_IPC_PAYLOAD_SIZE - (sizeof(socket_recvfrom_rparams_t) - 1));
    }
    else
    {
      datalen = len;
    }
    rp_size = (sizeof(socket_recvfrom_rparams_t) - 1 + datalen);
    rp = (socket_recvfrom_rparams_t *)MX_WIFI_MALLOC(rp_size);
    if (NULL != rp)
    {
      rp->received = 0;
      cp.socket = sockfd;
      cp.size = datalen;
      cp.flags = flags;
      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_RECVFROM_CMD,
                                            (uint8_t *)&cp, sizeof(cp),
                                            (uint8_t *)rp, &rp_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        if ((rp->received <= datalen) && (rp->received > 0))
        {
          memcpy(buf, &(rp->buffer[0]), rp->received);
          ret = rp->received;
          memcpy(fromaddr, &(rp->addr), sizeof(rp->addr));
          *fromaddrlen = rp->length;
        }
      }
      MX_WIFI_FREE(rp);
    }
  }
  return ret;
}

/**
  * @brief  Gethostbyname, only for IPv4 address.
  * @param  Obj: pointer to module handle
  * @param  addr: address of the host
  * @param  name: hostname
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_gethostbyname(MX_WIFIObject_t *Obj, struct sockaddr *addr, char *name)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  mx_sockaddr_in_t addr_in;
  socket_gethostbyname_cparams_t cp;
  socket_gethostbyname_rparams_t rp;
  uint16_t rp_size = sizeof(rp);
  rp.status =  MIPC_CODE_ERROR;

  if ((NULL == Obj) || (NULL == addr) || (NULL == name) || (strlen(name) > (uint32_t)MX_MAX_DNSNAME_LEN))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    uint32_t msize;
    memset(&cp, 0, sizeof(cp));
    msize = MIN(sizeof(cp.name), strlen(name) + 1);
    memcpy(&(cp.name[0]), name, msize);
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_GETHOSTBYNAME_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &rp_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.status == MIPC_CODE_SUCCESS)
      {
        addr_in.sin_addr.s_addr = rp.s_addr;
        addr_in.sin_family = AF_INET;
        addr_in.sin_len = sizeof(addr_in);
        memcpy(addr, &addr_in, sizeof(struct sockaddr));
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Ping a host, only for IPv4 address.
  * @param  Obj: pointer to module handle
  * @param  addr: hostname
  * @param  count: ping max count
  * @param  delay: ping delay in millisecond
  * @param  response: response time array of ping result
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_ping(MX_WIFIObject_t *Obj, const char *hostname,
                            int32_t count, int32_t delay, int32_t response[])
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  wifi_ping_cparams_t cp;
  int32_t ping_resp[1 + MX_WIFI_PING_MAX];
  wifi_ping_rparams_t *rp;
  uint16_t rp_size = sizeof(ping_resp);
  int32_t i = 0;

  if ((NULL == Obj) || (NULL == hostname) || (count <= 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    int32_t msize;
    memset(&cp, 0, sizeof(cp));
    msize = MIN(sizeof(cp.hostname), strlen(hostname) + 1);
    memcpy(&(cp.hostname[0]), hostname, msize);
    cp.count = count;
    cp.delay_ms = delay;
    memset(ping_resp, 0, sizeof(ping_resp));
    rp = (wifi_ping_rparams_t *)&ping_resp;
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_PING_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)rp, &rp_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp->num > 0)
      {
        for (i = 0; i < rp->num; i++)
        {
          response[i] = rp->delay_ms[i];
        }
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

#if 0
/**
  * @brief  Get IPv4/v6 address info by nodename.
  * @param  Obj: pointer to module handle
  * @param  nodename: descriptive name or address string of the host
  * @param  servname: not used, set NULL
  * @param  hints: structure containing input values that set socktype and protocol
  * @param  res: buf to store the result (set to NULL on failure)
  * @retval Operation Status.
  */
int32_t MX_WIFI_Socket_getaddrinfo(MX_WIFIObject_t *Obj, const char *nodename, const char *servname,
                                   const struct addrinfo *hints, struct mc_addrinfo *res)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  int32_t ret_flag = MIPC_RET_FLAG_CLR;
  int32_t addr_ret_val;
  int32_t addr_ret_flag;
  int32_t *p_addr_ret_val;
  int32_t *p_addr_ret_flag;

  if ((NULL == Obj) || (NULL == nodename) || (NULL == hints) || (NULL == res))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    p_addr_ret_val = &ret;
    p_addr_ret_flag = &ret_flag;
    (void)memcpy(&addr_ret_val, &p_addr_ret_val, sizeof(addr_ret_val));
    (void)memcpy(&addr_ret_flag, &p_addr_ret_flag, sizeof(addr_ret_flag));
    if (MIPC_CODE_SUCCESS == socket_getaddrinfo_cmd(nodename, (int32_t)strlen(nodename),
                                                    servname, (int32_t)strlen(servname),
                                                    (void *)hints, sizeof(struct addrinfo), (int)res,
                                                    addr_ret_val, addr_ret_flag))
    {
      if (MIPC_CODE_SUCCESS == mipc_poll(&ret_flag, NULL, Obj->Runtime.Timeout))
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Monitor multiple file descriptors for sockets
  * @attention  Never doing operations in different threads
  * @param  Obj: pointer to module handle
  * @param  nfds: is the highest-numbered file descriptor in any of the three
  *         sets, plus 1
  * @param  readfds: A file descriptor sets will be watched to see if characters
  *         become available for reading
  * @param  writefds: A file descriptor sets will be watched to see if a write
  *         will not block.
  * @param  exceptfds: A file descriptor sets will be watched for exceptions.
  * @param  timeout: The timeout argument specifies the interval that select()
  *         should block waiting for a file descriptor to become ready.
  *         If timeout is NULL (no timeout), select() can block until API timeout.
  * @retval On success, return the number of file descriptors contained in the
  *         three returned descriptor sets (that is, the total number of bits
  *         that are set in readfds, writefds, exceptfds) which may be zero if
  *         the timeout expires before anything interesting happens.  On error,
  *         -1 is returned, the file descriptor sets are unmodified, and timeout
  *         becomes undefined.
  */
int32_t MX_WIFI_Socket_select(MX_WIFIObject_t *Obj, int32_t nfds, fd_set *readfds, fd_set *writefds,
                              fd_set *exceptfds, struct mc_timeval *timeout)
{
  int32_t ret = -1; /* error */
  int32_t ret_flag = MIPC_RET_FLAG_CLR;
  int32_t *p_ret_flag;
  int32_t addr_ret_flag;

  if ((NULL != Obj) && (nfds > 0) && ((NULL != readfds) || (NULL != writefds) || (NULL != exceptfds)))
  {
    p_ret_flag = &ret_flag;
    (void)memcpy(&addr_ret_flag, &p_ret_flag, sizeof(addr_ret_flag));
    if (MIPC_CODE_SUCCESS == socket_select_cmd(nfds, readfds, sizeof(fd_set),
                                               writefds, sizeof(fd_set),
                                               exceptfds, sizeof(fd_set),
                                               timeout, sizeof(struct mc_timeval),
                                               addr_ret_flag))
    {
      if (MIPC_CODE_SUCCESS == mipc_poll(&ret_flag, NULL, 200))
      {
        ret = mc_select_ret_value;
        if (ret > 0)
        {
          if (readfds)
          {
            (void)memcpy(readfds, &mc_select_rfds, sizeof(fd_set));
          }
          if (writefds)
          {
            (void)memcpy(writefds, &mc_select_wfds, sizeof(fd_set));
          }
          if (exceptfds)
          {
            (void)memcpy(exceptfds, &mc_select_efds, sizeof(fd_set));
          }
        }
        mc_select_ret_value = -1;
      }
    }
  }
  return ret;
}
#endif /* 0 */

/**
  * @brief  socket getpeername.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: address buffer
  * @param  addrlen: size of address buffer
  * @retval get address of peer socket, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_getpeername(MX_WIFIObject_t *Obj, int32_t sockfd, struct sockaddr *addr, uint32_t *addrlen)
{
  int32_t ret = -1;
  socket_getpeername_cparams_t cp;
  socket_getpeername_rparams_t rp;
  uint16_t rp_size = sizeof(rp);

  if ((NULL == Obj) || (sockfd < 0) || (NULL == addr) || (NULL == addrlen) || (*addrlen <= 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.s = sockfd;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_GETPEERNAME_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &rp_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == rp.status)
      {
        memcpy(addr, &(rp.name), sizeof(rp.name));
        *addrlen = rp.namelen;
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  socket getsockname.
  * @param  Obj: pointer to module handle
  * @param  sockfd: socket fd
  * @param  addr: address buffer
  * @param  addrlen: size of address buffer
  * @retval get address of local socket, return < 0 if failed.
  */
int32_t MX_WIFI_Socket_getsockname(MX_WIFIObject_t *Obj, int32_t sockfd, struct sockaddr *addr, uint32_t *addrlen)
{
  int32_t ret = -1;
  socket_getsockname_cparams_t cp;
  socket_getsockname_rparams_t rp;
  uint16_t rp_size = sizeof(rp);
  rp.status =  MIPC_CODE_ERROR;

  if ((NULL == Obj) || (sockfd < 0) || (NULL == addr) || (NULL == addrlen) || (*addrlen <= 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.s = sockfd;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_SOCKET_GETSOCKNAME_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &rp_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == rp.status)
      {
        memcpy(addr, &(rp.name), sizeof(rp.name));
        *addrlen = rp.namelen;
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/* mDNS */

/**
  * @brief  start mDNS service.
  * @param  Obj: pointer to module handle
  * @param  domain: domain of service
  * @param  name: hostname
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_start(MX_WIFIObject_t *Obj, const char *domain, char *hostname)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  mdns_start_cparams_t cparams;
  int32_t status;
  uint16_t out_size = sizeof(status);
  status =  MIPC_CODE_ERROR;

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    memset(&cparams, 0, sizeof(cparams));
    memcpy(&(cparams.domain[0]), domain, sizeof(cparams.domain));
    memcpy(&(cparams.hostname[0]), hostname, sizeof(cparams.hostname));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_MDNS_START_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }

  return ret;
}

/**
  * @brief  stop mDNS service.
  * @param  Obj: pointer to module handle
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_stop(MX_WIFIObject_t *Obj)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t rparams_size;

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    rparams_size = sizeof(status);
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_MDNS_STOP_CMD, NULL, 0,
                                          (uint8_t *)&status, &rparams_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  announce a service.
  * @param  Obj: pointer to module handle
  * @param  service: service to announce
  * @param  interface: wifi interface
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_announce_service(MX_WIFIObject_t *Obj,
                                      struct mc_mdns_service *service, mwifi_if_t interface)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  mdns_announce_cparams_t cparams;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t out_size = sizeof(status);

  if ((NULL == Obj) || (NULL == service))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    memset(&cparams, 0, sizeof(cparams));
    cparams.iface = interface;
    memcpy(&(cparams.service_data), service, sizeof(cparams.service_data));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_MDNS_ANNOUNCE_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }

  return ret;
}

/**
  * @brief  deannounce a service.
  * @param  Obj: pointer to module handle
  * @param  service: service to deannounce
  * @param  interface: wifi interface
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_deannounce_service(MX_WIFIObject_t *Obj,
                                        struct mc_mdns_service *service,
                                        mwifi_if_t interface)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  mdns_deannounce_cparams_t cparams;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t out_size = sizeof(status);

  if ((NULL == Obj) || (NULL == service))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    memset(&cparams, 0, sizeof(cparams));
    cparams.iface = interface;
    memcpy(&(cparams.service_data), service, sizeof(cparams.service_data));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_MDNS_DEANNOUNCE_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }

  return ret;
}

/**
  * @brief  deannounce all services.
  * @param  Obj: pointer to module handle
  * @param  interface: wifi interface
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_deannounce_service_all(MX_WIFIObject_t *Obj, mwifi_if_t interface)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  mdns_deannounce_all_cparams_t cparams;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t out_size = sizeof(status);

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    memset(&cparams, 0, sizeof(cparams));
    cparams.iface = interface;
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_MDNS_DEANNOUNCE_ALL_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }

  return ret;
}

/**
  * @brief  Send interface state change event to mdns
  * @param  Obj: pointer to module handle
  * @param  interface: wifi interface
  * @param  state: state event, valid interface state from \ref iface_state
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_iface_state_change(MX_WIFIObject_t *Obj, mwifi_if_t interface,
                                        enum iface_state state)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  mdns_iface_state_change_cparams_t cparams;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t out_size = sizeof(status);

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    memset(&cparams, 0, sizeof(cparams));
    cparams.iface = interface;
    cparams.state = state;
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_MDNS_IFACE_STATE_CHANGE_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }

  return ret;
}

/**
  * @brief  Set new host name, use mdns_iface_state_change(interface, REANNOUNCE) to anounce
  *         the new host name.
  * @param  Obj: pointer to module handle
  * @param  hostname: new hostname
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_set_hostname(MX_WIFIObject_t *Obj, char *hostname)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  mdns_set_hostname_cparams_t cparams;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t out_size = sizeof(status);

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    memset(&cparams, 0, sizeof(cparams));
    strncpy(cparams.hostname, hostname, MDNS_MAX_LABEL_LEN);
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_MDNS_SET_HOSTNAME_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  sets the TXT record field for a given mDNS service.
  * @param  Obj: pointer to module handle
  * @param  service: mDNS service
  * @param  keyvals: new txt record string
  * @param  separator: the separator used to separate individual key value pairs
  * @retval Operation Status.
  */
int32_t MX_WIFI_MDNS_set_txt_rec(MX_WIFIObject_t *Obj, struct mc_mdns_service *service, char *keyvals,
                                 char separator)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  mdns_set_txt_rec_cparams_t cparams;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t out_size = sizeof(status);

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    memset(&cparams, 0, sizeof(cparams));
    memcpy(&(cparams.service_data), service, sizeof(cparams.service_data));
    strncpy(cparams.keyvals, keyvals, MDNS_MAX_KEYVAL_LEN);
    cparams.separator = separator;
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_MDNS_SET_TXT_REC_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/* TLS */

/**
  * @brief  set the TLS protocol version.
  * @param  Obj: pointer to module handle
  * @param  version: TLS protocol version
  * @retval Operation Status.
  * @note   This function should be called before TLS is ready to function (before
  *         mtls_connect and mtls_accept is called by application ).
  */
int32_t MX_WIFI_TLS_set_ver(MX_WIFIObject_t *Obj, mtls_ver_t version)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  tls_set_ver_cparams_t cp;
  tls_set_ver_rparams_t rp;
  uint16_t out_size = sizeof(rp);

  if ((NULL == Obj))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.version = version;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_TLS_SET_VER_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == rp.ret)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }

  return ret;
}

/**
  * @brief   TLS set client certificate
  * @param   Obj: pointer to module handle
  * @param   client_cert: Point to buffer of client cert.
  * @param   cert_len: length of the client cert.
  * @retval  Operation Status.
  */
int32_t MX_WIFI_TLS_set_clientCertificate(MX_WIFIObject_t *Obj, uint8_t *client_cert, uint16_t cert_len)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  tls_set_client_cert_cparams_t *cp;
  uint16_t cp_size;
  tls_set_client_cert_rparams_t rp;
  uint16_t out_size = sizeof(rp);

  cp_size = sizeof(tls_set_client_cert_cparams_t) -1 + cert_len;
  if ((NULL == Obj) || (NULL == client_cert) || (cert_len == (uint16_t)0)
      || (cp_size > (uint16_t)(MX_WIFI_IPC_PAYLOAD_SIZE)))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp = (tls_set_client_cert_cparams_t *)MX_WIFI_MALLOC(cp_size);
    if (NULL != cp)
    {
      memset(cp, 0, cp_size);
      cp->cert_pem_size = cert_len;
      cp->private_key_pem_size = 0;
      memcpy(&(cp->cert_data[0]), client_cert, cert_len);
      memset(&rp, 0, sizeof(rp));
      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_TLS_SET_CLIENT_CERT_CMD,
                                            (uint8_t *)cp, cp_size,
                                            (uint8_t *)&rp, &out_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        if (MIPC_CODE_SUCCESS == rp.ret)
        {
          ret = MX_WIFI_STATUS_OK;
        }
      }
      MX_WIFI_FREE(cp);
    }
  }

  return ret;
}

/**
  * @brief   TLS set client private key
  * @param   Obj: pointer to module handle
  * @param   client_private_key: Point to buffer of client private key.
  * @param   key_len: length of the client cert.
  * @retval  Operation Status.
  */
int32_t MX_WIFI_TLS_set_clientPrivateKey(MX_WIFIObject_t *Obj, uint8_t *client_private_key, uint16_t key_len)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  tls_set_client_cert_cparams_t *cp;
  uint16_t cp_size;
  tls_set_client_cert_rparams_t rp;
  uint16_t out_size = sizeof(rp);

  cp_size = sizeof(tls_set_client_cert_cparams_t) -1 + key_len;
  if ((NULL == Obj) || (NULL == client_private_key) || (key_len == (uint16_t)0)
      || (cp_size > (uint16_t)(MX_WIFI_IPC_PAYLOAD_SIZE)))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp = (tls_set_client_cert_cparams_t *)MX_WIFI_MALLOC(cp_size);
    if (NULL != cp)
    {
      memset(cp, 0, cp_size);
      cp->cert_pem_size = 0;
      cp->private_key_pem_size = key_len;
      memcpy(&(cp->cert_data[0]), client_private_key, key_len);
      memset(&rp, 0, sizeof(rp));
      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_TLS_SET_CLIENT_CERT_CMD,
                                            (uint8_t *)cp, cp_size,
                                            (uint8_t *)&rp, &out_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        if (MIPC_CODE_SUCCESS == rp.ret)
        {
          ret = MX_WIFI_STATUS_OK;
        }
      }
      MX_WIFI_FREE(cp);
    }
  }
  return ret;
}

/**
  * @brief   TLS client create a TLS connection.
  * @param   Obj: pointer to module handle
  * @details This function is called on the client side and initiates an TLS/TLS handshake with a
  *          server. When this function is called, the underlying communication channel has already
  *          been set up. This function is called after TCP 3-way handshake finished.
  * @param   domain: Specifies a communication domain
  * @param   type: Specifies the communication semantics.
  * @param   protocol: specifies a particular protocol to be used with the socket.
  * @param   addr: Point to the target address to be connected
  * @param   addrlen: containing the size of the buffer pointed to by addr
  * @param   ca: pointer to the CA certificate string, used to verify server's certificate.
  * @param   calen: the string length of ca. 0=do not verify server's certificate.
  * @retval  return the TLS context pointer on success or NULL for fail.
  */
int32_t MX_WIFI_TLS_connect(MX_WIFIObject_t *Obj, int32_t domain, int32_t type, int32_t protocol,
                            const struct sockaddr *addr, int32_t addrlen, char *ca, int32_t calen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  (void)domain;
  (void)type;
  (void)protocol;

  if ((NULL == Obj) || (NULL == addr) || (addrlen <= 0))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    ret = MX_WIFI_TLS_connect_sni(Obj, NULL, 0, addr, addrlen, ca, calen);
  }

  return ret;
}

/**
  * @brief   TLS client create a TLS connection with SNI.
  * @param   Obj: pointer to module handle
  * @details This function is called on the client side and initiates an TLS/TLS handshake with a
  *          server. When this function is called, the underlying communication channel has already
  *          been set up. This function is called after TCP 3-way handshake finished.
  * @param   sni_servername: Specifies the SNI server name
  * @param   sni_servername_len: Specifies the SNI server name length, max size MX_TLS_SNI_SERVERNAME_SIZE
  * @param   addr: Point to the target address to be connected
  * @param   addrlen: containing the size of the buffer pointed to by addr
  * @param   ca: pointer to the CA certificate string, used to verify server's certificate.
  * @param   calen: the string length of ca. 0=do not verify server's certificate.
  * @retval  return the TLS context pointer on success or NULL for fail.
  */
int32_t MX_WIFI_TLS_connect_sni(MX_WIFIObject_t *Obj, const char *sni_servername, int32_t sni_servername_len,
                                const struct sockaddr *addr, int32_t addrlen, char *ca, int32_t calen)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  tls_connect_sni_cparams_t *cp;
  uint16_t cp_size;
  tls_connect_sni_rparams_t rp;
  uint16_t out_size = sizeof(rp);

  cp_size = sizeof(tls_connect_sni_cparams_t) -1 + calen;
  if ((NULL == Obj) || (NULL == addr) || (addrlen <= 0) || (cp_size > (uint16_t)(MX_WIFI_IPC_PAYLOAD_SIZE)))
  {
    ret = 0; /* null for MX_WIFI_STATUS_PARAM_ERROR */
  }
  else
  {
    rp.tls = NULL;
    cp = (tls_connect_sni_cparams_t *)MX_WIFI_MALLOC(cp_size);
    if (NULL != cp)
    {
      memset(cp, 0, cp_size);
      if ((sni_servername_len > 0) && (NULL != sni_servername))
      {
        strncpy(&(cp->sni_servername[0]), sni_servername, sizeof(cp->sni_servername));
      }
      memcpy(&(cp->addr), addr, sizeof(cp->addr));
      cp->length = addrlen;
      cp->calen = calen;
      if ((calen > 0) && (NULL != ca))
      {
        memcpy(&(cp->ca[0]), ca, calen);
      }
      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_TLS_CONNECT_SNI_CMD,
                                            (uint8_t *)cp, cp_size,
                                            (uint8_t *)&rp, &out_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        if (NULL == rp.tls)
        {
          ret = 0;  /* NULL */
        }
        else
        {
          ret = (int32_t)rp.tls;
        }
      }
      MX_WIFI_FREE(cp);
    }
  }

  return ret;
}

/**
  * @brief   TLS send data
  * @param   Obj: pointer to module handle
  * @param   tls: Point to the TLS context.
  * @param   data: Point to data to send.
  * @param   len: data length.
  * @retval  On success, return the number of bytes sent.
  *          On error, error code (< 0) is returned.
  */
int32_t MX_WIFI_TLS_send(MX_WIFIObject_t *Obj, mtls_t tls, void *data, int32_t len)
{
  int32_t ret = -1;
  tls_send_cparams_t *cp;
  uint16_t cp_size;
  tls_send_rparams_t rp;
  uint16_t rp_size = sizeof(rp);
  int32_t datalen;

  if ((NULL == Obj) || (NULL == tls) || (NULL == data) || (len <= 0))
  {
    ret = -1;
  }
  else
  {
    rp.sent = 0;
    if ((len + sizeof(tls_send_cparams_t) - 1) > MX_WIFI_IPC_PAYLOAD_SIZE)
    {
      datalen = (MX_WIFI_IPC_PAYLOAD_SIZE - (sizeof(tls_send_cparams_t) - 1));
    }
    else
    {
      datalen = len;
    }
    cp_size = (sizeof(tls_send_cparams_t) - 1 + datalen);
    cp = (tls_send_cparams_t *)MX_WIFI_MALLOC(cp_size);
    if (NULL != cp)
    {
      cp->tls = tls;
      memcpy(&(cp->buffer[0]), data, datalen);
      cp->size = datalen;
      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_TLS_SEND_CMD,
                                            (uint8_t *)cp, cp_size,
                                            (uint8_t *)&rp, &rp_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        ret = rp.sent;
      }
      MX_WIFI_FREE(cp);
    }
  }
  return ret;
}

/**
  * @brief   TLS redeive data
  * @param   Obj: pointer to module handle
  * @param   tls: Point to the TLS context.
  * @param   data: Point to buffer to receive TLS data.
  * @param   len: max receive buffer length.
  * @retval  On success, return the number of bytes received.
  *          On error, error code (< 0) is returned.
  */
int32_t MX_WIFI_TLS_recv(MX_WIFIObject_t *Obj, mtls_t tls, void *buf, int32_t len)
{
  int32_t ret = -1;
  tls_recv_cparams_t cp;
  tls_recv_rparams_t *rp;
  uint16_t rp_size;
  int32_t datalen;

  if ((NULL == Obj) || (NULL == tls) || (NULL == buf) || (len <= 0))
  {
    ret = -1;
  }
  else
  {
    ret = 0;
    if ((len + sizeof(tls_recv_rparams_t) - 1) > MX_WIFI_IPC_PAYLOAD_SIZE)
    {
      datalen = (MX_WIFI_IPC_PAYLOAD_SIZE - (sizeof(tls_recv_rparams_t) - 1));
    }
    else
    {
      datalen = len;
    }
    rp_size = (sizeof(tls_recv_rparams_t) - 1 + datalen);
    rp = (tls_recv_rparams_t *)MX_WIFI_MALLOC(rp_size);
    if (NULL != rp)
    {
      rp->received = 0;
      cp.tls = tls;
      cp.size = datalen;
      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_TLS_RECV_CMD,
                                            (uint8_t *)&cp, sizeof(cp),
                                            (uint8_t *)rp, &rp_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        if ((rp->received <= datalen) && (rp->received > 0))
        {
          memcpy(buf, &(rp->buffer[0]), rp->received);
        }
        ret = rp->received;
      }
      MX_WIFI_FREE(rp);
    }
  }
  return ret;
}

/**
  * @brief   Close the TLS session, release resource.
  * @param   Obj: pointer to module handle
  * @param   tls: Point to the TLS context.
  * @retval  Operation Status.
  */
int32_t MX_WIFI_TLS_close(MX_WIFIObject_t *Obj, mtls_t tls)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  tls_close_cparams_t cp;
  tls_close_rparams_t rp;
  uint16_t out_size = sizeof(rp);

  if ((NULL == Obj) || (NULL == tls))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.tls = tls;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_TLS_CLOSE_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.ret == MIPC_CODE_SUCCESS)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief   Set TLS nonblock mode.
  * @param   Obj: pointer to module handle
  * @param   tls: Point to the TLS context.
  * @param   nonblock: true - nonblock, false - block
  * @retval  Operation Status.
  */
int32_t MX_WIFI_TLS_set_nonblock(MX_WIFIObject_t *Obj, mtls_t tls, int32_t nonblock)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  tls_set_nonblock_cparams_t cp;
  tls_set_nonblock_rparams_t rp;
  uint16_t out_size = sizeof(rp);

  if ((NULL == Obj) || (NULL == tls))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cp.tls = tls;
    cp.nonblock = nonblock;
    memset(&rp, 0, sizeof(rp));
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_TLS_SET_NONBLOCK_CMD,
                                          (uint8_t *)&cp, sizeof(cp),
                                          (uint8_t *)&rp, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (rp.ret == MIPC_CODE_SUCCESS)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief   Start webserver.
  * @param   Obj: pointer to module handle
  * @retval  Operation Status.
  */
int32_t MX_WIFI_Webserver_start(MX_WIFIObject_t *Obj)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;

#if 0
  int32_t ret_flag = MIPC_RET_FLAG_CLR;
  int32_t addr_ret_val;
  int32_t addr_ret_flag;
  int32_t *p_addr_ret_val;
  int32_t *p_addr_ret_flag;

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    p_addr_ret_val = &ret;
    p_addr_ret_flag = &ret_flag;
    (void)memcpy(&addr_ret_val, &p_addr_ret_val, sizeof(addr_ret_val));
    (void)memcpy(&addr_ret_flag, &p_addr_ret_flag, sizeof(addr_ret_flag));
    if (MIPC_CODE_SUCCESS == web_start_cmd(addr_ret_val, addr_ret_flag))
    {
      if (MIPC_CODE_SUCCESS == mipc_poll(&ret_flag, &ret, Obj->Runtime.Timeout))
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
#endif /* 0 */

  return ret;
}

/**
  * @brief   Stop webserver.
  * @param   Obj: pointer to module handle
  * @retval  Operation Status.
  */
int32_t MX_WIFI_Webserver_stop(MX_WIFIObject_t *Obj)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;

#if 0
  int32_t ret_flag = MIPC_RET_FLAG_CLR;
  int32_t addr_ret_val;
  int32_t addr_ret_flag;
  int32_t *p_addr_ret_val;
  int32_t *p_addr_ret_flag;

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    p_addr_ret_val = &ret;
    p_addr_ret_flag = &ret_flag;
    (void)memcpy(&addr_ret_val, &p_addr_ret_val, sizeof(addr_ret_val));
    (void)memcpy(&addr_ret_flag, &p_addr_ret_flag, sizeof(addr_ret_flag));
    if (MIPC_CODE_SUCCESS == web_stop_cmd(addr_ret_val, addr_ret_flag))
    {
      if (MIPC_CODE_SUCCESS == mipc_poll(&ret_flag, &ret, Obj->Runtime.Timeout))
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
#endif /* 0 */

  return ret;
}

/**
  * @brief   Start FOTA.
  * @param   Obj: pointer to module handle
  * @param   url: HTTP/HTTPS url of bin file to update
  * @param   md5: MD5 string(32Bytes) of bin file to update
  * @param   ota_status_callback: callback function for ota status
  * @retval  Operation Status.
  */
int32_t MX_WIFI_FOTA_start(MX_WIFIObject_t *Obj, const char *url, const char *md5,
                           mx_wifi_fota_status_cb_t fota_status_callback, uint32_t user_args)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;

#if 0
  int32_t ret_flag = MIPC_RET_FLAG_CLR;
  int32_t addr_ret_val;
  int32_t addr_ret_flag;
  int32_t *p_addr_ret_val;
  int32_t *p_addr_ret_flag;

  if ((NULL == Obj) || (NULL == url) || (NULL == md5))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    if (NULL != fota_status_callback)
    {
      Obj->Runtime.fota_status_cb = fota_status_callback;
      Obj->Runtime.fota_user_args = user_args;
    }
    else
    {
      Obj->Runtime.fota_status_cb = NULL;
      Obj->Runtime.fota_user_args = 0;
    }

    p_addr_ret_val = &ret;
    p_addr_ret_flag = &ret_flag;
    (void)memcpy(&addr_ret_val, &p_addr_ret_val, sizeof(addr_ret_val));
    (void)memcpy(&addr_ret_flag, &p_addr_ret_flag, sizeof(addr_ret_flag));
    if (MIPC_CODE_SUCCESS == fota_start_cmd(url, (int32_t)strlen(url), md5, (int32_t)strlen(md5),
                                            addr_ret_val, addr_ret_flag))
    {
      if (MIPC_CODE_SUCCESS == mipc_poll(&ret_flag, &ret, Obj->Runtime.Timeout))
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
#endif /* 0 */

  return ret;
}

#else /* MX_WIFI_NETWORK_BYPASS_MODE */

/**
  * @brief  Set network bypass mode
  * @param  Obj: pointer to module handle
  * @param  enable: 0=disable, 1=enable
  * @param  netlink_input_callbck: input data callback function
  * @param  user_args: user arguments for callback function
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_Network_bypass_mode_set(MX_WIFIObject_t *Obj, int32_t enable,
                                                 mx_wifi_netlink_input_cb_t netlink_input_callbck,
                                                 void *user_args)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  wifi_bypass_set_cparams_t cparams;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t out_size = sizeof(status);

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    if ((NULL != netlink_input_callbck) && (1 == enable))
    {
      Obj->Runtime.netlink_input_cb = netlink_input_callbck;
      Obj->Runtime.netlink_user_args = user_args;
    }
    else
    {
      Obj->Runtime.netlink_input_cb = NULL;
      Obj->Runtime.netlink_user_args = NULL;
    }

    cparams.mode = enable;
    if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_BYPASS_SET_CMD,
                                          (uint8_t *)&cparams, sizeof(cparams),
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

/**
  * @brief  Network bypass mode data output
  * @param  Obj: pointer to module handle
  * @param  pbuf: pbuf data to send
  * @param  size: pbuf size
  * @param  data: pbuf payload
  * @param  len:  payload len
  * @param  interface: STATION_IDX, SOFTAP_IDX(not supported now)
  * @retval Operation Status.
  */
MX_WIFI_STATUS_T MX_WIFI_Network_bypass_netlink_output(MX_WIFIObject_t *Obj, void *data,
                                                       int32_t len,
                                                       int32_t interface)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  wifi_bypass_out_cparams_t *cparams;
  uint16_t cparams_size;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t rparams_size = sizeof(status);

  if ((NULL == Obj) || (len <= 0) || \
      (((int32_t)STATION_IDX != interface) && ((int32_t)SOFTAP_IDX != interface)))
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    cparams_size = sizeof(wifi_bypass_out_cparams_t) + len;
    if (cparams_size > MX_WIFI_IPC_PAYLOAD_SIZE)
    {
      len = MX_WIFI_IPC_PAYLOAD_SIZE - sizeof(wifi_bypass_out_cparams_t);
      return MX_WIFI_STATUS_PARAM_ERROR;
    }
#if MX_WIFI_TX_BUFFER_NO_COPY
    /* structure of data buffer must support head room provision to add information in from of data payload */
    cparams = (wifi_bypass_out_cparams_t *)((uint8_t *)data - sizeof(wifi_bypass_out_cparams_t));
#else
    cparams = (wifi_bypass_out_cparams_t *)MX_WIFI_MALLOC(len + sizeof(wifi_bypass_out_cparams_t));
#endif /* MX_WIFI_TX_BUFFER_NO_COPY */

    if (NULL != cparams)
    {
      cparams->idx = interface;
      cparams->data_len = len;

#if MX_WIFI_TX_BUFFER_NO_COPY == 0
      /* copy the whole payload */
      uint8_t *dst;
      dst = (uint8_t *)cparams + sizeof(wifi_bypass_out_cparams_t);
      memcpy(dst, data, len);
#endif  /* MX_WIFI_TX_BUFFER_NO_COPY */

      if (MIPC_CODE_SUCCESS == mipc_request(MIPC_API_WIFI_BYPASS_OUT_CMD,
                                            (uint8_t *)cparams, cparams_size,
                                            (uint8_t *)&status, &rparams_size,
                                            MX_WIFI_CMD_TIMEOUT))
      {
        if (MIPC_CODE_SUCCESS == status)
        {
          ret = MX_WIFI_STATUS_OK;
        }
      }
#if MX_WIFI_TX_BUFFER_NO_COPY == 0
      MX_WIFI_FREE(cparams);
#endif /* MX_WIFI_TX_BUFFER_NO_COPY */
    }
    else
    {
      /*  no memory */
      DEBUG_LOG("No memory!!!\n");
    }
  }

  return ret;
}
#endif /* MX_WIFI_NETWORK_BYPASS_MODE */

/**
  * @brief   set powersave onoff for wifi station mode.
  * @param   Obj: pointer to module handle
  * @param   ps_onoff: 0 -- powersave off, 1 -- powersave on
  * @retval  Operation Status.
  */
int32_t MX_WIFI_station_powersave(MX_WIFIObject_t *Obj, int32_t ps_onoff)
{
  int32_t ret = MX_WIFI_STATUS_ERROR;
  uint16_t mipc_ps_cmd;
  int32_t status = MIPC_CODE_ERROR;
  uint16_t out_size = sizeof(status);

  if (NULL == Obj)
  {
    ret = MX_WIFI_STATUS_PARAM_ERROR;
  }
  else
  {
    if (ps_onoff)
    {
      mipc_ps_cmd = MIPC_API_WIFI_PS_ON_CMD;
    }
    else
    {
      mipc_ps_cmd = MIPC_API_WIFI_PS_OFF_CMD;
    }
    if (MIPC_CODE_SUCCESS == mipc_request(mipc_ps_cmd, NULL, 0,
                                          (uint8_t *)&status, &out_size,
                                          MX_WIFI_CMD_TIMEOUT))
    {
      if (MIPC_CODE_SUCCESS == status)
      {
        ret = MX_WIFI_STATUS_OK;
      }
    }
  }
  return ret;
}

