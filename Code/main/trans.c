#include <stdio.h>
#include "i2c_main.h"
#include "trans.h"

static uint8_t trans_active = 0;

void trans_write(uint8_t reg_addr, uint8_t *data, size_t len){
    i2c_register_write(TRANS_ADDR, reg_addr, data, len);
}

void trans_read(uint8_t reg_addr, uint8_t *data, size_t len){
    i2c_register_read(TRANS_ADDR, reg_addr, data, len);
}

void trans_init(void){
    if (trans_active) {
        // TODO Reinitialize transmitter if already active
    }

    gpio_reset_pin(TRANS_RST_PIN);

    /* Set the GPIO pin as an output */
    gpio_set_direction(TRANS_RST_PIN, GPIO_MODE_OUTPUT);
    
    /* Reset transmitter IC */
    gpio_set_level(TRANS_RST_PIN, trans_active);

    // TODO Pre-reset IC configuration

    /* Enable transmitter IC */
    trans_active = 1;
    gpio_set_level(TRANS_RST_PIN, trans_active);

    // TODO Post-reset IC configuration
}

void trans_freq(uint16_t freq){

}