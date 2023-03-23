#include <stdio.h>
#include "i2c_main.h"

#define DISP_ADDR       0x3C
#define DISP_RST_PIN    6
#define MAX_CHARACTERS  20

#define GENERIC_REG     0x00
#define Co_REG          0x80
#define DISP_RS         0x40

#define CLEAR_DISPLAY   0x01

#define RETURN_HOME     0x02

#define ENTRY_MODE_SET  0x04
#define DISP_I_D        0x02
#define DISP_S          0x01

#define DISPLAY_ON_OFF  0x08
#define ENTIRE_DISPLAY_ON 0x04
#define CURSOR_ON       0x02
#define CURSOR_POSITION_ON 0x01

#define FUNCTION_SET    0x20
#define INTERFACE_DATA  0x10
#define NUMBER_OF_LINE  0x08
#define DOUBLE_HEIGHT_FONT 0x04
#define INSTRUCTION_TABLE_0 0x00
#define INSTRUCTION_TABLE_1 0x01
#define INSTRUCTION_TABLE_2 0x02

#define SET_DDRAM_ADDRESS 0x80
#define DDRAM_SECOND_ROW_START 0x40

#define CURSOR_OR_DISPLAY_SHIFT 0x10
#define DISP_S_C        0x08
#define DISP_R_L        0x04

#define SET_CGRAM       0x40

#define BIAS_SET        0x14
#define DISP_B_S        0x08
#define DISP_F_X        0x01

#define SET_ICON_ADDRESS 0x40

#define POWER_ICON_CONTROL_CONTRAST_SET 0x50
#define ICON_DISPLAY_ON_OFF 0x08
#define BOOSTER_CIRCUIT_ON_OFF 0x04
#define DISP_C_5        0x02
#define DISP_C_4        0x01

#define FOLLOWER_CONTROL 0x60
#define FOLLOWER_ON_OFF 0x08
#define DISP_RAB_2 0x04
#define DISP_RAB_1 0x02
#define DISP_RAB_0 0x01

#define CONTRAST_SET    0x70

#define DOUBLE_HEIGHT_POSITION_SELECT 0x10
#define DISP_U_D 0x08

/**
 * @brief Write to display registers
 */
void disp_write(uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Read from display registers
 */
void disp_read(uint8_t reg_addr, uint8_t *data, size_t len);

/**
 * @brief Initialize the display
 */
void disp_init(void);

/**
 * @brief Clear the display
 */
void disp_clear(void);

/**
 * @brief Clear and write new text to selected row
 * 
 * @param text String of text to write. 40 characters max
 * @param len Number of characters in string
 * @param row Row to display text in
 */
void disp_text(unsigned char *text, uint8_t len, uint8_t row);