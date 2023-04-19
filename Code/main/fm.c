#include <stdio.h>
#include "trans.h"
#include "rec.h"
#include "driver/ledc.h"
#include "fm.h"


void rclk_init(void) {
    // PWM
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_4_BIT,
        .freq_hz = RCLK_FREQ,  // Set output frequency at 5 kHz
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = RCLK_PIN,
        .duty = 7, // Set duty to 50%
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void change_freq(void) {

}