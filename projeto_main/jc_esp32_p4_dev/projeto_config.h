#ifndef PROJETO_CONFIG_H
#define PROJETO_CONFIG_H

#include <Arduino.h>
#include <ETH.h>
#include <Wire.h>     
#include "PCA9685.h"
#include <RTClib.h>  
#include "config_sd.h"
#include "config_webserver.h"


// Definição dos pinos I2C
#define I2C_MASTER_SDA_IO    32   
#define I2C_MASTER_SCL_IO    33   
#define PCA9685_ADDR         0x40


// --- Configurações UART (Comunicação com ESP32-S3) ---
#define UART_S3_TX_PIN 4
#define UART_S3_RX_PIN 5
#define UART_BAUD_RATE 115200


// Torna o objeto pwm acessível em outros arquivos (.cpp)
extern PCA9685 pwm; 

void setupEthernet();
void setup_uart_s3();

void setup_rtc_and_time();
void enviar_hora_para_escravo();


#endif