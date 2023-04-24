#include "fm.h"

static uint8_t chan_counter = 0;
static uint8_t sum_counter = 0;
static struct Chan chans[NUM_CHANS];
static uint16_t rssi_thresh = RSSI_THRESH * SUM_LEN;
static uint8_t rec_tune_response[8] = {};
static uint8_t rssi;
static uint8_t len;
static char text[20];

void rclk_init(void) {
    // PWM
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .duty_resolution = LEDC_TIMER_4_BIT,
        .freq_hz = RCLK_FREQ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = RCLK_PIN,
        .duty = 7, // Set duty to 50%
        .hpoint = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void fm_init(void) {
    uint16_t set_freq = 0;
    uint8_t set_rssi = 255;
    uint16_t freq;

    /* Initialize the receiver */
    rec_init();

    /* Step through each frequency, find the rssi, and determine the best initial channel */
    for (int i = 0; i < NUM_CHANS; i++) {
        freq = START_FREQ + (i * FREQ_STEP);
        chans[i].freq = freq;
        rec_tune_freq(chans[i].freq);
        rec_rsq_status(rec_tune_response);
        rssi = rec_tune_response[RSSI];
        chans[i].rssi[sum_counter] = rssi;
        chans[i].rssi_sum = rssi;

        if (rssi <= set_rssi) {
            set_freq = freq;
            set_rssi = rssi;
        }
    }

    /* Update the average counter since all channels have been checked */
    sum_counter++;

    /* Initialize the transmitter, set the best frequency, and update the display */
    trans_init();
    trans_tune_freq(set_freq);
    len = freq_to_string(set_freq, text);
    disp_text(text, len, 0);

    for (int i = 0; i < NUM_CHANS; i++) {
        printf("Frequency:  %d\tRSSI: %d\n", chans[i].freq, chans[i].rssi[0]);
    }
    printf("Set Frequency:  %d\n", set_freq);
}

void step_freq(void) {
    /* Set receiver to next channel and record rssi */
    rec_tune_freq(chans[chan_counter].freq);
    rec_rsq_status(rec_tune_response);
    rssi = rec_tune_response[RSSI];
    printf("Frequency:  %d\tRSSI: %d\n", chans[chan_counter].freq, rssi);

    /* Update rolling sum and record rssi to channel */
    chans[chan_counter].rssi_sum = chans[chan_counter].rssi_sum - chans[chan_counter].rssi[sum_counter] + rssi;
    chans[chan_counter].rssi[sum_counter] = rssi;

    /* Increment the channel counter. If channel counter rolls over update the average counter */
    chan_counter++;
    if (chan_counter >= NUM_CHANS) {
        chan_counter = 0;
        sum_counter++;
        if (sum_counter >= SUM_LEN) sum_counter = 0;
    }
}

void change_freq(void) {
    uint16_t set_freq = 0;
    uint8_t mins[50] = {};

    /* Reset rssi threshold if it had previously been raised */
    rssi_thresh = RSSI_THRESH * SUM_LEN;

    /* Find the best possible channel */
    uint8_t num_mins = find_mins(chans, mins);

    /* If no possible channels were found raise threshold and try again */
    while (num_mins == 0) {
        rssi_thresh += 2;
        num_mins = find_mins(chans, mins);
    }

    /* Search through possible channels and select one with lowest rssi and with neighbors below threshold */
    for (int i = 0; i < num_mins; i++) {
        if (mins[i] == 0) {
            if (chans[mins[i + 1]].rssi_sum < rssi_thresh) {
                set_freq = chans[mins[i]].freq;
                break;
            }
        }
        else if (mins[i] == (NUM_CHANS - 1)) {
            if (chans[mins[i - 1]].rssi_sum < rssi_thresh) {
                set_freq = chans[mins[i]].freq;
                break;
            }
        }
        else if ((chans[mins[i - 1]].rssi_sum < rssi_thresh) && (chans[mins[i + 1]].rssi_sum < rssi_thresh)) {
            set_freq = chans[mins[i]].freq;
            break;
        }
    }

    /* If no channel selected choose one with lowest rssi */
    if (set_freq == 0) {
        set_freq = chans[mins[0]].freq;
    }

    /* Tune the transmitter and update display */
    trans_tune_freq(set_freq);
    len = freq_to_string(set_freq, text);
    disp_text(text, len, 0);


    for (int i = 0; i < NUM_CHANS; i++) {
        printf("Frequency:  %d\tSUM: %d\tRSSI0: %d\tRSSI1: %d\tRSSI2: %d\tRSSI3: %d\tRSSI4: %d\n", chans[i].freq, chans[i].rssi_sum, chans[i].rssi[0], chans[i].rssi[1], chans[i].rssi[2], chans[i].rssi[3], chans[i].rssi[4]);
    }
    printf("Set Frequency:  %d\n", set_freq);
}

uint8_t find_mins(struct Chan* chans, uint8_t* mins) {
    uint8_t size = 0;
    uint16_t sum;

    /* Search through channels and find all minimums with acceptable rssi */
    for (int i = 0; i < NUM_CHANS; i++) {
        sum = chans[i].rssi_sum;
        if (i == 0) {
            if ((sum <= chans[i + 1].rssi_sum) && (sum < rssi_thresh)) {
                size++;
                mins[size] = i;
            }
        }
        else if (i == (NUM_CHANS - 1)) {
            if ((sum <= chans[i - 1].rssi_sum) && (sum < rssi_thresh)) {
                size++;
                mins[size] = i;
            }
        }
        else {
            if ((sum <= chans[i - 1].rssi_sum) && (sum <= chans[i + 1].rssi_sum) && (sum < rssi_thresh)) {
                size++;
                mins[size] = i;
            }
        }
    }

    bool sort = true;
    uint8_t ind;

    /* Sort indices in order of lowest rssi */
    while (sort) {
        sort = false;
        for (int i = 0; i < size - 1; i++) {
            sum = chans[mins[i]].rssi_sum;
            if (sum > chans[mins[i + 1]].rssi_sum) {
                sort = true;
                ind = mins[i];
                mins[i] = mins[i + 1];
                mins[i + 1] = ind;
            }
        }
    }

    /* Return number of minimums */
    return size;
}