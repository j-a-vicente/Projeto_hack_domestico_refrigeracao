#include <Arduino.h>
#include "SD_MMC.h"
#include "config_sd.h"
#include <sqlite3.h>
#include <string>

// Ponteiro global para o banco de dados
sqlite3* db; 

bool setupSDCard() {
    Serial.println("SD: Iniciando interface SDMMC...");

    // No ESP32-P4, o SDMMC usa pinos específicos. 
    // O begin() sem parâmetros usa a configuração padrão da placa selecionada.
    if(!SD_MMC.begin()){
        Serial.println("SD: Falha ao montar o cartão. Verifique se está inserido.");
        return false;
    }

    uint8_t cardType = SD_MMC.cardType();
    if(cardType == CARD_NONE){
        Serial.println("SD: Nenhum cartão encontrado no slot.");
        return false;
    }

    Serial.print("SD: Tipo do Cartão: ");
    if(cardType == CARD_MMC) Serial.println("MMC");
    else if(cardType == CARD_SD) Serial.println("SDSC");
    else if(cardType == CARD_SDHC) Serial.println("SDHC");
    else Serial.println("Desconhecido");

    uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
    Serial.printf("SD: Capacidade: %llu MB\n", cardSize);
    
    return true;
}

void readFile(fs::FS &fs, const char * path) {
    File file = fs.open(path);
    if(!file){
        Serial.printf("SD: Arquivo %s nao encontrado.\n", path);
        return;
    }

    Serial.println("SD: Lendo arquivo...");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
    Serial.println("\nSD: Leitura concluida.");
}

void criarTabelasDeMonitoramento(sqlite3* db) {
    const char* sqlBusca = 
        "SELECT DISTINCT t.sigla, REPLACE(e.nome, ' ', '_') AS nome_limpo "
        "FROM objeto_monitor m "
        "JOIN objetos e ON m.id_executor = e.id_objeto "
        "JOIN objeto_tipo t ON e.id_tipo = t.id_tipo "
        "WHERE m.ativo = 1;";

    sqlite3_stmt* stmt;
    
    // Prepara a consulta
    if (sqlite3_prepare_v2(db, sqlBusca, -1, &stmt, NULL) == SQLITE_OK) {
        
        // Itera sobre todos os executores encontrados
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* sigla = sqlite3_column_text(stmt, 0);
            const unsigned char* nome = sqlite3_column_text(stmt, 1);
            
            // Monta o nome da tabela dinamicamente: monitor_SIGLA_NOME
            std::string nomeTabela = "monitor_" + std::string((const char*)sigla) + "_" + std::string((const char*)nome);
            
            // Monta o comando CREATE TABLE
            std::string sqlCreate = 
                "CREATE TABLE IF NOT EXISTS " + nomeTabela + " ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "id_objeto INTEGER, "
                "datahora DATETIME DEFAULT CURRENT_TIMESTAMP, "
                "objeto TEXT, "
                "valor REAL, "
                "FOREIGN KEY (id_objeto) REFERENCES objetos (id_objeto) ON DELETE CASCADE"
                ");";
            
            // Executa a criação da tabela
            char* errMsg = nullptr;
            if (sqlite3_exec(db, sqlCreate.c_str(), 0, 0, &errMsg) != SQLITE_OK) {
                Serial.printf("SQLite: Erro ao criar tabela %s: %s\n", nomeTabela.c_str(), errMsg);
                sqlite3_free(errMsg);
            } else {
                Serial.printf("SQLite: Tabela %s verificada/criada com sucesso.\n", nomeTabela.c_str());
            }
        }
    } else {
        Serial.println("SQLite: Erro ao preparar a consulta de monitores ativos.");
    }
    sqlite3_finalize(stmt);
}


void verificarHoraSQLite(sqlite3* db) {
    Serial.println("\n--- Verificando Relógio Interno do SQLite ---");
    
    // Consulta a hora UTC (padrão do banco) e a Hora Local (corrigida pelo fuso do ESP32)
    const char* sql = "SELECT datetime('now') AS hora_utc, datetime('now', 'localtime') AS hora_local;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* horaUTC = sqlite3_column_text(stmt, 0);
            const unsigned char* horaLocal = sqlite3_column_text(stmt, 1);
            
            Serial.printf(" -> Hora UTC (Padrão SQLite) : %s\n", (const char*)horaUTC);
            Serial.printf(" -> Hora Local (Fuso Brasil): %s\n", (const char*)horaLocal);
        }
    } else {
        Serial.printf("Erro ao consultar a hora no banco: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    Serial.println("---------------------------------------------\n");
}


void listarTabelasDoBanco(sqlite3* db) {
    Serial.println("\n--- Listando Tabelas no Banco de Dados ---");
    
    // Consulta a tabela de sistema do SQLite ignorando as tabelas internas (sqlite_sequence, etc)
    const char* sql = "SELECT name FROM sqlite_master WHERE type='table' AND name NOT LIKE 'sqlite_%';";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        int count = 0;
        
        // Loop passando por cada tabela encontrada
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const unsigned char* nomeTabela = sqlite3_column_text(stmt, 0);
            Serial.printf(" -> %s\n", (const char*)nomeTabela);
            count++;
        }
        
        if (count == 0) {
            Serial.println(" -> Nenhuma tabela encontrada no banco.");
        } else {
            Serial.printf(" -> Total: %d tabela(s).\n", count);
        }
    } else {
        Serial.printf("Erro ao buscar tabelas: %s\n", sqlite3_errmsg(db));
    }
    sqlite3_finalize(stmt);
    Serial.println("------------------------------------------\n");
}





// NOVA FUNÇÃO: Abre o banco e dispara a criação das tabelas
bool iniciarBancoDeDados() {
    Serial.println("SQLite: Abrindo banco de dados...");
    
    // Caminho do banco no SD (o VFS do ESP32 geralmente monta o SD_MMC em /sdcard)
    // Usando a pasta /data conforme a estrutura do seu projeto
    if (sqlite3_open("/sdcard/data/database/banco_skyonix.db", &db) == SQLITE_OK) {
        Serial.println("SQLite: Banco de dados aberto com sucesso!");
        
        // 3. EXIBE NO SERIAL O QUE REALMENTE EXISTE NO BANCO AGORA
        listarTabelasDoBanco(db);
        
        // CHAME A VERIFICAÇÃO AQUI
        verificarHoraSQLite(db);

        Serial.println("SQLite: Inicializando tabelas dinâmicas...");
        criarTabelasDeMonitoramento(db);
        
        return true;
    } else {
        Serial.printf("SQLite: Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return false;
    }
}