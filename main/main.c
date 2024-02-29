/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h" 

const int LED_Y= 4; 
const int LED_R = 21;
const int LED_B = 20;
const int LED_G= 2;

const int BUTTON_Y= 18;
const int BUTTON_R = 19;
const int BUTTON_B = 16;
const int BUTTON_G = 17;

const int BUZZ = 3;

const int MICRO_TIME = 100000;
const int HALF_TIME = 500000; 

int selectedColor = -1;
int colors[100]; // Adjust the size as needed
int sequenceIndex = 0;

// FLAGS DOS BOTOES DAS CORES
int volatile flag_f_Y= 0;
int volatile  flag_f_R = 0;
int volatile flag_f_B = 0;
int volatile  flag_f_G = 0;


void buttonCallback1(uint gpio, uint32_t events) {
    // fall edge - apertar BOTÃO
  if (gpio == BUTTON_B)
    flag_f_B = 1;
  else if (gpio == BUTTON_G)
    flag_f_G = 1; 
  else if (gpio == BUTTON_Y)
    flag_f_Y = 1;   
  else if (gpio == BUTTON_R)
    flag_f_R = 1; 

    // playNoteAndLight(LED_B);
}

void playNoteAndLight(int led);
void addColor();
void playEndGame();
void getColorSequence();
void beepToStart();

void setup() { // FUNÇÃO QUE SETA TODAS AS NOSSAS VARIAVEIS 
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

    gpio_put(BUZZ, 1);
    sleep_us(MICRO_TIME);
    gpio_put(LED_B, 1);
    sleep_us(MICRO_TIME);
    gpio_put(LED_G, 1);
    sleep_us(MICRO_TIME);
    gpio_put(LED_R, 1);
    sleep_us(MICRO_TIME);
    gpio_put(LED_Y, 1);
    sleep_us(HALF_TIME);
    gpio_put(LED_Y, 0);
    sleep_us(MICRO_TIME);
    gpio_put(LED_R, 0);
    sleep_us(MICRO_TIME);
    gpio_put(LED_G, 0);
    sleep_us(MICRO_TIME);
    gpio_put(LED_B, 0);
    sleep_us(MICRO_TIME);
    gpio_put(BUZZ, 0);

    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &buttonCallback1);

    // callback led g (nao usar _with_callback)
    gpio_set_irq_enabled(BUTTON_R, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BUTTON_G, GPIO_IRQ_EDGE_FALL, true); 
    gpio_set_irq_enabled(BUTTON_Y, GPIO_IRQ_EDGE_FALL, true);
} 

void playNoteAndLight(int led) {
    gpio_put(led, 1);
    gpio_put(BUZZ, 1);
    sleep_us(300000);
    gpio_put(BUZZ, 0);
    gpio_put(led, 0);
}

void playSequence() {
    for (int i = 0; i < sequenceIndex; i++) {
        playNoteAndLight(colors[i]);
        sleep_us(300000);
    }
}

void addColor() {
    int randomColor = (rand() % 4) + 6;
    colors[sequenceIndex++] = randomColor;
}

void playEndGame() {
    printf("\n\n*** THE USER HAS LOST! ***\n\n");
    sleep_us(100000);
    gpio_put(LED_B, 1);
    gpio_put(LED_G, 1);
    gpio_put(LED_R, 1);
    gpio_put(LED_Y, 1);

    for (int i = 0; i < 7; i++) {
        playNoteAndLight(LED_R);
        sleep_us(150000);
    }

    gpio_put(LED_B, 0);
    gpio_put(LED_G, 0);
    gpio_put(LED_R, 0);
    gpio_put(LED_Y, 0);
    gpio_put(BUZZ, 0);
    exit(0);
}

void getColorSequence() {
    selectedColor = -1;
    for (int i = 0; i < sequenceIndex; i++) {
        playNoteAndLight(colors[i]);
        sleep_us(300000);
        if (selectedColor != colors[i]) {
            playEndGame();
        }
    }
}

void beepToStart() {
    for (int i = 0; i < 5; i++) {
        playNoteAndLight(LED_G);
        sleep_us(150000);
    }
    sleep_us(1000000);
}

int main() {
    setup();

    beepToStart();

    while (1) {
        addColor();
        playSequence();
        getColorSequence();
        sleep_us(500000);
    }

    return 0;
}

