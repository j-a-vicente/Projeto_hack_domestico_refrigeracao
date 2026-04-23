#include "comandos.h" // Inclusão correta do cabeçalho
#include "button.h"
#include "projeto_config.h"

void processarComando(String comando) {
    if (comando == "ON") {
        acaoLigar(); 
    } 
    else if (comando == "OFF") {
        acaoDesligar(); 
    }
    else if (comando.startsWith("TS:")) {
        sincronizarTempo(comando.substring(3));
    }
}

void sincronizarTempo(String strTimestamp) {
    time_t recebido = (time_t)strTimestamp.toInt();
    if (recebido > 0) {
        struct timeval tv = { .tv_sec = recebido, .tv_usec = 0 };
        settimeofday(&tv, NULL);
        Serial.print("[RTC] Sincronizado: ");
        imprimirRelogioAtual();
    }
}

void imprimirRelogioAtual() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    char buffer[25];
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
    Serial.println(buffer);
}