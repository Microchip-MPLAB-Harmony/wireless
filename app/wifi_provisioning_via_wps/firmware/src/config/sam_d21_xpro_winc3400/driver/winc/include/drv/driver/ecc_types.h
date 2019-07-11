/*******************************************************************************
  File Name:
    ecc_types.h

  Summary:
    Elliptic Curve Cryptography Module Interface.

  Description:
    Elliptic Curve Cryptography Module Interface.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
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

#ifndef __ECC_TYPES_H__
#define __ECC_TYPES_H__

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "m2m_types.h"

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/



#define ECC_LARGEST_CURVE_SIZE                      (32)
/*!<
    The size of the the largest supported EC. For now, assuming
    the 256-bit EC is the largest supported curve type.
*/


#define ECC_POINT_MAX_SIZE                          ECC_LARGEST_CURVE_SIZE
/*!<
    Maximum size of one coordinate of an EC point.
*/


#define ECC_POINT_MAX_SIZE_WORDS                    (ECC_POINT_MAX_SIZE / 4)
/*!<
    SIZE in 32-bit words.
*/


#define ECC_NUM_SUPP_CURVES                         ((sizeof(gastrECCSuppList)) / (sizeof(tstrEllipticCurve)))
/*!<
*/


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
DATA TYPES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/


/*!
@enum   \
    tenuEcNamedCurve

@brief  EC Named Curves

    Defines a list of supported ECC named curves.
*/
typedef enum EcNamedCurve{
    EC_SECP192R1        = 19,
    /*!<
        It is defined by NIST as P192 and by the SEC Group as secp192r1.
    */
    EC_SECP256R1        = 23,
    /*!<
        It is defined by NIST as P256 and by the SEC Group as secp256r1.
    */
    EC_SECP384R1        = 24,
    /*!<
        It is defined by NIST as P384 and by the SEC Group as secp384r1.
    */
    EC_SECP521R1        = 25,
    /*!<
        It is defined by NIST as P521 and by the SEC Group as secp521r1.
    */
    EC_UNKNOWN          = 255
}tenuEcNamedCurve;


/*!
@struct \
    tstrECPoint

@brief  Elliptic Curve point representation
*/
typedef struct EcPoint{
    uint8_t     X[ECC_POINT_MAX_SIZE];
    /*!<
        The X-coordinate of the ec point.
    */
    uint8_t     Y[ECC_POINT_MAX_SIZE];
    /*!<
        The Y-coordinate of the ec point.
    */
    uint16_t    u16Size;
    /*!<
        Point size in bytes (for each of the coordinates).
    */
    uint16_t    u16PrivKeyID;
    /*!<
        ID for the corresponding private key.
    */
}tstrECPoint;


/*!
@struct \
    tstrECDomainParam

@brief  ECC Curve Domain Parameters

    The structure defines the ECC domain parameters for curves defined over prime finite fields.
*/
typedef struct EcDomainParam{
    uint32_t    p[ECC_POINT_MAX_SIZE_WORDS];
    uint32_t    a[ECC_POINT_MAX_SIZE_WORDS];
    uint32_t    b[ECC_POINT_MAX_SIZE_WORDS];
    tstrECPoint G;
}tstrECDomainParam;


/*!
@struct \
    tstrEllipticCurve

@brief
    Definition of an elliptic curve
*/
typedef struct{
    tenuEcNamedCurve    enuType;
    tstrECDomainParam   strParam;
}tstrEllipticCurve;


typedef enum{
    ECC_REQ_NONE,
    ECC_REQ_CLIENT_ECDH,
    ECC_REQ_SERVER_ECDH,
    ECC_REQ_GEN_KEY,
    ECC_REQ_SIGN_GEN,
    ECC_REQ_SIGN_VERIFY
}tenuEccREQ;


typedef struct{
    tstrECPoint strPubKey;
    uint8_t     au8Key[ECC_POINT_MAX_SIZE];
}tstrEcdhReqInfo;


typedef struct{
    uint32_t    u32nSig;
}tstrEcdsaVerifyReqInfo;


typedef struct{
    uint16_t    u16CurveType;
    uint16_t    u16HashSz;
}tstrEcdsaSignReqInfo;


typedef struct{
    uint16_t    u16REQ;
    uint16_t    u16Status;
    uint32_t    u32UserData;
    uint32_t    u32SeqNo;
    union{
        tstrEcdhReqInfo         strEcdhREQ;
        tstrEcdsaSignReqInfo    strEcdsaSignREQ;
        tstrEcdsaVerifyReqInfo  strEcdsaVerifyREQ;
    };
}tstrEccReqInfo;

#endif /* __ECC_TYPES_H__ */
