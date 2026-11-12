#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "oled.h"
#include "light_sensor.h"

static const char *TAG = "SMART_CLOCK";

// Stavke menija
const char *menu_titles[] = {
    "Vreme", 
    "Temperatura", 
    "Vlaznost", 
    "Osvetljenje", 
    "CO2"
};
int current_index = 0;
const int total_items = 5;

// Definisanje pinova
#define LIGHT_SENSOR_PIN GPIO_NUM_4 
#define LED_BLUE         GPIO_NUM_12
#define LED_RED          GPIO_NUM_13 
#define LED_GREEN        GPIO_NUM_14
#define BUZZER_PIN       GPIO_NUM_2 

// Pragovi za alarme
#define TEMP_THRESHOLD   25.0
#define HUMID_THRESHOLD  70
#define CO2_THRESHOLD    1000

void app_main(void) {
    i2c_master_dev_handle_t oled_dev;
    
    oled_init(&oled_dev);
    oled_clear(oled_dev);
    light_sensor_init();

    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL<<LED_BLUE) | (1ULL<<LED_RED) | (1ULL<<LED_GREEN) | (1ULL<<BUZZER_PIN),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0, 
        .pull_down_en = 0, 
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    oled_display_text(oled_dev, 0, "SMART SAT");

    while (1) {
    
        int enc = oled_read_encoder();
        if (enc != 0) {
            current_index = (current_index + enc + total_items) % total_items;
            ESP_LOGI(TAG, "Izabran meni: %s", menu_titles[current_index]);
        }

        float current_temp = 24.5;
        int current_humid = 55;
        int current_co2 = 650;
        int light_val = light_sensor_read(); 

        char buffer[30];
        oled_display_text(oled_dev, 1, menu_titles[current_index]); // Naslov opcije

        if (current_index == 0) {
            snprintf(buffer, sizeof(buffer), "12:30:45       ");
        } else if (current_index == 1) {
            snprintf(buffer, sizeof(buffer), "Temp: %.1f C   ", current_temp);
        } else if (current_index == 2) {
            snprintf(buffer, sizeof(buffer), "Vlaznost: %d%%  ", current_humid);
        } else if (current_index == 3) {
            if (light_val == 1) snprintf(buffer, sizeof(buffer), "Svetlo: Visoko ");
            else snprintf(buffer, sizeof(buffer), "Svetlo: Normal ");
        } else if (current_index == 4) {
            snprintf(buffer, sizeof(buffer), "CO2: %d ppm   ", current_co2);
        }
        oled_display_text(oled_dev, 3, buffer);

        if (light_val == 1) {
            // Veoma osvetljeno -> Plava dioda
            gpio_set_level(LED_BLUE, 1);
            gpio_set_level(LED_RED, 0);
            gpio_set_level(LED_GREEN, 0);
        } else {
           
            gpio_set_level(LED_BLUE, 0);
            gpio_set_level(LED_RED, 0);
            gpio_set_level(LED_GREEN, 1); 
        }

        if (current_temp > TEMP_THRESHOLD || current_humid > HUMID_THRESHOLD || current_co2 > CO2_THRESHOLD) {
            gpio_set_level(BUZZER_PIN, 1); 
            oled_display_text(oled_dev, 6, "ALARM: VISOK PRAG!");
            vTaskDelay(pdMS_TO_TICKS(3000));
            gpio_set_level(BUZZER_PIN, 0);
            oled_display_text(oled_dev, 6, "                   "); // Brisanje poruke
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}