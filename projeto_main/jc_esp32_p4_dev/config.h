#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <ETH.h>

// ===== CONFIGURAÇÃO DE REDE =====
extern IPAddress ip;
extern IPAddress gateway;
extern IPAddress subnet;
extern IPAddress dns;

// Protótipos de funções
void setupEthernet();

#endif