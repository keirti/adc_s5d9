/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "r_crypto_api.h"
#include "r_aes_api.h"
#include "r_rsa_api.h"
#include "r_ecc_api.h"
#include "r_hash_api.h"
#include "r_trng_api.h"
#include "r_sce.h"
#include "nx_crypto_sce_config.h"
#include "nx_secure_tls.h"
#include "nx_secure_tls_api.h"
#include "nx_api.h"
#include "sf_el_nx_cfg.h"
#include "../src/framework/sf_el_nx/nx_renesas_synergy.h"
#include "nx_api.h"
#include "nx_api.h"

#include "r_ioport.h"
#include "r_ioport_api.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#include "r_fmi.h"
#include "r_fmi_api.h"
#include "r_elc.h"
#include "r_elc_api.h"
#ifdef __cplusplus
extern "C"
{
#endif

#include "ux_api.h"

/* USBX Host Stack initialization error callback function. User can override the function if needed. */
void ux_v2_err_callback(void *p_instance, void *p_data);

#if !defined(NULL)
/* User Callback for Host Event Notification (Only valid for USB Host). */
extern UINT NULL(ULONG event, UX_HOST_CLASS *host_class, VOID *instance);
#endif

#if !defined(NULL)
/* User Callback for Device Event Notification (Only valid for USB Device). */
extern UINT NULL(ULONG event);
#endif

#ifdef UX_HOST_CLASS_STORAGE_H
/* Utility function to get the pointer to a FileX Media Control Block for a USB Mass Storage device. */
UINT ux_system_host_storage_fx_media_get(UX_HOST_CLASS_STORAGE * instance, UX_HOST_CLASS_STORAGE_MEDIA ** p_storage_media, FX_MEDIA ** p_fx_media);
#endif
void ux_common_init0(void);
#include "ux_api.h"
#include "ux_dcd_synergy.h"
#include "sf_el_ux_dcd_fs_cfg.h"
void g_sf_el_ux_dcd_fs_0_err_callback(void *p_instance, void *p_data);
#include "ux_api.h"
#include "ux_dcd_synergy.h"

/* USBX Device Stack initialization error callback function. User can override the function if needed. */
void ux_device_err_callback(void *p_instance, void *p_data);
void ux_device_init0(void);
void ux_device_remove_compiler_padding(unsigned char *p_device_framework, UINT length);
/* Header section starts for g_ux_device_class_cdc_acm0 */
#include "ux_api.h"
#include "ux_device_class_cdc_acm.h"
/* USBX CDC-ACM Instance Activate User Callback Function */
extern VOID ux_cdc_device0_instance_activate(VOID *cdc_instance);
/* USBX CDC-ACM Instance Deactivate User Callback Function */
extern VOID ux_cdc_device0_instance_deactivate(VOID *cdc_instance);
/* Header section ends for g_ux_device_class_cdc_acm0 */
void ux_device_class_cdc_acm_init0(void);
void g_ux_device_class_cdc_acm0_ux_device_open_init(void);
extern const crypto_instance_t g_sce_0;
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_ECB   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_CBC   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_CTR   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_GCM   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_128_XTS   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_192_ECB   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_192_CBC   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_192_CTR   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_192_GCM   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_ECB   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_CBC   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_CTR   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_GCM   (1)
#define R_SCE_SERVICES_AES_PLAIN_TEXT_256_XTS   (1)
#define R_SCE_SERVICES_AES_WRAPPED_128_ECB      (1)
#define R_SCE_SERVICES_AES_WRAPPED_128_CBC      (1)
#define R_SCE_SERVICES_AES_WRAPPED_128_CTR      (1)
#define R_SCE_SERVICES_AES_WRAPPED_128_GCM      (1)
#define R_SCE_SERVICES_AES_WRAPPED_128_XTS      (1)
#define R_SCE_SERVICES_AES_WRAPPED_192_ECB      (1)
#define R_SCE_SERVICES_AES_WRAPPED_192_CBC      (1)
#define R_SCE_SERVICES_AES_WRAPPED_192_CTR      (1)
#define R_SCE_SERVICES_AES_WRAPPED_192_GCM      (1)
#define R_SCE_SERVICES_AES_WRAPPED_256_ECB      (1)
#define R_SCE_SERVICES_AES_WRAPPED_256_CBC      (1)
#define R_SCE_SERVICES_AES_WRAPPED_256_CTR      (1)
#define R_SCE_SERVICES_AES_WRAPPED_256_GCM      (1)
#define R_SCE_SERVICES_AES_WRAPPED_256_XTS      (1)
#define R_SCE_SERVICES_RSA_PLAIN_TEXT_1024      (1)
#define R_SCE_SERVICES_RSA_PLAIN_TEXT_2048      (1)
#define R_SCE_SERVICES_RSA_WRAPPED_1024         (1)
#define R_SCE_SERVICES_RSA_WRAPPED_2048         (1)
#define R_SCE_SERVICES_ECC_PLAIN_TEXT_192       (1)
#define R_SCE_SERVICES_ECC_PLAIN_TEXT_256       (1)
#define R_SCE_SERVICES_ECC_WRAPPED_192          (1)
#define R_SCE_SERVICES_ECC_WRAPPED_256          (1)
#define R_SCE_SERVICES_HASH_SHA1                (1)
#define R_SCE_SERVICES_HASH_SHA256              ((1) || (1))
#define R_SCE_SERVICES_HASH_MD5                 (1)
#define R_SCE_SERVICES_TRNG                     (1)
/** Software based crypto ciphers for use with nx_secure_tls_session_create. */
extern const NX_SECURE_TLS_CRYPTO nx_crypto_tls_ciphers;
#ifndef NULL
void NULL(nx_mac_address_t *p_mac_config);
#endif
#ifndef NULL
void NULL(NX_PACKET *packet_ptr, USHORT packet_type);
#endif
VOID nx_ether_driver_eth0(NX_IP_DRIVER *driver_req_ptr);
extern VOID (*g_sf_el_nx_ethernet)(NX_IP_DRIVER *driver_req_ptr);
void nx_common_init0(void);
extern ssp_err_t sce_initialize(void);
extern void nx_secure_tls_initialize(void);
extern void nx_secure_common_init(void);
extern NX_PACKET_POOL g_packet_pool0;
void g_packet_pool0_err_callback(void *p_instance, void *p_data);
void packet_pool_init0(void);
extern NX_IP g_ip0;
void g_ip0_err_callback(void *p_instance, void *p_data);
void ip_init0(void);
/** IOPORT Instance */
extern const ioport_instance_t g_ioport;
/** CGC Instance */
extern const cgc_instance_t g_cgc;
/** FMI on FMI Instance. */
extern const fmi_instance_t g_fmi;
/** ELC Instance */
extern const elc_instance_t g_elc;
void g_common_init(void);
#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* COMMON_DATA_H_ */
