#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "oled.h"
#include "LightSensor.h"
#include "Buzzer.h"
#include "DHT11.h"

static const char *TAG = "SMART_CLOCK";

const char *menu_titles[] = {
    "Vreme",
    "Temperatura",
    "Vlaznost",
    "Osvetljenje",
    "CO2"
};

int current_index = 0;
const int total_items = 5;

#define LED_BLUE         GPIO_NUM_15
#define LED_RED          GPIO_NUM_6
#define LED_GREEN        GPIO_NUM_7

#define ENC_CLK_PIN      GPIO_NUM_10
#define ENC_DT_PIN       GPIO_NUM_11

#define DHT11_PIN        GPIO_NUM_42

#define TEMP_THRESHOLD   30.0

// Čitanje rotacionog enkodera
int read_rotary_encoder(void) {
    static int last_clk_state = 1;

    int current_clk_state = gpio_get_level(ENC_CLK_PIN);
    int direction = 0;

    if (current_clk_state != last_clk_state &&
        current_clk_state == 0) {

        if (gpio_get_level(ENC_DT_PIN) == 1) {
            direction = 1;
        } else {
            direction = -1;
        }
    }

    last_clk_state = current_clk_state;

    return direction;
}

void app_main(void) {

    i2c_master_dev_handle_t oled_dev;

    // Inicijalizacija drajvera
    oled_init(&oled_dev);
    oled_clear(oled_dev);
    light_sensor_init();
    buzzer_init(); 

    dht11_t dht_dev;
    dht11_init(&dht_dev, DHT11_PIN);

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LED_BLUE) |
                        (1ULL << LED_RED) |
                        (1ULL << LED_GREEN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    gpio_config_t enc_conf = {
        .pin_bit_mask = (1ULL << ENC_CLK_PIN) |
                        (1ULL << ENC_DT_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = 1,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&enc_conf);

    oled_display_text(oled_dev, 0, "SMART SAT");

    int last_index = -1;

    float current_temp = 0.0;
    int current_humid = 0;
    int current_co2 = 650;

    int dht_valid = 0;
    int64_t last_dht_read = 0;

    while (1) {

        // Citanje enkodera
        int enc = read_rotary_encoder();

        if (enc != 0) {
            current_index =
                (current_index + enc + total_items) % total_items;

            ESP_LOGI(TAG, "Izabran meni: %s",
                     menu_titles[current_index]);
        }

        // DHT11
        int64_t now = esp_timer_get_time();

        if (now - last_dht_read >= 3000000) {
            last_dht_read = now;

            int hum = 0, temp = 0;

            if (dht11_read(&dht_dev, &hum, &temp) == ESP_OK) {
                current_temp = (float)temp;
                current_humid = hum;
                dht_valid = 1;

                ESP_LOGI(TAG,
                         "DHT11 -> Temperatura: %.1f C, Vlaznost: %d%%",
                         current_temp, current_humid);
            } else {
                dht_valid = 0;
                ESP_LOGW(TAG, "Neuspesno citanje DHT11 senzora!");
            }
        }

        // Čitanje senzora svetlosti
        int light_val = light_sensor_read();

        
        if (current_index != last_index) {
            oled_clear(oled_dev);
            oled_display_text(oled_dev, 0, "SMART SAT");
            last_index = current_index;
        }

        char buffer[30];
        oled_display_text(oled_dev, 2, menu_titles[current_index]);

       
        if (current_index == 0) {
            snprintf(buffer, sizeof(buffer), "Vreme: 12:30:45");
        } else if (current_index == 1) {
            if (dht_valid) {
                snprintf(buffer, sizeof(buffer), "Temp: %.1f C", current_temp);
            } else {
                snprintf(buffer, sizeof(buffer), "DHT11 ERROR   ");
            }
        } else if (current_index == 2) {
            if (dht_valid) {
                snprintf(buffer, sizeof(buffer), "Vlaznost: %d%%", current_humid);
            } else {
                snprintf(buffer, sizeof(buffer), "DHT11 ERROR   ");
            }
        } else if (current_index == 3) {
            if (light_val == 0) {
                snprintf(buffer, sizeof(buffer), "Svetlo: Visoko ");
            } else {
                snprintf(buffer, sizeof(buffer), "Svetlo: Nisko");
            }
        } else if (current_index == 4) {
            snprintf(buffer, sizeof(buffer), "CO2: %d ppm   ", current_co2);
        }

        oled_display_text(oled_dev, 4, buffer);

        // Kontrola LED dioda
        if (light_val == 0) {
            gpio_set_level(LED_BLUE, 0);
            gpio_set_level(LED_RED, 0);
            gpio_set_level(LED_GREEN, 1);
        } else {
            gpio_set_level(LED_BLUE, 1);
            gpio_set_level(LED_RED, 0);
            gpio_set_level(LED_GREEN, 0);
        }

        if (dht_valid && current_temp >= TEMP_THRESHOLD) {
            buzzer_on();
            ESP_LOGW(TAG, "ALARM! Temperatura: %.1f C", current_temp);
        } else {
            buzzer_off();
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}