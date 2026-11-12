#ifndef OLED_H
#define OLED_H

#include "driver/i2c_master.h"
#include "driver/gpio.h"

void oled_init(i2c_master_dev_handle_t *out_dev_handle);
void oled_display_text(i2c_master_dev_handle_t dev, uint8_t page, const char *text);
void oled_clear(i2c_master_dev_handle_t dev);
int oled_read_encoder(void);

#endif