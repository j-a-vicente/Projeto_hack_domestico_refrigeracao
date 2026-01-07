#include <Wire.h>
#include "PCA9685.h"

PCA9685 pwm(0x40);   // endereço padrão da placa
int contador = 0;     // Declarar contador como variável global

void setup() {
  Serial.begin(9600);  // Inicializar comunicação serial
  Wire.begin();
  pwm.init();                 // INICIALIZA PCA9685
  pwm.setPWMFrequency(1000);  // Frequência para LEDs
}

void loop() {    
    Serial.println("Sequência de 1 a 4095 de 10 em 10:");
    contador = 0;  // Resetar contador a cada ciclo
    
    for (int i = 1; i <= 4095; i += 100) {
        pwm.setChannelPWM(0, i);   // acende LED no canal 0
        Serial.print(i);
        Serial.print(" ");
        
        contador++;
        
        // Quebra de linha a cada 10 números para melhor visualização
        if (contador % 100 == 0) {
            Serial.println();
        }
        
        delay(1000);
    }
    
    Serial.println("\nSequência completa!");
    delay(5000);  // Aguarda 5 segundos antes de reiniciar
}