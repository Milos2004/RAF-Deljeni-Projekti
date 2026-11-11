#ifndef DIODE_H
#define DIODE_H

#include "driver/gpio.h"
#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RGB_COMMON_ANODE,   
    RGB_COMMON_CATHODE 
} rgb_common_type_t;

typedef struct {
    gpio_num_t red_pin;
    gpio_num_t green_pin;
    gpio_num_t blue_pin;
    rgb_common_type_t common_type;
} diode_config_t;

typedef struct {
    gpio_num_t red_pin;
    gpio_num_t green_pin;
    gpio_num_t blue_pin;
    rgb_common_type_t common_type;
} diode_handle_t;

esp_err_t diode_init(const diode_config_t *config, diode_handle_t *handle);

esp_err_t diode_set_color(diode_handle_t *handle, bool red, bool green, bool blue);

esp_err_t diode_off(diode_handle_t *handle);

#ifdef __cplusplus
}
#endif

#endif
