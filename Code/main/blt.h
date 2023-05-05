#ifndef __BLT_H__
#define __BLT_H__

#include <string.h>
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_a2dp_api.h"
#include "esp_avrc_api.h"
#include "esp_peripherals.h"

#include "nvs_flash.h"
#include "audio_element.h"
#include "audio_pipeline.h"
#include "audio_event_iface.h"
#include "i2s_stream.h"
#include "input_key_service.h"
#include "periph_button.h"
#include "a2dp_stream.h"
#include "bluetooth_service.h"
#include "board.h"
#include "audio_mem.h"
#include "driver/gpio.h"
#include "trans.h"

#define TAG             "BT"
#define DEVICE_NAME     "ECE445 FM Transmitter"

#define SD_PIN              GPIO_NUM_25
#define WS_PIN              GPIO_NUM_33
#define BCLK_PIN            GPIO_NUM_32

#define AVRC_PLAY_PAUSE_PIN GPIO_NUM_18
#define AVRC_NEXT_PIN       GPIO_NUM_16
#define AVRC_PREV_PIN       GPIO_NUM_21
#define CHANGE_FREQ_PIN		GPIO_NUM_19
#define CTRL_PINS       (GPIO_SEL_18|GPIO_SEL_21|GPIO_SEL_16|GPIO_SEL_19)

#define METADATA_LEN		22



/**
 * @brief Callback function for the Bluetooth system
 * 
 * @param event The type of event 
 * @param p_param The data for the event
 */

// void bt_app_avrc_ct_cb(esp_avrc_ct_cb_event_t event, esp_avrc_ct_cb_param_t *p_param);


/**
 * @brief Initializes the Bluetooth system.
 */
void blt_init(void);

/**
 * @brief Sets up GPIO pins specified in "blt.h" as buttons and sends AVRC 
 * commands to the Bluetooth device when pressed.
 * 
 * @return uin8_t The GPIO number of the button if pressed, 0 otherwise
 */
uint8_t check_buttons(void);

void blt_metadata(uint8_t *tmp, uint8_t id, uint8_t len);


#endif