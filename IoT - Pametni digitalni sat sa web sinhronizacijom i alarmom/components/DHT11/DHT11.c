#include "DHT11.h"
#include "rom/ets_sys.h"
#include "esp_timer.h"
#include "esp_log.h"

static const char *TAG = "DHT11";

static int wait_for_state(gpio_num_t gpio, int state, int timeout_us) {
    int us = 0;
    while (gpio_get_level(gpio) != state) {
        if (us >= timeout_us) return -1;
        ets_delay_us(1);
        us++;
    }
    return us;
}

void dht11_init(dht11_t *dev, gpio_num_t gpio_num) {
    dev->gpio_num = gpio_num;
    gpio_reset_pin(dev->gpio_num);
}

esp_err_t dht11_read(dht11_t *dev, int *humidity, int *temperature) {
    uint8_t data[5] = {0};

    // Slati START signal
    gpio_set_direction(dev->gpio_num, GPIO_MODE_OUTPUT);
    gpio_set_level(dev->gpio_num, 0);
    ets_delay_us(20000); // Drzimo LOW bar 18ms
    gpio_set_level(dev->gpio_num, 1);
    ets_delay_us(30);

    // Prebaciti pin na ulaz sa Pull-up
    gpio_set_direction(dev->gpio_num, GPIO_MODE_INPUT);
    gpio_set_pull_mode(dev->gpio_num, GPIO_PULLUP_ONLY);

    // Cekaj odziv senzora (LOW pa HIGH)
    if (wait_for_state(dev->gpio_num, 0, 100) < 0) return ESP_ERR_TIMEOUT;
    if (wait_for_state(dev->gpio_num, 1, 100) < 0) return ESP_ERR_TIMEOUT;
    if (wait_for_state(dev->gpio_num, 0, 100) < 0) return ESP_ERR_TIMEOUT;

    // Citaj 40 bitova (5 bajtova)
    for (int i = 0; i < 40; i++) {
        if (wait_for_state(dev->gpio_num, 1, 70) < 0) return ESP_ERR_TIMEOUT;

        int duration = wait_for_state(dev->gpio_num, 0, 100);
        if (duration < 0) return ESP_ERR_TIMEOUT;

        // Ako je HIGH trajao duze od 40us, bit je 1
        if (duration > 40) {
            data[i / 8] |= (1 << (7 - (i % 8)));
        }
    }

    // Provera Checksum-a
    if (data[4] != ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
        ESP_LOGE(TAG, "Greška u Checksum-u!");
        return ESP_ERR_INVALID_CRC;
    }

    *humidity = data[0];
    *temperature = data[2];

    return ESP_OK;
}

