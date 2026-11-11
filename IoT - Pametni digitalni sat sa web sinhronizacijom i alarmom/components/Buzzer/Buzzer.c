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

    esp_err_t err = gpio_config(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Greska pri konfiguraciji GPIO: %s", esp_err_to_name(err));
        return err;
    }

    gpio_set_level(BUZZER_GPIO, 1);
    ESP_LOGI(TAG, "Buzzer inicijalizovan na GPIO1");
    return ESP_OK;
}

esp_err_t buzzer_on(void)
{
    ESP_LOGI(TAG, "Ukljucujem buzzer");
    return gpio_set_level(BUZZER_GPIO, 0); 
}

esp_err_t buzzer_off(void)
{
    ESP_LOGI(TAG, "Iskljucujem buzzer");
    return gpio_set_level(BUZZER_GPIO, 1);
}
