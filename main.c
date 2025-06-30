/**
 * @file main.c
 * @brief Aplicação de teste para o driver do sensor de cor TCS34725.
 *
 * Este programa inicializa o sensor e lê continuamente os dados brutos
 * dos quatro canais de cor (Clear, Red, Green, Blue), exibindo-os
 * no monitor serial.
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "tcs34725.h" // Incluindo nosso driver modular

// --- Configuração da Porta I2C da BitDogLab ---
// Alterne os comentários para escolher a porta desejada para o teste.

// Configuração para I2C 1 (Esquerda) - Padrão
#define I2C_PORT i2c1
const uint I2C_SDA_PIN = 2;
const uint I2C_SCL_PIN = 3;

// Configuração para I2C 0 (Direita) - Comentado
// #define I2C_PORT i2c0
// const uint I2C_SDA_PIN = 0;
// const uint I2C_SCL_PIN = 1;


int main() {
    stdio_init_all();
    
    // Espera ativa pela conexão do monitor serial.
    while(!stdio_usb_connected()) {
        sleep_ms(100);
    }

    printf("--- Iniciando Sensor de Cor TCS34725 ---\n");
    printf("Usando a porta I2C com SDA no pino %d e SCL no pino %d\n", I2C_SDA_PIN, I2C_SCL_PIN);

    // Inicializa o barramento I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Inicializa o sensor e verifica se foi bem-sucedido.
    if (!tcs34725_init(I2C_PORT)) {
        printf("ERRO: Falha ao inicializar o sensor TCS34725.\n");
        printf("Verifique a conexão e se o ID do chip é 0x44 ou 0x4D.\n");
        while(1);
    }
    
    printf("Sensor inicializado. Coletando dados de cor...\n");

    while (1) {
        tcs34725_color_data_t colors;
        tcs34725_read_colors(I2C_PORT, &colors);

        // Imprime os valores brutos (0-65535) para cada canal de cor.
        printf("Clear: %5u, Red: %5u, Green: %5u, Blue: %5u\n", 
                colors.clear, colors.red, colors.green, colors.blue);
        
        sleep_ms(500); // Pausa de meio segundo entre as leituras.
    }
    return 0;
}