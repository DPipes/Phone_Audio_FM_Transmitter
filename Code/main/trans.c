#include <stdio.h>
#include "i2c_main.h"
#include "trans.h"

static uint8_t trans_active = 0;

void trans_command(uint8_t command, uint8_t *args, size_t num_args, uint8_t *response, size_t resp_len){
    /* Initialize write buffer with command followed by arguments */
    size_t write_len = num_args + 1;
    uint8_t write_buf[write_len];
    
    write_buf[0] = command;
    for(int i = 0; i < num_args; i++){
        write_buf[i + 1] = args[i];
    }
    /*send the command and get the response*/
    i2c_write_read(TRANS_ADDR, write_buf, write_len, response, resp_len);
}

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

void power_up_std(uint8_t *response){
    /*set up the parameters*/
    uint8_t cmd = 0x1;
    uint8_t arg1 = 0x12;
    uint8_t arg2 = 0x0f;
    size_t num_args = 2;
    size_t resp_len = 1;
    uint8_t args[] = {arg1,arg2};

    /*send the command and get the response*/
    
    trans_command(cmd, args, num_args, response, resp_len);

}