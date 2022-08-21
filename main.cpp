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

class L293D_STEPPER {
private: int reductionFactor;
private: uint coilOne[2];
private: uint coilTwo[2];
public: L293D_STEPPER(uint firstCoil[2], uint secondCoil[2], uint gearReduction[2]) {
    for (int i = 0;i < 2;i++) {
        gpio_init(firstCoil[i]);
        gpio_set_dir(firstCoil[i], GPIO_OUT);
        gpio_put(firstCoil[i], 0);
    }
    for (int i = 0;i < 2;i++) {
        gpio_init(secondCoil[i]);
        gpio_set_dir(secondCoil[i], GPIO_OUT);
        gpio_put(secondCoil[i], 0);
    }

    reductionFactor = gearReduction[1] / gearReduction[2];
}
private: void setCoilDirection(char direction, uint coil[2]) {
    if (direction != 'r' && direction != 'f') return;//invalid direction may throw in future;
    if (coil != coilOne && coil != coilTwo) return; // invalid coil set throw event;

    uint coilSequence[2];

    if (direction == 'f') {
        coilSequence[0] = coil[0];
        coilSequence[1] = coil[1];
    }
    if (direction == 'r') {
        coilSequence[0] = coil[1];
        coilSequence[1] = coil[0];
    }

}
};

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
        sleep_ms(2);
        gpio_put(leads[2], 1);
        gpio_put(leads[3], 0);
        sleep_ms(2);
        gpio_put(leads[0], 0);
        gpio_put(leads[1], 1);
        sleep_ms(2);
        gpio_put(leads[2], 0);
        gpio_put(leads[3], 1);
        sleep_ms(2);
    }

    return 0;
}
