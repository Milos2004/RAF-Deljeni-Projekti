#ifndef DHT11_H
#define DHT11_H

#include "esp_err.h"
#include "driver/gpio.h"

typedef struct {
    gpio_num_t gpio_num;
} dht11_t;

void dht11_init(dht11_t *dev, gpio_num_t gpio_num);

esp_err_t dht11_read(dht11_t *dev, int *humidity, int *temperature);

#endif
