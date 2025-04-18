/*****************************************************************************
**
**  Name:           app_ble_main.c
**
**  Description:    Bluetooth BLE Main application
**
**  Copyright (c) 2015-2016, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/

#include <stdlib.h>
#include "menu.h"
#include "app_ble.h"
#include "app_utils.h"
#include "app_disc.h"
#include "app_mgt.h"
#include "app_dm.h"
#include "app_ble_client.h"
#include "app_ble_server.h"
#if defined(APP_BLE_OTA_FW_DL_INCLUDED) && (APP_BLE_OTA_FW_DL_INCLUDED == TRUE)
#include "app_ble_client_otafwdl.h"
#endif

#if defined(APP_BLE2_BRCM_INCLUDED) && (APP_BLE2_BRCM_INCLUDED == TRUE)
#include "app_ble2_brcm.h"
#endif
/*
 * Defines
 */
#define APP_BLE_MAIN_DEFAULT_APPL_UUID    9000
#define APP_BLE_MAIN_INVALID_UUID         0
#define APP_BLE_ADV_VALUE_LEN             6  /*This is temporary value, Total Adv data including all fields should be <31bytes*/
#define APP_BLE_ADDR_LEN                  20

#define HCI_BLE_EVENT                       0x3e
#define HCI_VENDOR_SPECIFIC_EVT             0xFF
#define HCI_BLE_CONN_COMPLETE_EVT           0x01
#define HCI_BLE_ENHANCED_CONN_COMPLETE_EVT  0x0a
#define HCI_BLE_PHY_UPDATE_COMPLETE_EVT     0x0c


static UINT16 s_u16_le_handle =  0xFFFF;

/* BLE Menu items */
enum
{
    APP_BLE_MENU_ABORT_DISC = 2,
    APP_BLE_MENU_DISCOVERY,
    APP_BLE_MENU_CONFIG_BLE_BG_CONN,
    APP_BLE_MENU_CONFIG_BLE_SCAN_PARAM,
    APP_BLE_MENU_CONFIG_BLE_CONN_PARAM,
    APP_BLE_MENU_CONFIG_BLE_ADV_PARAM,
    APP_BLE_MENU_WAKE_ON_BLE,
#if defined (APP_BLE_DLE_TEST) /* for test purposes only */
    APP_BLE_MENU_READ_DEFAULT_DATA_LEN,
    APP_BLE_MENU_WRITE_DEFAULT_DATA_LEN,
    APP_BLE_MENU_READ_MAX_DATA_LEN,
#endif

    APP_BLECL_MENU_REGISTER,
    APP_BLECL_MENU_DEREGISTER,
    APP_BLECL_MENU_OPEN,
    APP_BLECL_MENU_CLOSE,
    APP_BLECL_MENU_REMOVE,
    APP_BLECL_MENU_READ,
    APP_BLECL_MENU_WRITE,
    APP_BLECL_MENU_SERVICE_DISC,
    APP_BLECL_MENU_REG_FOR_NOTI,
    APP_BLECL_MENU_DEREG_FOR_NOTI,
    APP_BLECL_MENU_DISPLAY_CLIENT,
    APP_BLECL_MENU_SEARCH_DEVICE_INFORMATION_SERVICE,
    APP_BLECL_MENU_READ_MFR_NAME,
    APP_BLECL_MENU_READ_MODEL_NUMBER,
    APP_BLECL_MENU_READ_SERIAL_NUMBER,
    APP_BLECL_MENU_READ_HARDWARE_REVISION,
    APP_BLECL_MENU_READ_FIRMWARE_REVISION,
    APP_BLECL_MENU_READ_SOFTWARE_REVISION,
    APP_BLECL_MENU_READ_SYSTEM_ID,
    APP_BLECL_MENU_SEARCH_BATTERY_SERVICE,
    APP_BLECL_MENU_READ_BATTERY_LEVEL,
#if defined(APP_BLE_OTA_FW_DL_INCLUDED) && (APP_BLE_OTA_FW_DL_INCLUDED == TRUE)
    APP_BLECL_MENU_FW_UPGRADE,
#endif
    APP_BLESE_MENU_REGISTER,
    APP_BLESE_MENU_DEREGISTER,
    APP_BLESE_MENU_OPEN,
    APP_BLESE_MENU_CLOSE,
    APP_BLESE_MENU_CREATE_SERVICE,
    APP_BLESE_MENU_ADD_CHAR,
    APP_BLESE_MENU_START_SERVICE,
    APP_BLESE_MENU_STOP_SERVICE,
    APP_BLESE_MENU_CONFIG_BLE_ADV_DATA,
    APP_BLESE_MENU_DISPLAY_SERVER,
    APP_BLESE_MENU_SEND_IND,
    APP_BLESE_MENU_START_BATTERY_SERVICE,
    APP_BLESE_MENU_START_DEVICE_INFO_SERVICE,
    APP_BLESE_MENU_START_HOGP_DEVICE_SERVICE,
    APP_BLESE_MENU_SEND_HOGP_KEY,

#if defined(APP_BLE2_BRCM_INCLUDED) && (APP_BLE2_BRCM_INCLUDED == TRUE)
    APP_BLE_MENU_LE2_CONTROL = 50,
    APP_BLE_MENU_LE2_STATUS,
#endif

    APP_BLE_MENU_GET_PHY,
    APP_BLE_MENU_SET_PHY,
    APP_BLE_MENU_SET_DEFAULT_PHY
};

/*
 * Global Variables
 */


/*
 * Local functions
 */


/*******************************************************************************
 **
 ** Function         app_display_ble_menu
 **
 ** Description      This is the BLE menu
 **
 ** Returns          void
 **
 *******************************************************************************/
