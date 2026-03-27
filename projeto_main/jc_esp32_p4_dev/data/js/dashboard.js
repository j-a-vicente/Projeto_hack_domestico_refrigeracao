let currentUptimeSec = 0;
let uptimeInterval = null;
let systemSyncInterval = null;

// Função para formatar segundos no formato hh:mm:ss
function formatUptime(seconds) {
    const h = Math.floor(seconds / 3600);
    const m = Math.floor((seconds % 3600) / 60);
    const s = seconds % 60;
    
    // Adiciona um zero à esquerda se for menor que 10
    return [h, m, s].map(v => v < 10 ? "0" + v : v).join(":");
}

// Busca os dados do ESP32 (Lembrete: /api/system_status)
function fetchSystemStatus() {
    fetch('/api/system_status')
        .then(response => response.json())
        .then(data => {
            const statusEl = document.getElementById('sys-status');
            const bootEl = document.getElementById('sys-boot');
            const uptimeEl = document.getElementById('sys-uptime');

            if(statusEl) statusEl.innerText = data.status;
            if(bootEl) bootEl.innerText = data.boot_time;
            
            // Atualiza a variável global com o valor real da placa
            currentUptimeSec = data.uptime_sec; 
            if(uptimeEl) uptimeEl.innerText = formatUptime(currentUptimeSec);
        })
        .catch(error => {
            console.error('Erro ao buscar status:', error);
            const statusEl = document.getElementById('sys-status');
            if(statusEl) {
                statusEl.innerText = "FALHA";
                statusEl.style.background = "#e74c3c";
            }
        });
}

// === FUNÇÃO DE ENTRADA ===
// Só é chamada quando a página home.html for realmente carregada na tela
function inicializarDashboardHome() {
    // 1. Busca os dados iniciais
    fetchSystemStatus();

    // 2. Limpa intervalos fantasmas (segurança)
    pararDashboardHome();

    // 3. Faz o relógio do Uptime "bater" a cada segundo localmente
    uptimeInterval = setInterval(() => {
        currentUptimeSec++;
        const uptimeEl = document.getElementById('sys-uptime');
        if(uptimeEl) uptimeEl.innerText = formatUptime(currentUptimeSec);
    }, 1000);

    // 4. A cada 5 minutos, sincroniza novamente com a placa
    systemSyncInterval = setInterval(fetchSystemStatus, 300000);
}

// === FUNÇÃO DE SAÍDA ===
// Chamada quando o usuário sai da home, poupando a memória do navegador
function pararDashboardHome() {
    if (uptimeInterval !== null) {
        clearInterval(uptimeInterval);
        uptimeInterval = null;
    }
    if (systemSyncInterval !== null) {
        clearInterval(systemSyncInterval);
        systemSyncInterval = null;
    }
}