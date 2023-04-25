#include "trans.h"

static uint8_t trans_active = 0;

void trans_write(uint8_t command, uint8_t* args, size_t num_args) {
    i2c_register_write(TRANS_ADDR, command, args, num_args);
}

void trans_read(uint8_t* data, size_t len) {
    i2c_address_only_read(TRANS_ADDR, data, len);
}

void trans_init(void) {
    if (trans_active) {
        // TODO Reinitialize transmitter if already active
    }

    gpio_reset_pin(TRANS_RST_PIN);

    /* Set the GPIO pin as an output */
    gpio_set_direction(TRANS_RST_PIN, GPIO_MODE_OUTPUT);

    /* Reset transmitter IC */
    gpio_set_level(TRANS_RST_PIN, trans_active);

    /* Enable transmitter IC */
    trans_active = 1;
    gpio_set_level(TRANS_RST_PIN, trans_active);

    vTaskDelay(GENERAL_DELAY / portTICK_PERIOD_MS);

    /* Power up transmitter and write initialization properties */
    trans_power_up();
    trans_set_property(REFCLK_FREQ, RCLK_FREQ);
    trans_set_property(RCLK_PRESCALE, 0x0001);
    trans_set_property(TX_LINE_INPUT_LEVEL_MUTE, 0x0000);
    trans_set_property(TX_PREEMPHASIS, 0x0000);
    trans_set_property(TX_PILOT_FREQUENCY, 0x4a38);
    trans_set_property(TX_AUDIO_DEVIATION, 0x1aa9);
    trans_set_property(TX_PILOT_DEVIATION, 0x02a3);
    trans_tune_power(115);
    trans_tune_freq(8800);
    trans_set_property(TX_COMPONENT_ENABLE, 0x0003);
    trans_set_property(DIGITAL_INPUT_SAMPLE_RATE, AUDIO_SAMPLE_RATE);
    trans_set_property(DIGITAL_INPUT_FORMAT, 0x0000);


    trans_set_property(TX_AUDIO_DEVIATION, 0x19E1);
    trans_set_property(TX_RDS_DEVIATION, 0x00C8);
    trans_set_property(TX_RDS_INTERRUPT_SOURCE, 0x0000);
    trans_set_property(TX_RDS_PI, 0x40A7);
    trans_set_property(TX_RDS_PS_MIX, 0x0006);
    trans_set_property(TX_RDS_PS_MISC, 0x1008);
    trans_set_property(TX_RDS_PS_REPEAT_COUNT, 0x0003);
    trans_set_property(TX_RDS_PS_MESSAGE_COUNT, 0x0003);
    // trans_set_property(TX_RDS_PS_AF, 0xE102);           // Alt frequency, probably don't need
    trans_set_property(TX_RDS_FIFO_SIZE, 0x0004);
    trans_set_property(TX_COMPONENT_ENABLE, 0x7);
}

void trans_power_up(void) {
    /* Setup arguments */
    uint8_t args[2] = {
        POWER_TRANSMIT,
        POWER_DIGITAL_AUDIO
    };

    /* Write command to transmitter and delay */
    trans_write(POWER_UP, args, 2);
    vTaskDelay(POWER_UP_DELAY / portTICK_PERIOD_MS);
}

void trans_tune_freq(uint16_t freq) {
    /* Setup arguments */
    uint8_t args[3] = {
        0x00,
        (freq >> 8),
        (freq & 0xff)
    };

    /* Write command to transmitter and delay */
    trans_write(TX_TUNE_FREQ, args, 3);
    vTaskDelay(TRANS_STC_DELAY / portTICK_PERIOD_MS);
}

void trans_tune_power(uint8_t power) {
    /* Setup arguments */
    uint8_t args[4] = {
        0x00,
        0x00,
        power,
        0x00
    };

    /* Write command to transmitter and delay */
    trans_write(TX_TUNE_POWER, args, 4);
    vTaskDelay(20 / portTICK_PERIOD_MS);
}

uint8_t trans_get_int_status(void) {
    uint8_t args;
    uint8_t response;

    /* Write command to transmitter, delay, and read response */
    trans_write(GET_INT_STATUS, &args, 0);
    vTaskDelay(GENERAL_DELAY / portTICK_PERIOD_MS);
    trans_read(&response, 1);

    return response;
}

void trans_tune_status(uint8_t* response) {
    /* Setup arguments */
    uint8_t arg = TUNE_INT_CLR;

    /* Write command to transmitter, delay, and read response */
    trans_write(TX_TUNE_STATUS, &arg, 1);
    vTaskDelay(GENERAL_DELAY / portTICK_PERIOD_MS);
    trans_read(response, 8);
}

void trans_set_property(uint16_t prop, uint16_t val) {
    /* Setup arguments */
    uint8_t args[5] = {
        0x00,
        (prop >> 8),
        (prop & 0xff),
        (val >> 8),
        (val & 0xff)
    };

    /* Write command to transmitter and delay */
    trans_write(SET_PROPERTY, args, 5);
    vTaskDelay(PROP_DELAY / portTICK_PERIOD_MS);
}

void trans_rds_ps_write(uint8_t psid, char* text) {
    /* Setup arguments */
    uint8_t args[5] = {
        psid,
        text[0],
        text[1],
        text[2],
        text[3],
    };

    /* Write command to transmitter and delay */
    trans_write(TX_RDS_PS, args, 5);
    vTaskDelay(GENERAL_DELAY / portTICK_PERIOD_MS);

}

void trans_rds_write(char* text, uint8_t len) {
    uint8_t psid = 0;

    while ((len >= 4) && (psid < 24)) {
        trans_rds_ps_write(psid, text);
        text += 4;
        len -= 4;
        psid++;
    }
    if ((len > 0) && (psid < 24)) {
        char* tex[4] = {};
        for (int i = 0; i < 4; i++) {
            if (i < len) tex[i] = text[i];
            else tex[i] = 0x20;
        }
        trans_rds_ps_write(psid, tex);
        psid++;
    }
    while (psid < 24) {
        char* tex[4] = {0x20, 0x20, 0x20, 0x20};
        trans_rds_ps_write(psid, tex);
        psid++;
    }
}