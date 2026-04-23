let currentUptimeSec = 0;
let uptimeInterval = null;
let systemSyncInterval = null;
let dynamicDataInterval = null; // Novo intervalo para dados dinâmicos (ativos e slave)

// Função para formatar segundos no formato hh:mm:ss
function formatUptime(seconds) {
    const h = Math.floor(seconds / 3600);
    const m = Math.floor((seconds % 3600) / 60);
    const s = seconds % 60;
    
    return [h, m, s].map(v => v < 10 ? "0" + v : v).join(":");
}

// Busca os dados gerais do sistema do ESP32 Master (/api/system_status)
function fetchSystemStatus() {
    fetch('/api/system_status')
        .then(response => response.json())
        .then(data => {
            const statusEl = document.getElementById('sys-status');
            const bootEl = document.getElementById('sys-boot');
            const uptimeEl = document.getElementById('sys-uptime');

            if(statusEl) {
                statusEl.innerText = data.status; // O C++ já envia "ONLINE"
                statusEl.style.background = "#2ecc71"; // Garante cor verde
            }
            if(bootEl) bootEl.innerText = data.boot_time;
            
            // Atualiza a variável global com o valor real da placa
            currentUptimeSec = data.uptime_sec; 
            if(uptimeEl) uptimeEl.innerText = formatUptime(currentUptimeSec);
        })
        .catch(error => {
            console.error('Erro ao buscar status do sistema:', error);
            const statusEl = document.getElementById('sys-status');
            if(statusEl) {
                statusEl.innerText = "OFFLINE"; // Trocado de "FALHA" para "OFFLINE"
                statusEl.style.background = "#e74c3c";
            }
        });
}

// Busca o status de conexão da placa Slave (/api/slave_status)
function fetchSlaveStatus() {
    fetch('/api/slave_status')
        .then(response => response.json())
        .then(data => {
            const statusElement = document.getElementById('slave-status');
            if(!statusElement) return;

            if (data.online) {
                statusElement.innerText = "ONLINE"; // Trocado de "Slaver ONLINE"
                statusElement.style.background = "#2ecc71"; // Verde
            } else {
                statusElement.innerText = "OFFLINE"; // Trocado de "Slaver OFFLINE"
                statusElement.style.background = "#e74c3c"; // Vermelho
            }
        })
        .catch(error => console.error('Erro ao buscar status da escrava:', error));
}

// Busca a lista de ativos e seus status do banco de dados (/api/ativos)
function fetchAtivos() {
    fetch('/api/ativos')
        .then(response => response.json())
        .then(dadosAtivos => {
            const lista = document.getElementById('lista-ativos');
            if(!lista) return;

            lista.innerHTML = ''; // Limpa a mensagem de carregamento

            dadosAtivos.forEach((ativo, index) => {
                const isOnline = ativo.Status === 1;
                const borderStyle = index === dadosAtivos.length - 1 ? '' : 'border-bottom: 1px solid #eee;';
                
                const li = document.createElement('li');
                li.style = `padding: 15px 0; ${borderStyle} display: flex; justify-content: space-between; align-items: center;`;
                
                const statusHTML = isOnline 
                    ? '<span style="color: #2ecc71; font-weight: bold;">🟢 ON</span>' 
                    : '<span style="color: #95a5a6; font-weight: bold;">⚪ OFF</span>';

                li.innerHTML = `<span>${ativo.Ativos}</span>${statusHTML}`;
                lista.appendChild(li);
            });
        })
        .catch(error => {
            console.error('Erro ao buscar ativos:', error);
            const lista = document.getElementById('lista-ativos');
            if(lista) lista.innerHTML = '<li style="text-align:center; color:red;">Erro ao carregar dados</li>';
        });
}

// === FUNÇÃO DE ENTRADA ===
// Chamada quando a página home for carregada
function inicializarDashboardHome() {
    // 1. Limpa intervalos fantasmas (segurança)
    pararDashboardHome();

    // 2. Busca os dados iniciais na hora em que abre a tela
    fetchSystemStatus();
    fetchSlaveStatus();
    fetchAtivos();

    // 3. Faz o relógio do Uptime "bater" a cada segundo localmente
    uptimeInterval = setInterval(() => {
        currentUptimeSec++;
        const uptimeEl = document.getElementById('sys-uptime');
        if(uptimeEl) uptimeEl.innerText = formatUptime(currentUptimeSec);
    }, 1000);

    // 4. A cada 5 segundos, atualiza apenas os dados dinâmicos (Slave e Ativos)
    dynamicDataInterval = setInterval(() => {
        fetchSlaveStatus();
        fetchAtivos();
    }, 5000);

    // 5. A cada 5 minutos, sincroniza o uptime geral novamente com a placa
    systemSyncInterval = setInterval(fetchSystemStatus, 300000);
}

// === FUNÇÃO DE SAÍDA ===
// Chamada quando o usuário sai da home
function pararDashboardHome() {
    if (uptimeInterval !== null) {
        clearInterval(uptimeInterval);
        uptimeInterval = null;
    }
    if (systemSyncInterval !== null) {
        clearInterval(systemSyncInterval);
        systemSyncInterval = null;
    }
    if (dynamicDataInterval !== null) {
        clearInterval(dynamicDataInterval);
        dynamicDataInterval = null;
    }
}