#include <stdio.h>
#include "Buzzer.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BUZZER_GPIO GPIO_NUM_2

static const char *TAG = "BUZZER";

esp_err_t buzzer_init(void)
{
    gpio_config_t config = {
        .pin_bit_mask = (1ULL << BUZZER_GPIO),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    gpio_config(&config);
    gpio_set_level(BUZZER_GPIO, 1); // Početno stanje: Isključeno (1)
    return ESP_OK;
}

esp_err_t buzzer_on(void)
{
    return gpio_set_level(BUZZER_GPIO, 0); // Pali se sa 0 (aktivno niska logika)
}

esp_err_t buzzer_off(void)
{
    return gpio_set_level(BUZZER_GPIO, 1); // Gasi se sa 1
}