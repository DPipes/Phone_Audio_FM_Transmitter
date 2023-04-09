#include <stdio.h>
#include "i2c_main.h"
#include "rec.h"

static uint8_t rec_active = 0;

void rec_command_full(uint8_t command, uint8_t *args, size_t num_args, 
uint16_t delay, uint8_t *response, size_t resp_len){

    /*send the command*/
    i2c_register_write(REC_ADDR, command, args, num_args);

    //delay by "delay" ms
    vTaskDelay(delay / portTICK_PERIOD_MS);

    //recieve the response
    i2c_address_only_read(REC_ADDR, response, resp_len);
}

void rec_command_write(uint8_t command, uint8_t *args, size_t num_args){
    i2c_register_write(REC_ADDR, command, args, num_args);
}

void rec_read(uint8_t *data, size_t len){
    i2c_register_read(REC_ADDR, data, data, len);
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

void rec_power_up_std(uint8_t *response){
    /*set up the parameters*/
    uint8_t cmd = 0x1;
    uint8_t arg1 = 0x10;
    uint8_t arg2 = 0x05;
    size_t num_args = 2;
    uint8_t args[] = {arg1,arg2};
    
    uint16_t delay = 112;

    size_t resp_len = 1;

    /*send the command and get the response*/
    
    //not using trans_command anymore cause of delay
    //trans_command(cmd, args, num_args, response, resp_len);

    trans_command_full(cmd, args, num_args, delay, response, resp_len);
}


void rec_set_freq_full(uint16_t freq){
    /*set up the parameters*/
    uint8_t cmd = 0x20;

    uint8_t arg1 = 0x0;
    uint8_t arg2 = freq >> 8;
    uint8_t arg3 = freq & 0xff;
    uint8_t arg4 = 0;
    size_t num_args = 4; // automatic capacitor tuning
    uint8_t args[] = {arg1,arg2,arg3,arg4};
    
    uint16_t delay = 61; //change this to what is appropriate

    unit8_t response;
    size_t resp_len = 1;

    rec_command_full(cmd, args, num_args, delay, &response, resp_len);
}

void rec_set_freq_write(uint16_t freq){
    /*set up the parameters*/
    uint8_t cmd = 0x20;

    uint8_t arg1 = 0x0;
    uint8_t arg2 = freq >> 8;
    uint8_t arg3 = freq & 0xff;
    uint8_t arg4 = 0;
    size_t num_args = 4; // automatic capacitor tuning
    uint8_t args[] = {arg1,arg2,arg3,arg4};

    rec_command_write(cmd, args, num_args);
}

void rec_get_int_status(uint8_t *response){
    /*set up parameters*/
    uint8_t cmd = 0x14;
    uint8_t num_args = 0;
    uint8_t args[] = NULL;
    
    uint16_t delay = 1;

    size_t resp_len = 1;

    trans_command_full(cmd, args, num_args, delay, response, num_args);
}

uint8_t rec_get_int_status(void){
    /*set up parameters*/
    uint8_t cmd = 0x14;
    size_t num_args = 0;
    uint8_t args[] = NULL;
    
    uint16_t delay = 1;

    size_t resp_len = 1;
    uint8_t response;

    trans_command_full(cmd, args, num_args, delay, &response, num_args);

    return response;
}

rec_tune_status

void rec_param(uint8_t *rssi, uint8_t *snr, uint8_t *mpi){

}