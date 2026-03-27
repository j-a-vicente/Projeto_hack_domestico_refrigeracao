# Documentação de Monitoramento: Painel Home (Rack Hack)

## 1. Introdução

Esta documentação descreve a interface de monitoramento em tempo real do projeto **Hack**. O objetivo desta página é fornecer uma visão centralizada e analítica de todos os componentes críticos do rack, desde o sistema de refrigeração líquida e pastilhas Peltier até o consumo energético AC/DC. A interface foi projetada para permitir a identificação rápida de anomalias e a análise de tendências de eficiência térmica e elétrica.

## 2. Glossário de Siglas e Métricas

Para facilitar a leitura dos gráficos e painéis, utilizamos as seguintes convenções:

### Prefixos de Medição

* **VL (Valor Atual):** Leitura instantânea capturada no último ciclo do sensor.
* **VC (Valor Cumulativo):** Soma total baseada nos últimos 30 dias de operação.
* **VCD (Valor Diário):** Acumulado das últimas 24 horas (00:00 às 23:59:59).

### Componentes e Sensores

| Sigla | Descrição | Sigla | Descrição |
| --- | --- | --- | --- |
| **DG** | Dreno do Degelo | **SNL** | Sensor de Nível |
| **CL** | Cooler (Geral) | **RL** | Relé |
| **CLHOT** | Cooler (Ar Quente) | **P** | Pastilha Peltier |
| **CLCOL** | Cooler (Ar Frio) | **DS** | Distribuidor Central 110V |
| **SCDC** | Sensor de Corrente DC | **DJ** | Disjuntor |
| **SCAC** | Sensor de Corrente AC | **NB** | Nobreak |
| **SCL** | Sensor de Corrente Líquida | **MG** | Gerenciador do Rack |
| **MF** | MOSFET | **CHAVE** | Controle de Fonte Principal |
| **STA** | Sensor de Temperatura Ar | **F / N** | Fase / Neutro |

---

## 3. Estrutura da Página de Monitoramento

### 3.1 Painel de Indicadores (KPIs)

**Objetivo:** Oferecer uma visão imediata da saúde do sistema através de números absolutos.

* **Como interpretar:** Estes valores devem estar dentro das faixas nominais. Números em vermelho indicam que o sistema está operando fora dos limites de segurança (ex: Amperagem acima de 20A ou Watts acima de 4400W).

| Métrica | Escala Operacional |
| --- | --- |
| Temperatura do Rack | -10°C a 40°C |
| Líquido de Refrigeração | -10°C a 100°C |
| Nível do Reservatório | 0% a 100% |
| Potência da Peltier / Cooler | 0% a 100% (PWM) |
| Consumo Elétrico | 0A a 20A / 0W a 4400W |

### 3.2 Gráficos de Evolução Temporal (Linha - VC)

**Objetivo:** Analisar o comportamento do rack ao longo do tempo e identificar padrões de consumo ou falhas de refrigeração.

* **Curva de Temperatura (STA):** Se a linha subir constantemente enquanto a potência da Peltier (P) está em 100%, indica subdimensionamento ou falha térmica.
* **Consumo (SCDC/SCAC):** Permite visualizar picos de partida de motores ou componentes.
* **Fluxo de Líquido (SCL):** Uma linha constante indica estabilidade; quedas bruscas podem sugerir obstruções ou falha na bomba.

### 3.3 Status de Atuadores (Painel On/Off)

**Objetivo:** Mostrar o estado binário (Ativo/Inativo) dos componentes de controle de potência.

* **Componentes:** MOSFETs (MF), Pastilhas (P), Relés (RL) e Coolers (CL).

---

## 4. Gestão de Alertas e Segurança

O sistema monitora criticamente dois pontos de falha que podem causar danos físicos ao hardware:

1. **Alerta de Degelo (DG):** Ativado se houver acúmulo de água no dreno acima do nível de segurança. Indica entupimento ou condensação excessiva.
2. **Nível Crítico (SNL):** Ativado quando o reservatório de fluido refrigerante está abaixo do limite. **Risco de queima da bomba e superaquecimento das pastilhas.**

---

### Como interpretar os dados de forma cruzada?

> **Exemplo de diagnóstico:** Se o gráfico de **SCDC** mostra consumo alto, mas a temperatura no **STA** continua subindo, verifique o status do **MF** (Mosfet) e do **P** (Peltier). Se ambos estiverem "ON", o problema pode ser a troca de calor no radiador (verifique os Coolers **CLHOT/CLCOL**).

---
### Exemplo dos layout:
![alt text](../data/img/pagina_dashboard_exemplo.png)