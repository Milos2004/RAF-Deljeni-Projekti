#include "mq7.h"
#include "driver/gpio.h"

#define MQ7_PIN GPIO_NUM_1

void mq7_init(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << MQ7_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

int mq7_read(void) {
    return gpio_get_level(MQ7_PIN);
}