void ble_menu_display (void)
{
    APP_INFO0("Bluetooth Application BLE menu:");
    APP_INFO1("\t%d => Abort Discovery", APP_BLE_MENU_ABORT_DISC);
    APP_INFO1("\t%d => Start BLE Discovery", APP_BLE_MENU_DISCOVERY);
    APP_INFO1("\t%d => Configure BLE Background Connection Type", APP_BLE_MENU_CONFIG_BLE_BG_CONN);
    APP_INFO1("\t%d => Configure BLE Scan Parameter",APP_BLE_MENU_CONFIG_BLE_SCAN_PARAM);
    APP_INFO1("\t%d => Configure BLE Connection Parameter",APP_BLE_MENU_CONFIG_BLE_CONN_PARAM);
    APP_INFO1("\t%d => Configure BLE Advertisement Parameters",APP_BLE_MENU_CONFIG_BLE_ADV_PARAM);
    APP_INFO1("\t%d => Configure for Wake on BLE",APP_BLE_MENU_WAKE_ON_BLE);
#if defined (APP_BLE_DLE_TEST) /* for test purposes only */
    APP_INFO1("\t%d => HCI LE Read Default Data Length",APP_BLE_MENU_READ_DEFAULT_DATA_LEN);
    APP_INFO1("\t%d => HCI LE Write Default Data Length",APP_BLE_MENU_WRITE_DEFAULT_DATA_LEN);
    APP_INFO1("\t%d => HCI LE Read Max Data Length",APP_BLE_MENU_READ_MAX_DATA_LEN);
#endif
    APP_INFO0("\t**** Bluetooth Low Energy Client menu ****");
    APP_INFO1("\t%d => Register client app", APP_BLECL_MENU_REGISTER);
    APP_INFO1("\t%d => Deregister Client app", APP_BLECL_MENU_DEREGISTER);
    APP_INFO1("\t%d => Connect to Server", APP_BLECL_MENU_OPEN);
    APP_INFO1("\t%d => Close Connection", APP_BLECL_MENU_CLOSE);
    APP_INFO1("\t%d => Remove BLE device", APP_BLECL_MENU_REMOVE);
    APP_INFO1("\t%d => Read information", APP_BLECL_MENU_READ);
    APP_INFO1("\t%d => Write information", APP_BLECL_MENU_WRITE);
    APP_INFO1("\t%d => Service Discovery", APP_BLECL_MENU_SERVICE_DISC);
    APP_INFO1("\t%d => Register Notification", APP_BLECL_MENU_REG_FOR_NOTI);
    APP_INFO1("\t%d => Deregister Notification", APP_BLECL_MENU_DEREG_FOR_NOTI);
    APP_INFO1("\t%d => Display Clients", APP_BLECL_MENU_DISPLAY_CLIENT);
    APP_INFO1("\t%d => Search Device Information Service", APP_BLECL_MENU_SEARCH_DEVICE_INFORMATION_SERVICE);
    APP_INFO1("\t%d => Read Manufacturer Name", APP_BLECL_MENU_READ_MFR_NAME);
    APP_INFO1("\t%d => Read Model Number", APP_BLECL_MENU_READ_MODEL_NUMBER);
    APP_INFO1("\t%d => Read Serial Number", APP_BLECL_MENU_READ_SERIAL_NUMBER);
    APP_INFO1("\t%d => Read Hardware Revision", APP_BLECL_MENU_READ_HARDWARE_REVISION);
    APP_INFO1("\t%d => Read Firmware Revision", APP_BLECL_MENU_READ_FIRMWARE_REVISION);
    APP_INFO1("\t%d => Read Software Revision", APP_BLECL_MENU_READ_SOFTWARE_REVISION);
    APP_INFO1("\t%d => Read System ID", APP_BLECL_MENU_READ_SYSTEM_ID);
    APP_INFO1("\t%d => Search Battery Service", APP_BLECL_MENU_SEARCH_BATTERY_SERVICE);
    APP_INFO1("\t%d => Read Battery Level", APP_BLECL_MENU_READ_BATTERY_LEVEL);
#if defined(APP_BLE_OTA_FW_DL_INCLUDED) && (APP_BLE_OTA_FW_DL_INCLUDED == TRUE)
    APP_INFO1("\t%d => Upgrade FW by LE",APP_BLECL_MENU_FW_UPGRADE);
#endif
    APP_INFO0("\t**** Bluetooth Low Energy Server menu ****");
    APP_INFO1("\t%d => Register server app", APP_BLESE_MENU_REGISTER);
    APP_INFO1("\t%d => Deregister server app", APP_BLESE_MENU_DEREGISTER);
    APP_INFO1("\t%d => Connect to client", APP_BLESE_MENU_OPEN);
    APP_INFO1("\t%d => Close connection", APP_BLESE_MENU_CLOSE);
    APP_INFO1("\t%d => Create service", APP_BLESE_MENU_CREATE_SERVICE);
    APP_INFO1("\t%d => Add character", APP_BLESE_MENU_ADD_CHAR);
    APP_INFO1("\t%d => Start service", APP_BLESE_MENU_START_SERVICE);
    APP_INFO1("\t%d => Stop service", APP_BLESE_MENU_STOP_SERVICE);
    APP_INFO1("\t%d => Configure BLE Advertisement data",APP_BLESE_MENU_CONFIG_BLE_ADV_DATA);
    APP_INFO1("\t%d => Display Servers", APP_BLESE_MENU_DISPLAY_SERVER);
    APP_INFO1("\t%d => Send indication", APP_BLESE_MENU_SEND_IND);
    APP_INFO1("\t%d => Start a battery service", APP_BLESE_MENU_START_BATTERY_SERVICE);
    APP_INFO1("\t%d => Start a device information service", APP_BLESE_MENU_START_DEVICE_INFO_SERVICE);
    APP_INFO1("\t%d => Start HOGP device services", APP_BLESE_MENU_START_HOGP_DEVICE_SERVICE);
    APP_INFO1("\t%d => Send a key to hogp host", APP_BLESE_MENU_SEND_HOGP_KEY);

#if defined(APP_BLE2_BRCM_INCLUDED) && (APP_BLE2_BRCM_INCLUDED == TRUE)
    APP_INFO0("\t**** Bluetooth Low Energy 2MBPS menu ****");
    APP_INFO1("\t%d => BLE2 Control", APP_BLE_MENU_LE2_CONTROL);
    APP_INFO1("\t%d => BLE2 Status", APP_BLE_MENU_LE2_STATUS);
#endif

    APP_INFO1("\t%d => Get LE Phy", APP_BLE_MENU_GET_PHY);
    APP_INFO1("\t%d => Set LE Phy", APP_BLE_MENU_SET_PHY);
    APP_INFO1("\t%d => Set LE Default Phy", APP_BLE_MENU_SET_DEFAULT_PHY);

    APP_INFO1("\t%d => Exit", MENU_QUIT);
}


/*******************************************************************************
 **
 ** Function         app_ble_menu
 **
 ** Description      Handle the BLE menu
 **
 ** Returns          void
 **
 *******************************************************************************/
