/*******************************************************************************
  PIC32MZW Driver STA Implementation

  File Name:
    wdrv_pic32mzw_sta.c

  Summary:
    PIC32MZW wireless driver STA implementation.

  Description:
    PIC32MZW wireless driver STA implementation.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <string.h>

#include "wdrv_pic32mzw.h"
#include "wdrv_pic32mzw_common.h"
#include "wdrv_pic32mzw_sta.h"
#include "wdrv_pic32mzw_cfg.h"

// *****************************************************************************
// *****************************************************************************
// Section: PIC32MZW Driver STA Implementation
// *****************************************************************************
// *****************************************************************************

DRV_PIC32MZW_11I_MASK DRV_PIC32MZW_Get11iMask
(
    WDRV_PIC32MZW_AUTH_TYPE authType,
    WDRV_PIC32MZW_AUTH_MOD_MASK authMod
);

//*******************************************************************************
/*
  Function:
    WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_BSSConnect
    (
        DRV_HANDLE handle,
        const WDRV_PIC32MZW_BSS_CONTEXT *const pBSSCtx,
        const WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx,
        const WDRV_PIC32MZW_BSSCON_NOTIFY_CALLBACK pfNotifyCallback
    )

  Summary:
    Connects to a BSS in infrastructure station mode.

  Description:
    Using the defined BSS and authentication contexts this function requests
      the PIC32MZW connect to the BSS as an infrastructure station.

  Remarks:
    See wdrv_pic32mzw_sta.h for usage information.

*/

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_BSSConnect
(
    DRV_HANDLE handle,
    const WDRV_PIC32MZW_BSS_CONTEXT *const pBSSCtx,
    const WDRV_PIC32MZW_AUTH_CONTEXT *const pAuthCtx,
    const WDRV_PIC32MZW_BSSCON_NOTIFY_CALLBACK pfNotifyCallback
)
{
    WDRV_PIC32MZW_DCPT *pDcpt = (WDRV_PIC32MZW_DCPT *)handle;
    DRV_PIC32MZW_WIDCTX wids;
    uint8_t channel;
    DRV_PIC32MZW_11I_MASK dot11iInfo = 0;

    /* Ensure the driver handle and user pointer is valid. */
    if ((NULL == pDcpt) || (NULL == pBSSCtx))
    {
        return WDRV_PIC32MZW_STATUS_INVALID_ARG;
    }

    /* Ensure the driver instance has been opened for use. */
    if (false == pDcpt->isOpen)
    {
        return WDRV_PIC32MZW_STATUS_NOT_OPEN;
    }

    /* Ensure the BSS context is valid. */
    if (false == WDRV_PIC32MZW_BSSCtxIsValid(pBSSCtx, false))
    {
        return WDRV_PIC32MZW_STATUS_INVALID_CONTEXT;
    }

    /* NULL authentication context is OK - no encryption. */
    if (NULL != pAuthCtx)
    {
        /* Ensure the authentication context is valid. */
        if (false == WDRV_PIC32MZW_AuthCtxIsValid(pAuthCtx))
        {
            return WDRV_PIC32MZW_STATUS_INVALID_CONTEXT;
        }

        /* Convert authentication type to an 11i bitmap. */
        dot11iInfo = DRV_PIC32MZW_Get11iMask(pAuthCtx->authType, pAuthCtx->authMod);
    }

    channel = pBSSCtx->channel;

    /* Convert public API representation of all channels to
       internal representation. */
    if (WDRV_PIC32MZW_CID_ANY == channel)
    {
        channel = 0xff;
    }

    /* Allocate memory for the WIDs. */
    DRV_PIC32MZW_MultiWIDInit(&wids, 512);

    /* Switch to STA mode (0). */
    DRV_PIC32MZW_MultiWIDAddValue(&wids, DRV_WIFI_WID_SWITCH_MODE, 0);

    /* Set Transmit Rate to Autorate */
    DRV_PIC32MZW_MultiWIDAddValue(&wids, DRV_WIFI_WID_CURRENT_TX_RATE, 0);

    /* Set BSSID if provided. */
    if (true == pBSSCtx->bssid.valid)
    {
        DRV_PIC32MZW_MultiWIDAddData(&wids, DRV_WIFI_WID_BSSID, pBSSCtx->bssid.addr, 6);
    }

    /* Set SSID. */
    DRV_PIC32MZW_MultiWIDAddData(&wids, DRV_WIFI_WID_SSID, pBSSCtx->ssid.name, pBSSCtx->ssid.length);

    /* Set channel. */
    DRV_PIC32MZW_MultiWIDAddValue(&wids, DRV_WIFI_WID_USER_PREF_CHANNEL, channel);

    /* Set 11i info as derived above. */
    DRV_PIC32MZW_MultiWIDAddValue(&wids, DRV_WIFI_WID_11I_SETTINGS, (uint32_t)dot11iInfo);

    /* Set credentials for whichever authentication types are enabled. */
    if (DRV_PIC32MZW_PRIVACY == dot11iInfo)
    {
        /* Set WEP credentials. */
        DRV_PIC32MZW_MultiWIDAddValue(&wids, DRV_WIFI_WID_KEY_ID, pAuthCtx->authInfo.WEP.idx-1);
        DRV_PIC32MZW_MultiWIDAddData(&wids, DRV_WIFI_WID_WEP_KEY_VALUE, pAuthCtx->authInfo.WEP.key, pAuthCtx->authInfo.WEP.size);
    }

    if (dot11iInfo & DRV_PIC32MZW_11I_PSK)
    {
        /* Set PSK credentials. */
        DRV_PIC32MZW_MultiWIDAddData(&wids, DRV_WIFI_WID_11I_PSK,
                pAuthCtx->authInfo.personal.password,
                pAuthCtx->authInfo.personal.size);
    }
    if (dot11iInfo & DRV_PIC32MZW_11I_SAE)
    {
        /* Set SAE credentials. */
        DRV_PIC32MZW_MultiWIDAddData(&wids, DRV_WIFI_WID_RSNA_PASSWORD,
                pAuthCtx->authInfo.personal.password,
                pAuthCtx->authInfo.personal.size);
    }

    /* Set 11g compatibility mode 1 (2). */
    DRV_PIC32MZW_MultiWIDAddValue(&wids, DRV_WIFI_WID_11G_OPERATING_MODE, 2);
    /* Set Ack policy: Normal Ack (0). */
    DRV_PIC32MZW_MultiWIDAddValue(&wids, DRV_WIFI_WID_ACK_POLICY, 0);
    /* Set 11n enabled (1). */
    DRV_PIC32MZW_MultiWIDAddValue(&wids, DRV_WIFI_WID_11N_ENABLE, 1);

    /* Write the WIDs. */
    if (false == DRV_PIC32MZW_MultiWid_Write(&wids))
    {
        return WDRV_PIC32MZW_STATUS_CONNECT_FAIL;
    }

    pDcpt->pCtrl->pfConnectNotifyCB = pfNotifyCallback;
    pDcpt->pCtrl->connectedState    = WDRV_PIC32MZW_CONN_STATE_CONNECTING;

    return WDRV_PIC32MZW_STATUS_OK;
}

