/* Copyright 2022, Laboratorio de Microprocesadores 
 * Facultad de Ciencias Exactas y Tecnología 
 * Universidad Nacional de Tucuman
 * http://www.microprocesadores.unt.edu.ar/
 * Copyright 2022, Esteban Volentini <evolentini@herrera.unt.edu.ar>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** @file digital.c
 **
 ** @brief Plantilla de archivos fuente
 **
 ** Plantilla para los archivos de codigo fuente de prácticos de las 
 ** asignaturas Diseño Integrado de Sistemas Emebebidos y Sistemas Embebidos
 ** de Tiempo Real dictadas en de la Especialización en Integración de
 ** Sistemas Informaticos de la Univesidad Nacional de Tucumán
 ** 
 ** | RV | YYYY.MM.DD | Autor      | Descripción de los cambios              |
 ** |----|------------|-------------|-----------------------------------------|
 ** |  1 | 2022.08.27 | evolentini  | Version inicial del archivo             |
 ** 
 ** @defgroup plantilla Plantilals de Archivos
 ** @brief Plantillas de archivos normalizadas
 ** @{ 
 */

/* === Inclusiones de cabeceras ============================================ */
#include <digital.h>
#include <chip.h>

/* === Definicion y Macros privados ======================================== */
#ifndef OUTPUT_INSTANCE
 #define OUTPUT_INSTANCE 4
#endif

#ifndef INPUT_INSTANCE
 #define INPUT_INSTANCE 5
#endif

/* === Declaraciones de tipos de datos privados ============================ */


/* === Definiciones de variables privadas ================================== */


struct digital_output_s{
    uint8_t gpio;
    uint8_t bit;
    bool allocated;
};

struct digital_input_s{
    uint8_t port;
    uint8_t pin;
    bool allocated;
    bool last_state;
};


/* === Definiciones de variables publicas ================================== */

bool DigitalInputGetState(digital_input_t input);
bool DigitalInputHasChanged(digital_input_t input);
bool DigitalInputHasAvtivated(digital_input_t input);
bool DigitalInputHasDeactivated(digital_input_t input);

bool current_state, last_state = false;

/* === Declaraciones de funciones privadas ================================= */

static struct digital_output_s instance_output[OUTPUT_INSTANCE] = {0};
static struct digital_input_s instance_input[INPUT_INSTANCE] = {0};



/* === Definiciones de funciones privadas ================================== */

digital_output_t DigitalOutputAllocated(void){
    digital_output_t output = NULL; 

    for(int index = 0; index < OUTPUT_INSTANCE; index++){
        if (instance_output[index].allocated == false) {
            instance_output[index].allocated = true;
            output = &instance_output[index];
            break;
        }        
    }
    return output;
}

digital_input_t DigitalInputAllocated(void){
    digital_input_t input = NULL; 

    for(int index = 0; index < INPUT_INSTANCE; index++){
        if (instance_input[index].allocated == false) {
            instance_input[index].allocated = true;
            input = &instance_input[index];
            break;
        }        
    }
    return input;
}



/* === Definiciones de funciones publicas ================================== */
digital_input_t DigitalInputCreate(uint8_t port, uint8_t pin, bool inverted){
    digital_input_t input = DigitalInputAllocated();
    
    if (input) {
        input->port = port;
        input->pin = pin;
        Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, input->port, input->pin);
    }

    return input;
}

bool DigitalInputGetState(digital_input_t input){
    return input->allocated ^ Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, input->port, input->pin);
}

bool DigitalInputHasChanged(digital_input_t input){
    bool state = DigitalInputGetState(input);
    bool result = state != input->last_state;
    input->last_state = state;
    return result;
}

bool DigitalInputHasActivated(digital_input_t input){
    bool state = DigitalInputGetState(input);
    bool result = state && !input->last_state;
    input->last_state = state;
    return result;
}

bool DigitalInputHasDeactivated(digital_input_t input){
    bool state = DigitalInputGetState(input);
    bool result = !state && !input->last_state;
    input->last_state = state;
    return result;
}


// OUTPUT -----------------------
digital_output_t DigitalOutputCreate(uint8_t gpio, uint8_t bit){
    digital_output_t output = DigitalOutputAllocated();
    
    if (output) {
        output->gpio = gpio;
        output->bit = bit;
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->gpio, output->bit, false);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, output->gpio, output->bit, true);

    }


    return output;
}
void DigitalOutputActivate(digital_output_t output){
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->gpio, output->bit,true);

}
void DigitalOutputDeactivate(digital_output_t output){
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, output->gpio, output->bit,false);
}
void DigitalOutputToggle(digital_output_t output) {

}

/* === Ciere de documentacion ============================================== */

/** @} Final de la definición del modulo para doxygen */

