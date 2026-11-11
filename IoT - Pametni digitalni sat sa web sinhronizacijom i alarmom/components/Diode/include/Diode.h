#ifndef DIODE_H
#define DIODE_H

#include "driver/gpio.h"
#include "esp_err.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Tip zajedničkog pina RGB diode
typedef enum {
    RGB_COMMON_ANODE,   // Zajednički vod je na 3.3V (pali se sa LOW / 0)
    RGB_COMMON_CATHODE  // Zajednički vod je na GND (pali se sa HIGH / 1)
} rgb_common_type_t;

// Konfiguraciona struktura
typedef struct {
    gpio_num_t red_pin;
    gpio_num_t green_pin;
    gpio_num_t blue_pin;
    rgb_common_type_t common_type;
} diode_config_t;

// Instanca RGB diode
typedef struct {
    gpio_num_t red_pin;
    gpio_num_t green_pin;
    gpio_num_t blue_pin;
    rgb_common_type_t common_type;
} diode_handle_t;

/**
 * @brief Inicijalizacija Diode
 */
esp_err_t diode_init(const diode_config_t *config, diode_handle_t *handle);

/**
 * @brief Postavljanje stanja pojedinačnih boja (true/false)
 */
esp_err_t diode_set_color(diode_handle_t *handle, bool red, bool green, bool blue);

/**
 * @brief Brzo isključivanje svih boja
 */
esp_err_t diode_off(diode_handle_t *handle);

#ifdef __cplusplus
}
#endif

#endif // DIODE_H
