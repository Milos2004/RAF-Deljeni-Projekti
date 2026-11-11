#ifndef DHT11_H
#define DHT11_H

#include "esp_err.h"
#include "driver/gpio.h"

typedef struct {
    gpio_num_t gpio_num;
} dht11_t;

// Inicijalizacija pina
void dht11_init(dht11_t *dev, gpio_num_t gpio_num);

// Citanje temperature i vlaznosti
esp_err_t dht11_read(dht11_t *dev, int *humidity, int *temperature);

#endif // DHT11_H
