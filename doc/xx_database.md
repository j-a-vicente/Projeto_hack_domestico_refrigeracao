


Tabelas:
- objetos: são os items do sistema fisico que serão controlados e monitorados pelo app.
    - id_objeto, tipo int, descrição: pk
    - id_objeto_pai, tipo int, descrição: fk ligada ao pk objeto id_objeto
    - id_objeto_tipo, tipo int, descrição: fk ligado ao pk objeto_tipo id_objeto_tipo
    - objeto_name, tipo string, descrição: nome do objeto
    - status_inicializacao, bit, descrição: com o objeto deve ser está no monento que o sistema liga, 1 ligado, 2 desligado.
    - status_ultimo_status, bit, descrição: como o ojeto estava quando o sistema foi desligado pela ultima vez
    - status_ultima_status_dt, datatime, descrição: data hora da alteração
    - confg_valor, real, descrição: parametro de configuração, valor
    - confg_valor_min, real, descrição: parametro de configuração, valor
    - confg_valor_max, real, descrição: parametro de configuração, valor
    - dt_cadastro, datetime, descrição: data e hora do cadastro
    - dt_alteracao, datetime, descrição: data e hora da alteração do cadastro
    - ativo, bit, descrição: se o objeto está ativo para ser gerenciado pelo sistema

- objeto_historio: historio de alterações no objeto.
    - id_objeto_historio, int, descrição: pk


- objeto_tipo: tabela dicionario, se o objeto é reler, mosfet, sensor de corrente ac ou dc....
    - id_objeto_tipo, int, descrição: pk 
    - objeto_sigla, string, descrição: sigla de letra que vai representar o tipo do objeto.
    - objeto_tipo_nome, string, descrição: nome do tipo do objeto.
    
- objeto_monitor
    - id_objeto_monitor, int, descrição: pk    
    - id_objeto_executor, int, descrição: objeto que vai executar o monitoramento.
    - id_objeto_monitorado, int, descrição: objeto que vai ser monitorador pelo executor.
    - dt_cadastro, datetime, descrição: data e hora do cadastro
    - dt_alteracao, datetime, descrição: data e hora da alteração do cadastro    
    - ativo, bit, descrição: monitoramento ativo 1 ou desativado 0
    

- logs uma para cada objeto, criada autoamticamente, segudno a tabelas objeto_monitor, a coluna id_executor, tem o id do objeto, que será criado a tabela do log de monitoramento o nome da tabela será composta por "monitor_" mais a "sigla" do tipo do objeto e o "nome" do objeto mais,"monitor_" + "SCDC" +  "SARF"
    - colunas:
        - id, int, descrição: pk
        - id_objeto, int, descrição: fk ligada ao pk objeto id_objeto
        - datahora, datatime, descrição: data e hora do registro do log
        - objeto, string, descrição: nome do objeto
        - valor, real, descrição: metrica fornecido pelo objeto.


Regra de consolidação de dados:
- Todos os logs serão consolidados a cada 30 dias


Regra de expurgos de dados:
- todos os logs consolidados a mais de 90 dias serão apagados.