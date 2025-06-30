/**
 * @file tcs34725.c
 * @brief Implementação do driver para o sensor de cor I2C TCS34725.
 *
 * Contém a lógica de baixo nível para comunicação I2C, incluindo o uso
 * do bit de comando para acesso aos registradores específicos do sensor.
 */

#include "tcs34725.h"

// --- Registradores Internos e Comandos ---
#define TCS34725_COMMAND_BIT 0x80 // Bit que deve ser setado para acessar registradores.

// Endereços dos Registradores
#define TCS34725_ENABLE_REG 0x00 // Usado para ligar/desligar o sensor.
#define TCS34725_ATIME_REG  0x01 // Usado para configurar o tempo de integração do ADC.
#define TCS34725_CONTROL_REG 0x0F // Usado para configurar o ganho (gain).
#define TCS34725_ID_REG     0x12 // Contém o ID do chip.
#define TCS34725_CDATAL_REG 0x14 // Registrador inicial dos dados de cor (Clear, low byte).


bool tcs34725_init(i2c_inst_t* i2c) {
    // 1. Verifica a identidade do chip para garantir que estamos falando com o sensor correto.
    uint8_t id_reg = TCS34725_COMMAND_BIT | TCS34725_ID_REG;
    uint8_t chip_id;
    i2c_write_blocking(i2c, TCS34725_ADDR, &id_reg, 1, true);
    i2c_read_blocking(i2c, TCS34725_ADDR, &chip_id, 1, false);

    // O ID de um TCS34725 é 0x44 e de um TCS34727 é 0x4D. Ambos usam este driver.
    if (chip_id != 0x44 && chip_id != 0x4D) {
        return false; // Falha se o ID não corresponder.
    }

    // 2. Configura o tempo de integração do ADC.
    // O valor 0xEB resulta em (256 - 235) * 2.4ms = 50.4ms.
    uint8_t atime_cmd[] = {TCS34725_COMMAND_BIT | TCS34725_ATIME_REG, 0xEB}; // ~50ms
    i2c_write_blocking(i2c, TCS34725_ADDR, atime_cmd, 2, false);

    // 3. Configura o ganho do amplificador.
    uint8_t control_cmd[] = {TCS34725_COMMAND_BIT | TCS34725_CONTROL_REG, 0x00}; // Ganho 1x
    i2c_write_blocking(i2c, TCS34725_ADDR, control_cmd, 2, false);
    
    // 4. Liga o oscilador interno (PON) e habilita o ADC (AEN).
    uint8_t enable_cmd[] = {TCS34725_COMMAND_BIT | TCS34725_ENABLE_REG, 0x03}; // PON=1, AEN=1
    i2c_write_blocking(i2c, TCS34725_ADDR, enable_cmd, 2, false);

    // Pequena pausa para a primeira conversão após ligar o ADC.
    sleep_ms(3); 

    return true;
}

void tcs34725_read_colors(i2c_inst_t* i2c, tcs34725_color_data_t* colors) {
    uint8_t buffer[8];
    uint8_t start_reg = TCS34725_COMMAND_BIT | TCS34725_CDATAL_REG;

    // Pede ao sensor para ler 8 bytes em sequência a partir do registrador CDATAL.
    i2c_write_blocking(i2c, TCS34725_ADDR, &start_reg, 1, true);
    i2c_read_blocking(i2c, TCS34725_ADDR, buffer, 8, false);

    // Converte os pares de bytes lidos (little-endian) em valores de 16 bits.
    colors->clear = (buffer[1] << 8) | buffer[0];
    colors->red   = (buffer[3] << 8) | buffer[2];
    colors->green = (buffer[5] << 8) | buffer[4];
    colors->blue  = (buffer[7] << 8) | buffer[6];
}