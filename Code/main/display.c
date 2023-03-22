#include <stdio.h>
#include "i2c_main.h"
#include "display.h"

static uint8_t disp_active = 0;

void disp_write(uint8_t reg_addr, uint8_t *data, size_t len){
    i2c_register_write(DISP_ADDR, reg_addr, data, len);
}

void disp_read(uint8_t reg_addr, uint8_t *data, size_t len){
    i2c_register_read(DISP_ADDR, reg_addr, data, len);
}

void disp_init(void){
    if (disp_active) {
        // TODO Reinitialize display if already active
    }

    gpio_reset_pin(DISP_RST_PIN);

    /* Set the GPIO pin as an output */
    gpio_set_direction(DISP_RST_PIN, GPIO_MODE_OUTPUT);
    
    /* Reset display */
    gpio_set_level(DISP_RST_PIN, disp_active);

    // TODO Pre-reset display configuration

    /* Enable display */
    disp_active = 1;
    gpio_set_level(DISP_RST_PIN, disp_active);

    // TODO Post-reset display configuration
}

void disp_clear(void){
    uint8_t data = CLEAR_DISPLAY;
    disp_write(GENERIC_REG, &data, 1);
}

void disp_text(unsigned char *text, uint8_t len, uint8_t row){
    int offset = (MAX_CHARACTERS - len)/2;
    
    uint8_t *data = (uint8_t *) malloc(sizeof(uint8_t) * (MAX_CHARACTERS + 2));
    data[0] = SET_DDRAM_ADDRESS | (row * DDRAM_SECOND_ROW_START);
    data[1] = DISP_RS;
    for(int i = 0; i < MAX_CHARACTERS; i++){
        if ((i >= offset) && (i < offset + len)) data[i + 2] = text[i - offset];
        else data[i + 2] = 0x20;
    }

    disp_write(Co_REG, data, len + 2);
}