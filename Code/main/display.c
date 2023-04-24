#include "display.h"

static uint8_t disp_active = 0;

void disp_write(uint8_t reg_addr, uint8_t* data, size_t len) {
    i2c_register_write(DISP_ADDR, reg_addr, data, len);
}

void disp_read(uint8_t reg_addr, uint8_t* data, size_t len) {
    i2c_register_read(DISP_ADDR, reg_addr, data, len);
}

void disp_init(void) {
    uint8_t val[9] = {
        0x38,
        0x39,
        0x14,
        0x78,
        0x5E,
        0x6D,
        0x0C,
        0x01,
        0x06
    };

    if (disp_active) {
        // TODO Reinitialize display if already active
    }

    gpio_reset_pin(DISP_RST_PIN);

    /* Set the GPIO pin as an output */
    gpio_set_direction(DISP_RST_PIN, GPIO_MODE_OUTPUT);

    /* Reset display */
    gpio_set_level(DISP_RST_PIN, disp_active);

    /* Enable display */
    disp_active = 1;
    gpio_set_level(DISP_RST_PIN, disp_active);

    /* Write initialization commands with required timing between each command */
    disp_write(0x00, val, 1);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    disp_write(0x00, val + 1, 1);
    vTaskDelay(10 / portTICK_PERIOD_MS);
    disp_write(0x00, val + 2, 7);

    /* Setup backlight pin and turn on backlight */
    gpio_reset_pin(BKLIT_PIN);
    gpio_set_direction(BKLIT_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(BKLIT_PIN, 1);

    /* Delay required to finalize display setup */
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

void disp_clear(void) {
    uint8_t data = CLEAR_DISPLAY;
    disp_write(GENERIC_REG, &data, 1);
}

int freq_to_string(uint16_t freq, char* text) {
    int len, k;
    int x = 0;
    char str[5];

    /* Remove lowest decimal value because it is unnecessary */
    freq /= 10;

    /* Extract fractional decimal value */
    k = freq % 10;
    freq /= 10;

    /* Convert integer decimal value to string and store it to text pointer  */
    sprintf(str, "%d", freq);
    text[0] = str[0];
    text[1] = str[1];
    if (freq > 99) {
        text[2] = str[2];
        len = 5;
        x = 1;
    }
    else len = 4;

    /* Store the decimal point and fractional decimal value to text pointer */
    text[2 + x] = '.';
    sprintf(str, "%d", k);
    text[3 + x] = str[0];

    /* Return string length */
    return len;
}

void disp_text(char* text, uint8_t len, uint8_t row) {
    uint8_t data[MAX_CHARACTERS];
    uint8_t command = SET_DDRAM_ADDRESS | (row * DDRAM_SECOND_ROW_START);

    /* Calculate number of blank spaces on either side of text to center it */
    int offset = (MAX_CHARACTERS - len) / 2;

    /* Store blank characters and text to data array */
    /* This fills the entire row of display to erase any previous text */
    for (int i = 0; i < MAX_CHARACTERS; i++) {
        if ((i >= offset) && (i < offset + len)) data[i] = text[i - offset];
        else data[i] = 0x20;
    }

    /* Set DDRAM address for desired row and write characters to display */
    disp_write(0x00, &command, 1);
    disp_write(0x40, data, MAX_CHARACTERS);
}