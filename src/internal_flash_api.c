/***********************************************************************************************************************
* File Name    : internal_flash_api.c
* Description  : This file contains internal flash access functions(init/read/write/erase) from the application level
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

#include "internal_flash.h"
#include "common_util.h"
#include "MQTT_Thread.h"
#include "MQTT_Config.h"

static UINT wait_for_flash_idle(int event_type)
{
    UINT  retval        = 0;
    ULONG actual_events = 0;

    //print_to_console("Check for Internal Flash for idle \r\n");


    if( event_type == ERASE_COMPLETE_EVT)
    {
        retval = tx_event_flags_get(&g_int_storage_evt_grp, ERASE_COMPLETE_EVT, TX_AND_CLEAR, &actual_events, TX_WAIT_FOREVER);
    }
    else if (event_type == WRITE_COMPLETE_EVT)
    {
        retval = tx_event_flags_get(&g_int_storage_evt_grp, (WRITE_COMPLETE_EVT | ERR_FAILURE_EVT), TX_OR_CLEAR, &actual_events, TX_WAIT_FOREVER);
    }
    else
    {
        retval = SSP_ERR_INVALID_ARGUMENT;
    }

    //print_to_console("Internal Flash is idle to operate\r\n");

    return retval;
}

void flash_callback(flash_callback_args_t * p_args)
{
	if(NULL == p_args)
    {
        print_to_console("\r\n Null pointer in flash callback \r\n");
        return;
    }
    
    if (p_args->event == FLASH_EVENT_ERASE_COMPLETE)
    {
        tx_event_flags_set(&g_int_storage_evt_grp, ERASE_COMPLETE_EVT, TX_OR);
    }
    else if (p_args->event == FLASH_EVENT_WRITE_COMPLETE)
    {
        tx_event_flags_set(&g_int_storage_evt_grp, WRITE_COMPLETE_EVT, TX_OR);
    }
    else if ((p_args->event == FLASH_EVENT_ERR_DF_ACCESS) || (p_args->event == FLASH_EVENT_ERR_CMD_LOCKED) ||
            (p_args->event == FLASH_EVENT_ERR_FAILURE))
    {
        tx_event_flags_set(&g_int_storage_evt_grp, ERR_FAILURE_EVT, TX_OR);
    }
}

ssp_err_t int_storage_init(void)
{
    ssp_err_t ssp_err = SSP_SUCCESS;

    ssp_err = (g_int_storageInst.p_api->open(g_int_storageInst.p_ctrl, g_int_storageInst.p_cfg));
    if (ssp_err != SSP_SUCCESS)
    {
        print_to_console("Initializing data flash: failed\r\n");
        APP_ERR_TRAP(ssp_err);
    }

    return ssp_err;
}

ssp_err_t int_storage_read(uint8_t *data_buffer, unsigned int data_size,unsigned int info_type)
{
    unsigned int _dataflash_addr = 0 ;

    switch(info_type)
    {
        case NET_INPUT_CFG:
            _dataflash_addr = NET_CONFIG_BLOCK_ADDR;
            break;
        case IOT_INPUT_CFG:
            _dataflash_addr = IOT_CONFIG_BLOCK_ADDR;
            break;
        case ROOTCA_CERT_CFG:
            _dataflash_addr = ROOTCA_BLOCK_ADDR;
            break;
        case DEVCERT_CFG:
            _dataflash_addr = DEVCERT_BLOCK_ADDR;
            break;
        case PRI_KEY_CFG:
            _dataflash_addr = PRIV_KEY_BLOCK_ADDR;
            break;
        case CERT_CFG_TYPE:
            _dataflash_addr = CERT_INFO_BLOCKS_ADDR;
            break;
        default:
            return SSP_ERR_INVALID_ARGUMENT;
    }

    return (g_int_storageInst.p_api->read(g_int_storageInst.p_ctrl,data_buffer, _dataflash_addr, data_size));
}

ssp_err_t int_storage_write(uint8_t *data_buffer , unsigned int data_size, unsigned int info_type)
{
    ssp_err_t ret_val = SSP_SUCCESS;
    unsigned int _dataflash_addr = 0;
    unsigned int _num_of_blocks = 1;
    uint32_t copy_size = 0;

    switch(info_type)
    {
        case NET_INPUT_CFG:
            _dataflash_addr = NET_CONFIG_BLOCK_ADDR;
            _num_of_blocks  = (unsigned int)NUM_OF_BLOCKS(NET_CONFIG_BLOCK_LEN);
            break;
        case IOT_INPUT_CFG:
            _dataflash_addr = IOT_CONFIG_BLOCK_ADDR;
            _num_of_blocks = (unsigned int)NUM_OF_BLOCKS(IOT_CONFIG_BLOCK_LEN);
            break;
        case ROOTCA_CERT_CFG:
            _dataflash_addr = ROOTCA_BLOCK_ADDR;
            _num_of_blocks = (unsigned int)NUM_OF_BLOCKS(ROOTCA_BLOCK_LEN);
            break;
        case DEVCERT_CFG:
            _dataflash_addr = DEVCERT_BLOCK_ADDR;
            _num_of_blocks  = (unsigned int)NUM_OF_BLOCKS(DEVCERT_BLOCK_LEN);
            break;
        case PRI_KEY_CFG:
            _dataflash_addr = PRIV_KEY_BLOCK_ADDR;
            _num_of_blocks = (unsigned int)NUM_OF_BLOCKS(PRIV_KEY_BLOCK_LEN);
            break;

        case CERT_CFG_TYPE:
            _dataflash_addr = CERT_INFO_BLOCKS_ADDR;
            _num_of_blocks = NUM_OF_BLOCKS(CERT_INFO_BLOCKS_LEN);
            break;
        default:
            return SSP_ERR_INVALID_ARGUMENT;
    }

    if( data_size > (_num_of_blocks * FLASH_BLOCK_SIZE))
    {
        //print_to_console("Input data buffer size is greater than allocated size\r\n");
        return SSP_ERR_INVALID_ARGUMENT;
    }

    /* Erase flash memory location */
    if( SSP_SUCCESS == g_int_storageInst.p_api->erase(g_int_storageInst.p_ctrl, _dataflash_addr, _num_of_blocks))
    {
        /* wait for idle  */
        wait_for_flash_idle(ERASE_COMPLETE_EVT);

        while (data_size > 0)
        {
            if (data_size > FLASH_BLOCK_SIZE)
            {
                copy_size = FLASH_BLOCK_SIZE;
            }
            else
            {
                copy_size = data_size;
            }

            ret_val = g_int_storageInst.p_api->write(g_int_storageInst.p_ctrl, (uint32_t)data_buffer, _dataflash_addr, FLASH_BLOCK_SIZE);
            if ( ret_val == SSP_SUCCESS)
            {
                /* wait for idle */
                wait_for_flash_idle(WRITE_COMPLETE_EVT);

                data_size -= (copy_size);
                data_buffer += (copy_size);
                _dataflash_addr += FLASH_BLOCK_SIZE;
            }
            else
            {
                print_to_console("Failed to write the given buffer content to internal flash memory \r\n");
                ret_val = SSP_ERR_WRITE_FAILED;
                break;
            }
        }
    }
    else
    {
        /* Flash erase failure */
        print_to_console("Failed to erase given block of internal flash memory \r\n");
        ret_val = SSP_ERR_ERASE_FAILED;
    }

    return ret_val;
}

ssp_err_t int_storage_deinit(void)
{
    return (g_int_storageInst.p_api->close(g_int_storageInst.p_ctrl));
}