void app_ble_menu(void)
{
    int choice, type,i;
    UINT16 ble_scan_interval, ble_scan_window;
    tBSA_DM_BLE_CONN_PARAM conn_param;
    tBSA_DM_BLE_ADV_CONFIG adv_conf;
    tBSA_DM_BLE_ADV_PARAM adv_param;
    UINT16 number_of_services;
    UINT8 app_ble_adv_value[APP_BLE_ADV_VALUE_LEN] = {0x2b, 0x1a, 0xaa, 0xbb, 0xcc, 0xdd}; /*First 2 byte is Company Identifier Eg: 0x1a2b refers to Bluetooth ORG, followed by 4bytes of data*/
    UINT16 all_phys, tx_phys, rx_phys, phy_options;


    do
    {
        ble_menu_display();

        choice = app_get_choice("Select action");

        switch(choice)
        {
        case APP_BLE_MENU_ABORT_DISC:
            app_disc_abort();
            break;

        case APP_BLE_MENU_DISCOVERY:
            app_disc_start_ble_regular(NULL);
            break;

        case APP_BLE_MENU_CONFIG_BLE_BG_CONN:
            type = app_get_choice("Select conn type(0 = None, 1 = Auto)");
            if(type == 0 || type == 1)
            {
                app_dm_set_ble_bg_conn_type(type);
            }
            else
            {
                APP_ERROR1("Unknown type:%d", type);
            }
            break;

        case APP_BLE_MENU_CONFIG_BLE_SCAN_PARAM:
            ble_scan_interval = app_get_choice("BLE scan interval(N x 625us)");
            ble_scan_window = app_get_choice("BLE scan window(N x 625us)");
            app_dm_set_ble_scan_param(ble_scan_interval, ble_scan_window);
            break;

        case APP_BLE_MENU_CONFIG_BLE_CONN_PARAM:
            conn_param.min_conn_int = app_get_choice("min_conn_int(N x 1.25 msec)");
            conn_param.max_conn_int = app_get_choice("max_conn_int(N x 1.25 msec)");
            conn_param.slave_latency = app_get_choice("slave_latency");
            conn_param.supervision_tout = app_get_choice("supervision_tout(N x 10 msec)");
            /*If TRUE,send udpate conn param request(slave) or update param to controller(master).*/
            /*Otherwise, set preferred value for master*/
            conn_param.is_immediate_updating = app_get_choice("Update immediately or set preferred value(1:Update immediately 0:set preferred):");
            APP_INFO0("Enter the BD address to configure Conn Param (AA.BB.CC.DD.EE.FF): ");
            if (scanf("%hhx.%hhx.%hhx.%hhx.%hhx.%hhx",
                &conn_param.bd_addr[0], &conn_param.bd_addr[1],
                &conn_param.bd_addr[2], &conn_param.bd_addr[3],
                &conn_param.bd_addr[4], &conn_param.bd_addr[5]) != 6)
            {
                APP_ERROR0("BD address not entered correctly");
                break;
            }
            app_dm_set_ble_conn_param(&conn_param);
            break;

        case APP_BLE_MENU_CONFIG_BLE_ADV_PARAM:
        {
            char addrstr[APP_BLE_ADDR_LEN] = {0};
            memset(&adv_param, 0, sizeof(tBSA_DM_BLE_ADV_PARAM));
            adv_param.adv_int_min = app_get_choice("min_adv_int(N x 0.625 msec)");
            adv_param.adv_int_max = app_get_choice("max_adv_int(N x 0.625 msec)");

            APP_INFO0("Enter the BD address to configure Conn Param (AA.BB.CC.DD.EE.FF): ");

            int len = app_get_string("Enter BD Addr for directed advertisement", addrstr, sizeof(addrstr));

            if (len && (sscanf(addrstr, "%hhx.%hhx.%hhx.%hhx.%hhx.%hhx",
                &adv_param.dir_bda.bd_addr[0], &adv_param.dir_bda.bd_addr[1],
                &adv_param.dir_bda.bd_addr[2], &adv_param.dir_bda.bd_addr[3],
                &adv_param.dir_bda.bd_addr[4], &adv_param.dir_bda.bd_addr[5]) != 6))
            {
                APP_ERROR0("BD address not entered correctly");
                break;
            }
            app_dm_set_ble_adv_param(&adv_param);
        }
            break;

        case APP_BLE_MENU_WAKE_ON_BLE:
            app_ble_wake_configure();
            break;

#if defined (APP_BLE_DLE_TEST) /* for test purposes only */
        case APP_BLE_MENU_READ_DEFAULT_DATA_LEN:
            app_ble_read_default_data_len();
            break;

        case APP_BLE_MENU_WRITE_DEFAULT_DATA_LEN:
            app_ble_write_default_data_len();
            break;

       case APP_BLE_MENU_READ_MAX_DATA_LEN:
            app_ble_read_max_data_len();
            break;
#endif
        case APP_BLECL_MENU_REGISTER:
            app_ble_client_register(APP_BLE_MAIN_INVALID_UUID);
            break;

        case APP_BLECL_MENU_OPEN:
            app_ble_client_open();
            break;

        case APP_BLECL_MENU_SERVICE_DISC:
            app_ble_client_service_search();
            break;

        case APP_BLECL_MENU_READ:
            app_ble_client_read();
            break;

        case APP_BLECL_MENU_WRITE:
            app_ble_client_write();
            break;

        case APP_BLECL_MENU_REMOVE:
            app_ble_client_unpair();
            break;

        case APP_BLECL_MENU_REG_FOR_NOTI:
            app_ble_client_register_notification();
            break;

        case APP_BLECL_MENU_CLOSE:
            app_ble_client_close();
            break;

        case APP_BLECL_MENU_DEREGISTER:
            app_ble_client_deregister();
            break;

        case APP_BLECL_MENU_DEREG_FOR_NOTI:
            app_ble_client_deregister_notification();
            break;

        case APP_BLECL_MENU_DISPLAY_CLIENT:
            app_ble_client_display(1);
            break;

        case APP_BLECL_MENU_SEARCH_DEVICE_INFORMATION_SERVICE:
            app_ble_client_service_search_execute(BSA_BLE_UUID_SERVCLASS_DEVICE_INFORMATION);
            break;

        case APP_BLECL_MENU_READ_MFR_NAME:
            app_ble_client_read_mfr_name();
            break;

        case APP_BLECL_MENU_READ_MODEL_NUMBER:
            app_ble_client_read_model_number();
            break;

        case APP_BLECL_MENU_READ_SERIAL_NUMBER:
            app_ble_client_read_serial_number();
            break;

        case APP_BLECL_MENU_READ_HARDWARE_REVISION:
            app_ble_client_read_hardware_revision();
            break;

        case APP_BLECL_MENU_READ_FIRMWARE_REVISION:
            app_ble_client_read_firmware_revision();
            break;

        case APP_BLECL_MENU_READ_SOFTWARE_REVISION:
            app_ble_client_read_software_revision();
            break;

        case APP_BLECL_MENU_READ_SYSTEM_ID:
            app_ble_client_read_system_id();
            break;

        case APP_BLECL_MENU_SEARCH_BATTERY_SERVICE:
            app_ble_client_service_search_execute(BSA_BLE_UUID_SERVCLASS_BATTERY_SERVICE);
            break;

        case APP_BLECL_MENU_READ_BATTERY_LEVEL:
            app_ble_client_read_battery_level();
            break;
#if (defined(APP_BLE_OTA_FW_DL_INCLUDED) && (APP_BLE_OTA_FW_DL_INCLUDED == TRUE))
        case APP_BLECL_MENU_FW_UPGRADE:
             if(app_ble_client_otafwdl() != 0)
             {
                APP_DEBUG0("fw upgrade failed");
             }
             break;
#endif

        case APP_BLESE_MENU_REGISTER:
            app_ble_server_register(APP_BLE_MAIN_INVALID_UUID, NULL);
            break;

        case APP_BLESE_MENU_DEREGISTER:
            app_ble_server_deregister();
            break;

        case APP_BLESE_MENU_OPEN:
            app_ble_server_open();
            break;

        case APP_BLESE_MENU_CLOSE:
            app_ble_server_close();
            break;

        case APP_BLESE_MENU_CREATE_SERVICE:
            app_ble_server_create_service();
            break;

        case APP_BLESE_MENU_ADD_CHAR:
            app_ble_server_add_char();
            break;

        case APP_BLESE_MENU_START_SERVICE:
            app_ble_server_start_service();
            break;

        case APP_BLESE_MENU_STOP_SERVICE:
            app_ble_server_stop_service();
            break;

        case APP_BLESE_MENU_CONFIG_BLE_ADV_DATA:
#if 0
            /* This is just sample code to show how BLE Adv data can be sent from application */
            /*Adv.Data should be < 31bytes including Manufacturer data,Device Name, Appearance data, Services Info,etc.. */
            /* We are not receving all fields from user to reduce the complexity */
            memset(&adv_conf, 0, sizeof(tBSA_DM_BLE_ADV_CONFIG));
           /* start advertising */
           adv_conf.len = APP_BLE_ADV_VALUE_LEN;
           adv_conf.flag = BSA_DM_BLE_ADV_FLAG_MASK;
           memcpy(adv_conf.p_val, app_ble_adv_value, APP_BLE_ADV_VALUE_LEN);
           /* All the masks/fields that are set will be advertised*/
           adv_conf.adv_data_mask = BSA_DM_BLE_AD_BIT_FLAGS|BSA_DM_BLE_AD_BIT_SERVICE|BSA_DM_BLE_AD_BIT_APPEARANCE|BSA_DM_BLE_AD_BIT_MANU;
           adv_conf.appearance_data = app_get_choice("Enter appearance value Eg:0x1122");
           number_of_services = app_get_choice("Enter number of services between <1-6> Eg:2");
           adv_conf.num_service = number_of_services;
           for(i=0; i< adv_conf.num_service; i++)
           {
                adv_conf.uuid_val[i]= app_get_choice("Enter service UUID eg:0xA108");
           }
           adv_conf.is_scan_rsp = app_get_choice("Is this scan response? (0:FALSE, 1:TRUE)");
           app_dm_set_ble_adv_data(&adv_conf);
#else      //service data UUID128 and tx power  config test
           memset(&adv_conf, 0, sizeof(tBSA_DM_BLE_ADV_CONFIG));
           adv_conf.adv_data_mask = BSA_DM_BLE_AD_BIT_FLAGS|BSA_DM_BLE_AD_BIT_SERVICE_DATA|BSA_DM_BLE_AD_BIT_TX_PWR;
           adv_conf.tx_power = 9;
           adv_conf.is_scan_rsp = FALSE;
           UINT8 serviceuuiddef[2] = {0xEB,0x23};
           memcpy(adv_conf.service_data_uuid[0].uu.uuid128, serviceuuiddef, sizeof(serviceuuiddef));
           adv_conf.service_data_uuid[0].len = LEN_UUID_16;

           adv_conf.service_data_len[0] = 3;
           UINT8 *p;
           p = &adv_conf.service_data_val[0][0];
           UINT8_TO_STREAM(p, 0x07);
           UINT8_TO_STREAM(p, 0x01);
           UINT8_TO_STREAM(p, 0x02);

           UINT8 serviceuuiddef2[2] = {0xEB,0x24};
           memcpy(adv_conf.service_data_uuid[1].uu.uuid128, serviceuuiddef2, sizeof(serviceuuiddef2));
           adv_conf.service_data_uuid[1].len = LEN_UUID_16;

           adv_conf.service_data_len[1] = 3;
           p = &adv_conf.service_data_val[1][0];
           UINT8_TO_STREAM(p, 0x04);
           UINT8_TO_STREAM(p, 0x05);
           UINT8_TO_STREAM(p, 0x06);

           app_dm_set_ble_adv_data(&adv_conf);
#endif
           break;

        case APP_BLESE_MENU_DISPLAY_SERVER:
            app_ble_server_display();
            break;

        case APP_BLESE_MENU_SEND_IND:
            app_ble_server_send_indication();
            break;

        case APP_BLESE_MENU_START_BATTERY_SERVICE:
            app_ble_server_start_battery_service();
            break;

        case APP_BLESE_MENU_START_DEVICE_INFO_SERVICE:
            app_ble_server_start_device_info_service();
            break;

        case APP_BLESE_MENU_START_HOGP_DEVICE_SERVICE:
            app_ble_server_start_hogp_device_service();
            break;

        case APP_BLESE_MENU_SEND_HOGP_KEY:
            app_ble_server_send_key_by_hogp();
            break;

#if defined(APP_BLE2_BRCM_INCLUDED) && (APP_BLE2_BRCM_INCLUDED == TRUE)
        case APP_BLE_MENU_LE2_CONTROL:
            choice = app_get_choice("BLE2 Enable/Disable (0 = Disable, 1 = Enable)");
            if(choice == 0)
            {
                APP_INFO0("Disabling BLE2");
                /* Disable BLE2 */
                app_ble2_brcm_config_flags_write(0, 0, 0);
            }
            else
            {
                APP_INFO0("Enabling BLE2");
                /* Enable LE2 (Advertising, Scanning and Connection) */
                app_ble2_brcm_config_flags_write(LE2_CFG_FLG_ADV_ENABLE |
                        LE2_CFG_FLG_SCAN_ENABLE | LE2_CFG_FLG_CON_ENABLE,
                        LE2_CFG_FLG_ACCESS_CODE_DEFAULT,
                        LE2_CFG_FLG_ACCESS_CODE_DEFAULT);
                /* Configure Extended Packet Length mode 3 (251 bytes) */
                app_ble2_brcm_ext_packet_len_write(
                        LE2_EXT_PKT_LEN_MODE_3, LE2_EXT_PKT_LEN_MODE_3_MAX_LEN);
            }
            break;

        case APP_BLE_MENU_LE2_STATUS:
            {
                uint8_t flags, mode, packet_len;
                uint32_t lsb_access_code, msb_access_code;

                if (app_ble2_brcm_config_flags_read(&flags, &lsb_access_code,
                        &msb_access_code) < 0)
                {
                    break;
                }
                if (flags)
                {
                    APP_INFO1("BLE2 Enabled. Advertising:%d Scanning:%d Connection:%d",
                            flags & LE2_CFG_FLG_ADV_ENABLE?1:0,
                            flags & LE2_CFG_FLG_SCAN_ENABLE?1:0,
                            flags & LE2_CFG_FLG_CON_ENABLE?1:0);
                    APP_INFO1("BLE2 Access code LSB:0x%08X MSB:0x%08X",
                            lsb_access_code, msb_access_code);

                    if (app_ble2_brcm_ext_packet_len_read(&mode, &packet_len) < 0)
                    {
                        break;
                    }
                    APP_INFO1("BLE2 Mode:%d PacketLen:%d", mode, packet_len);
                }
                else
                {
                    APP_INFO0("BLE2 Disabled");
                }
            }
            break;
#endif

        case APP_BLE_MENU_GET_PHY:
            //choice = app_get_choice("Input ble connection handle: such as 64");
            app_ble_read_phy(s_u16_le_handle);
            break;
        case APP_BLE_MENU_SET_PHY:
            //handle = app_get_choice("Input ble connection handle: such as 64");
            tx_phys = app_get_choice("Input tx_phy (1:1M, 2:2M, 4:codec phy):");
            rx_phys = app_get_choice("Input rx_phy (1:1M, 2:2M, 4:codec phy):");
            phy_options = app_get_choice("Input phy_option (1:no prefer, 2:S=2, 4:S=8):");
            app_ble_set_phy(s_u16_le_handle, tx_phys, rx_phys, phy_options);
            break;
        case APP_BLE_MENU_SET_DEFAULT_PHY:
            all_phys = app_get_choice("Input all_phy (1:no prefer tx, 2:no prefer rx):");
            tx_phys = app_get_choice("Input tx_phy (1:1M, 2:2M, 4:codec phy):");
            rx_phys = app_get_choice("Input rx_phy (1:1M, 2:2M, 4:codec phy):");
            app_ble_set_default_phy(all_phys, tx_phys, rx_phys);
            break;

        case MENU_QUIT:
            APP_INFO0("Quit");
            break;

        default:
            APP_ERROR1("Unknown choice:%d", choice);
            break;
        }
    } while (choice != MENU_QUIT); /* While user don't exit application */
}


