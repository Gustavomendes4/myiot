
#include <stdio.h>


#include "myiot/Gpio.hpp"

#include "driver/gpio.h"

void Gpio::setPinMode(uint8_t pin, Mode mode) {
    if (!GPIO_IS_VALID_GPIO(pin)) {
        return;
    }

    gpio_num_t gpio_num = static_cast<gpio_num_t>(pin);
    gpio_reset_pin(gpio_num);

    switch (mode) {
        case Mode::INPUT:
            gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
            gpio_set_pull_mode(gpio_num, GPIO_FLOATING);
            break;

        case Mode::INPUT_PULLUP:
            gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
            gpio_set_pull_mode(gpio_num, GPIO_PULLUP_ONLY);
            break;

        case Mode::INPUT_PULLDOWN:
            gpio_set_direction(gpio_num, GPIO_MODE_INPUT);
            gpio_set_pull_mode(gpio_num, GPIO_PULLDOWN_ONLY);
            break;

        case Mode::OUTPUT:
            if (GPIO_IS_VALID_OUTPUT_GPIO(gpio_num)) {
                // GPIO_MODE_INPUT_OUTPUT permite ler de volta o estado do pino com readPin()
                gpio_set_direction(gpio_num, GPIO_MODE_INPUT_OUTPUT);
                gpio_set_pull_mode(gpio_num, GPIO_FLOATING);
            }
            break;
    }
}

bool Gpio::readPin(uint8_t pin) {
    if (!GPIO_IS_VALID_GPIO(pin)) {
        return false;
    }

    gpio_num_t gpio_num = static_cast<gpio_num_t>(pin);
    return gpio_get_level(gpio_num) != 0;
}

bool Gpio::writePin(uint8_t pin, bool value) {
    if (!GPIO_IS_VALID_OUTPUT_GPIO(pin)) {
        return false;
    }

    gpio_num_t gpio_num = static_cast<gpio_num_t>(pin);
    esp_err_t err = gpio_set_level(gpio_num, value ? 1 : 0);
    return (err == ESP_OK);
}

void Gpio::begin(){

    /* Define auto update*/
    this->autoUpdate = false;

    /* Initialize INPUTS */
    for(int i = 0; i < Gpio::NUM_OF_INPUTS; i++){

        Gpio::setPinMode( Gpio::input_pins[i], Gpio::Mode::INPUT_PULLUP);

        this->input[i] = false;
    }

    /* Initialize OUTPUTS */
    for(int i = 0; i < Gpio::NUM_OF_OUTPUTS; i++){

        Gpio::setPinMode( Gpio::output_pins[i], Gpio::Mode::OUTPUT);

        writePin(Gpio::output_pins[i], false);

        this->output[i] = false;
    }

    /* Initialize LED*/
    Gpio::setPinMode( Gpio::led_pin, Gpio::Mode::OUTPUT);
    Gpio::writePin(Gpio::led_pin, false);

    /* Initialize BUTTON */
    Gpio::setPinMode( Gpio::btn_pin, Gpio::Mode::INPUT_PULLUP);
    
}

void Gpio::setAutoUpdate(bool value){
    this->autoUpdate = value;
}

bool Gpio::inputUpdate(){

    bool changed = false;

    for(int i = 0; i < Gpio::NUM_OF_INPUTS; i++){

        bool current = !Gpio::readPin( Gpio::input_pins[i] );

        if( input[i] != current){
            input[i] = current;
            changed = true;
        } 
    }
    return changed;
}

bool Gpio::outputUpdate(){

    bool changed = false;

    for(int i = 0; i < Gpio::NUM_OF_OUTPUTS; i++){

        bool current = Gpio::readPin( Gpio::output_pins[i] );

        if( output[i] != current){
            Gpio::writePin(Gpio::output_pins[i], output[i]);
            changed = true;
        }
    }
    return changed;
}

bool Gpio::update(bool read_only_inputs){

    bool in_changed  = inputUpdate();
    bool out_changed = outputUpdate();

    return in_changed || (  out_changed && !read_only_inputs );
}

bool Gpio::read(uint8_t input_num){

    if( input_num >= Gpio::NUM_OF_INPUTS){
        return false;
    }

    if( autoUpdate ){
        bool current = !Gpio::readPin( Gpio::input_pins[input_num] );

        input[input_num] = current;

        return current;
    }

    return input[input_num];
}

bool Gpio::write(uint8_t output_num, bool value){
    
    if( output_num >= Gpio::NUM_OF_OUTPUTS){
        return false;
    }

    output[ output_num ] = value;

    if( autoUpdate ){
        Gpio::writePin(output_pins[ output_num], value);
    }

    return true;
}

bool Gpio::readOutput(uint8_t output_num){
    
    if( output_num >= Gpio::NUM_OF_OUTPUTS){
        return false;
    }
    
    return output[output_num];
    
}

bool Gpio::led(){
    return Gpio::readPin(Gpio::led_pin);
}

bool Gpio::led(bool value){

    Gpio::writePin(Gpio::led_pin, value);

    return Gpio::led();
}

bool Gpio::button(){
    return !Gpio::readPin(Gpio::btn_pin);
}
