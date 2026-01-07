# ESP32-S3 – Documentação Técnica, Pinagem e Homologação

## 1. Objetivo do Documento

Este documento tem como objetivo padronizar a documentação técnica de projetos baseados no **ESP32-S3**, atendendo a requisitos de **desenvolvimento**, **auditoria técnica** e **processos de homologação**. O conteúdo aqui descrito é aplicável a placas **customizadas** baseadas no módulo **ESP32-S3-WROOM**.

---

## 2. Identificação do Hardware

* **Microcontrolador:** ESP32-S3
* **Módulo:** ESP32-S3-WROOM-1
* **Arquitetura:** Dual-core (Xtensa LX7)
* **Conectividade:** Wi-Fi 2.4 GHz / Bluetooth Low Energy (BLE)
* **Revisão do silício:** v0.2
* **Clock da CPU:** 160 MHz
* **Flash externa configurada:** 2 MB (SPI)
* **Heap livre mínimo:** 392.752 bytes

---

## 3. Ambiente de Software

* **Framework:** ESP-IDF
* **Versão do ESP-IDF:** v5.5.2
* **Bootloader:** Multicore (2nd stage)
* **Data de compilação do bootloader:** 28/12/2025
* **Sistema operacional:** FreeRTOS (embarcado no ESP-IDF)

---

## 4. Configuração de Boot e Flash

* **Modo de boot:** SPI_FAST_FLASH_BOOT
* **Modo SPI:** DIO
* **Clock SPI:** 80 MHz
* **Fabricante da Flash:** MXIC

> **Nota de Homologação:**
> O chip Flash detectado possui capacidade superior (16 MB), porém o firmware está deliberadamente configurado para utilizar apenas **2 MB**, conforme política de particionamento definida no projeto.

---

## 5. Tabela de Partições

| Label    | Uso         | Tipo | Subtipo | Offset     | Tamanho    |
| -------- | ----------- | ---- | ------- | ---------- | ---------- |
| nvs      | Dados Wi-Fi | 01   | 02      | 0x00009000 | 0x00006000 |
| phy_init | Dados RF    | 01   | 01      | 0x0000F000 | 0x00001000 |
| factory  | Aplicação   | 00   | 00      | 0x00010000 | 0x00100000 |

---

## 6. Pinagem da Placa (Placa Customizada)

> **Importante:** A pinagem abaixo refere-se à **placa desenvolvida internamente**, não ao DevKit oficial da Espressif. A validação deve sempre considerar o esquemático elétrico do projeto.

### 6.1 UART (Console / Programação)

| Função       | GPIO    |
| ------------ | ------- |
| TX (Console) | GPIO 44 |
| RX (Console) | GPIO 43 |

---

### 6.2 I2C (Sensores / Expansores)

| Função | GPIO    |
| ------ | ------- |
| SDA    | GPIO __ |
| SCL    | GPIO __ |

> Preencher conforme definição do projeto elétrico.

---

### 6.3 SPI (Flash / SD / Sensores)

| Função | GPIO    |
| ------ | ------- |
| MOSI   | GPIO __ |
| MISO   | GPIO __ |
| SCK    | GPIO __ |
| CS     | GPIO __ |

---

### 6.4 GPIOs de Uso Geral

| GPIO    | Função no Projeto      |
| ------- | ---------------------- |
| GPIO __ | Relé / MOSFET / Sensor |
| GPIO __ | Relé / MOSFET / Sensor |

---

## 7. Inicialização da Aplicação

* **Nome do projeto:** primeiro_projeto
* **Versão:** 1
* **Task principal:** main_task
* **Função de entrada:** app_main()
* **CPU inicial:** CPU0

---

## 8. Gerenciamento de Memória

| Região  | Tamanho | Tipo              |
| ------- | ------- | ----------------- |
| RAM     | 337 KB  | RAM interna       |
| RAM     | 21 KB   | RAM interna       |
| DRAM    | 32 KB   | Dados             |
| RTC RAM | 7 KB    | Retenção em sleep |

---

## 9. Gerenciamento de Energia

* Isolamento automático de GPIOs em modo sleep
* Suporte a modos de baixo consumo
* RTC RAM preservada durante sleep profundo

---

## 10. Interfaces de Programação e Debug

### 10.1 UART

* Programação de firmware
* Logs de execução
* Comunicação serial com sistemas externos

### 10.2 JTAG / USB-JTAG

* Debug passo a passo
* Breakpoints
* Inspeção de memória e registradores
* Suporte a depuração profissional sem hardware externo adicional

---

## 11. Conformidade para Auditoria Técnica

Este projeto atende aos seguintes critérios:

* Uso de bootloader oficial Espressif
* Framework suportado e atualizado
* Particionamento explícito de memória
* Log detalhado de boot e versão
* Capacidade de debug via UART e JTAG

---

## 12. Template Padrão para Novos Projetos ESP32-S3

> **Checklist de Documentação**

* [ ] Identificação do hardware
* [ ] Revisão do silício
* [ ] Versão do ESP-IDF
* [ ] Configuração de Flash
* [ ] Tabela de partições
* [ ] Pinagem validada
* [ ] Estratégia de debug
* [ ] Estratégia de atualização (OTA ou local)
* [ ] Requisitos de energia
* [ ] Critérios de homologação

---

## 13. Observações Finais

Este documento deve ser versionado junto ao código-fonte do projeto e atualizado sempre que houver:

* Alteração de hardware
* Mudança de framework
* Revisão de layout da placa

---

**Documento técnico aplicável a auditorias internas, homologação e manutenção de longo prazo.**
