#include "projeto_config.h"

// Instancia o objeto PWM
PCA9685 pwm(PCA9685_ADDR);

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("\n=== JC-ESP32P4 WEB SERVER + PCA9685 ===");

    // === INICIALIZAÇÃO I2C E PCA9685 ===
    Wire.begin(I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
    pwm.init();
    pwm.setPWMFrequency(1000); 
    Serial.println("I2C: PCA9685 Inicializado (1kHz)");

    // 1. Inicializa Ethernet
    setupEthernet();

    // 2. Inicializa MicroSD
    if(setupSDCard()) {
        
        // 3. Inicia o Banco de Dados (só é chamado se o SD funcionou)
        // Usamos a função iniciarBancoDeDados() que definimos no start_sd.cpp
        iniciarBancoDeDados(); 
        
        // 4. Inicializa o WebServer (já com o SD e Banco prontos)
        setupWebServer();
        
    } else {
        Serial.println("ERRO: Servidor e Banco nao iniciados pois o SD falhou.");
    }
}

void loop() {
    handleWebServer();
    yield(); 
}