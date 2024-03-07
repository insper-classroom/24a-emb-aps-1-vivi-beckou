

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "math.h"

// definindo onde estao os botoes e leds

const int LED_Y = 4;
const int LED_R = 20;
const int LED_B = 21;
const int LED_G = 2;

const int COR_Y = 4;
const int COR_R = 20;
const int COR_B = 21;
const int COR_G = 2;

const int BUTTON_Y = 18;
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
int btn_colors[100];
volatile int randomIndex = 0;

#define DEBOUNCE_DELAY_MS 200 // Defina o delay de debouncing para 200 milissegundos

// FLAGS DOS BOTOES DAS CORES
int volatile flag_f_Y = 0;
int volatile flag_f_R = 0;
int volatile flag_f_B = 0;
int volatile flag_f_G = 0;

void btn_callback(uint gpio, uint32_t events)
{
    static uint64_t lastInterruptTime = 0;
    uint64_t currentTime = to_us_since_boot(get_absolute_time());

    // Verifica se o intervalo desde a última interrupção é maior que o delay de debouncing

    if (events == GPIO_IRQ_EDGE_FALL)
    {
        if ((currentTime - lastInterruptTime) > (DEBOUNCE_DELAY_MS * 1000))
        {
            if (gpio == BUTTON_B)
            {
                flag_f_B = 1;
                printf("apertou azul");
            }
            else if (gpio == BUTTON_G)
            {
                flag_f_G = 1;
                printf("apertou verde");
            }
            else if (gpio == BUTTON_Y)
            {
                flag_f_Y = 1;
                printf("apertou amarelo");
            }
            else if (gpio == BUTTON_R)
            {
                flag_f_R = 1;
                printf("apertou vermelho");

                lastInterruptTime = currentTime; // Atualiza o tempo da última interrupção
            }
        }
    }
}

void sound(int freq, int tempo, int pin_b)
{
    int periodo = 1000000 / freq;
    for (int i = 0; i < (tempo * 1000 / periodo); i++)
    {
        gpio_put(pin_b, 1);
        sleep_us(periodo / 2);
        gpio_put(pin_b, 0);
        sleep_us(periodo / 2);
    }
}

