# Documentação Atualizada - Banco de Dados

## ERD Atualizado
(ERD corrigido com auditoria e grupos N:N)

## Auditoria
Todas as tabelas possuem:
- dt_cadastro
- dt_alteracao

## Tabelas

### objeto_tipo
Define tipos de objetos.

### objetos
Entidades físicas e lógicas do sistema.

### objeto_monitor
Relaciona executor e monitorado.

### logs_historico
Logs brutos de alta frequência.

### logs_consolidados
Dados agregados mensais.

### configuracoes
Parâmetros globais do sistema.

### grupo_objetos
Define grupos lógicos de controle.

### objetos_agrupados
Relacionamento N:N entre grupos e objetos.

## Fluxo de Logs
logs_historico → logs_consolidados → expurgo

