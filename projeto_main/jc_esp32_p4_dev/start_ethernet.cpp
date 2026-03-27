#include "config.h"
#include <time.h>
// Definição das variáveis de IP
IPAddress ip(192, 168, 10, 210);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

// Callback de eventos de rede
void onEvent(arduino_event_id_t event, arduino_event_info_t info) {
    switch (event) {
        case ARDUINO_EVENT_ETH_START:
            Serial.println("ETH: Iniciado");
            break;
        case ARDUINO_EVENT_ETH_CONNECTED:
            Serial.println("ETH: Cabo Conectado");
            break;
        case ARDUINO_EVENT_ETH_GOT_IP:
            Serial.print("ETH: IP obtido: ");
            Serial.println(ETH.localIP());
            break;
        case ARDUINO_EVENT_ETH_DISCONNECTED:
            Serial.println("ETH: Cabo Desconectado");
            break;
        default:
            break;
    }
}

// Função para buscar a hora certa na internet
void sincronizarHoraNTP() {
    Serial.print("NTP: Sincronizando hora com a internet ");
    
    // Configuração para o fuso horário de Brasília (GMT-3 = -10800 segundos)
    // O segundo parâmetro é o horário de verão (0 = desligado no Brasil atualmente)
    configTime(-10800, 0, "a.st1.ntp.br", "b.st1.ntp.br", "pool.ntp.org");

    struct tm timeinfo;
    int tentativas = 0;
    
    // O ESP32 fica tentando buscar a hora (o ano deixará de ser 1970)
    while (!getLocalTime(&timeinfo) && tentativas < 10) {
        Serial.print(".");
        delay(1000);
        tentativas++;
    }

    if (tentativas < 10) {
        Serial.println("\nNTP: Hora sincronizada com sucesso!");
        Serial.println(&timeinfo, "NTP: Data/Hora atual: %d/%m/%Y %H:%M:%S");
    } else {
        Serial.println("\nNTP: Falha ao sincronizar a hora. Verifique a conexao de internet.");
    }
}

void setupEthernet() {
    Network.onEvent(onEvent);
    
    // Inicializa a Ethernet
    if (!ETH.begin()) {
        Serial.println("Erro crítico: Falha ao iniciar hardware Ethernet");
        return;
    }

    // Aplica IP fixo
    if (!ETH.config(ip, gateway, subnet, dns)) {
        Serial.println("Erro: Falha ao configurar IP fixo");
    }

    // Atualiza data e hora do esp32
    if (ETH.linkUp()) {
        Serial.println("ETH: Conectado com sucesso!");
        Serial.print("ETH: IP obtido: ");
        Serial.println(ETH.localIP());
        
        // CHAMA A FUNÇÃO DE HORA AQUI
        sincronizarHoraNTP(); 
    }    
}



