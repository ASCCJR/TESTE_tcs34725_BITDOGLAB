#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "tcs34725.h" // Incluindo a nossa biblioteca

// Pinos do conector I2C 1 da BitDogLab
#define I2C_PORT i2c1
const uint I2C_SDA_PIN = 2;
const uint I2C_SCL_PIN = 3;

int main() {
    stdio_init_all();
    sleep_ms(4000);

    printf("--- Iniciando Sensor de Cor TCS34725 ---\n");

    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Inicializa o sensor
    if (!tcs34725_init(I2C_PORT)) {
        printf("ERRO: Falha ao inicializar o sensor TCS34725. Verifique o ID do chip.\n");
        while(1);
    }
    
    printf("Sensor inicializado. Coletando dados de cor...\n");

    while (1) {
        tcs34725_color_data_t colors;
        tcs34725_read_colors(I2C_PORT, &colors);

        // Imprime os valores brutos de 16 bits para cada canal
        // 'u' é o formatador para inteiros sem sinal (unsigned int)
        printf("Clear: %u, Red: %u, Green: %u, Blue: %u\n", 
                colors.clear, colors.red, colors.green, colors.blue);
        
        sleep_ms(500);
    }
    return 0;
}