#include <Arduino.h>
#include "SD_MMC.h"
#include "config_webserver.h"
#include "projeto_config.h" // Necessário para acessar o objeto 'pwm'
#include <sqlite3.h>        // ADICIONADO: Para a rota da API

// ADICIONADO: Puxa o ponteiro do banco de dados que foi aberto lá no start_sd.cpp
extern sqlite3* db; 

WebServer server(80);


// Função para inicializar a UART no Setup do P4
void setup_uart_s3() {
    // Inicializa a Serial1 (Baud, Formato, RX, TX)
    Serial1.begin(UART_BAUD_RATE, SERIAL_8N1, UART_S3_RX_PIN, UART_S3_TX_PIN);
    Serial.println("UART para ESP32-S3 inicializada nos pinos 4 e 5.");
}


// Rota para tratar o botão do painel
void handlePowerToggle() {
    if (server.hasArg("state")) {
        String state = server.arg("state");
        
        // Envia o comando via Serial para a S3
        if (state == "on") {
            Serial1.print("ON\n");
            server.send(200, "text/plain", "Comando ON enviado");
        } else if (state == "off") {
            Serial1.print("OFF\n");
            server.send(200, "text/plain", "Comando OFF enviado");
        } else {
            server.send(400, "text/plain", "Estado invalido");
        }
    } else {
        server.send(400, "text/plain", "Faltando parametro state");
    }
}




// 1. Função auxiliar para determinar o tipo de arquivo
String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

// 2. Função que lê do SD e envia ao cliente
bool loadFromSD(String path) {
  String dataType = getContentType(path);
  
  if (path == "/") path = "/data/index.html";
  else if (!path.startsWith("/data/")) path = "/data" + path;

  if (SD_MMC.exists(path)) {
    File dataFile = SD_MMC.open(path, "r");
    server.streamFile(dataFile, dataType);
    dataFile.close();
    return true;
  }
  return false;
}

// 3. Configuração do Servidor
void setupWebServer() {

  // Rota para a comunicação com a ESP32-S3
  server.on("/api/power", HTTP_GET, handlePowerToggle);

  // Rota para processar o comando PWM vindo do site
  server.on("/setPWM", HTTP_GET, []() {
    if (server.hasArg("ch") && server.hasArg("val")) {
      int canal = server.arg("ch").toInt();
      int valor = server.arg("val").toInt();
      
      // Limita o valor entre 0 e 4095 para o PCA9685
      if (valor < 0) valor = 0;
      if (valor > 4095) valor = 4095;

      // Comando real para o hardware
      pwm.setChannelPWM(canal, valor);
      
      Serial.printf("Comando PWM: Canal %d, Valor %d\n", canal, valor);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Parametros ausentes");
    }
  });

// --- NOVA ROTA API: Retorna o log do KPI com margem de 3 minutos ---
  server.on("/api/kpi/sta_sarf", HTTP_GET, []() {
    String jsonResposta = "{\"valor\": null}"; // Resposta padrão se não achar nada
    
    if (db != nullptr) {
      // Query Otimizada para ESP32: 
      // Busca o último valor (DESC LIMIT 1) inserido nos últimos 3 minutos
      const char* sql = 
        "SELECT valor FROM monitor_TMPTF_STA "
        "WHERE datahora >= datetime('now', 'localtime', '-3 minutes') "
        "AND datahora <= datetime('now', 'localtime', '+3 minutes') "
        "ORDER BY datahora DESC LIMIT 1;";
        
      sqlite3_stmt* stmt;

      if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
          float valorAtual = sqlite3_column_double(stmt, 0);
          jsonResposta = "{\"valor\": " + String(valorAtual) + "}";
        }
        sqlite3_finalize(stmt);
      } else {
        Serial.printf("SQLite: Erro na API do KPI: %s\n", sqlite3_errmsg(db));
      }
    }

    // Libera o CORS para o Live Server do VS Code funcionar
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonResposta);
  });


// --- NOVA ROTA API: Retorna Status, Boot e Uptime ---
  server.on("/api/system_status", HTTP_GET, []() {
    String bootTime = "Desconhecido";
    
    if (db != nullptr) {
      // Busca a data/hora do boot no SQLite
      const char* sql = "SELECT valor FROM configuracoes WHERE sigla = 'START_BOOT_ESP32P4CWEB';";
      sqlite3_stmt* stmt;

      if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
          bootTime = String((const char*)sqlite3_column_text(stmt, 0));
        }
        sqlite3_finalize(stmt);
      }
    }

    // millis() retorna o tempo ligado em milissegundos. Dividimos por 1000 para segundos.
    unsigned long uptimeSec = millis() / 1000;

    // Monta o JSON de resposta
    String jsonResposta = "{\"status\":\"OPERACIONAL\", \"boot_time\":\"" + bootTime + "\", \"uptime_sec\":" + String(uptimeSec) + "}";

    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", jsonResposta);
  });


  // --- NOVA ROTA API: Retorna a data e hora atual do ESP32 ---
    server.on("/api/time", HTTP_GET, []() {
      time_t now;
      struct tm timeinfo;
      time(&now);
      localtime_r(&now, &timeinfo);

      char buffer[30];
      // Formata no padrão: DD/MM/YYYY HH:MM:SS
      strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeinfo);

      String jsonResposta = "{\"hora_atual\": \"" + String(buffer) + "\"}";

      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "application/json", jsonResposta);
    });

  // -----------------------------------------------------------
  // Handler para arquivos do SD
  server.onNotFound([]() {
    if (!loadFromSD(server.uri())) {
      server.send(404, "text/plain", "Arquivo nao encontrado no MicroSD");
    }
  });

  server.begin();
  Serial.println("HTTP: Servidor iniciado");
}



void handleWebServer() {
  server.handleClient();
}