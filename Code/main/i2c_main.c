#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "i2c_main.h"

void i2c_register_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len)
{
    /* Write register address and read response of desired length */
    i2c_master_write_read_device(I2C_MASTER_NUM, dev_addr, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
}

void i2c_register_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len)
{
    /* Initialize write buffer with register address followed by data to write */
    uint8_t* write_buf = (uint8_t *) malloc(sizeof(uint8_t) * (len + 1));
    write_buf[0] = reg_addr;
    for(int i = 0; i < len; i++){
        write_buf[i + 1] = data[i];
    }

    /* Write data */
    i2c_master_write_to_device(I2C_MASTER_NUM, dev_addr, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

    /* Release buffer */
    free(write_buf);
}

void i2c_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}