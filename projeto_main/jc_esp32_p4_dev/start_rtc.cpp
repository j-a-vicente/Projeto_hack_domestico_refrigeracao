#include "projeto_config.h"
#include <time.h>

RTC_DS3231 rtc;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -10800; // -3 horas
const int   daylightOffset_sec = 0;

void setup_rtc_and_time() {
    // CORREÇÃO: Passar &Wire obriga o RTC a usar o I2C já configurado nos pinos 7 e 8
    // Isso evita que a biblioteca reinicie o I2C nos pinos errados e afete a UART
    if (!rtc.begin(&Wire)) {
        Serial.println("Erro: DS3231 não encontrado no barramento I2C!");
    } else {
        Serial.println("I2C: DS3231 Inicializado");
        if (rtc.lostPower()) {
            Serial.println("Aviso: RTC perdeu energia, defina o tempo!");
        }
    }

    // Tenta obter hora da internet 
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    bool ntpSuccess = getLocalTime(&timeinfo, 5000); 

    if (ntpSuccess) {
        Serial.println("Tempo: Hora obtida via NTP. Atualizando DS3231...");
        rtc.adjust(DateTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, 
                            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec));
    } else {
        Serial.println("Tempo: Falha NTP. Resgatando hora do DS3231...");
        DateTime now = rtc.now();
        struct timeval tv;
        tv.tv_sec = now.unixtime();
        tv.tv_usec = 0;
        settimeofday(&tv, NULL);
    }
}


void enviar_hora_para_escravo() {
    time_t now;
    struct tm timeinfo;
    
    // Obtém o tempo atual
    time(&now);
    
    // Converte o formato de tempo para um texto limpo (String) para evitar erros na Serial1
    String timestampStr = String((unsigned long)now);
    
    // Envia o pacote para o Escravo S3
    Serial1.print("TS:");
    Serial1.println(timestampStr);
    
    // --- LÓGICA DE PRINT NO MESTRE ---
    // Converte o tempo bruto em uma estrutura de data legível
    localtime_r(&now, &timeinfo);
    char buffer[50];
    // Formata para o padrão Brasileiro (DD/MM/YYYY HH:MM:SS)
    strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
    
    // Imprime no Monitor Serial do Mestre
    Serial.print("\n[Mestre P4] -> Sincronizacao enviada para o S3: ");
    Serial.println(buffer);
}