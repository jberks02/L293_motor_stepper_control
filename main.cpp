/*
 * Blink for Raspberry Pi Pico
 *
 * @version     1.0.0
 * @author     Justin Berkshire
 * @copyright   2022
 * @licence     MIT
 *
 */
#include "main.h"
using namespace std;

int main() {

    stdio_init_all();

    const uint LED = PICO_DEFAULT_LED_PIN;

    gpio_init(LED);

    gpio_set_dir(LED, GPIO_OUT);

    uint leads[4] = { 2,3,4,5 };

    for (int i = 0; i < 4;i++) {
        gpio_init(leads[i]);
        gpio_set_dir(leads[i], GPIO_OUT);
        // gpio_set_function(leads[i], GPIO_FUNC_GPCK);
        gpio_put(leads[i], 0);
    };

    gpio_put(LED, 1);

    gpio_put(leads[0], 1);
    gpio_put(leads[1], 0);
    sleep_ms(20);
    gpio_put(leads[2], 1);
    gpio_put(leads[3], 0);
    sleep_ms(20);

    while (true) {
        gpio_put(leads[0], 1);
        gpio_put(leads[1], 0);
        sleep_ms(20);
        gpio_put(leads[2], 1);
        gpio_put(leads[3], 0);
        sleep_ms(20);
        gpio_put(leads[0], 0);
        gpio_put(leads[1], 1);
        sleep_ms(20);
        gpio_put(leads[2], 0);
        gpio_put(leads[3], 1);
        sleep_ms(20);
    }

    return 0;
}
