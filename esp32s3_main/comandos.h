#ifndef COMANDOS_H
#define COMANDOS_H

#include <Arduino.h>
#include <time.h>
#include <sys/time.h>

// Protótipos das funções de comando e tempo
void processarComando(String comando);
void sincronizarTempo(String strTimestamp);
void imprimirRelogioAtual();

#endif