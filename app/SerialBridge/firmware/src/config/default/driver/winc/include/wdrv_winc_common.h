/*******************************************************************************
  WINC Driver Common Header File

  Company:
    Microchip Technology Inc.

  File Name:
    wdrv_winc_common.h

  Summary:
    WINC wireless driver common header file.

  Description:
    This file provides common elements of the WINC driver API.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2017 released Microchip Technology Inc. All rights reserved.

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
// DOM-IGNORE-END

#ifndef _WDRV_WINC_COMMON_H
#define _WDRV_WINC_COMMON_H

// *****************************************************************************
// *****************************************************************************
// Section: File includes
// *****************************************************************************
// *****************************************************************************

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "configuration.h"
#include "definitions.h"
#include "osal/osal.h"
#include "wdrv_winc_debug.h"
#include "m2m_types.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus // Provide C++ Compatibility
    extern "C" {
#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: WINC Driver Common Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* WINC Driver All Channels

  Summary:
    WINC Driver All Channels.

  Description:
    This constant is used to represent all WiFi channels.

  Remarks:
    None.
*/

#define WDRV_WINC_ALL_CHANNELS      0xff

// *****************************************************************************
/*  Common API Return Status Code

  Summary:
    API return status codes.

  Description:
    All API functions which return a status code will use one of these to be
      consistent.

  Remarks:
    None.
*/

typedef enum _WDRV_WINC_STATUS
{
    /* Operation was successful. */
    WDRV_WINC_STATUS_OK = 0,

    /* Driver instance has not been opened. */
    WDRV_WINC_STATUS_NOT_OPEN,

    /* The arguments supplied are not valid. */
    WDRV_WINC_STATUS_INVALID_ARG,

    /* A scan operation is currently in progress. */
    WDRV_WINC_STATUS_SCAN_IN_PROGRESS,

    /* No BSS information is available. */
    WDRV_WINC_STATUS_NO_BSS_INFO,

    /* No more BSS scan results are available. */
    WDRV_WINC_STATUS_BSS_FIND_END,

    /* The connection attempt has failed. */
    WDRV_WINC_STATUS_CONNECT_FAIL,

    /* The disconnection attempt has failed. */
    WDRV_WINC_STATUS_DISCONNECT_FAIL,

    /* The requested operation could not be completed. */
    WDRV_WINC_STATUS_REQUEST_ERROR,

    /* The context being referenced is invalid. */
    WDRV_WINC_STATUS_INVALID_CONTEXT,

    /* Request could not complete, but may if tried again. */
    WDRV_WINC_STATUS_RETRY_REQUEST,

    /* Out of space in resource. */
    WDRV_WINC_STATUS_NO_SPACE,

    /* No more root cert results are available. */
    WDRV_WINC_STATUS_ROOT_CERT_FIND_END,

    /* No Ethernet buffer was available. */
    WDRV_WINC_STATUS_NO_ETH_BUFFER
} WDRV_WINC_STATUS;

// *****************************************************************************
/*  Connection State

  Summary:
    Defines possible connection states.

  Description:
    A connection can currently either be connected or disconnect.

  Remarks:
    None.
*/

typedef enum
{
    /* Association state is disconnected. */
    WDRV_WINC_CONN_STATE_DISCONNECTED /*DOM-IGNORE-BEGIN*/ = M2M_WIFI_DISCONNECTED /*DOM-IGNORE-END*/,

    /* Association state is connected. */
    WDRV_WINC_CONN_STATE_CONNECTED /*DOM-IGNORE-BEGIN*/ = M2M_WIFI_CONNECTED /*DOM-IGNORE-END*/
} WDRV_WINC_CONN_STATE;

// *****************************************************************************
/*  Connection Error

  Summary:
    Defines possible connection errors.

  Description:
    A connection (and default connection) can result in one of these errors.

  Remarks:
    None.
*/

typedef enum
{
    WDRV_WINC_CONN_ERROR_SCAN /*DOM-IGNORE-BEGIN*/ = M2M_ERR_SCAN_FAIL /*DOM-IGNORE-END*/,
    WDRV_WINC_CONN_ERROR_AUTH /*DOM-IGNORE-BEGIN*/ = M2M_ERR_AUTH_FAIL /*DOM-IGNORE-END*/,
    WDRV_WINC_CONN_ERROR_ASSOC /*DOM-IGNORE-BEGIN*/ = M2M_ERR_ASSOC_FAIL /*DOM-IGNORE-END*/,
    WDRV_WINC_CONN_ERROR_INPROGRESS /*DOM-IGNORE-BEGIN*/ = M2M_ERR_CONN_INPROGRESS /*DOM-IGNORE-END*/,
    WDRV_WINC_CONN_ERROR_NOCRED,
    WDRV_WINC_CONN_ERROR_UNKNOWN
} WDRV_WINC_CONN_ERROR;

// *****************************************************************************
/*  SSID

  Summary:
    Structure to hold an SSID.

  Description:
    The SSID consist of a buffer and a length field.

  Remarks:
    None.
*/

typedef struct _WDRV_WINC_SSID
{
    /* SSID name, up to 32 characters long. */
    uint8_t name[32];

    /* Length of SSID name. */
    uint8_t length;
} WDRV_WINC_SSID;

// *****************************************************************************
/*  Connection Notify Callback

  Summary:
    Callback to notify the user of a change in connection state.

  Description:
    When the connection state changes this callback enable the driver to signal
      the user about that event and reason.

  Parameters:
    handle          - Client handle obtained by a call to WDRV_WINC_Open.
    currentState    - Current connection state.
    errorCode       - Error code.

  Returns:
    None.

  Remarks:
    None.
*/

typedef void (*WDRV_WINC_BSSCON_NOTIFY_CALLBACK)
(
    DRV_HANDLE handle,
    WDRV_WINC_CONN_STATE currentState,
    WDRV_WINC_CONN_ERROR errorCode
);

// *****************************************************************************
/* Generic Status Callback Function Pointer

  Summary:
    Pointer to a generic status callback function.

  Description:
    This defines a generic status function callback type which can be passed
    into certain functions to receive feedback.

  Parameters:
    handle  - Client handle obtained by a call to WDRV_WINC_Open.
    status  - A status value.

  Returns:
    None.

  Remarks:
    The value of the status passed to the function is dependant on the function
    used to register the callback.

    See WDRV_WINC_OTAUpdateFromURL, WDRV_WINC_SwitchActiveFirmwareImage,
    WDRV_WINC_HostFileRead and WDRV_WINC_HostFileErase.
*/

typedef void (*WDRV_WINC_STATUS_CALLBACK)
(
    DRV_HANDLE handle,
    uint8_t status
);

// *****************************************************************************
// *****************************************************************************
// Section: WINC Driver BSS Find Routines
// *****************************************************************************
// *****************************************************************************

//*******************************************************************************
/*
  Function:
    void WDRV_MSDelay(uint32_t ms)

  Summary:
    Performs a delay specified in milli-seconds.

  Description:
    Will block for the number of milli-seconds specified.

  Precondition:
    None.

  Parameters:
    ms - Number of milli-seconds to block.

  Returns:
    None.

  Remarks:
    None.

*/

void WDRV_MSDelay(uint32_t ms);

// DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
// DOM-IGNORE-END

#endif /* _WDRV_WINC_COMMON_H */
