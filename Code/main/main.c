#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "i2c_main.h"
#include "display.h"
#include "blt.h"
#include "rec.h"
#include "trans.h"
#include "input.h"
#include "fm.h"

void app_main(void)
{
    /* Initialize */
    i2c_init();
    rclk_init();
    blt_init();
    trans_init(); //be sure to set digital input format
    rec_init();
    //input_init();
    disp_init();
    
    

    /* Variables for current and previous GPIO state */
    //int freq = 1;
    //int play = 1;
    //int prev = 1;
    //int next = 1;
    //int freq_ = 1;
    //int play_ = 1;
    //int prev_ = 1;
    //int next_ = 1;

    char text[20];
    int len;

    struct Chan chan;
    chan.freq = 8790;
    chan.rssi = 255;
    chan.freq2 = 8900;
    chan.rssi2 = 255;

    uint16_t temp_freq = chan.freq;
    uint8_t temp_rssi = chan.rssi;

    //tesing the transmitter and reciever
    uint8_t trans_tune_response[8] = {};
    uint16_t trans_freq = 0;

    uint8_t int_status = 0;

    uint8_t rssi = 0;
    uint8_t snr = 0;
    uint8_t mpi = 0;

    uint16_t rec_freq = 0;
    uint8_t rec_tune_response[8] = {};
    uint16_t set_freq = 8810;
    rec_set_freq_full(set_freq);
    
    for (int i = 8810; i < 10800; i += 20) {
        rec_set_freq_full(i);

        rec_tune_status(rec_tune_response);
        rssi = rec_tune_response[4];
        //printf("Frequency  : %d\tRSSI: %d\n\n", i, rssi);

        if (rssi <= chan.rssi) {
            chan.freq2 = chan.freq;
            chan.rssi2 = chan.rssi;
            chan.freq = i;
            chan.rssi = rssi;
        }
        else if (i == chan.freq) {
            if (rssi <= chan.rssi2) chan.rssi = rssi;
            else {
                temp_freq = chan.freq;
                chan.freq = chan.freq2;
                chan.rssi = chan.rssi2;
                chan.freq2 = temp_freq;
                chan.rssi2 = rssi;
            }
        }
        else if (rssi <= chan.rssi2) {
            chan.freq2 = i;
            chan.rssi2 = rssi;
        }
        else if (i == chan.freq2) {
            chan.rssi2 = rssi;
        }
        
    }

    //printf("Frequency 1: %d\tRSSI: %d\n", chan.freq, chan.rssi);
    //printf("Frequency 2: %d\tRSSI: %d\n\n", chan.freq2, chan.rssi2);

    trans_set_freq_full(chan.freq);
    len = freq_to_string(chan.freq, text);
    disp_text(text, len, 0);

    temp_freq = chan.freq;
    temp_rssi = chan.rssi;
    chan.freq = chan.freq2;
    chan.rssi = chan.rssi2;
    chan.freq2 = temp_freq;
    chan.rssi2 = temp_rssi;

    /* Main loop */
    while (true) {

        // TODO Check if this can be done with any GPIO functions rather than having to track current and previous pin state
        /* Get current GPIO state */
        //freq_ = gpio_get_level(CHANGE_FREQ_PIN);
        //play_ = gpio_get_level(PLAY_PAUSE_PIN);
        //prev_ = gpio_get_level(PREV_PIN);
        //next_ = gpio_get_level(NEXT_PIN);

        /////* On button press perform functions */
        //if (!freq_ && freq) {
        //    trans_set_freq_full(chan.freq);
        //    len = freq_to_string(chan.freq, text);
        //    disp_text(text, len, 0);
        //    temp_freq = chan.freq;
        //    chan.freq = chan.freq2;
        //    chan.rssi = chan.rssi2;
        //    chan.freq2 = temp_freq;
        //    chan.rssi2 = 70;
        //    //printf("Frequency 1: %d\tRSSI: %d\n", chan.freq, chan.rssi);
        //    //printf("Frequency 2: %d\tRSSI: %d\n\n", chan.freq2, chan.rssi2);
        //}
        //if (!play_ && play) blt_play_pause();
        //if (!prev_ && prev) blt_prev();
        //if (!next_ && next) blt_next();

        /////* Update previous GPIO state */
        //freq = freq_;
        //play = play_;
        //prev = prev_;
        //next = next_;
           
        set_freq = set_freq + 20;
        if(set_freq > 10800) set_freq = 8810;
        rec_set_freq_full(set_freq);

        rec_tune_status(rec_tune_response);
        rssi = rec_tune_response[4];

        if (rssi <= chan.rssi) {
            if (set_freq == chan.freq) chan.rssi = rssi;
            else {
                chan.freq2 = chan.freq;
                chan.rssi2 = chan.rssi;
                chan.freq = set_freq;
                chan.rssi = rssi;
            }
        }
        else if (set_freq == chan.freq) {
            if (rssi <= chan.rssi2) chan.rssi = rssi;
            else {
                temp_freq = chan.freq;
                chan.freq = chan.freq2;
                chan.rssi = chan.rssi2;
                chan.freq2 = temp_freq;
                chan.rssi2 = rssi;
            }
        }
        else if (rssi <= chan.rssi2) {
            chan.freq2 = set_freq;
            chan.rssi2 = rssi;
        }
        else if (set_freq == chan.freq2) {
            chan.rssi2 = rssi;
        }

        //printf("Frequency  : %d\tRSSI: %d\n", set_freq, rssi);
        //printf("Frequency 1: %d\tRSSI: %d\n", chan.freq, chan.rssi);
        //printf("Frequency 2: %d\tRSSI: %d\n\n", chan.freq2, chan.rssi2);

        //reciever and transmitter stuff

        /*int_status = trans_get_int_status();
        printf("int status = %x\n", int_status);
        
        trans_tune_status(trans_tune_response);
        trans_freq = (trans_tune_response[2] << 8) + (trans_tune_response[3]);

        printf("transmitting frequency: %d\n", trans_freq);*/
        
        // rec_set_freq_full(set_freq);

        // int_status = rec_get_int_status();
        // printf("int status = %x\n", int_status);

        // rec_tune_status(rec_tune_response);
        // rec_freq = (rec_tune_response[2] << 8) + (rec_tune_response[3]);
        // rssi = rec_tune_response[4];
        // snr = rec_tune_response[5];
        // mpi = rec_tune_response[6];

        // printf("receiving frequency: %d\n", rec_freq);
        // printf("rssi: %d\n", rssi);
        // printf("snr: %d\n", snr);
        // printf("mpi: %d\n", mpi);

        // sprintf(text, "%d", rssi);
        // disp_text(text, strlen(text), 1);

        if(check_buttons() == CHANGE_FREQ_PIN) {
            trans_set_freq_full(chan.freq);
            len = freq_to_string(chan.freq, text);
            disp_text(text, len, 0);
            temp_freq = chan.freq;
            chan.freq = chan.freq2;
            chan.rssi = chan.rssi2;
            chan.freq2 = temp_freq;
            chan.rssi2 = 70;
            //printf("Frequency 1: %d\tRSSI: %d\n", chan.freq, chan.rssi);
            //printf("Frequency 2: %d\tRSSI: %d\n\n", chan.freq2, chan.rssi2);
        }
    }
    
    /* De-Initialize */
    i2c_driver_delete(I2C_MASTER_NUM);
    printf("I2C de-initialized successfully");
}
