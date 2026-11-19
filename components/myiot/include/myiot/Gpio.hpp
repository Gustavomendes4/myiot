#ifndef MYIOT_GPIO_HPP_INCLUDED
#define MYIOT_GPIO_HPP_INCLUDED

#include <stdint.h>

class Gpio{

    /* Class consts */
    enum class Mode{INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN};

    /* Device definition */
    static constexpr int NUM_OF_INPUTS  = 6;
    static constexpr int NUM_OF_OUTPUTS = 3;

    const int input_pins[Gpio::NUM_OF_INPUTS]  = {13, 14, 27, 26, 25, 33};
    const int output_pins[Gpio::NUM_OF_OUTPUTS] = {15, 23, 22};

    const int led_pin = 2;
    const int btn_pin = 0;

    /* Device variables */
    private:

    bool input[NUM_OF_INPUTS];
    bool output[NUM_OF_OUTPUTS];

    bool autoUpdate;

    /* Hardware interface */
    static void setPinMode(uint8_t pin, Mode mode);

    static bool readPin(uint8_t pin);

    static bool writePin(uint8_t pin, bool value);

    public:

    /* Initialization */
    void begin();

    /* set auto update */
    void setAutoUpdate(bool value);

    /* Update */
    bool inputUpdate();

    bool outputUpdate();

    bool update(bool read_only_inputs = false);

    /* Read / Write */
    bool read(uint8_t input_num);

    bool write(uint8_t output_num, bool value);

    bool readOutput(uint8_t output_num);

    /* LED read and write */
    bool led();

    bool led(bool value);

    /* BUTTON read */
    bool button();


};

#endif // MYIOT_GPIO_HPP_INCLUDED