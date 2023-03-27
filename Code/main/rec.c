#include <stdio.h>
#include "i2c_main.h"
#include "rec.h"

static uint8_t rec_active = 0;

void rec_write(uint8_t reg_addr, uint8_t *data, size_t len){
    i2c_register_write(REC_ADDR, reg_addr, data, len);
}

void rec_read(uint8_t reg_addr, uint8_t *data, size_t len){
    i2c_register_read(REC_ADDR, reg_addr, data, len);
}

void rec_init(void){
    if (rec_active) {
        // TODO Reinitialize receiver if already active
    }

    gpio_reset_pin(REC_RST_PIN);

    /* Set the GPIO pin as an output */
    gpio_set_direction(REC_RST_PIN, GPIO_MODE_OUTPUT);
    
    /* Reset reciever IC */
    gpio_set_level(REC_RST_PIN, rec_active);

    // TODO Pre-reset IC configuration

    /* Enable reciever IC */
    rec_active = 1;
    gpio_set_level(REC_RST_PIN, rec_active);

    // TODO Post-reset IC configuration
}

void rec_freq(uint16_t freq){
    // TODO set new recieveing frequency
}

void rec_param(uint8_t *rssi, uint8_t *snr, uint8_t *mpi){
    // TODO recieve signal strength parameters
}