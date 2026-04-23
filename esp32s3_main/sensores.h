#ifndef SENSORES_H
#define SENSORES_H

#include <Arduino.h>
#include "projeto_config.h"

// Inicializa os pinos e as interrupções dos sensores
void setupSensoresFluxo();

// Realiza o cálculo de vazão e imprime no Serial (deve ser chamado no loop)
void lerSensoresFluxo();

#endif