//*******************************************************************************
/*
  Function:
    WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_BSSDisconnect(DRV_HANDLE handle)

  Summary:
    Disconnects from a BSS.

  Description:
    Disconnects from an existing BSS.

  Remarks:
    See wdrv_pic32mzw_sta.h for usage information.

*/

WDRV_PIC32MZW_STATUS WDRV_PIC32MZW_BSSDisconnect(DRV_HANDLE handle)
{
    WDRV_PIC32MZW_DCPT *pDcpt = (WDRV_PIC32MZW_DCPT *)handle;
    DRV_PIC32MZW_WIDCTX wids;

    /* Ensure the driver handle is valid. */
    if (NULL == pDcpt)
    {
        return WDRV_PIC32MZW_STATUS_INVALID_ARG;
    }

    /* Ensure the driver instance has been opened for use. */
    if (false == pDcpt->isOpen)
    {
        return WDRV_PIC32MZW_STATUS_NOT_OPEN;
    }

    /* Ensure PIC32MZW is not connected or attempting to connect. */
    if (WDRV_PIC32MZW_CONN_STATE_DISCONNECTED == pDcpt->pCtrl->connectedState)
    {
        return WDRV_PIC32MZW_STATUS_REQUEST_ERROR;
    }

    /* Disconnect PIC32MZW. */
    DRV_PIC32MZW_MultiWIDInit(&wids, 16);
    DRV_PIC32MZW_MultiWIDAddString(&wids, DRV_WIFI_WID_SSID, "\0");
    DRV_PIC32MZW_MultiWIDAddValue(&wids, DRV_WIFI_WID_DISCONNECT, 1);

    if (false == DRV_PIC32MZW_MultiWid_Write(&wids))
    {
        return WDRV_PIC32MZW_STATUS_DISCONNECT_FAIL;
    }

    return WDRV_PIC32MZW_STATUS_OK;
}
