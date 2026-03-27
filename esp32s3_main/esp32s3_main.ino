#include <Arduino.h>
#include <time.h>
#include <sys/time.h> // Necessário para settimeofday

#define RX_PIN 16
#define TX_PIN 17
#define LED_PIN 2 

// Variável para controlar o intervalo do print do relógio
unsigned long lastTimePrint = 0; 

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    // Inicializa a comunicação com a JC-ESP32P4
    Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
    Serial.println("ESP32-S3 Pronta e aguardando comandos...");

    // Configura o fuso horário interno do S3 para GMT-3 (Brasil)
    setenv("TZ", "<-03>3", 1);
    tzset();
}

void loop() {
    // 1. Bloco de escuta da comunicação com a placa Mestre
    if (Serial1.available() > 0) {
        String comando = Serial1.readStringUntil('\n');
        comando.trim(); 
        
        Serial.print("\n--- Novo Comando Recebido: ");
        Serial.println(comando);
        
        if (comando == "ON") {
            digitalWrite(LED_PIN, HIGH);
            Serial.println("Ação executada: LED LIGADO");
        } 
        else if (comando == "OFF") {
            digitalWrite(LED_PIN, LOW);
            Serial.println("Ação executada: LED DESLIGADO");
        }
        // NOVA LÓGICA: Sincronização de Tempo
        else if (comando.startsWith("TS:")) {
            String strTimestamp = comando.substring(3); // Extrai os números após "TS:"
            time_t recebido = strTimestamp.toInt();
            
            Serial.print("Ação: Sincronização de Tempo via UART.\n");
            Serial.print(" > Timestamp bruto recebido: ");
            Serial.println(recebido);
            
            if (recebido > 0) {
                // Aplica a nova hora no relógio interno do S3
                struct timeval tv;
                tv.tv_sec = recebido;
                tv.tv_usec = 0;
                settimeofday(&tv, NULL);
                
                // Lê a hora atualizada para confirmar
                time_t now;
                struct tm timeinfo;
                time(&now);
                localtime_r(&now, &timeinfo);
                
                char buffer[50];
                strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
                Serial.print(" > Relógio do ESP32-S3 atualizado para: ");
                Serial.println(buffer);
            } else {
                Serial.println(" > Erro: Timestamp inválido.");
            }
        } else {
            Serial.println("Ação: Comando desconhecido ignorado.");
        }
    }

    // 2. Bloco do Relógio Local (Print a cada 5 segundos)
    // if (millis() - lastTimePrint > 5000) { 
    //     time_t now;
    //     struct tm timeinfo;
        
    //     // Força a leitura do tempo atual (mesmo que seja 1970)
    //     time(&now);
    //     localtime_r(&now, &timeinfo);
        
    //     char buffer[50];
    //     // Formata a data para o padrão Brasileiro
    //     strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);
        
    //     Serial.print("[Status S3] Data/Hora Local Interna: ");
    //     Serial.println(buffer);
        
    //     // Verifica se o ano é antigo (antes de 2020) para dar um aviso visual no log
    //     if (timeinfo.tm_year + 1900 < 2020) {
    //         Serial.println("            (Aviso: Aguardando Mestre enviar a hora correta...)");
    //     }
        
    //     // Atualiza a variável para contar mais 5 segundos
    //     lastTimePrint = millis(); 
    // }
}