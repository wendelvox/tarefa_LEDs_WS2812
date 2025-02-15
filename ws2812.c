#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"  // Inclui o arquivo gerado pelo CMake para o PIO


#define LED_PIN_RED    13
#define LED_PIN_GREEN  11
#define LED_PIN_BLUE   12
#define BUTTON_A_PIN   5
#define BUTTON_B_PIN   6
#define WS2812_PIN     7

// Variáveis globais
int current_number = 0;  // Número a ser exibido na matriz
bool button_a_pressed = false;
bool button_b_pressed = false;
uint64_t last_button_a_time = 0;
uint64_t last_button_b_time = 0;
const uint64_t DEBOUNCE_DELAY = 200 * 1000; // 200ms debounce

// Variáveis globais para controle do LED e cor
uint8_t selected_led = 0; // Índice do LED a ser controlado (0 a 24)
uint8_t selected_r = 0;   // Intensidade do vermelho (0 a 255)
uint8_t selected_g = 0;   // Intensidade do verde (0 a 255)
uint8_t selected_b = 80;   // Intensidade do azul (0 a 255)

// Mapeamento dos padrões de LEDs para os números de 0 a 9 (em formato 5x5)
const bool matriz_numeros[10][25] = {
    {
        1, 1, 1, 1, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 0, 0, 0, 1,
        1, 1, 1, 1, 1  
    },// Número 0
    {
        0, 1, 1, 1, 0,  
        0, 0, 1, 0, 0,  
        0, 0, 1, 0, 0,  
        0, 1, 1, 0, 0,  
        0, 0, 1, 0, 0
    },   // Número 

     {
        1, 1, 1, 1, 1, 
        1, 0, 0, 0, 0,  
        1, 1, 1, 1, 1,  
        0, 0, 0, 0, 1,  
        1, 1, 1, 1, 1   
    }, // Número 2 

    {
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1,
        0, 0, 0, 0, 1,
        1, 1, 1, 1, 1  
    }, // Número 3

    {
    1, 0, 0, 0, 0,  
    0, 0, 0, 0, 1,  
    1, 1, 1, 1, 1,  
    1, 0, 0, 0, 1,  
    1, 0, 0, 0, 1   
},  // Número 4

{
    1, 1, 1, 1, 1,  
    0, 0, 0, 0, 1,  
    1, 1, 1, 1, 1,  
    1, 0, 0, 0, 0,  
    1, 1, 1, 1, 1   
},  // Número 5

{
    1, 1, 1, 1, 1,  
    1, 0, 0, 0, 1,  
    1, 1, 1, 1, 1,  
    1, 0, 0, 0, 0,  
    1, 1, 1, 1, 1   
},  // Número 6

{
    0, 0, 0, 0, 1, 
    0, 1, 0, 0, 0,  
    0, 0, 1, 0, 0,  
    0, 0, 0, 1, 0,  
    1, 1, 1, 1, 1
},  // Número 7

{
    1, 1, 1, 1, 1,  
    1, 0, 0, 0, 1, 
    1, 1, 1, 1, 1, 
    1, 0, 0, 0, 1,  
    1, 1, 1, 1, 1   
},  // Número 8

{
    1, 1, 1, 1, 1, 
    0, 0, 0, 0, 1, 
    1, 1, 1, 1, 1,  
    1, 0, 0, 0, 1,  
    1, 1, 1, 1, 1   
}   // Número 9

};


void display_number(int number, PIO pio, uint sm) {
    printf("Exibindo número %d\n", number);  // Depuração
    const bool *pattern = matriz_numeros[number];      
    uint32_t color = 0; // Inicializa a variável para armazenar a cor

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            int index = row * 5 + col;
            
            // Se o LED da matriz estiver "aceso", aplica a cor com brilho
            if (pattern[index]) {
                color = (51 << 24) | 0 | 0 ; // Forma a cor no formato RGB  (G<<24)|(R<<16)|(B<<8);
            } else {
                color = 0; // Apaga o LED (sem brilho)
            }

            // Envia a cor para o PIO para o controle do LED
            pio_sm_put_blocking(pio, sm, color);
        }
    }
}



// Função para piscar o LED vermelho 5 vezes por segundo
void blink_red_led() {
    gpio_put(LED_PIN_RED, 1);  // Acende o LED vermelho
    sleep_ms(100);              // Mantém aceso por 100ms
    gpio_put(LED_PIN_RED, 0);  // Apaga o LED vermelho
    sleep_ms(100);              // Mantém apagado por 100ms
}

// Função de interrupção 
void button_irq_handler(uint gpio, uint32_t events) {
    uint64_t current_time = time_us_32();

    if (gpio == BUTTON_A_PIN && current_time - last_button_a_time > DEBOUNCE_DELAY) {
        button_a_pressed = true;
        last_button_a_time = current_time;
    } 
    else if (gpio == BUTTON_B_PIN && current_time - last_button_b_time > DEBOUNCE_DELAY) {
        button_b_pressed = true;
        last_button_b_time = current_time;
    }
}

// Função principal
int main() {
    // Inicializa LEDs comuns
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);

    // Inicializa os botões com interrupções
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, button_irq_handler);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, button_irq_handler);


    // Inicializa o PIO para controle do WS2812
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, false);

    stdio_init_all();    

    while (true) {
        blink_red_led();  // Pisca o LED vermelho

        // Verifica se o botão A foi pressionado
        if (button_a_pressed) {
            button_a_pressed = false;
            current_number = (current_number + 1) % 10;
            display_number(current_number, pio, sm);
        }

        // Verifica se o botão B foi pressionado
        if (button_b_pressed) {
            button_b_pressed = false;
            current_number = (current_number - 1 + 10) % 10;
            display_number(current_number, pio, sm);
        }

        sleep_ms(10); // Aguarda um pouco para a estabilidade
    }

    return 0;
}
