/* Play music from Bluetooth device

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "blt.h"

static void bt_app_avrc_ct_cb(esp_avrc_ct_cb_event_t event, esp_avrc_ct_cb_param_t *p_param)
{
    esp_avrc_ct_cb_param_t *rc = p_param;
    switch (event) {
        case ESP_AVRC_CT_METADATA_RSP_EVT: {
            uint8_t *tmp = audio_calloc(1, rc->meta_rsp.attr_length + 1);
            memcpy(tmp, rc->meta_rsp.attr_text, rc->meta_rsp.attr_length);
            ESP_LOGI(TAG, "AVRC metadata rsp: attribute id 0x%x, %s", rc->meta_rsp.attr_id, tmp);
            audio_free(tmp);
            break;
        }
        default:
            break;
    }
}

void blt_init(void) {
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    bluetooth_service_cfg_t blt_config = {
        .device_name = DEVICE_NAME,
        .mode = BLUETOOTH_A2DP_SINK,
        .user_callback.user_avrc_ct_cb = bt_app_avrc_ct_cb, 
    };
    bluetooth_service_start(&blt_config);

    audio_pipeline_cfg_t pipeline_config = DEFAULT_AUDIO_PIPELINE_CONFIG();
    audio_pipeline_handle_t pipeline = audio_pipeline_init(&pipeline_config);

    i2s_stream_cfg_t i2s_config = I2S_STREAM_CFG_DEFAULT();
    audio_element_handle_t i2s_element = i2s_stream_init(&i2s_config);
    i2s_pin_config_t i2s_pins = {
        .data_out_num = SD_PIN,
        .ws_io_num = WS_PIN,
        .bck_io_num = BCLK_PIN
    };
    i2s_set_pin(i2s_config.i2s_port, &i2s_pins);

    audio_element_handle_t blt_element = bluetooth_service_create_stream();

    audio_pipeline_register(pipeline, blt_element, "bt");
    audio_pipeline_register(pipeline, i2s_element, "i2s");

    const char *link_tag[] = {"bt", "i2s"};
    audio_pipeline_link(pipeline, link_tag, 2);

    esp_periph_config_t periph_config = DEFAULT_ESP_PERIPH_SET_CONFIG();
    esp_periph_set_handle_t periph_handle = esp_periph_set_init(&periph_config);
    periph_button_cfg_t button_config = {
        .gpio_mask = CTRL_PINS
    };
    esp_periph_handle_t button_periph = periph_button_init(&button_config);
    esp_periph_handle_t bt_periph = bluetooth_service_create_periph();
    esp_periph_start(periph_handle, bt_periph);
    esp_periph_start(periph_handle, button_periph);

    audio_event_iface_cfg_t event_config = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    audio_event_iface_handle_t event_handle = audio_event_iface_init(&event_config);
    audio_event_iface_set_listener(esp_periph_set_get_event_iface(periph_handle), event_handle);

    audio_pipeline_run(pipeline);

    while(1) {
        audio_event_iface_msg_t msg;
        audio_event_iface_listen(event_handle, &msg, portMAX_DELAY);
        if((int)msg.source_type == PERIPH_ID_BUTTON && (int)msg.cmd == PERIPH_BUTTON_PRESSED) {
            printf("button = %d, command = %d\n", (int)msg.data, (int)msg.cmd);
            // periph_bluetooth_pause(bt_periph);
        }
    }
    return;
}

