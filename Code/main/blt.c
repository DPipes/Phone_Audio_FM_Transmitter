/* Play music from Bluetooth device

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "blt.h"

static esp_avrc_playback_stat_t playback_status = ESP_AVRC_PLAYBACK_PAUSED;
static esp_periph_handle_t bt_periph;
static audio_event_iface_handle_t event_handle;

static void bt_app_avrc_ct_cb(esp_avrc_ct_cb_event_t event, esp_avrc_ct_cb_param_t *p_param) {
    esp_avrc_ct_cb_param_t *rc = p_param;
    switch (event) {
        // Request for metadata, return artist, album, track
        case ESP_AVRC_CT_METADATA_RSP_EVT: {
            uint8_t *tmp = audio_calloc(1, rc->meta_rsp.attr_length + 1);
            memcpy(tmp, rc->meta_rsp.attr_text, rc->meta_rsp.attr_length);
            ESP_LOGI(TAG, "AVRC metadata rsp: attribute id 0x%x, %s", rc->meta_rsp.attr_id, tmp);
            audio_free(tmp);
            break;
        }
        // Playback changed event
        case ESP_AVRC_CT_CHANGE_NOTIFY_EVT: 
            if(rc->change_ntf.event_id == ESP_AVRC_RN_PLAY_STATUS_CHANGE) {
                playback_status = rc->change_ntf.event_parameter.playback;
                ESP_LOGI(TAG, "AVRC playback status changed: 0x%x", rc->change_ntf.event_parameter.playback);
            }
            break;
        default:
            break;
    }
}

void blt_init(void) {
    // Setting up non-volatile storage, make room if needed
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES) {
        nvs_flash_erase();
        nvs_flash_init();
    }

    // Bluetooth configuration and starting service
    bluetooth_service_cfg_t blt_config = {
        .device_name = DEVICE_NAME,
        .mode = BLUETOOTH_A2DP_SINK,
        .user_callback.user_avrc_ct_cb = bt_app_avrc_ct_cb, // callback command for Bluetooth events
    };
    bluetooth_service_start(&blt_config);

    // Create audio pipeline from ADF
    audio_pipeline_cfg_t pipeline_config = DEFAULT_AUDIO_PIPELINE_CONFIG();
    audio_pipeline_handle_t pipeline = audio_pipeline_init(&pipeline_config);

    // I2S configuration and start
    i2s_stream_cfg_t i2s_config = I2S_STREAM_CFG_DEFAULT();
    // i2s_config.i2s_config.bits_per_sample = I2S_BITS_PER_SAMPLE_8BIT;
    audio_element_handle_t i2s_element = i2s_stream_init(&i2s_config);
    i2s_pin_config_t i2s_pins = {   // Reassign I2S pins from default
        .data_out_num = SD_PIN,
        .ws_io_num = WS_PIN,
        .bck_io_num = BCLK_PIN
    };
    i2s_set_pin(i2s_config.i2s_port, &i2s_pins);

    audio_element_handle_t blt_element = bluetooth_service_create_stream();

    // Adding Bluetooth and I2S as elements in the audio pipeline
    audio_pipeline_register(pipeline, blt_element, "bt");
    audio_pipeline_register(pipeline, i2s_element, "i2s");
    const char *link_tag[] = {"bt", "i2s"};
    audio_pipeline_link(pipeline, link_tag, 2);

    // Add buttons as a peripheral for controlling the Bluetooth stream
    esp_periph_config_t periph_config = DEFAULT_ESP_PERIPH_SET_CONFIG();
    esp_periph_set_handle_t periph_handle = esp_periph_set_init(&periph_config);
    periph_button_cfg_t button_config = {   // Setting GPIO pins to configure as buttons
        .gpio_mask = CTRL_PINS
    };
    esp_periph_handle_t button_periph = periph_button_init(&button_config);
    bt_periph = bluetooth_service_create_periph();
    esp_periph_start(periph_handle, bt_periph);
    esp_periph_start(periph_handle, button_periph);
 
    // Create listener for audio events (playback status changed/metadata requests)
    audio_event_iface_cfg_t event_config = AUDIO_EVENT_IFACE_DEFAULT_CFG();
    event_handle = audio_event_iface_init(&event_config);
    audio_event_iface_set_listener(esp_periph_set_get_event_iface(periph_handle), event_handle);

    // Pipeline ready to use
    audio_pipeline_run(pipeline);
    return;
}

void check_buttons(void) {
    audio_event_iface_msg_t msg;
    // Check for events
    audio_event_iface_listen(event_handle, &msg, portMAX_DELAY);
    if((int)msg.source_type == PERIPH_ID_BUTTON && (int)msg.cmd == PERIPH_BUTTON_PRESSED) {
        // Button pressed 
        // ESP_LOGI(TAG, "Button pressed: gpio_num %d", (int)msg.data, (int)msg.cmd);
        switch((int)msg.data) {
            case 18: 
                if(playback_status == ESP_AVRC_PLAYBACK_PLAYING) {
                    periph_bluetooth_pause(bt_periph);
                    // ESP_LOGI(TAG, "AVRC playback: pause");
                } else if(playback_status == ESP_AVRC_PLAYBACK_PAUSED) {
                    periph_bluetooth_play(bt_periph);
                    // ESP_LOGI(TAG, "AVRC playback: start");
                }
                break;
            case 16:
                periph_bluetooth_next(bt_periph);
                // ESP_LOGI(TAG, "AVRC playback: next track");
                break;
            case 21:
                periph_bluetooth_prev(bt_periph);
                // ESP_LOGI(TAG, "AVRC playback: previous track");
                break;
            default:
                break; 
        }
    }
    return;
}
