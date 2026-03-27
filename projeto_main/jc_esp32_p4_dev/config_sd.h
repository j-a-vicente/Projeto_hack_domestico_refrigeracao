#ifndef CONFIG_SD_H
#define CONFIG_SD_H

#include <Arduino.h>
#include "SD_MMC.h"  // Corrigido de SDMMC.h para SD_MMC.h
#include "FS.h"

// Protótipos
bool setupSDCard();
bool iniciarBancoDeDados();
void readFile(fs::FS &fs, const char * path);

#endif