/*******************************************************************************
 **
 ** Function         app_ble_mgt_callback
 **
 ** Description      This callback function is called in case of server
 **                  disconnection (e.g. server crashes)
 **
 ** Parameters
 **
 ** Returns          BOOLEAN
 **
 *******************************************************************************/
BOOLEAN app_ble_mgt_callback(tBSA_MGT_EVT event, tBSA_MGT_MSG *p_data)
{
    switch(event)
    {
    case BSA_MGT_STATUS_EVT:
        APP_DEBUG0("BSA_MGT_STATUS_EVT");
        if (p_data->status.enable)
        {
            APP_DEBUG0("Bluetooth restarted => re-initialize the application");
            app_ble_start();
        }
        break;

    case BSA_MGT_DISCONNECT_EVT:
        APP_DEBUG1("BSA_MGT_DISCONNECT_EVT reason:%d", p_data->disconnect.reason);
        /* Connection with the Server lost => Just exit the application */
        exit(-1);
        break;

    default:
        break;
    }
    return FALSE;
}

/*******************************************************************************
 **
 ** Function         app_ble_vse_callback
 **
 ** Description      This callback function is used for
 **                  Vendor Specific Events
 **
 ** Parameters       event: TM event received
 **                  p_data: TM event data
 **
 ** Returns          void
 **
 ********************************************************************************/
