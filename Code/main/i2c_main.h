#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

#define I2C_MASTER_SCL_IO           9                          /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           10                         /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0                          /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          300000                     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0                          /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

/**
 * @brief Read a sequence of bytes from the I2C bus from device address and register address 
 * 
 * @param dev_addr Address of the I2C slave device to read from
 * @param reg_addr Address of the register to read from on the slave device
 * @param data Buffer to hold read data
 * @param len Number of bytes in the data array
 */
void i2c_register_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Read a sequence of bytes from the I2C bus from device address only
 * @param dev_addr Address of the I2C slave device to read from
 * @param data Buffer to hold read data
 * @param len Number of bytes in the data array
 */
void i2c_address_only_read(uint8_t dev_addr, uint8_t *data, size_t len);

/**
 * @brief Write a sequence of bytes to the I2C bus
 * 
 * @param dev_addr Address of the I2C slave device to write to
 * @param reg_addr Address of the register to write to on the slave device
 * @param data Array of bytes to send on the bus
 * @param len Number of bytes in the data array
 */
void i2c_register_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Write a single byte to the I2C bus
 * 
 * @param dev_addr Address of the I2C slave device to write to
 * @param data Array of bytes to send on the bus
 */
void i2c_write_byte(uint8_t dev_addr, uint8_t data);

/**
 * @brief i2c master initialization
 */
void i2c_init(void);