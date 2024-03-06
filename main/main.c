

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h" 
#include "math.h"

// definindo onde estao os botoes e leds

const int LED_Y = 4; 
const int LED_R = 20;
const int LED_B = 21;
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
int lenght = -1;
int colors[100]; // Adjust the size as neede
volatile int randomIndex = 0; 


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
        if (gpio == BUTTON_B){
            flag_f_B = 1;
            printf("apertou azul");
        }
        else if (gpio == BUTTON_G){
            flag_f_G = 1;
            printf("apertou verde");
        }
        else if (gpio == BUTTON_Y){ 
            flag_f_Y = 1;
            printf("apertou amarelo");
        }
        else if (gpio == BUTTON_R) {
            flag_f_R = 1;
            printf("apertou vermelho");
        }

        lastInterruptTime = currentTime; // Atualiza o tempo da última interrupção
    }
    }
}

void sound(int freq, int tempo, int pin_b){
  int periodo = 1000000/freq; 
  for (int i = 0; i < (tempo*1000/periodo); i++){
    gpio_put(pin_b, 1);
    sleep_us(periodo/2);
    gpio_put(pin_b, 0);
    sleep_us(periodo/2); 
  } 
}

void beepToStart() {
    for (int i = 0; i < 5; i++) {
        gpio_put(LED_G, 1);
        sound(GREEN_FREQ, 900, BUZZ);
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
    randomIndex = rand() % 4; // Gera um índice aleatório entre 0 e 3
    // printf('cor = %lf', randomIndex); 
    lenght += 1 ; 
    colors[lenght] = leds[randomIndex]; // Adiciona o pino do LED correspondente à sequência
    printf("lenseq = %ld\n", lenght); 
    printf("colors[len] = %ld\n", colors[lenght] ); 

    for (int i = 0; i < lenght; i++) {
        printf("Elemento %d: %d", i, colors[i]);
    }
}

void playSequence() {
    for (int i = 0; i <= lenght; i++) {
        if (colors[i] == LED_B) {
            gpio_put(LED_B, 1);
            sound(BLUE_FREQ, 2000, BUZZ);
            gpio_put(LED_B, 0);
            // flag_f_B = 1;
        } else if (colors[i] == LED_G) {
            gpio_put(LED_G, 1);
            sound(GREEN_FREQ, 2000, BUZZ);
            gpio_put(LED_G, 0);
            // flag_f_G = 1;
        } else if (colors[i] == LED_R) {
            gpio_put(LED_R, 1);
            sound(RED_FREQ, 2000, BUZZ);
            gpio_put(LED_R, 0);
            // flag_f_R = 1;

        } else if (colors[i] == LED_Y) {
            gpio_put(LED_Y, 1);
            sound(YELLOW_FREQ, 2000, BUZZ);
            gpio_put(LED_Y, 0);
            // flag_f_Y = 1;

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

    gpio_pull_up(BUTTON_B);
    gpio_pull_up(BUTTON_R);
    gpio_pull_up(BUTTON_Y);
    gpio_pull_up(BUTTON_G);

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

// Função para encerrar o jogo
void endGame() {
    gpio_put(LED_R, 1);
    sound(RED_FREQ, 3000, BUZZ);  // Toca o som por 3 segundos
    gpio_put(LED_R, 0);
    sleep_ms(1000);  // Adicione um atraso antes de reiniciar o jogo ou realizar outras ações
}


int main() {
    setup();

    beepToStart();
    while (1) {

        // add nova cor na seq
        printf("add nova cor na seq"); 
        addColor();

        // tocar a sequencia
        printf("tocar seq"); 
        playSequence(); 

        // espera inputs
        for (int i = 0; i <= lenght; i++) {
            // Verifica se o botão correspondente foi pressionado
            while (1) {
                printf("color[i] = %ld", colors[i]); 
                if (flag_f_B) {
                    if (LED_B == colors[i]) {
                        gpio_put(LED_B, 1);
                        sound(BLUE_FREQ, 300, BUZZ);
                        gpio_put(LED_B, 0);
                        flag_f_B = 0;
                        break;  
                    } else {
                        endGame();
                        return 0;  
                    }
                } else if (flag_f_G) {
                    if (LED_G == colors[i]) {
                        gpio_put(LED_G, 1);
                        sound(GREEN_FREQ, 300, BUZZ);
                        gpio_put(LED_G, 0);
                        flag_f_G = 0;
                        break;  
                    } else {
                        endGame();
                        return 0;  
                    }
                } else if (flag_f_Y) {
                    if (LED_Y == colors[i]) {
                        gpio_put(LED_Y, 1);
                        sound(YELLOW_FREQ, 300, BUZZ);
                        gpio_put(LED_Y, 0);
                        flag_f_Y = 0;
                        break;  
                    } else {
                        endGame();
                        return 0;   
                    }
                } else if (flag_f_R) {
                    if (LED_R == colors[i]) {
                        gpio_put(LED_R, 1);
                        sound(RED_FREQ, 300, BUZZ);
                        gpio_put(LED_R, 0);
                        flag_f_R = 0;
                        break;  
                    } else {
                        endGame();
                        return 0;    
                    }
                }
            }

            sleep_ms(500);  // Adicione um atraso entre os LEDs e sons
        }
    }

    return 0;
}