#ifndef CORRENTE_H
#define CORRENTE_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include "projeto_config.h"

// Inicializa o barramento I2C e o ADS1115
void setupCorrente();

// Realiza a leitura dos dois sensores ACS712 e imprime no Serial
void lerCorrente();

#endif