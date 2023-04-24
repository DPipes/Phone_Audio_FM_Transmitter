#include "rec.h"

static uint8_t rec_active = 0;

void rec_write(uint8_t command, uint8_t* args, size_t num_args) {
    i2c_register_write(REC_ADDR, command, args, num_args);
}

void rec_read(uint8_t* data, size_t len) {
    i2c_address_only_read(REC_ADDR, data, len);
}

void rec_init(void) {
    if (rec_active) {
        // TODO Reinitialize receiver if already active
    }

    gpio_reset_pin(REC_RST_PIN);

    /* Set the GPIO pin as an output */
    gpio_set_direction(REC_RST_PIN, GPIO_MODE_OUTPUT);

    /* Reset reciever IC */
    gpio_set_level(REC_RST_PIN, rec_active);

    /* Enable reciever IC */
    rec_active = 1;
    gpio_set_level(REC_RST_PIN, rec_active);

    vTaskDelay(GENERAL_DELAY / portTICK_PERIOD_MS);

    /* Power up receiver and write initialization properties */
    rec_power_up();
    rec_set_property(FM_ANTENNA_INPUT, 1);
    rec_set_property(REFCLK_FREQ, RCLK_FREQ);
    rec_set_property(RCLK_PRESCALE, 1);
    rec_set_property(RX_VOLUME, 63);
    rec_set_property(FM_DEEMPHASIS, 1);
    rec_set_property(RX_HARD_MUTE, 0);
    rec_set_property(FM_MAX_TUNE_ERROR, 20);
    rec_tune_freq(8800);
}

void rec_power_up(void) {
    /* Setup arguments */
    uint8_t args[2] = {
        POWER_RECEIVE,
        POWER_ANALOG_AUDIO
    };

    /* Write command to receiver and delay */
    rec_write(POWER_UP, args, 2);
    vTaskDelay(POWER_UP_DELAY / portTICK_PERIOD_MS);
}


void rec_tune_freq(uint16_t freq) {
    /* Setup arguments */
    uint8_t args[4] = {
        0x00,
        (freq >> 8),
        (freq & 0xff),
        0x00
    };

    /* Write command to receiver and delay */
    rec_write(FM_TUNE_FREQ, args, 4);
    vTaskDelay(REC_STC_DELAY / portTICK_PERIOD_MS);
}

uint8_t rec_get_int_status(void) {
    uint8_t args;
    uint8_t response;

    /* Write command to receiver, delay, and read response */
    rec_write(GET_INT_STATUS, &args, 0);
    vTaskDelay(GENERAL_DELAY / portTICK_PERIOD_MS);
    rec_read(&response, 1);

    return response;
}

void rec_tune_status(uint8_t* response) {
    /* Setup arguments */
    uint8_t arg = SEEK_CANCEL & TUNE_INT_CLR;

    /* Write command to receiver, delay, and read response */
    rec_write(FM_TUNE_STATUS, &arg, 1);
    vTaskDelay(GENERAL_DELAY / portTICK_PERIOD_MS);
    rec_read(response, 8);
}

void rec_rsq_status(uint8_t* response) {
    /* Setup arguments */
    uint8_t arg = RSQ_INT_CLR;

    /* Write command to receiver, delay, and read response */
    rec_write(FM_RSQ_STATUS, &arg, 1);
    vTaskDelay(GENERAL_DELAY / portTICK_PERIOD_MS);
    rec_read(response, 8);
}

void rec_set_property(uint16_t prop, uint16_t val) {
    /* Setup arguments */
    uint8_t args[5] = {
        0x00,
        (prop >> 8),
        (prop & 0xff),
        (val >> 8),
        (val & 0xff)
    };

    /* Write command to receiver and delay */
    rec_write(SET_PROPERTY, args, 5);
    vTaskDelay(PROP_DELAY / portTICK_PERIOD_MS);
}