#ifndef PROJETO_CONFIG_H
#define PROJETO_CONFIG_H

#include <Arduino.h>
#include <ETH.h>
#include <Wire.h>     // Adicionado
#include "PCA9685.h"  // Adicionado
#include "config_sd.h"
#include "config_webserver.h"


// Definição dos pinos I2C
#define I2C_MASTER_SDA_IO    7   
#define I2C_MASTER_SCL_IO    8   
#define PCA9685_ADDR         0x40

// Torna o objeto pwm acessível em outros arquivos (.cpp)
extern PCA9685 pwm; 

void setupEthernet();

#endif