function inicializarGraficos() {
    console.log("Iniciando carregamento do gráfico...");
    
    // É importante usar async/await ou .then aqui dentro
    fetch('logs/SCDC/SCDC0_01032026.log')
        .then(response => response.text())
        .then(csvTexto => {
            const somaWatts = Array(24).fill(0);
            const somaAmpere = Array(24).fill(0);
            const contagemHoras = Array(24).fill(0);

            const linhas = csvTexto.split('\n');
            for (let i = 1; i < linhas.length; i++) {
                const linha = linhas[i].replace(/"/g, '').trim();
                if (!linha) continue;
                const partes = linha.split(',');
                const dataHora = partes[0];
                const valorOriginal = parseFloat(partes[2]);
                const hora = parseInt(dataHora.substring(8, 10));

                if (!isNaN(hora) && !isNaN(valorOriginal)) {
                    somaWatts[hora] += (valorOriginal * 12);
                    somaAmpere[hora] += valorOriginal;
                    contagemHoras[hora]++;
                }
            }

            const dadosWatts = somaWatts.map((s, i) => contagemHoras[i] > 0 ? (s / contagemHoras[i]).toFixed(2) : 0);
            const dadosAmpere = somaAmpere.map((s, i) => contagemHoras[i] > 0 ? (s / contagemHoras[i]).toFixed(2) : 0);

            renderizarChartJS(dadosWatts, dadosAmpere);
        });
}
function renderizarChartJS(watts, amperes) {
    const canvas = document.getElementById('meuGrafico');
    if (!canvas) return;
    
    const ctx = canvas.getContext('2d');

    // Destruir gráfico anterior para evitar bugs de memória no ESP32
    if (window.chartMosfet) window.chartMosfet.destroy();

    window.chartMosfet = new Chart(ctx, {
        type: 'line',
        plugins: [ChartDataLabels], 
        data: {
            labels: Array.from({length: 24}, (_, i) => i + "h"),
            datasets: [
                { 
                    label: 'Watts', 
                    data: watts, 
                    borderColor: '#4bc0c0', 
                    fill: true, 
                    backgroundColor: 'rgba(75, 192, 192, 0.2)',
                    datalabels: {
                        display: true, 
                        align: 'top',
                        backgroundColor: '#2c3e50',
                        borderRadius: 4,
                        color: 'white',
                        font: { weight: 'bold', size: 10 },
                        formatter: (value) => value > 0 ? value + ' W' : '' 
                    }
                },
                { 
                    label: 'Ampere', 
                    data: amperes, 
                    borderColor: '#9966ff', 
                    fill: false,
                    datalabels: { display: false } 
                },
                { 
                    label: 'Limite (15W)', 
                    data: Array(24).fill(15), 
                    borderColor: 'red', 
                    borderDash: [5, 5], 
                    pointRadius: 0,
                    datalabels: { display: false } 
                }
            ]
        },
        options: { 
            responsive: true,
            maintainAspectRatio: false,
            layout: {
                padding: { 
                    top: 30,
                    bottom: 10 // Pequeno espaço extra na base
                } 
            },
            scales: { 
                y: { beginAtZero: true } 
            },
            plugins: {
                // CONFIGURAÇÃO DA LEGENDA:
                legend: {
                    display: true,
                    position: 'bottom', // Move a legenda para baixo
                    labels: {
                        padding: 20, // Espaçamento entre a legenda e o gráfico
                        usePointStyle: true // Deixa o marcador da legenda redondo/estilizado
                    }
                },
                datalabels: {
                    offset: 8
                }
            }
        }
    });
}



// kpi-wrapper
function atualizarKPI() {
    fetch('/api/kpi/sta_sarf')
        .then(response => {
            if (!response.ok) throw new Error('Erro na resposta da rede');
            return response.json();
        })
        .then(dados => {
            if (dados && dados.valor !== null) {
                const valorEncontrado = parseFloat(dados.valor);

                // Atualiza o texto com o símbolo de graus º
                document.getElementById('kpi-value').innerText = valorEncontrado.toFixed(1) + "º";
                
                // Atualiza o gráfico Gauge
                renderizarGauge(valorEncontrado);
            } else {
                console.warn("Nenhum dado de temperatura encontrado nos últimos 3 minutos.");
                document.getElementById('kpi-value').innerText = "--º"; // Exibe traços se não achar dado
            }
        })
        .catch(err => console.error("Erro ao buscar KPI no banco de dados:", err));
}



function renderizarGauge(valorAtual) {
    const canvas = document.getElementById('gaugeSTAX0');
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    
    // Trava o limite em 45
    const valorGrafico = valorAtual > 45 ? 45 : valorAtual;

    if (window.chartKPI) window.chartKPI.destroy();

    window.chartKPI = new Chart(ctx, {
        type: 'doughnut',
        data: {
            datasets: [
                {
                    // Fundo (Zonas)
                    data: [20, 15, 10],
                    backgroundColor: ['#4CAF50', '#FFC107', '#F44336'],
                    weight: 0.5,
                    borderWidth: 0
                },
                {
                    // Ponteiro (Valor Atual)
                    data: [valorGrafico, 45 - valorGrafico],
                    backgroundColor: ['#2c3e50', 'transparent'],
                    borderWidth: 2,
                    borderColor: '#fff',
                    weight: 1
                }
            ]
        },
        options: {
            responsive: true,
            maintainAspectRatio: true,
            aspectRatio: 1.8, // Força uma proporção mais achatada para o canvas
            circumference: 180,
            rotation: 270,
            cutout: '75%',
            layout: {
                padding: {
                    top: -40,    // Puxa o gráfico para bem perto do título
                    bottom: 0    // Remove o recuo exagerado de baixo
                }
            },
            plugins: {
                legend: { display: false },
                tooltip: { enabled: false }
            }
        }
    });
}
/* //função que busca os valores no arquivo de log
function inicializarKPI() {
    fetch('logs/STA/STAX0_01032026.log')
        .then(response => response.text())
        .then(csvTexto => {
            const linhas = csvTexto.trim().split('\n');
            if (linhas.length < 2) return;

            const agora = new Date();
            const horaAtualStr = String(agora.getHours()).padStart(2, '0');
            
            let valorEncontrado = null;

            // Percorre de trás para frente para pegar o registro mais recente da hora atual
            for (let i = linhas.length - 1; i >= 1; i--) {
                const colunas = linhas[i].replace(/"/g, '').split(',');
                const dataHora = colunas[0]; // Formato: 20260301050000
                const horaNoLog = dataHora.substring(8, 10);

                if (horaNoLog === horaAtualStr) {
                    valorEncontrado = parseFloat(colunas[2]);
                    break;
                }
            }

            // Fallback: se não tiver dados da hora atual no log, pega o último valor do arquivo
            if (valorEncontrado === null) {
                const ultimaLinha = linhas[linhas.length - 1].replace(/"/g, '').split(',');
                valorEncontrado = parseFloat(ultimaLinha[2]);
            }

            // Atualiza o texto com o símbolo de graus º
            document.getElementById('kpi-value').innerText = valorEncontrado.toFixed(1) + "º";
            
            renderizarGauge(valorEncontrado);
        })
        .catch(err => console.error("Erro ao carregar log de KPI:", err));
}
*/
