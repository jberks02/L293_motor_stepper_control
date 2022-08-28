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

uint stepper_one_coil_one[2] = { 2,3 };
uint stepper_one_coil_two[2] = { 4,5 };
uint stepper_one_gear_redx[2] = { 1,4 };

class L293D_STEPPER {
    private: int reductionFactor;
    private: uint coilOne[2];
    private: uint coilTwo[2];
    private: float currentAngle = 0.0f;
    private: float stepAngleMultiplier = 1.8;
           //Nema 17 provides 1.8 degrees per step
    public: L293D_STEPPER(uint firstCoil[2], uint secondCoil[2], uint gearReduction[2], float motorAnglePerStep = 1.8) {
        for (int i = 0;i < 2;i++) {
            gpio_init(firstCoil[i]);
            gpio_set_dir(firstCoil[i], GPIO_OUT);
            gpio_put(firstCoil[i], 0);
            coilOne[i] = firstCoil[i];
        }
        for (int i = 0;i < 2;i++) {
            gpio_init(secondCoil[i]);
            gpio_set_dir(secondCoil[i], GPIO_OUT);
            gpio_put(secondCoil[i], 0);
            coilTwo[i] = secondCoil[i];
        }

        reductionFactor = gearReduction[1] / gearReduction[0];

        stepAngleMultiplier = motorAnglePerStep / (gearReduction[1] / gearReduction[0]);

        releaseMotor();
    }
    public: float getCurrentAngle() {
        return currentAngle;
    }
    public: float setCurrentAngle(float angle) {
        currentAngle = angle;
        return currentAngle;
    }
    public: void moveToNewAngle(float newAngle, int speed = 2, bool hold = true) {
        float angleDifference;
        char newDirection;
        int steps;

        if (newAngle > currentAngle) {
            newDirection = 'f';
            angleDifference = newAngle - currentAngle;
        }
        else {
            newDirection = 'r';
            angleDifference = currentAngle - newAngle;
        }

        steps = angleDifference / stepAngleMultiplier;

        currentAngle = newAngle;

        step(newDirection, steps, speed, hold);

    }
    private: void setCoilDirection(char direction, uint coil[2]) {
        // if (direction != 'r' && direction != 'f') return;//invalid direction may throw in future;
        // if (coil != coilOne && coil != coilTwo) return; // invalid coil set throw event;

        uint coilSequence[2];

        if (direction == 'f') {
            coilSequence[0] = coil[0];
            coilSequence[1] = coil[1];
        }
        if (direction == 'r') {
            coilSequence[0] = coil[1];
            coilSequence[1] = coil[0];
        }

        gpio_put(coilSequence[0], 1);
        gpio_put(coilSequence[1], 0);

    }
    public: void releaseMotor() {
        for (int i = 0;i < 2;i++) {
            gpio_put(coilOne[i], 0);
        }
        for (int i = 0;i < 2;i++) {
            gpio_put(coilTwo[i], 0);
        }
    }
    public: void step(char direction, int steps, int speed = 2, bool hold = true) {
        if (speed < 2) return; //can't move shaft faster than 60rpm
        if (direction != 'r' && direction != 'f') return;// direction can only be r or f
        char polarities[2];
        uint coils[2][2];

        if (direction == 'f') {
            polarities[0] = 'f';
            polarities[1] = 'r';
            coils[0][0] = coilOne[0];
            coils[0][1] = coilOne[1];
            coils[1][0] = coilTwo[0];
            coils[1][1] = coilTwo[1];
        }
        if (direction == 'r') {
            polarities[0] = 'r';
            polarities[1] = 'f';
            coils[0][0] = coilTwo[0];
            coils[0][1] = coilTwo[1];
            coils[1][0] = coilOne[0];
            coils[1][1] = coilOne[1];
        }

        while (steps > 0) {
            for (int d = 0; d < 2;d++) {
                for (int c = 0; c < 2; c++) {
                    setCoilDirection(polarities[d], coils[c]);
                    sleep_ms(speed);
                    steps--;
                }
            }
        }


        if (!hold) {
            releaseMotor();
        }

    }
};

int main() {

    stdio_init_all();

    const uint LED = PICO_DEFAULT_LED_PIN;

    gpio_init(LED);

    gpio_set_dir(LED, GPIO_OUT);

    gpio_put(LED, 1);

    uint leads[4] = { 2,3,4,5 };

    L293D_STEPPER stepper_one(stepper_one_coil_one, stepper_one_coil_two, stepper_one_gear_redx);

    while (true) {
        // stepper_one.step('f', 200, 2, false);
        // sleep_ms(500);
        stepper_one.moveToNewAngle(0, 2, false);
        sleep_ms(500);
        stepper_one.moveToNewAngle(90, 2, false);
        sleep_ms(500);
        stepper_one.moveToNewAngle(180, 2, false);
        sleep_ms(500);
        stepper_one.moveToNewAngle(270, 2, false);
        sleep_ms(500);
        stepper_one.moveToNewAngle(360, 2, false);
        sleep_ms(500);
    }

    return 0;
}
