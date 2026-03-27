// Inclui as configurações globais do projeto (definição de pinos, endereços I2C e assinaturas de funções)
#include "projeto_config.h"

// Instancia o objeto para controle do módulo expansor de PWM (PCA9685) utilizando o endereço I2C configurado (ex: 0x40)
PCA9685 pwm(PCA9685_ADDR);

// Variável de controle (temporizador) para o envio periódico da sincronização de hora para o ESP32-S3.
// O valor 3.600.000 ms equivale a 1 hora de intervalo.
unsigned long lastTimeSync = 0;

void setup() {
    // Inicializa a comunicação serial padrão para depuração e monitoramento no computador
    Serial.begin(115200);
    delay(1000); // Aguarda um momento para estabilização da porta serial antes de imprimir
    Serial.println("\n=== JC-ESP32P4 WEB SERVER + PCA9685 + RTC ===");

    // 1. INICIALIZAÇÃO DA REDE (LAN8720)
    // Deve ser a primeira etapa do hardware, pois o sistema de relógio (RTC) precisará 
    // da conexão com a internet ativa para tentar buscar a hora atualizada via servidor NTP.
    setupEthernet();

    // 2. INICIALIZAÇÃO DO BARRAMENTO I2C
    // Configura fisicamente os pinos 7 (SDA) e 8 (SCL) como o barramento I2C principal da placa P4.
    Wire.begin(I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
    
    // 3. INICIALIZAÇÃO DO RELÓGIO (RTC DS3231 E NTP)
    // Inicializa o DS3231 passando a referência do I2C já criado (&Wire) para evitar conflito de pinos.
    // Tenta atualizar a hora via internet; se não houver conexão, resgata a hora do módulo físico.
    setup_rtc_and_time();

    // 4. INICIALIZAÇÃO DO MÓDULO PWM (PCA9685)
    // Agora que o I2C está estável e seguro, inicia o controlador PCA9685 e define a frequência (ex: 1kHz).
    pwm.init();
    pwm.setPWMFrequency(1000); 
    Serial.println("I2C: PCA9685 Inicializado (1kHz)");

    // 5. INICIALIZAÇÃO DA UART (Comunicação P4 -> S3)
    // Estrategicamente posicionada APÓS as inicializações do I2C e RTC. Isso garante que bibliotecas
    // de terceiros não "roubem" ou resetem acidentalmente os pinos 4 e 5 destinados à Serial1.
    setup_uart_s3();

    // Sincronização imediata: Envia o pacote de tempo (prefixo "TS:") recém-obtido para atualizar o 
    // relógio interno da placa escrava ESP32-S3 no momento exato em que o sistema liga.
    enviar_hora_para_escravo();

    // 6. INICIALIZAÇÃO DE DADOS E SERVIDOR (MicroSD e SQLite)
    // Tenta montar o sistema de arquivos no SDMMC. Se falhar, impede a subida do banco e do servidor 
    // para evitar travamentos ou corrupção de dados.
    if(setupSDCard()) {
        iniciarBancoDeDados();
        setupWebServer(); // Sobe as APIs e serve a pasta /data do SD
    } else {
        Serial.println("ERRO CRÍTICO: Servidor Web e Banco de Dados abortados devido a falha no Cartão SD.");
    }
}

void loop() {
    // Mantém o servidor web escutando e respondendo ativamente às requisições do front-end (HTML/JS)
    handleWebServer();
    
    // TEMPORIZADOR NÃO-BLOQUEANTE: SINCRONIZAÇÃO DE HORA
    // Verifica continuamente se já se passou 1 hora desde a última sincronização. 
    // Não utiliza delay() para não travar a resposta do WebServer e a leitura de comandos do painel.
    if (millis() - lastTimeSync > 10000) {
        // enviar_hora_para_escravo();
        lastTimeSync = millis();
    }

    // Cede tempo de processamento para as tarefas ocultas do ESP32 (como manter a rede Ethernet operando)
    yield(); 
}