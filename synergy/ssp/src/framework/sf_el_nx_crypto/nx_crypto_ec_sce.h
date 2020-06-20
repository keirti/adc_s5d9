/**************************************************************************/
/*                                                                        */
/*            Copyright (c) 1996-2018 by Express Logic Inc.               */
/*                                                                        */
/*  This software is copyrighted by and is the sole property of Express   */
/*  Logic, Inc.  All rights, title, ownership, or other interests         */
/*  in the software remain the property of Express Logic, Inc.  This      */
/*  software may only be used in accordance with the corresponding        */
/*  license agreement.  Any unauthorized use, duplication, transmission,  */
/*  distribution, or disclosure of this software is expressly forbidden.  */
/*                                                                        */
/*  This Copyright notice may not be removed or modified without prior    */
/*  written consent of Express Logic, Inc.                                */
/*                                                                        */
/*  Express Logic, Inc. reserves the right to modify this software        */
/*  without notice.                                                       */
/*                                                                        */
/*  Express Logic, Inc.                     info@expresslogic.com         */
/*  11423 West Bernardo Court               http://www.expresslogic.com   */
/*  San Diego, CA  92127                                                  */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** NetX Crypto Component                                                 */
/**                                                                       */
/**   Elliptic Curve                                                      */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/


/**************************************************************************/
/*                                                                        */
/*  APPLICATION INTERFACE DEFINITION                       RELEASE        */
/*                                                                        */
/*    nx_crypto_ec_sce.h                                  PORTABLE C      */
/*                                                          5.11 SP1      */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Timothy Stapko, Express Logic, Inc.                                 */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This file defines the symbols, structures and operations for        */
/*    Elliptic Curve Crypto.                                              */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  07-15-2018     Timothy Stapko           Initial Version 5.11 SP1      */
/*                                                                        */
/**************************************************************************/

#ifndef NX_CRYPTO_EC_SCE_H
#define NX_CRYPTO_EC_SCE_H

/* Determine if a C++ compiler is being used.  If so, ensure that standard
   C is used to process the API information.  */
#ifdef __cplusplus

/* Yes, C++ compiler is present.  Use standard C.  */
extern   "C" {

#endif

#include "nx_crypto.h"

#include "common_data.h"
#include "r_ecc_api.h"

#define NX_CRYPTO_EC_192_PRIVATE_KEY_SIZE_BYTES     24
#define NX_CRYPTO_EC_256_PRIVATE_KEY_SIZE_BYTES     32

typedef struct NX_CRYPTO_EC_SCE_STRUCT
{
    /* Pointer to the ECC domain. */
    const r_crypto_data_handle_t *nx_crypto_ecc_domain_with_order;
    const r_crypto_data_handle_t *nx_crypto_ecc_domain_without_order;

    const r_crypto_data_handle_t *nx_crypto_ecc_g;

    UINT nx_crypto_ecc_key_size;

    const ecc_instance_t *nx_crypto_ecc_instance;
    const ecc_ctrl_t *nx_crypto_ecc_control;
} NX_CRYPTO_EC_SCE;

extern const ecc_instance_t g_sce_ecc192;
extern const ecc_instance_t g_sce_ecc256;
extern ecc_ctrl_t g_sce_ecc_ctrl192;
extern ecc_ctrl_t g_sce_ecc_ctrl256;
extern const NX_CRYPTO_EC_SCE _nx_crypto_ec_secp192r1_sce;
extern const NX_CRYPTO_EC_SCE _nx_crypto_ec_secp256r1_sce;
extern const NX_CRYPTO_EC_SCE _nx_crypto_ec_secp192r1_sce_be;
extern const NX_CRYPTO_EC_SCE _nx_crypto_ec_secp256r1_sce_be;

UINT _nx_crypto_method_ec_secp192r1_sce_operation(UINT op,
                                                  VOID *handle,
                                                  struct NX_CRYPTO_METHOD_STRUCT *method,
                                                  UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                                  UCHAR *input, ULONG input_length_in_byte,
                                                  UCHAR *iv_ptr,
                                                  UCHAR *output, ULONG output_length_in_byte,
                                                  VOID *crypto_metadata, ULONG crypto_metadata_size,
                                                  VOID *packet_ptr,
                                                  VOID (*nx_crypto_hw_process_callback)(VOID *, UINT));
UINT _nx_crypto_method_ec_secp256r1_sce_operation(UINT op,
                                                  VOID *handle,
                                                  struct NX_CRYPTO_METHOD_STRUCT *method,
                                                  UCHAR *key, NX_CRYPTO_KEY_SIZE key_size_in_bits,
                                                  UCHAR *input, ULONG input_length_in_byte,
                                                  UCHAR *iv_ptr,
                                                  UCHAR *output, ULONG output_length_in_byte,
                                                  VOID *crypto_metadata, ULONG crypto_metadata_size,
                                                  VOID *packet_ptr,
                                                  VOID (*nx_crypto_hw_process_callback)(VOID *, UINT));

#ifdef __cplusplus
}
#endif

#endif /* NX_CRYPTO_EC_SCE_H */

