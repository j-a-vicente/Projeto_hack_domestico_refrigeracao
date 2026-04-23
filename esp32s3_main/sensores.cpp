#include "sensores.h"

// Variáveis voláteis (modificadas dentro da interrupção precisam do 'volatile')
volatile long pulseCountFrio = 0;
volatile long pulseCountQuente = 0;

// Variáveis de armazenamento Frio
float flowRateFrio = 0.0;

// Variáveis de armazenamento Quente
float flowRateQuente = 0.0;

unsigned long oldTimeSensores = 0;

// Funções de interrupção (chamadas a cada pulso) - IRAM_ATTR é obrigatório
void IRAM_ATTR pulseCounterFrio() {
    pulseCountFrio++;
}

void IRAM_ATTR pulseCounterQuente() {
    pulseCountQuente++;
}

void setupSensoresFluxo() {
    pinMode(SENSOR_FLUXO_FRIO_PIN, INPUT_PULLUP);
    pinMode(SENSOR_FLUXO_QUENTE_PIN, INPUT_PULLUP);
    
    // Configura as interrupções
    attachInterrupt(digitalPinToInterrupt(SENSOR_FLUXO_FRIO_PIN), pulseCounterFrio, FALLING);
    attachInterrupt(digitalPinToInterrupt(SENSOR_FLUXO_QUENTE_PIN), pulseCounterQuente, FALLING);
    
    oldTimeSensores = millis();
}

void lerSensoresFluxo() {
    unsigned long currentTime = millis();
    unsigned long timeDiff = currentTime - oldTimeSensores;

    // Realiza o cálculo a cada 1 segundo (1000 ms)
    if (timeDiff >= 1000) {
        
        // Pausa as interrupções globalmente só para copiar os valores com segurança.
        // Isso evita o uso do detachInterrupt() que causa instabilidade.
        noInterrupts();
        long pulsosFrio = pulseCountFrio;
        long pulsosQuente = pulseCountQuente;
        pulseCountFrio = 0;
        pulseCountQuente = 0;
        interrupts(); // Reativa as interrupções imediatamente
        
        // Fórmulas para o YF-B1: (Frequência / 11) = L/min
        // --- CÁLCULO FRIO ---
        flowRateFrio = ((1000.0 / timeDiff) * pulsosFrio) / 11.0;

        // --- CÁLCULO QUENTE ---
        flowRateQuente = ((1000.0 / timeDiff) * pulsosQuente) / 11.0;
        
        // Atualiza o tempo
        oldTimeSensores = currentTime;
        
        // Exibe os resultados no Serial Monitor
        Serial.print("[FLUXO FRIO] Vazao: ");
        Serial.print(flowRateFrio);
        Serial.print(" L/min  ||  ");
        
        Serial.print("[FLUXO QUENTE] Vazao: ");
        Serial.print(flowRateQuente);
        Serial.println(" L/min");
    }
}