# 📘 Documentação Técnica – Pipeline `pl_extract_tables`

## 1. Visão Geral

O pipeline **`pl_extract_tables`** foi desenvolvido no Azure Data Factory com o objetivo de realizar a ingestão de dados do banco PostgreSQL (SIMEC), persistindo-os no Data Lake em formato **Parquet** e posteriormente estruturando-os no Azure Databricks.

### 🎯 Objetivos do pipeline

* Extrair dados do banco PostgreSQL (SIMEC)
* Persistir os dados no Data Lake (camada Bronze)
* Executar notebook para criação de tabelas no Databricks

---

## 2. Arquitetura do Pipeline

### Fluxo de execução:

1. Leitura do JSON de configuração (lista de tabelas)
2. Iteração sobre cada tabela
3. Extração via query SQL
4. Gravação em Parquet no Data Lake
5. Execução de notebook no Databricks

---

## 3. Componentes do Pipeline

## 3.1 Activity: `list_table` (Lookup)

### 📌 Função

Responsável por carregar o arquivo JSON contendo a lista de tabelas a serem extraídas.

### 🔧 Detalhes técnicos

* Tipo: `Lookup`
* Fonte: JSON no Data Lake
* Dataset: `ls_table_simec_prd`
* Caminho:

  ```
  bronze-fnde/Obras/config/lista_tabelas_extracao.json
  ```

### 📥 Estrutura esperada do JSON

```json
{
  "datasetorigem": {
    "tabelas": [
      {
        "nome_tabela": "...",
        "schema_origem": "...",
        "query_sql": "..."
      }
    ]
  }
}
```

### 📌 Saída utilizada

```adf
@activity('list_table').output.firstRow.datasetorigem.tabelas
```

---

## 3.2 Activity: `ForEachTables` (Loop)

### 📌 Função

Executa um loop sobre todas as tabelas retornadas pelo `Lookup`.

### 🔧 Configuração

* Tipo: `ForEach`
* Dependência: `list_table` (Succeeded)
* Iteração baseada em:

  ```adf
  @activity('list_table').output.firstRow.datasetorigem.tabelas
  ```

---

## 3.3 Activity interna: `cp_tables_datalake` (Copy)

### 📌 Função

Executa a extração dos dados do PostgreSQL e grava no Data Lake em formato Parquet.

### 🔧 Configurações principais

#### 🔹 Source (Origem)

* Tipo: `PostgreSqlV2Source`
* Query dinâmica:

  ```adf
  @item().query_sql
  ```
* Timeout: 2 horas

#### 🔹 Sink (Destino)

* Tipo: `Parquet`
* Compressão: `snappy`
* Caminho:

  ```
  bronze-fnde/Obras/simec_tabelas
  ```

#### 🔹 Nome do arquivo gerado

```adf
@concat(item().schema_origem,'_',item().nome_tabela, '.parquet')
```

✔ Exemplo:

```
public_usuarios.parquet
```

#### 🔹 Retry

* Tentativas: 10
* Intervalo: 30 segundos

---

## 3.4 Activity: `not_criar_tabelas_catalogo_databricks`

### 📌 Função

Executa um notebook no Databricks para criar tabelas a partir dos arquivos Parquet.

### 🔧 Configuração

* Tipo: `DatabricksNotebook`
* Caminho do notebook:

  ```
  /Shared/FNDE/Bronze/simec/not_criar_tabelas_catalogo_databricks
  ```
* Executa após:

  * Sucesso OU falha do `ForEachTables`

👉 Isso garante execução mesmo se alguma tabela falhar.

---

## 4. Datasets Utilizados

### 4.1 `ls_table_simec_prd`

* Tipo: JSON
* Origem: Data Lake
* Contém lista de tabelas e queries

---

### 4.2 `ds_simec_postgresql`

* Tipo: PostgreSQL
* Origem: Banco SIMEC
* Linked Service: `ls_simec_espelho_producao`

---

### 4.3 `ds_bronze_parquet_simec_list`

* Tipo: Parquet
* Destino: Data Lake
* Parametrizado (nome do arquivo dinâmico)

---

## 5. Parâmetros do Pipeline

| Parâmetro                    | Descrição                 |
| ---------------------------- | ------------------------- |
| factoryName                  | Nome do Data Factory      |
| AzureDatabricks_simec_tables | Linked Service Databricks |
| ls_stobronze_fnde_prd        | Storage Data Lake         |
| ls_simec_espelho_producao    | Conexão PostgreSQL        |

---

## 6. Estratégia de Execução

### 🔄 Padrão de processamento

* Controle central via JSON
* Execução dinâmica de queries
* Pipeline genérico (reutilizável)
* Processamento paralelo (dependendo config do ForEach)

---

## 7. Camadas de Dados

### Bronze (Data Lake)

* Dados brutos extraídos
* Formato: Parquet
* Compressão: Snappy

### Silver/Gold (Databricks)

* Tabelas estruturadas
* Governança e consulta analítica

---

## 8. Tratamento de Falhas

* Retry automático no Copy (10 tentativas)
* Execução do notebook mesmo com falhas parciais
* Falhas isoladas por tabela (não interrompe o pipeline inteiro)

---

## 9. Boas Práticas Implementadas

✔ Parametrização completa
✔ Separação de configuração (JSON externo)
✔ Uso de Parquet (performance e compressão)
✔ Pipeline desacoplado (ADF + Databricks)
✔ Reprocessamento simples (alterando JSON)

---

## 10. Considerações Técnicas

* Escalável para centenas de tabelas
* Permite carga full ou incremental (dependendo da query)
* Dependente da qualidade das queries no JSON
* Possível evolução:

  * Controle de logs
  * Monitoramento de performance
  * Particionamento de dados

---

## 11. Conclusão

O pipeline `pl_extract_tables` implementa um padrão moderno de ingestão de dados baseado em:

* Orquestração com Azure Data Factory
* Armazenamento em Data Lake
* Processamento com Azure Databricks

---


