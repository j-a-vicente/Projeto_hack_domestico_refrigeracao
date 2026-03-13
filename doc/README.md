# Documentação Técnica da Aplicação

## 1. Visão Geral do Projeto

Este projeto tem como objetivo controlar múltiplos periféricos conectados à placa central **JC-ESP32P4-M3**. A placa atua como um **controlador central** e expõe uma **interface web** via WebServer embarcado no ESP32, permitindo:

* Controle manual dos periféricos
* Visualização de status em tempo real
* Registro de logs operacionais

Os arquivos da interface web (HTML, CSS, JS) são carregados a partir de um **cartão microSD**, no diretório:

* `/data/` – arquivos da interface web
* `/log/` – arquivos de log dos periféricos, no formato:
  `periferico_name_yyyymmddhhmmss.log`

---

## 2. Topologia da Aplicação

### 2.1 Componentes Principais

```
┌────────────────────┐
│ Navegador Web      │
│ (Cliente)          │
└─────────┬──────────┘
          │ HTTP
┌─────────▼──────────┐
│ WebServer ESP32    │  ← start_webserver.cpp
│ (JC-ESP32P4-M3)    │
└─────────┬──────────┘
          │
 ┌────────▼─────────┐
 │ Lógica Principal │  ← jc_esp32_p4_dev.ino
 └────────┬─────────┘
          │
 ┌────────▼─────────┐
 │ Periféricos      │  GPIO / SPI / I2C
 └────────┬─────────┘
          │
 ┌────────▼─────────┐
 │ microSD          │  ← start_sd.cpp
 │ /data /log       │
 └──────────────────┘

┌────────────────────┐
│ Ethernet           │  ← start_ethernet.cpp
└────────────────────┘
```

---

## 3. Organização dos Arquivos

| Arquivo               | Responsabilidade                            |
| --------------------- | ------------------------------------------- |
| `jc_esp32_p4_dev.ino` | Arquivo principal, inicialização do sistema |
| `start_ethernet.cpp`  | Inicialização e configuração da Ethernet    |
| `start_sd.cpp`        | Inicialização do cartão microSD             |
| `start_webserver.cpp` | Criação e gerenciamento do WebServer        |
| `config.h`            | Configurações gerais do projeto             |
| `config_sd.h`         | Configurações específicas do microSD        |
| `config_webserver.h`  | Configurações do WebServer                  |
| `projeto_config.h`    | Parâmetros globais do projeto               |

---

## 4. Fluxo de Inicialização

1. Boot do ESP32
2. Execução do `setup()` em `jc_esp32_p4_dev.ino`
3. Inicialização da Ethernet (`startEthernet()`)
4. Inicialização do microSD (`startSD()`)
5. Inicialização do WebServer (`startWebServer()`)
6. Loop principal (`loop()`)

---

## 5. Dependências e Relações entre Arquivos

* `jc_esp32_p4_dev.ino`

  * Chama funções definidas em:

    * `start_ethernet.cpp`
    * `start_sd.cpp`
    * `start_webserver.cpp`

* Arquivos `config*.h`

  * São incluídos e herdados por múltiplos módulos
  * Não executam lógica, apenas fornecem parâmetros

---

## 6. Funções Existentes e Documentação Detalhada

### 6.1 Arquivo: `jc_esp32_p4_dev.ino`

#### Função: `setup()`

**Objetivo:**
Inicializar todos os subsistemas do projeto.

```cpp
void setup() {
    Serial.begin(115200);            // Inicializa comunicação serial para debug

    startEthernet();                 // Inicializa a interface Ethernet
    startSD();                       // Inicializa o cartão microSD
    startWebServer();                // Inicializa o WebServer HTTP
}
```

---

#### Função: `loop()`

**Objetivo:**
Executar o processamento contínuo do sistema.

```cpp
void loop() {
    // Atualmente vazio
    // Reservado para futuras leituras de sensores
    // e controle contínuo de periféricos
}
```

---

### 6.2 Arquivo: `start_ethernet.cpp`

#### Função: `startEthernet()`

**Objetivo:**
Configurar e iniciar a interface Ethernet do ESP32.

```cpp
void startEthernet() {
    ETH.begin();                     // Inicializa hardware Ethernet

    if (ETH.linkUp()) {              // Verifica se o link está ativo
        Serial.println("Ethernet conectada");
    } else {
        Serial.println("Falha na Ethernet");
    }
}
```

---

### 6.3 Arquivo: `start_sd.cpp`

#### Função: `startSD()`

**Objetivo:**
Montar o sistema de arquivos do cartão microSD.

```cpp
void startSD() {
    if (!SD.begin()) {               // Tenta inicializar o cartão SD
        Serial.println("Falha ao montar SD");
        return;
    }

    Serial.println("SD montado com sucesso");
}
```

---

### 6.4 Arquivo: `start_webserver.cpp`

#### Função: `startWebServer()`

**Objetivo:**
Criar e iniciar o servidor HTTP.

```cpp
void startWebServer() {
    server.on("/", HTTP_GET, []() {         // Rota principal
        server.send(200, "text/html", loadFile("/data/index.html"));
    });

    server.begin();                         // Inicia o servidor
    Serial.println("WebServer iniciado");
}
```

---

#### Função: `loadFile(const char* path)`

**Objetivo:**
Carregar um arquivo do microSD e retorná-lo como string.

```cpp
String loadFile(const char* path) {
    File file = SD.open(path);              // Abre o arquivo solicitado

    if (!file) {
        return "Arquivo não encontrado";
    }

    String content;
    while (file.available()) {
        content += (char)file.read();       // Lê byte a byte
    }

    file.close();                           // Fecha o arquivo
    return content;                         // Retorna o conteúdo
}
```

---

## 7. Estratégia de Logs

* Cada periférico deve gerar seu próprio arquivo de log
* Nome do arquivo:

```
/log/<periferico>_YYYYMMDDHHMMSS.log
```

* Logs armazenam eventos como:

  * Ativação
  * Desativação
  * Erros
  * Status

---

## 8. Pontos de Expansão Futuros

* API REST para controle dos periféricos
* WebSocket para atualização em tempo real
* Autenticação no WebServer
* Rotação automática de logs

---

## 9. Conclusão

Esta documentação descreve de forma completa a arquitetura, topologia, responsabilidades de cada módulo e o detalhamento funcional de todas as funções existentes no projeto atual. O modelo adotado facilita a manutenção, auditoria técnica e expansão futura do sistema.
