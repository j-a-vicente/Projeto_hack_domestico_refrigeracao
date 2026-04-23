#include "corrente.h"

// Instancia o ADS1115
Adafruit_ADS1115 ads;

// Multiplicador do ADS1115 para o ganho GAIN_TWOTHIRDS (+/- 6.144V)
// 1 bit = 0.1875 mV
const float multiplier = 0.0001875F; 

unsigned long oldTimeCorrente = 0;

void setupCorrente() {
    // Inicializa o barramento I2C com os pinos definidos
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    
    // Inicia o ADS1115 no endereço padrão 0x48 (pino ADDR conectado ao GND)
    if (!ads.begin(0x48, &Wire)) {
        Serial.println("[ERRO] Falha ao inicializar o ADS1115!");
        // Opcional: colocar um loop infinito aqui se for crítico
    } else {
        Serial.println("[SUCESSO] ADS1115 inicializado.");
    }
    
    // O ganho determina a tensão máxima que podemos ler.
    // GAIN_TWOTHIRDS lê de -6.144V até +6.144V (Seguro para o ACS712 em 5V)
    ads.setGain(GAIN_TWOTHIRDS);
    
    oldTimeCorrente = millis();
}

void lerCorrente() {
    // Realiza a leitura a cada 1 segundo (ajuste conforme necessário)
    if ((millis() - oldTimeCorrente) > 1000) {
        
        // Lê as portas A0 e A1 do ADS1115
        int16_t adc0 = ads.readADC_SingleEnded(0);
        int16_t adc1 = ads.readADC_SingleEnded(1);

        // Converte o valor digital (bits) para Tensão (Volts)
        float volts0 = adc0 * multiplier;
        float volts1 = adc1 * multiplier;

        // Calcula a corrente: I = (V_Lido - V_Zero) / Sensibilidade
        float corrente0 = (volts0 - ACS_TENSAO_ZERO) / ACS_SENSIBILIDADE;
        float corrente1 = (volts1 - ACS_TENSAO_ZERO) / ACS_SENSIBILIDADE;

        // Filtro rápido para evitar mostrar correntes residuais (ruído de leitura)
        if (abs(corrente0) < 0.05) corrente0 = 0.0;
        if (abs(corrente1) < 0.05) corrente1 = 0.0;

        Serial.print("[CORRENTE A0] Volts: ");
        Serial.print(volts0, 3);
        Serial.print("V | Corrente: ");
        Serial.print(corrente0, 2);
        Serial.println(" A");

        Serial.print("[CORRENTE A1] Volts: ");
        Serial.print(volts1, 3);
        Serial.print("V | Corrente: ");
        Serial.print(corrente1, 2);
        Serial.println(" A");

        oldTimeCorrente = millis();
    }
}