#ifndef OLED_H
#define OLED_H

#include "driver/i2c_master.h"
#include "esp_err.h"

// Definicija I2C pinova (Prilagodi pinove ako koristiš druge umesto GPIO 8 i 9)
#define OLED_SDA_PIN GPIO_NUM_4
#define OLED_SCL_PIN GPIO_NUM_5

void oled_init(i2c_master_dev_handle_t *dev_handle);
void oled_clear(i2c_master_dev_handle_t dev_handle);
void oled_display_text(i2c_master_dev_handle_t dev_handle, int line, const char *text);

#endif // OLED_H