void beepToStart()
{
    for (int i = 0; i < 3; i++)
    {
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

void addColor()
{

    int leds[4] = {LED_B, LED_G, LED_R, LED_Y}; // Array dos LEDs
    randomIndex = rand() % 4;                   // Gera um índice aleatório entre 0 e 3
    // printf('cor = %lf', randomIndex);
    lenght += 1;
    colors[lenght] = leds[randomIndex]; // Adiciona o pino do LED correspondente à sequência
    // printf("lenseq = %ld\n", lenght);
    // printf("colors[len] = %ld\n", colors[lenght] );
    printf("\nA sequencia");
    for (int i = 0; i < lenght; i++)
    {
        printf("E\nlemento da sequencia %d: %d\n", i, colors[i]);
    }
    if (leds[randomIndex] == 0)
    {
        btn_colors[COR_B];
    }
    else if (leds[randomIndex] == 1)
    {
        btn_colors[COR_G];
    }
    else if (leds[randomIndex] == 2)
    {
        btn_colors[COR_R];
    }
    else if (leds[randomIndex] == 3)
    {
        btn_colors[COR_Y];
    }
}

void playSequence()
{
    printf("Tamanho da sequencia a ser todacada: %d", lenght);
    for (int i = 0; i <= lenght; i++)
    {
        if (colors[i] == LED_B)
        {
            gpio_put(LED_B, 1);
            sound(BLUE_FREQ, 1000, BUZZ);
            gpio_put(LED_B, 0);
            // flag_f_B = 1;
        }
        else if (colors[i] == LED_G)
        {
            gpio_put(LED_G, 1);
            sound(GREEN_FREQ, 1000, BUZZ);
            gpio_put(LED_G, 0);
            // flag_f_G = 1;
        }
        else if (colors[i] == LED_R)
        {
            gpio_put(LED_R, 1);
            sound(RED_FREQ, 1000, BUZZ);
            gpio_put(LED_R, 0);
            // flag_f_R = 1;
        }
        else if (colors[i] == LED_Y)
        {
            gpio_put(LED_Y, 1);
            sound(YELLOW_FREQ, 1000, BUZZ);
            gpio_put(LED_Y, 0);
            // flag_f_Y = 1;
        }
        sleep_us(3000);
    }
}

void setup()
{

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
void endGame()
{
    printf("ERROU SEQUENCIA - end game");
    gpio_put(LED_R, 1);
    sound(RED_FREQ, 5000, BUZZ); // Toca o som por 3 segundos
    gpio_put(LED_R, 0);
    sleep_ms(1000); // Adicione um atraso antes de reiniciar o jogo ou realizar outras ações
}

int main()
{
    setup();

    beepToStart();

    while (1)
    {

        // add nova cor na seq
        // printf("add nova cor na seq");
        addColor();

        // tocar a sequencia
        // printf("tocar seq");
        playSequence();

        // Supondo que a sequência 'colors' já foi gerada e tocada
        int userIndex = 0; // Índice para acompanhar qual cor na sequência o usuário está tentando acertar

        while (userIndex <= lenght)
        {
            // Verifica se algum botão foi pressionado

            int expectedColor = colors[userIndex]; // Cor esperada da sequência
            int pressedColor;

            // Identifica qual botão foi pressionado
            if (flag_f_B)
            {
                pressedColor = COR_B;

                printf("\n Expected color = %ld", expectedColor);
                printf("\n Pressed color = %ld", pressedColor);

                if (pressedColor == COR_B && expectedColor == LED_B)
                {

                    gpio_put(LED_B, 1);
                    sound(BLUE_FREQ, 300, BUZZ);
                    flag_f_B = 0;
                    gpio_put(LED_B, 0);
                }
                else
                {
                    // gpio_put(LED_R, 1);
                    endGame();
                }
            }
            else if (flag_f_G)
            {
                pressedColor = COR_G;
                printf("\n Expected color = %ld", expectedColor);
                printf("\n Pressed color = %ld", pressedColor);
                if (pressedColor == COR_G && expectedColor == LED_G)
                {

                    gpio_put(LED_G, 1);
                    sound(GREEN_FREQ, 300, BUZZ);
                    flag_f_G = 0;
                    gpio_put(LED_G, 0);
                }
                else
                {
                    endGame();
                }
            }
            else if (flag_f_Y)
            {

                pressedColor = COR_Y;
                printf("\n Expected color = %ld", expectedColor);
                printf("\n Pressed color = %ld", pressedColor);

                if (pressedColor == COR_Y && (expectedColor == COR_Y))
                {
                    pressedColor = COR_Y;
                    gpio_put(LED_Y, 1);
                    sound(YELLOW_FREQ, 300, BUZZ);
                    flag_f_Y = 0;
                    gpio_put(LED_Y, 0);
                }
                else
                {
                    // gpio_put(LED_R, 1);
                    endGame();
                }
            }
            else if (flag_f_R)
            {
                printf("\n Expected color = %ld", expectedColor);
                printf("\n Pressed color = %ld", pressedColor);

                pressedColor = COR_R;
                if (pressedColor == COR_R && (expectedColor == COR_R))
                {
                    pressedColor = COR_R;
                    gpio_put(LED_R, 1);
                    sound(RED_FREQ, 300, BUZZ);
                    flag_f_R = 0;
                    gpio_put(LED_R, 0);
                }
                else
                {
                    // gpio_put(LED_R, 1);
                    endGame();
                }
            }
            else
            {
                // Se nenhuma flag estiver levantada, continua o loop
                continue;
            }

            // Compara a cor pressionada com a esperada na sequência
            if (pressedColor == expectedColor)
            {
                userIndex++; // Avança para a próxima cor na sequência se acertar
                // Opcional: Feedback visual/sonoro de sucesso
            }
            else
            {

                printf("jogo terminado"); // Termina o jogo se a cor pressionada não corresponder
                return 0;
            }
        }

        sleep_ms(10); // Pequena pausa para não sobrecarregar o loop
    }
}


// Se sair do loop, o usuário acertou toda a sequência
// Aqui, pode-se adicionar uma nova cor à sequência e repetir o processo

// int userIndex = 0;
// // espera inputs
// //for (int i = 0; i <= lenght; i++) {
//     // Verifica se o botão correspondente foi pressionado
//     while (userIndex <= lenght) {

//         for (int i = 0; i <= lenght; i++){
//                 printf("color[i] = %ld", colors[userIndex]);
//             if (flag_f_B) {
//                 if (btn_colors[i] == colors[userIndex]) {
//                     gpio_put(LED_B, 1);
//                     sound(BLUE_FREQ, 300, BUZZ);
//                     flag_f_B = 0;
//                     gpio_put(LED_B, 0);
//                     userIndex++;

//                 } else {
//                     endGame();
//                     return 0;
//                 }
//             } else if (flag_f_G) {
//                 if (btn_colors[i] == colors[userIndex]) {
//                     gpio_put(LED_G, 1);
//                     sound(GREEN_FREQ, 300, BUZZ);
//                     flag_f_G = 0;
//                     gpio_put(LED_G, 0);
//                     userIndex++;

//                 } else {
//                     endGame();
//                     return 0;
//                 }
//             } else if (flag_f_Y) {
//                 if (btn_colors[i] == colors[userIndex]) {
//                     gpio_put(LED_Y, 1);
//                     sound(YELLOW_FREQ, 300, BUZZ);
//                     flag_f_Y = 0;
//                     gpio_put(LED_Y, 0);
//                     userIndex++;

//                 } else {
//                     endGame();
//                     return 0;
//                 }
//             } else if (flag_f_R) {
//                 if (btn_colors[i] == colors[userIndex]) {
//                     gpio_put(LED_R, 1);
//                     sound(RED_FREQ, 300, BUZZ);
//                     flag_f_R = 0;
//                     gpio_put(LED_R, 0);
//                     userIndex++;

//                 } else {
//                     endGame();
//                     return 0;
//                 }
//             }
//     }

//         }

//     sleep_ms(500);  // Adicione um atraso entre os LEDs e sons
// }
