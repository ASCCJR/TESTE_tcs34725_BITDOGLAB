#include "tcs34725.h"

// Endereços dos Registradores do TCS34725
#define TCS34725_ENABLE_REG 0x00
#define TCS34725_ATIME_REG  0x01
#define TCS34725_CONTROL_REG 0x0F
#define TCS34725_ID_REG     0x12
#define TCS34725_CDATAL_REG 0x14 // Primeiro registrador de dados (Clear, low byte)

// Constante para acesso aos registradores de comando
#define TCS34725_COMMAND_BIT 0x80

bool tcs34725_init(i2c_inst_t* i2c) {
    uint8_t id_reg = TCS34725_COMMAND_BIT | TCS34725_ID_REG;
    uint8_t chip_id;
    i2c_write_blocking(i2c, TCS34725_ADDR, &id_reg, 1, true);
    i2c_read_blocking(i2c, TCS34725_ADDR, &chip_id, 1, false);

    // O ID de um TCS34725 pode ser 0x44 ou 0x4D
    if (chip_id != 0x44 && chip_id != 0x4D) {
        return false;
    }

    // Liga o oscilador interno e o conversor ADC
    uint8_t enable_cmd[] = {TCS34725_COMMAND_BIT | TCS34725_ENABLE_REG, 0x03}; // PON = 1, AEN = 1
    i2c_write_blocking(i2c, TCS34725_ADDR, enable_cmd, 2, false);

    // Define o tempo de integração (afeta a sensibilidade)
    uint8_t atime_cmd[] = {TCS34725_COMMAND_BIT | TCS34725_ATIME_REG, 0xEB}; // ~100ms
    i2c_write_blocking(i2c, TCS34725_ADDR, atime_cmd, 2, false);

    // Define o ganho do amplificador
    uint8_t control_cmd[] = {TCS34725_COMMAND_BIT | TCS34725_CONTROL_REG, 0x00}; // Ganho 1x
    i2c_write_blocking(i2c, TCS34725_ADDR, control_cmd, 2, false);

    sleep_ms(3); // Pequena pausa para estabilizar

    return true;
}

void tcs34725_read_colors(i2c_inst_t* i2c, tcs34725_color_data_t* colors) {
    uint8_t buffer[8];
    uint8_t start_reg = TCS34725_COMMAND_BIT | TCS34725_CDATAL_REG;

    // Pede ao sensor para ler 8 bytes a partir do registrador de dados do canal Clear
    i2c_write_blocking(i2c, TCS34725_ADDR, &start_reg, 1, true);
    i2c_read_blocking(i2c, TCS34725_ADDR, buffer, 8, false);

    // Os dados são de 16 bits, "little-endian" (byte menos significativo primeiro)
    colors->clear = (buffer[1] << 8) | buffer[0];
    colors->red   = (buffer[3] << 8) | buffer[2];
    colors->green = (buffer[5] << 8) | buffer[4];
    colors->blue  = (buffer[7] << 8) | buffer[6];
}