#ifndef CONFIG_H
#define CONFIG_H

#include <time.h>
#include <sys/time.h>
#include "projeto_config.h"
#include "comandos.h"

// Protótipos de funções (se desejar modularizar mais futuramente)
void processarComando(String comando);
void sincronizarTempo(String strTimestamp);
void imprimirRelogioAtual();

#endif