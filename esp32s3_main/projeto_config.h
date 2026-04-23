#ifndef PROJETO_CONFIG_H
#define PROJETO_CONFIG_H

#include <Arduino.h>

// Definições de Hardware
// Comunicação com Master entre Slaver
#define SLAVE_RX_PIN 16
#define SLAVE_TX_PIN 17

#define STATUS_LED_PIN 2

// Pinos dos Sensores de Fluxo YF-B1
#define SENSOR_FLUXO_FRIO_PIN 7
#define SENSOR_FLUXO_QUENTE_PIN 8

// --- Configurações I2C (ADS1115) ---
#define I2C_SDA_PIN 5  // Ajuste para o pino SDA que está usando no S3
#define I2C_SCL_PIN 4  // Ajuste para o pino SCL que está usando no S3

// --- Configurações do Sensor de Corrente ACS712 ---
// Ajuste a sensibilidade de acordo com o seu modelo do ACS712:
// 5A  = 0.185 (185 mV/A)
// 20A = 0.100 (100 mV/A)
// 30A = 0.066 (66 mV/A)
#define ACS_SENSIBILIDADE 0.185

// Configurações de Comunicação
#define UART_BAUD_RATE 115200
#define TIME_ZONE "<-03>3" // GMT-3 (Brasil)

// Tensão de ponto zero (quando a corrente é 0A). 
// Idealmente é VCC / 2. Se alimentar o módulo com 5V, será ~2.5V.
#define ACS_TENSAO_ZERO 2.500

#endif