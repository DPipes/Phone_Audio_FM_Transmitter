#include <stdio.h>
#include "i2c_main.h"
#include "trans.h"

static uint8_t trans_active = 0;

void trans_command_full(uint8_t command, uint8_t *args, size_t num_args, 
uint16_t delay, uint8_t *response, size_t resp_len){

    /*send the command*/
    i2c_register_write(TRANS_ADDR, command, args, num_args);

    //delay by "delay" ms
    vTaskDelay(delay / portTICK_PERIOD_MS);

    //recieve the response
    i2c_address_only_read(TRANS_ADDR, response, resp_len);
}

void trans_command_write(uint8_t command, uint8_t *args, size_t num_args){
    i2c_register_write(TRANS_ADDR, command, args, num_args);
}

void trans_read(uint8_t *data, size_t len){
    i2c_address_only_read(TRANS_ADDR, data, len);
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

    vTaskDelay(1 / portTICK_PERIOD_MS);

    uint8_t response;

    trans_power_up_std(&response);
}

void trans_power_up_std(uint8_t *response){
    /*set up the parameters*/
    uint8_t cmd = 0x1;
    uint8_t arg1 = 0x02;
    uint8_t arg2 = 0x0f;
    size_t num_args = 2;
    uint8_t args[] = {arg1,arg2};
    
    uint16_t delay = 112;

    size_t resp_len = 1;

    /*send the command and get the response*/
    
    //not using trans_command anymore cause of delay
    //trans_command(cmd, args, num_args, response, resp_len);

    trans_command_full(cmd, args, num_args, delay, response, resp_len);
}

void trans_set_freq_full(uint16_t freq){
    /*set up the parameters*/
    uint8_t cmd = 0x30;

    uint8_t arg1 = 0x0;
    uint8_t arg2 = freq >> 8;
    uint8_t arg3 = freq & 0xff;
    size_t num_args = 3;
    uint8_t args[] = {arg1,arg2,arg3};
    
    uint16_t delay = 101; //change this to what is appropriate

    unit8_t response;
    size_t resp_len = 1;

    trans_command_full(cmd, args, num_args, delay, &response, resp_len);
}

void trans_set_freq_write(uint16_t freq){
    /*set up the parameters*/
    uint8_t cmd = 0x30;

    uint8_t arg1 = 0x0;
    uint8_t arg2 = freq >> 8;
    uint8_t arg3 = freq & 0xff;
    size_t num_args = 3;
    uint8_t args[] = {arg1,arg2,arg3};
    
    trans_command_write(cmd, args, num_args);
}

void trans_set_power_write(uint8_t power){
    /*set up the parameters*/
    uint8_t cmd = 0x31;

    uint8_t arg1 = 0;
    uint8_t arg2 = 0;
    uint8_t arg3 = power;
    uint8_t arg4 = 0;
    size_t num_args = 4;
    uint8_t args[] = {arg1,arg2,arg3,arg4};
    
    uint16_t delay = 21; //change this to what is appropriate
    
    trans_command_write(cmd, args, num_args);
}

// void trans_get_int_status(uint8_t *response){
//     /*set up parameters*/
//     uint8_t cmd = 0x14;
//     size_t num_args = 0;
//     uint8_t args[] = NULL;
    
//     uint16_t delay = 1;

//     size_t resp_len = 1;

//     trans_command_full(cmd, args, num_args, delay, response, resp_len);
// }

uint8_t trans_get_int_status(void){
    /*set up parameters, command and args*/
    uint8_t cmd = 0x14;
    size_t num_args = 0;
    uint8_t args;
    
    /*define delay*/
    uint16_t delay = 1;

    /*set up parameters, response*/
    uint8_t response;
    size_t resp_len = 1;

    /*send the command, get the response*/
    trans_command_full(cmd, &args, num_args, delay, &response, resp_len);

    return response;
}

void trans_tune_status(uint8_t *response){
    /*set up parameters, command and args*/
    uint8_t cmd = 0x33;
    uint8_t arg1 = 0x1;
    size_t num_args = 1;
    uint8_t args[] = {arg1};
    
    /*define delay*/
    uint16_t delay = 1;

    /*set up parameters, response*/
    size_t resp_len = 8;

    /*send the command, get the response*/
    trans_command_full(cmd, args, num_args, delay, &response, resp_len);
}

void trans_set_property(uint16_t prop, uint16_t val, uint8_t *response){
    /*set up parameters, command and args*/
    uint8_t cmd = 0x12;
    uint8_t arg1 = 0x0;
    uint8_t arg2 = prop >> 8;
    uint8_t arg3 = prop & 0xff;
    uint8_t arg4 = val >> 8;
    uint8_t arg5 = val & 0xff;
    size_t num_args = 5;
    uint8_t args[] = {arg1, arg2, arg3, arg4, arg5};
    
    /*define delay*/
    uint16_t delay = 11;

    /*set up parameters, response*/
    size_t resp_len = 1;

    /*send the command, get the response*/
    trans_command_full(cmd, args, num_args, delay, &response, resp_len);
}

void trans_set_property_write(uint16_t prop, uint16_t val){
    /*set up parameters, command and args*/
    uint8_t cmd = 0x12;
    uint8_t arg1 = 0x0;
    uint8_t arg2 = prop >> 8;
    uint8_t arg3 = prop & 0xff;
    uint8_t arg4 = val >> 8;
    uint8_t arg5 = val & 0xff;

    size_t num_args = 5;
    uint8_t args[] = {arg1, arg2, arg3, arg4, arg5};

    /*send the command,*/
    trans_command_write(cmd, args, num_args);
}
