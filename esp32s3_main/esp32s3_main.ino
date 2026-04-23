#include "projeto_config.h"
#include "button.h"
#include "comandos.h" 
#include "sensores.h"
#include "corrente.h"

unsigned long lastTimePrint = 0;

void setup() {
    Serial.begin(115200);
    
    // Configura os pinos (lógica escondida no button.cpp)
    setupButtons();

    // Configura os sensores de fluxo
    setupSensoresFluxo(); 

    // inicializa o ADS1115 E I2C
    setupCorrente(); 

    // Inicializa a comunicação com a JC-ESP32P4
    Serial1.begin(UART_BAUD_RATE, SERIAL_8N1, SLAVE_RX_PIN, SLAVE_TX_PIN);
    Serial.println("--- ESP32-S3 Slave: Pronto e aguardando comandos ---");

    // Configura o fuso horário interno do S3 para GMT-3 (Brasil)
    setenv("TZ", TIME_ZONE, 1);
    tzset();
}

void loop() {
    
    // 1. Escuta da comunicação com a placa Mestre
    if (Serial1.available() > 0) {
        String comando = Serial1.readStringUntil('\n');
        comando.trim(); 
        processarComando(comando);
    }

    // 2. Leitura e cálculo dos Sensores de Fluxo
    lerSensoresFluxo(); // <-- Verifica a cada segundo se há fluxo

    // 3. Sensores de Corrente
    lerCorrente(); // <-- LÊ E CALCULA CORRENTE DOS ACS712

    // 4. Print do relógio a cada 30 segundos
    if (millis() - lastTimePrint > 30000) {
        imprimirRelogioAtual();
        lastTimePrint = millis();
    }
}