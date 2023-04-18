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
#include "filter_resample.h"
#include "periph_button.h"
#include "a2dp_stream.h"
#include "bluetooth_service.h"
#include "board.h"
#include "audio_mem.h"
#include "driver/gpio.h"

#define TAG             "BT"
#define DEVICE_NAME     "ECE445 FM Transmitter"

#define SD_PIN          GPIO_NUM_0
#define WS_PIN          GPIO_NUM_0
#define BCLK_PIN        GPIO_NUM_0
#define CTRL_PINS       (GPIO_SEL_36|GPIO_SEL_39|GPIO_SEL_36|GPIO_SEL_39)

void blt_init(void);


#endif