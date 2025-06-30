# 🎨 Teste Modular do Sensor de Cor TCS34725

![Linguagem](https://img.shields.io/badge/Linguagem-C-blue.svg)
![Plataforma](https://img.shields.io/badge/Plataforma-Raspberry%20Pi%20Pico-purple.svg)
![Sensor](https://img.shields.io/badge/Sensor-TCS34725-lightgrey.svg)

Este repositório contém um código de teste para o sensor de cor I2C TCS34725, desenvolvido em C para o Raspberry Pi Pico W (BITDOGLAB)

O projeto demonstra a implementação de um driver que lida com o protocolo de comunicação específico do sensor (usando um bit de comando) e realiza uma inicialização segura, verificando a identidade do chip antes de prosseguir. A arquitetura modular separa o driver da aplicação, facilitando sua reutilização em projetos maiores.

## ✨ Funcionalidades

* **Leitura de 4 Canais de Cor:** Coleta dados dos canais Vermelho, Verde, Azul e Clear (luz visível sem filtro).
* **Driver Modular (`tcs34725.c`, `tcs34725.h`):** Toda a lógica de comunicação com o sensor é encapsulada, fornecendo uma API limpa e reutilizável.
* **Protocolo Específico:** Implementa corretamente o acesso aos registradores utilizando o `COMMAND_BIT` (0x80), conforme exigido pelo datasheet.
* **Inicialização Robusta:** O driver verifica o ID do chip no início para garantir que a comunicação está sendo feita com um sensor compatível (TCS34725/7).
* **Configuração Flexível de Porta:** Permite alternar facilmente o uso entre os barramentos I2C 0 e I2C 1 através de uma simples alteração no `main.c`.
* **Saída Serial:** Exibe os valores brutos de 16 bits de cada canal de cor no monitor serial.

## 🛠️ Hardware e Software Necessários

### Hardware
* Placa com Raspberry Pi Pico (neste projeto, foi usada a **BitDogLab**)
* Sensor de Cor I2C **TCS34725**

### Software
* [Raspberry Pi Pico C/C++ SDK](https://github.com/raspberrypi/pico-sdk)
* Ambiente de compilação C/C++ (GCC para ARM, CMake)
* Um programa para monitor serial (ex: o integrado no VS Code, PuTTY, etc.)

## Configurar a Porta I2C
* Por padrão, o projeto está configurado para usar o conector I2C 1 (Esquerda) da BitDogLab.
* Para trocar para o conector I2C 0 (Direita), edite o arquivo main.c
* Comente o bloco de configuração do I2C 1.
* Descomente o bloco de configuração do I2C 0.

## Exemplo para usar a porta I2C 0:

```bash
// --- Configuração da Porta I2C 1 da BitDogLab (Comentado) ---
// #define I2C_PORT i2c1
// const uint I2C_SDA_PIN = 2;
// const uint I2C_SCL_PIN = 3;

// --- Configuração da Porta I2C 0 da BitDogLab (Ativo) ---
#define I2C_PORT i2c0
const uint I2C_SDA_PIN = 0;
const uint I2C_SCL_PIN = 1;
```

## Visualizar a Saída
* Conecte o sensor AHT10 na porta I2C configurada
* Abra um monitor serial conectado à porta COM do seu Pico.
* Você verá a seguinte saída, atualizada a cada 2 segundos:
```bash
--- Iniciando Sensor de Umidade e Temperatura AHT10 ---
Sensor inicializado. Coletando dados...
Temperatura: 27.50 C  |  Umidade Relativa: 62.34 %
Temperatura: 27.51 C  |  Umidade Relativa: 62.38 %
```

## 📂 Estrutura dos Arquivos
* main.c: A aplicação principal. Orquestra a inicialização do hardware e do sensor, e realiza as leituras em loop.
* tcs34725.c: A implementação do driver do sensor, contendo a lógica de comunicação I2C específica do chip.
* tcs34725.h: O arquivo de cabeçalho (a interface ou API) para o driver do TCS34725.
* CMakeLists.txt: O arquivo de build do projeto.

✍️ Autor

[ASSCJR]
