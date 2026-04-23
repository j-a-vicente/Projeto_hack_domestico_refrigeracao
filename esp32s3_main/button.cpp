#include "button.h"

void setupButtons() {
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, LOW);
}

void acaoLigar() {
    digitalWrite(STATUS_LED_PIN, HIGH);
    Serial.println("Ação executada: STATUS_LED LIGADO");
}

void acaoDesligar() {
    digitalWrite(STATUS_LED_PIN, LOW);
    Serial.println("Ação executada: STATUS_LED DESLIGADO");
}