#ifndef TCS34725_H
#define TCS34725_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define TCS34725_ADDR 0x29

// Estrutura para armazenar os 4 canais de cor lidos
typedef struct {
    uint16_t clear;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} tcs34725_color_data_t;

// Funções públicas
bool tcs34725_init(i2c_inst_t* i2c_port);
void tcs34725_read_colors(i2c_inst_t* i2c_port, tcs34725_color_data_t* colors);

#endif