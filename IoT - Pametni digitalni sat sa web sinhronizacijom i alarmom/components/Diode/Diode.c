#include "Diode.h"
#include "esp_log.h"

static const char *TAG = "DIODE";

esp_err_t diode_init(const diode_config_t *config, diode_handle_t *handle) {
    if (config == NULL || handle == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    handle->red_pin = config->red_pin;
    handle->green_pin = config->green_pin;
    handle->blue_pin = config->blue_pin;
    handle->common_type = config->common_type;

    uint64_t pin_mask = (1ULL << handle->red_pin) | 
                          (1ULL << handle->green_pin) | 
                          (1ULL << handle->blue_pin);

    gpio_config_t io_conf = {
        .pin_bit_mask = pin_mask,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    
    esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Greška pri konfiguraciji GPIO pinova za Diode komponentu");
        return err;
    }

    // Postavi početno stanje na isključeno
    diode_off(handle);

    ESP_LOGI(TAG, "Diode komponenta uspešno inicijalizovana");
    return ESP_OK;
}

esp_err_t diode_set_color(diode_handle_t *handle, bool red, bool green, bool blue) {
    if (handle == NULL) return ESP_ERR_INVALID_ARG;

    int r_level, g_level, b_level;

    if (handle->common_type == RGB_COMMON_ANODE) {
        r_level = red ? 0 : 1;
        g_level = green ? 0 : 1;
        b_level = blue ? 0 : 1;
    } else {
        r_level = red ? 1 : 0;
        g_level = green ? 1 : 0;
        b_level = blue ? 1 : 0;
    }

    gpio_set_level(handle->red_pin, r_level);
    gpio_set_level(handle->green_pin, g_level);
    gpio_set_level(handle->blue_pin, b_level);

    return ESP_OK;
}

esp_err_t diode_off(diode_handle_t *handle) {
    return diode_set_color(handle, false, false, false);
}