void app_ble_vse_callback(tBSA_TM_EVT event, tBSA_TM_MSG *p_data)
{
    tBSA_TM_VSE_MSG *p_vse;
    UINT8 length;
    int   index;
    UINT8 status;
    UINT8 addr[6];
    UINT16 i;
    UINT16 op_code;
    UINT16 handle;

    switch(event)
    {
    case BSA_TM_VSE_EVT:
        printf("\napp_ble_vse_callback BSA_TM_VSC_EVT \n");
        p_vse = (tBSA_TM_VSE_MSG *)p_data;
        length = p_vse->length;
        if (length > 0)
        {
            printf("04\n");
            printf("%02x %02x\n", p_vse->event, length);
            printf("%02x ", p_vse->sub_event);
            for(index=0;index<length;index++)
            {
                printf("%02x ",p_vse->data[index]);
            }
            printf("\n");
        }
        //You can get the ble connection handle as fowllows:
        if (p_vse->event == HCI_BLE_EVENT) {
            switch (p_vse->sub_event) {
            case HCI_BLE_CONN_COMPLETE_EVT:
            case HCI_BLE_ENHANCED_CONN_COMPLETE_EVT:
                status = p_vse->data[0];
                if (status != 0) {
                    printf("\n app_ble_vse_callback le connection failure status = %02x \n", status);
                    break;
                }
                s_u16_le_handle = p_vse->data[1] + (p_vse->data[2]<<8);
                for (i=0; i<sizeof(addr); i++) {
                    addr[i] = p_vse->data[10-i];
                }
                printf("\n app_ble_vse_callback le handle = 0x%04x, addr=%02x:%02x:%02x:%02x:%02x:%02x \n",
                    s_u16_le_handle, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
                break;
            case HCI_BLE_PHY_UPDATE_COMPLETE_EVT:
                status = p_vse->data[0];
                handle = p_vse->data[1] + (p_vse->data[2] << 8);
                printf("\n phy update handle=0x%04x, tx_phy=0x%02x(1:1M,2:2M,3:codec), rx_phy=0x%02x \n", handle, p_vse->data[3], p_vse->data[4]);
                break;
            }
        }else if (p_vse->event == HCI_COMMAND_COMPLETE_EVT) {
            if (length == 7) {
                op_code = p_vse->data[0] + (p_vse->data[1] << 8);
                printf("\n command complete evet opcode = %04x ", op_code);
                if (op_code == HCI_BLE_READ_PHY) {
                    status = p_vse->data[2];
                    handle = p_vse->data[3] + (p_vse->data[4] << 8);
                    printf("\n read phy handle=0x%04x, tx_phy=0x%02x(1:1M,2:2M,3:codec), rx_phy=0x%02x \n", handle, p_vse->data[5], p_vse->data[6]);
                }
            }
        }
        break;

    default:
        printf("Unsupported TM event\n");
        break;
    }

}

/*******************************************************************************
 **
 ** Function         app_ble_vse_register
 **
 ** Description      This function is used to register for Vendor Specific Events
 **
 ** Parameters
 **
 ** Returns          int
 **
 *******************************************************************************/
int app_ble_vse_register(void)
{
    tBSA_TM_VSE_REGISTER  vse_register_param;
    tBSA_STATUS status;

    BSA_TmVseRegisterInit(&vse_register_param);
    vse_register_param.p_callback = app_ble_vse_callback;
    vse_register_param.sub_event = BTA_TM_VSE_SUB_EVENT_ALL;
    status = BSA_TmVseRegister(&vse_register_param);
    if (status != BSA_SUCCESS)
    {
        fprintf(stderr, "BSA_TmVseRegister: Unable to register for VSE\n");
        return(status);
    }

    return 0;
}

/*******************************************************************************
 **
 ** Function         vse_deregister
 **
 ** Description      This function is used to deregister for Vendor Specific Events
 **
 ** Parameters
 **
 ** Returns          int
 **
 *******************************************************************************/
int app_ble_vse_deregister(void)
{
    tBSA_TM_VSE_DEREGISTER  vse_deregister_param;
    tBSA_STATUS status;

    BSA_TmVseDeregisterInit(&vse_deregister_param);
    vse_deregister_param.sub_event = BTA_TM_VSE_SUB_EVENT_ALL;
    vse_deregister_param.deregister_callback = FALSE;
    status = BSA_TmVseDeregister(&vse_deregister_param);
    if (status != BSA_SUCCESS)
    {
        fprintf(stderr, "BSA_TmVseDeregister: Unable to deregister for VSE\n");
        return(status);
    }

    return 0;
}

#ifdef DEMO_MODE
/*******************************************************************************
 **
 ** Function        main
 **
 ** Description     This is the main function
 **
 ** Parameters      Program's arguments
 **
 ** Returns         status
 **
 *******************************************************************************/
int main(int argc, char **argv)
{
    int status;

    /* Initialize BLE application */
    status = app_ble_init();
    if (status < 0)
    {
        APP_ERROR0("Couldn't Initialize BLE app, exiting");
        exit(-1);
    }

    /* Open connection to BSA Server */
    app_mgt_init();
    if (app_mgt_open(NULL, app_ble_mgt_callback) < 0)
    {
        APP_ERROR0("Unable to connect to server");
        return -1;
    }

    /* Start BLE application */
    status = app_ble_start();
    if (status < 0)
    {
        APP_ERROR0("Couldn't Start BLE app, exiting");
        return -1;
    }

    /* register one application */
    app_ble_client_register(APP_BLE_MAIN_DEFAULT_APPL_UUID);

    app_ble_vse_register();

    /* The main BLE loop */
    app_ble_menu();

    app_ble_vse_deregister();

    /* Exit BLE mode */
    app_ble_exit();

    /* Close BSA Connection before exiting (to release resources) */
    app_mgt_close();

    exit(0);
}
#else 
int ble_menu_handle(void)
{
    int status;
    int choice, type,i;
    UINT16 ble_scan_interval, ble_scan_window;
    tBSA_DM_BLE_CONN_PARAM conn_param;
    tBSA_DM_BLE_ADV_CONFIG adv_conf;
    tBSA_DM_BLE_ADV_PARAM adv_param;
    UINT16 number_of_services;
    UINT8 app_ble_adv_value[APP_BLE_ADV_VALUE_LEN] = {0x2b, 0x1a, 0xaa, 0xbb, 0xcc, 0xdd}; /*First 2 byte is Company Identifier Eg: 0x1a2b refers to Bluetooth ORG, followed by 4bytes of data*/
    UINT16 all_phys, tx_phys, rx_phys, phy_options;

    /* Initialize BLE application */
    status = app_ble_init();
    if (status < 0)
    {
        APP_ERROR0("Couldn't Initialize BLE, exiting");
        return 0;
    }

    app_mgt_custom_cback_update(app_ble_mgt_callback);

    /* Start BLE application */
    status = app_ble_start();
    if (status < 0)
    {
        APP_ERROR0("Couldn't Start BLE, exiting");
        return 0;
    }

    /* register one application */
    app_ble_client_register(APP_BLE_MAIN_DEFAULT_APPL_UUID);

    app_ble_vse_register();

    do
    {

        choice = app_get_choice("Select action");

        switch(choice)
        {
        case MENU_QUIT:
            app_ble_vse_deregister();

            /* Exit BLE mode */
            app_ble_exit();

            return -1;
        case MAIN_MENU:
            app_ble_vse_deregister();

            /* Exit BLE mode */
            app_ble_exit();

            APP_INFO0("Back to main menu...");
            return 0;
        case HELP_MENU:
            ble_menu_display();
            break;
        case APP_BLE_MENU_ABORT_DISC:
            app_disc_abort();
            break;

        case APP_BLE_MENU_DISCOVERY:
            app_disc_start_ble_regular(NULL);
            break;

        case APP_BLE_MENU_CONFIG_BLE_BG_CONN:
            type = app_get_choice("Select conn type(0 = None, 1 = Auto)");
            if(type == 0 || type == 1)
            {
                app_dm_set_ble_bg_conn_type(type);
            }
            else
            {
                APP_ERROR1("Unknown type:%d", type);
            }
            break;

        case APP_BLE_MENU_CONFIG_BLE_SCAN_PARAM:
            ble_scan_interval = app_get_choice("BLE scan interval(N x 625us)");
            ble_scan_window = app_get_choice("BLE scan window(N x 625us)");
            app_dm_set_ble_scan_param(ble_scan_interval, ble_scan_window);
            break;

        case APP_BLE_MENU_CONFIG_BLE_CONN_PARAM:
            conn_param.min_conn_int = app_get_choice("min_conn_int(N x 1.25 msec)");
            conn_param.max_conn_int = app_get_choice("max_conn_int(N x 1.25 msec)");
            conn_param.slave_latency = app_get_choice("slave_latency");
            conn_param.supervision_tout = app_get_choice("supervision_tout(N x 10 msec)");
            /*If TRUE,send udpate conn param request(slave) or update param to controller(master).*/
            /*Otherwise, set preferred value for master*/
            conn_param.is_immediate_updating = app_get_choice("Update immediately or set preferred value(1:Update immediately 0:set preferred):");
            APP_INFO0("Enter the BD address to configure Conn Param (AA.BB.CC.DD.EE.FF): ");
            if (scanf("%hhx.%hhx.%hhx.%hhx.%hhx.%hhx",
                &conn_param.bd_addr[0], &conn_param.bd_addr[1],
                &conn_param.bd_addr[2], &conn_param.bd_addr[3],
                &conn_param.bd_addr[4], &conn_param.bd_addr[5]) != 6)
            {
                APP_ERROR0("BD address not entered correctly");
                break;
            }
            app_dm_set_ble_conn_param(&conn_param);
            break;

        case APP_BLE_MENU_CONFIG_BLE_ADV_PARAM:
        {
            char addrstr[APP_BLE_ADDR_LEN] = {0};
            memset(&adv_param, 0, sizeof(tBSA_DM_BLE_ADV_PARAM));
            adv_param.adv_int_min = app_get_choice("min_adv_int(N x 0.625 msec)");
            adv_param.adv_int_max = app_get_choice("max_adv_int(N x 0.625 msec)");

            APP_INFO0("Enter the BD address to configure Conn Param (AA.BB.CC.DD.EE.FF): ");

            int len = app_get_string("Enter BD Addr for directed advertisement", addrstr, sizeof(addrstr));

            if (len && (sscanf(addrstr, "%hhx.%hhx.%hhx.%hhx.%hhx.%hhx",
                &adv_param.dir_bda.bd_addr[0], &adv_param.dir_bda.bd_addr[1],
                &adv_param.dir_bda.bd_addr[2], &adv_param.dir_bda.bd_addr[3],
                &adv_param.dir_bda.bd_addr[4], &adv_param.dir_bda.bd_addr[5]) != 6))
            {
                APP_ERROR0("BD address not entered correctly");
                break;
            }
            app_dm_set_ble_adv_param(&adv_param);
        }
            break;

        case APP_BLE_MENU_WAKE_ON_BLE:
            app_ble_wake_configure();
            break;

#if defined (APP_BLE_DLE_TEST) /* for test purposes only */
        case APP_BLE_MENU_READ_DEFAULT_DATA_LEN:
            app_ble_read_default_data_len();
            break;

        case APP_BLE_MENU_WRITE_DEFAULT_DATA_LEN:
            app_ble_write_default_data_len();
            break;

       case APP_BLE_MENU_READ_MAX_DATA_LEN:
            app_ble_read_max_data_len();
            break;
#endif
        case APP_BLECL_MENU_REGISTER:
            app_ble_client_register(APP_BLE_MAIN_INVALID_UUID);
            break;

        case APP_BLECL_MENU_OPEN:
            app_ble_client_open();
            break;

        case APP_BLECL_MENU_SERVICE_DISC:
            app_ble_client_service_search();
            break;

        case APP_BLECL_MENU_READ:
            app_ble_client_read();
            break;

        case APP_BLECL_MENU_WRITE:
            app_ble_client_write();
            break;

        case APP_BLECL_MENU_REMOVE:
            app_ble_client_unpair();
            break;

        case APP_BLECL_MENU_REG_FOR_NOTI:
            app_ble_client_register_notification();
            break;

        case APP_BLECL_MENU_CLOSE:
            app_ble_client_close();
            break;

        case APP_BLECL_MENU_DEREGISTER:
            app_ble_client_deregister();
            break;

        case APP_BLECL_MENU_DEREG_FOR_NOTI:
            app_ble_client_deregister_notification();
            break;

        case APP_BLECL_MENU_DISPLAY_CLIENT:
            app_ble_client_display(1);
            break;

        case APP_BLECL_MENU_SEARCH_DEVICE_INFORMATION_SERVICE:
            app_ble_client_service_search_execute(BSA_BLE_UUID_SERVCLASS_DEVICE_INFORMATION);
            break;

        case APP_BLECL_MENU_READ_MFR_NAME:
            app_ble_client_read_mfr_name();
            break;

        case APP_BLECL_MENU_READ_MODEL_NUMBER:
            app_ble_client_read_model_number();
            break;

        case APP_BLECL_MENU_READ_SERIAL_NUMBER:
            app_ble_client_read_serial_number();
            break;

        case APP_BLECL_MENU_READ_HARDWARE_REVISION:
            app_ble_client_read_hardware_revision();
            break;

        case APP_BLECL_MENU_READ_FIRMWARE_REVISION:
            app_ble_client_read_firmware_revision();
            break;

        case APP_BLECL_MENU_READ_SOFTWARE_REVISION:
            app_ble_client_read_software_revision();
            break;

        case APP_BLECL_MENU_READ_SYSTEM_ID:
            app_ble_client_read_system_id();
            break;

        case APP_BLECL_MENU_SEARCH_BATTERY_SERVICE:
            app_ble_client_service_search_execute(BSA_BLE_UUID_SERVCLASS_BATTERY_SERVICE);
            break;

        case APP_BLECL_MENU_READ_BATTERY_LEVEL:
            app_ble_client_read_battery_level();
            break;
#if (defined(APP_BLE_OTA_FW_DL_INCLUDED) && (APP_BLE_OTA_FW_DL_INCLUDED == TRUE))
        case APP_BLECL_MENU_FW_UPGRADE:
             if(app_ble_client_otafwdl() != 0)
             {
                APP_DEBUG0("fw upgrade failed");
             }
             break;
#endif

        case APP_BLESE_MENU_REGISTER:
            app_ble_server_register(APP_BLE_MAIN_INVALID_UUID, NULL);
            break;

        case APP_BLESE_MENU_DEREGISTER:
            app_ble_server_deregister();
            break;

        case APP_BLESE_MENU_OPEN:
            app_ble_server_open();
            break;

        case APP_BLESE_MENU_CLOSE:
            app_ble_server_close();
            break;

        case APP_BLESE_MENU_CREATE_SERVICE:
            app_ble_server_create_service();
            break;

        case APP_BLESE_MENU_ADD_CHAR:
            app_ble_server_add_char();
            break;

        case APP_BLESE_MENU_START_SERVICE:
            app_ble_server_start_service();
            break;

        case APP_BLESE_MENU_STOP_SERVICE:
            app_ble_server_stop_service();
            break;

        case APP_BLESE_MENU_CONFIG_BLE_ADV_DATA:
#if 0
            /* This is just sample code to show how BLE Adv data can be sent from application */
            /*Adv.Data should be < 31bytes including Manufacturer data,Device Name, Appearance data, Services Info,etc.. */
            /* We are not receving all fields from user to reduce the complexity */
            memset(&adv_conf, 0, sizeof(tBSA_DM_BLE_ADV_CONFIG));
           /* start advertising */
           adv_conf.len = APP_BLE_ADV_VALUE_LEN;
           adv_conf.flag = BSA_DM_BLE_ADV_FLAG_MASK;
           memcpy(adv_conf.p_val, app_ble_adv_value, APP_BLE_ADV_VALUE_LEN);
           /* All the masks/fields that are set will be advertised*/
           adv_conf.adv_data_mask = BSA_DM_BLE_AD_BIT_FLAGS|BSA_DM_BLE_AD_BIT_SERVICE|BSA_DM_BLE_AD_BIT_APPEARANCE|BSA_DM_BLE_AD_BIT_MANU;
           adv_conf.appearance_data = app_get_choice("Enter appearance value Eg:0x1122");
           number_of_services = app_get_choice("Enter number of services between <1-6> Eg:2");
           adv_conf.num_service = number_of_services;
           for(i=0; i< adv_conf.num_service; i++)
           {
                adv_conf.uuid_val[i]= app_get_choice("Enter service UUID eg:0xA108");
           }
           adv_conf.is_scan_rsp = app_get_choice("Is this scan response? (0:FALSE, 1:TRUE)");
           app_dm_set_ble_adv_data(&adv_conf);
#else      //service data UUID128 and tx power  config test
           memset(&adv_conf, 0, sizeof(tBSA_DM_BLE_ADV_CONFIG));
           adv_conf.adv_data_mask = BSA_DM_BLE_AD_BIT_FLAGS|BSA_DM_BLE_AD_BIT_SERVICE_DATA|BSA_DM_BLE_AD_BIT_TX_PWR;
           adv_conf.tx_power = 9;
           adv_conf.is_scan_rsp = FALSE;
           UINT8 serviceuuiddef[2] = {0xEB,0x23};
           memcpy(adv_conf.service_data_uuid[0].uu.uuid128, serviceuuiddef, sizeof(serviceuuiddef));
           adv_conf.service_data_uuid[0].len = LEN_UUID_16;

           adv_conf.service_data_len[0] = 3;
           UINT8 *p;
           p = &adv_conf.service_data_val[0][0];
           UINT8_TO_STREAM(p, 0x07);
           UINT8_TO_STREAM(p, 0x01);
           UINT8_TO_STREAM(p, 0x02);

           UINT8 serviceuuiddef2[2] = {0xEB,0x24};
           memcpy(adv_conf.service_data_uuid[1].uu.uuid128, serviceuuiddef2, sizeof(serviceuuiddef2));
           adv_conf.service_data_uuid[1].len = LEN_UUID_16;

           adv_conf.service_data_len[1] = 3;
           p = &adv_conf.service_data_val[1][0];
           UINT8_TO_STREAM(p, 0x04);
           UINT8_TO_STREAM(p, 0x05);
           UINT8_TO_STREAM(p, 0x06);

           app_dm_set_ble_adv_data(&adv_conf);
#endif
           break;

        case APP_BLESE_MENU_DISPLAY_SERVER:
            app_ble_server_display();
            break;

        case APP_BLESE_MENU_SEND_IND:
            app_ble_server_send_indication();
            break;

        case APP_BLESE_MENU_START_BATTERY_SERVICE:
            app_ble_server_start_battery_service();
            break;

        case APP_BLESE_MENU_START_DEVICE_INFO_SERVICE:
            app_ble_server_start_device_info_service();
            break;

        case APP_BLESE_MENU_START_HOGP_DEVICE_SERVICE:
            app_ble_server_start_hogp_device_service();
            break;

        case APP_BLESE_MENU_SEND_HOGP_KEY:
            app_ble_server_send_key_by_hogp();
            break;

#if defined(APP_BLE2_BRCM_INCLUDED) && (APP_BLE2_BRCM_INCLUDED == TRUE)
        case APP_BLE_MENU_LE2_CONTROL:
            choice = app_get_choice("BLE2 Enable/Disable (0 = Disable, 1 = Enable)");
            if(choice == 0)
            {
                APP_INFO0("Disabling BLE2");
                /* Disable BLE2 */
                app_ble2_brcm_config_flags_write(0, 0, 0);
            }
            else
            {
                APP_INFO0("Enabling BLE2");
                /* Enable LE2 (Advertising, Scanning and Connection) */
                app_ble2_brcm_config_flags_write(LE2_CFG_FLG_ADV_ENABLE |
                        LE2_CFG_FLG_SCAN_ENABLE | LE2_CFG_FLG_CON_ENABLE,
                        LE2_CFG_FLG_ACCESS_CODE_DEFAULT,
                        LE2_CFG_FLG_ACCESS_CODE_DEFAULT);
                /* Configure Extended Packet Length mode 3 (251 bytes) */
                app_ble2_brcm_ext_packet_len_write(
                        LE2_EXT_PKT_LEN_MODE_3, LE2_EXT_PKT_LEN_MODE_3_MAX_LEN);
            }
            break;

        case APP_BLE_MENU_LE2_STATUS:
            {
                uint8_t flags, mode, packet_len;
                uint32_t lsb_access_code, msb_access_code;

                if (app_ble2_brcm_config_flags_read(&flags, &lsb_access_code,
                        &msb_access_code) < 0)
                {
                    break;
                }
                if (flags)
                {
                    APP_INFO1("BLE2 Enabled. Advertising:%d Scanning:%d Connection:%d",
                            flags & LE2_CFG_FLG_ADV_ENABLE?1:0,
                            flags & LE2_CFG_FLG_SCAN_ENABLE?1:0,
                            flags & LE2_CFG_FLG_CON_ENABLE?1:0);
                    APP_INFO1("BLE2 Access code LSB:0x%08X MSB:0x%08X",
                            lsb_access_code, msb_access_code);

                    if (app_ble2_brcm_ext_packet_len_read(&mode, &packet_len) < 0)
                    {
                        break;
                    }
                    APP_INFO1("BLE2 Mode:%d PacketLen:%d", mode, packet_len);
                }
                else
                {
                    APP_INFO0("BLE2 Disabled");
                }
            }
            break;
#endif

        case APP_BLE_MENU_GET_PHY:
            //choice = app_get_choice("Input ble connection handle: such as 64");
            app_ble_read_phy(s_u16_le_handle);
            break;
        case APP_BLE_MENU_SET_PHY:
            //handle = app_get_choice("Input ble connection handle: such as 64");
            tx_phys = app_get_choice("Input tx_phy (1:1M, 2:2M, 4:codec phy):");
            rx_phys = app_get_choice("Input rx_phy (1:1M, 2:2M, 4:codec phy):");
            phy_options = app_get_choice("Input phy_option (1:no prefer, 2:S=2, 4:S=8):");
            app_ble_set_phy(s_u16_le_handle, tx_phys, rx_phys, phy_options);
            break;
        case APP_BLE_MENU_SET_DEFAULT_PHY:
            all_phys = app_get_choice("Input all_phy (1:no prefer tx, 2:no prefer rx):");
            tx_phys = app_get_choice("Input tx_phy (1:1M, 2:2M, 4:codec phy):");
            rx_phys = app_get_choice("Input rx_phy (1:1M, 2:2M, 4:codec phy):");
            app_ble_set_default_phy(all_phys, tx_phys, rx_phys);
            break;


        default:
            APP_ERROR1("Unknown choice:%d", choice);
            break;
        }
    } while (choice != MENU_QUIT); /* While user don't exit application */


    app_ble_vse_deregister();

    /* Exit BLE mode */
    app_ble_exit();


    return 0;
}
#endif

