/***********************************************************************************
    Filename: crc.cpp

    Copyright 2008 Texas Instruments, Inc.
***********************************************************************************/

#include "crc.hpp"

//-------------------------------------------------------------------------------------------------------
//  uint16_t crcCalc(uint16_t crcReg, uint8_t crcData) 
//
//  DESCRIPTION:
//      Calculates the 16-bit CRC. The function requires that the CRC_POLYNOM is defined,
//      which gives the wanted CRC polynom. 
//
//  ARGUMENTS: 
//      uint8_t  crcData  - Data to perform the CRC-16 operation on.
//      uint16_t crcReg   - Current or initial value of the CRC calculation
//
//  RETURN:
//      The value returned is the 16-bit CRC (of the data supplied so far). 
//-------------------------------------------------------------------------------------------------------
uint16_t crcCalc(uint16_t crcReg, uint8_t crcData) 
{
  uint8_t i;

  for (i = 0; i < 8; i++) {
    // If upper most bit is 1
    if (((crcReg & 0x8000) >> 8) ^ (crcData & 0x80)) {
      crcReg = (crcReg << 1)  ^ CRC_POLYNOM;
    }
    else {
      crcReg = (crcReg << 1);
    }

    crcData <<= 1;
  }

  return crcReg;
}
//-------------------------------------------------------------------------------------------------------
//  uint16_t crc16_EN13757(uint8_t *data, size_t len)
//
//  DESCRIPTION:
//      Calculates CRC value for given payload by data pointer.  
//
//  ARGUMENTS: 
//      uint8_t*  data  - Data to perform the CRC evaluation.
//      size_t len - Data length.
//
//  RETURN:
//      Evaluated CRC value (2 Bytes).
//-------------------------------------------------------------------------------------------------------
uint16_t crc16_EN13757(uint8_t *data, size_t len)
{
    uint16_t crc = 0x0000;

    //assert(len == 0 || data != NULL);

    for (size_t i=0; i<len; ++i)
    {
        crc = crcCalc(crc, data[i]);
    }

    return (~crc);
}


//-------------------------------------------------------------------------------------------------------
//  uint8_t crcRemove(uint8_t *data, uint8_t dataLen)
//
//  DESCRIPTION:
//      Remove CRC from data buffer 
//
//  ARGUMENTS: 
//      uint8_t*  data  - Data to perform the CRC remove operation on.
//      uint8_t dataLen - Current data length in buffer.
//
//  RETURN:
//      New data length in buffer.
//-------------------------------------------------------------------------------------------------------
uint8_t crcRemove(uint8_t *data, uint8_t dataLen)
{
  uint8_t *dst = data;
  uint8_t dstLen = 0;

  dst += 10;
  dstLen += 10;

  data += 12;
  dataLen -= 12;

  while (dataLen) {
    if (dataLen >= 18) {
      memmove(dst, data, 16);

      dst += 16;
      dstLen += 16;

      data += 18;
      dataLen -= 18;
    }
    else {
      memmove(dst, data, dataLen-2);

      dst += (dataLen-2);
      dstLen += (dataLen-2);

      data += dataLen;
      dataLen -= dataLen;
    }
  }
  return dstLen;
}


/***********************************************************************************
  Copyright 2008 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
***********************************************************************************/
