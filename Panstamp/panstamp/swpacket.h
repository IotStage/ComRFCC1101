/**
 * Copyright (c) 2011 panStamp <contact@panstamp.com>
 * 
 * This file is part of the panStamp project.
 * 
 * panStamp  is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * any later version.
 * 
 * panStamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA
 * 
 * Author: Daniel Berenguer
 * Creation date: 03/03/2011
 */

#ifndef _SWPACKET_H
#define _SWPACKET_H

#include "Arduino.h"
#include "ccpacket.h"

/**
 * SWAP definitions
 */
#define SWAP_DATA_HEAD_LEN     6
#define SWAP_REG_VAL_LEN       CC1101_DATA_LEN - SWAP_DATA_HEAD_LEN   // SWAP data payload - max length
#define SWAP_BCAST_ADDR        0x00                                   // SWAP broadcast address
#define SWAP_NB_TX_TRIES       3                                      // Number of transmission retries
#define SWAP_TX_DELAY          panstamp.cc1101.devAddress * 2         // Delay before sending

/**
 * SWAP message functions
 */
enum SWAPFUNCT
{
  SWAPFUNCT_STA = 0x00,
  SWAPFUNCT_QRY,
  SWAPFUNCT_CMD
};

/**
 * Macros
 */
#define smartDecrypt()         smartEncrypt(true)

/**
 * Structure: SWDATA
 * 
 * Description:
 * SWAP data structure
 */
struct SWDATA
{
    /**
     * Data buffer
     */
    byte *data;

    /**
     * Data length
     */
    byte length;
};

class SWPACKET : public CCPACKET
{
  private:
    /**
     * smartEncrypt
     * 
     * Apply Smart Encryption to the SWAP packet passed as argument
     *
     * 'decrypt': if true, Decrypt packet. Encrypt otherwise
     */
    void smartEncrypt(bool decrypt=false);

  public:
    /**
     * Destination address
     */
    byte destAddr;

    /**
     * Source address
     */
    byte srcAddr;

    /**
     * Hop counter. Incremented each time the message is repeated
     */
    byte hop;

    /**
     * Security option byte
     */
    byte security;

    /**
     * Security cyclic nonce
     */
    byte nonce;

    /**
     * Function byte
     */
    byte function;

    /**
     * Register address
     */
    byte regAddr;

    /**
     * Register id
     */
    byte regId;

    /**
     * Register value
     */
    SWDATA value;

    /**
     * SWPACKET
     * 
     * Class constructor
     * 
     * 'packet'	Raw CC1101 packet
     */
    SWPACKET(CCPACKET packet);

    /**
     * SWPACKET
     * 
     * Class constructor
     */
    SWPACKET(void);
    
    /**
     * send
     * 
     * Send SWAP packet
     *
     * Return:
     *  True if the transmission succeeds
     *  False otherwise
     */
    boolean send(void);
};

#endif
