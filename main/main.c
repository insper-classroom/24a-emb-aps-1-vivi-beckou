

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h" 

// definindo onde estao os botoes e leds

const int LED_Y = 4; 
const int LED_R = 21;
const int LED_B = 20;
const int LED_G = 2;

const int BUTTON_Y= 18;
const int BUTTON_R = 19;
const int BUTTON_B = 16;
const int BUTTON_G = 17;

const int BUZZ = 3;

const int GREEN_FREQ = 300;
const int RED_FREQ = 400;
const int BLUE_FREQ = 500;
const int YELLOW_FREQ = 600;


int selectedColor = -1;
int lenght = 0;
int colors[100]; // Adjust the size as needed


#define DEBOUNCE_DELAY_MS 200 // Defina o delay de debouncing para 200 milissegundos


// FLAGS DOS BOTOES DAS CORES
int volatile flag_f_Y= 0;
int volatile  flag_f_R = 0;
int volatile flag_f_B = 0;
int volatile  flag_f_G = 0;

void btn_callback(uint gpio, uint32_t events) {
    static uint64_t lastInterruptTime = 0;
    uint64_t currentTime = to_us_since_boot(get_absolute_time());

    // Verifica se o intervalo desde a última interrupção é maior que o delay de debouncing

    if (events == GPIO_IRQ_EDGE_FALL) {
        if ((currentTime - lastInterruptTime) > (DEBOUNCE_DELAY_MS * 1000)) {
            if (gpio == BUTTON_B) flag_f_B = 1;
            else if (gpio == BUTTON_G) flag_f_G = 1;
            else if (gpio == BUTTON_Y) flag_f_Y = 1;
            else if (gpio == BUTTON_R) flag_f_R = 1;

            lastInterruptTime = currentTime; // Atualiza o tempo da última interrupção
        }
    }
}

void sound(int frequencia, int duracao, int pino){
    int periodo = 1000000/frequencia;
    int sleep = periodo/2; 
    for (int i = 0; i < 1000*200/sleep*duracao; i++){
        gpio_put(pino, 1);
        sleep_us(sleep);
        gpio_put(pino, 0);
        sleep_us(sleep);
    }
}

void beepToStart() {
    for (int i = 0; i < 5; i++) {
        gpio_put(LED_G, 1);
        sound(GREEN_FREQ, 1, BUZZ);
        gpio_put(LED_G, 0);
        sleep_us(150000);
    }
    sleep_us(1000000);
}

// int buildSequence(int colors[], int length) {
//     int* leds[4] = {LED_B, LED_G, LED_R, LED_Y}; // Array dos LEDs
//     int random = leds[rand() % 4]; // Gera um índice aleatório entre 0 e 3
//     colors[length] = random; // Adiciona o pino do LED correspondente à sequência
//     printf('cores: %ld', colors);
//     return colors;
// }

void addColor(){

    int leds[4] = {LED_B, LED_G, LED_R, LED_Y}; // Array dos LEDs
    int randomIndex = rand() % 4; // Gera um índice aleatório entre 0 e 3
    colors[lenght++] = leds[randomIndex]; // Adiciona o pino do LED correspondente à sequência

}

void playSequence() {
    for (int i = 0; i < lenght; i++) {
        if (colors[i] == LED_B) {
            gpio_put(LED_B, 1);
            sound(BLUE_FREQ, 1, BUZZ);
            gpio_put(LED_B, 0);
            flag_f_B = 1;
            break;
        } else if (colors[i] == LED_G) {
            gpio_put(LED_G, 1);
            sound(GREEN_FREQ, 1, BUZZ);
            gpio_put(LED_G, 0);
            flag_f_G = 1;
            break;
        } else if (colors[i] == LED_R) {
            gpio_put(LED_R, 1);
            sound(RED_FREQ, 1, BUZZ);
            gpio_put(LED_R, 0);
            flag_f_R = 1;
            break;

        } else if (colors[i] == LED_Y) {
            gpio_put(LED_Y, 1);
            sound(YELLOW_FREQ, 1, BUZZ);
            gpio_put(LED_Y, 0);
            flag_f_Y = 1;
            break;

        }
        sleep_us(3000);
    }
}

void setup(){

    stdio_init_all();

    gpio_init(BUTTON_B);
    gpio_init(BUTTON_G);
    gpio_init(BUTTON_R);
    gpio_init(BUTTON_Y);

    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_set_dir(BUTTON_G, GPIO_IN);
    gpio_set_dir(BUTTON_R, GPIO_IN);
    gpio_set_dir(BUTTON_Y, GPIO_IN);

    gpio_init(LED_B);
    gpio_init(LED_G);
    gpio_init(LED_R);
    gpio_init(LED_Y);

    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_Y, GPIO_OUT);

    // setando o PINO do BUZZ 
    gpio_init(BUZZ);
    gpio_set_dir(BUZZ, GPIO_OUT);


    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    // callback led g (nao usar _with_callback)
    gpio_set_irq_enabled(BUTTON_R, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BUTTON_G, GPIO_IRQ_EDGE_FALL, true); 
    gpio_set_irq_enabled(BUTTON_Y, GPIO_IRQ_EDGE_FALL, true);
}

int main() {
    setup();

    beepToStart();
    addColor();
    while (1) {

        lenght++;
        addColor();
        
        if (flag_f_B) {
            sound(BLUE_FREQ, 300, LED_B);
            flag_f_B = 0;
        }
        if (flag_f_G) {
            sound(GREEN_FREQ, 300, LED_G);
            flag_f_G = 0;
        }
        if (flag_f_Y) {
            sound(YELLOW_FREQ, 300, LED_Y);
            flag_f_Y = 0;
        }
        if (flag_f_R) {
            sound(RED_FREQ, 300, LED_R);
            flag_f_R = 0;
        }
    }

    return 0;
}