/***********************************************************************************************************************
* File Name    : internal_flash.h
* Description  : This file contains Internal Data Flash Block Areas used for specific Data storage.
*              : Its Data structures and Helper functions.
***********************************************************************************************************************/

/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

#ifndef INTERNAL_FLASH_H_
#define INTERNAL_FLASH_H_

#include "bsp_api.h"
#include "tx_api.h"
#include "hal_data.h"
/* Provision info storage address ( internal flash ) */

#define FLASH_BLOCK_SIZE                (64)
#define DF_4K_OFFSET                    (0x00001000)
#define INTERNAL_DF_BASE_ADDR           (0x40100000)

#define CERT_KEYS_BASE_ADDR             (0x40104000)

#define AUTO_PROV_INFO_STORAGE_ADDR     (INTERNAL_DF_BASE_ADDR + (3*DF_4K_OFFSET))

#define NET_CONFIG_BLOCK_ADDR          (AUTO_PROV_INFO_STORAGE_ADDR)
#define NET_CONFIG_BLOCK_LEN           (FLASH_BLOCK_SIZE * 16) ///< 1024 bytes

#define IOT_CONFIG_BLOCK_ADDR          (NET_CONFIG_BLOCK_ADDR + NET_CONFIG_BLOCK_LEN)
#define IOT_CONFIG_BLOCK_LEN           (FLASH_BLOCK_SIZE * 16) ///< 1024 bytes

#define CERT_INFO_BLOCKS_ADDR          (IOT_CONFIG_BLOCK_ADDR + IOT_CONFIG_BLOCK_LEN)
#define CERT_INFO_BLOCKS_LEN           (FLASH_BLOCK_SIZE * 1) ///<64 bytes

#define ROOTCA_BLOCK_ADDR              (CERT_KEYS_BASE_ADDR)
#define ROOTCA_BLOCK_LEN               (FLASH_BLOCK_SIZE * 64) ///< 4096 bytes

#define DEVCERT_BLOCK_ADDR             (ROOTCA_BLOCK_ADDR + ROOTCA_BLOCK_LEN)
#define DEVCERT_BLOCK_LEN              (FLASH_BLOCK_SIZE * 64) ///< 4096 bytes

#define PRIV_KEY_BLOCK_ADDR            (DEVCERT_BLOCK_ADDR + DEVCERT_BLOCK_LEN)
#define PRIV_KEY_BLOCK_LEN             (FLASH_BLOCK_SIZE * 64) ///< 4096 bytes

#define NUM_OF_BLOCKS(x)                ((x) / FLASH_BLOCK_SIZE)


/* Flash operations event ids */
#define ERASE_COMPLETE_EVT          1
#define WRITE_COMPLETE_EVT          2
#define ERR_FAILURE_EVT             4

/* Info type IDs */
typedef enum e_int_storage_type
{
    NET_INPUT_CFG,             ///< Network Interface Input Configuration
    IOT_INPUT_CFG,             ///< IoT Service Input Configuration
    ROOTCA_CERT_CFG,           ///< Root CA Certificate
    DEVCERT_CFG,               ///< Device Certificate
    PRI_KEY_CFG,               ///< Device Private Key
	CERT_CFG_TYPE,            ///< Cert Configuration
} int_storage_type_t;

ssp_err_t int_storage_init(void);
ssp_err_t int_storage_deinit(void);
ssp_err_t int_storage_read(uint8_t *data_buffer, unsigned int data_size,unsigned int info_type);
ssp_err_t int_storage_write(uint8_t *data_buffer , unsigned int data_size, unsigned int info_type);
#endif /* INTERNAL_FLASH_H_ */
