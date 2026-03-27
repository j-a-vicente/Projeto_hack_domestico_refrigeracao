// VARIÁVEL GLOBAL: Guarda a referência do loop para podermos cancelá-lo
let kpiInterval = null; 

// Carrega a página inicial automaticamente e inicia o relógio global da header
document.addEventListener("DOMContentLoaded", () => {
    loadPage('home.html');
    atualizarRelogioESP(); 
    setInterval(atualizarRelogioESP, 1000); 
});

function loadPage(pageName) {
    console.log("Tentando carregar:", pageName);
    const contentDiv = document.getElementById('content');
    
    contentDiv.innerHTML = "Carregando...";

    // SEGURANÇA GERAL: Ao mudar de página, limpa os contadores das outras páginas
    if (kpiInterval !== null) {
        clearInterval(kpiInterval);
        kpiInterval = null;
    }
    // Desliga os contadores da Home (função lá do dashboard.js)
    if (typeof pararDashboardHome === "function") {
        pararDashboardHome();
    }

    fetch(pageName)
        .then(response => {
            if (!response.ok) throw new Error("Não foi possível encontrar o arquivo no SD.");
            return response.text();
        })
        .then(data => {
            contentDiv.innerHTML = data;
            
            // ROTEAMENTO E EXECUÇÃO DE SCRIPTS ESPECÍFICOS

            // 1. Controle PWM
            if (pageName === 'control_pwm.html') {
                if (typeof inicializarPainelPWM === "function") {
                    inicializarPainelPWM();
                }
            }
            
            // 2. Gráficos
            if (pageName === 'charts.html') {
                if (typeof inicializarGraficos === "function") inicializarGraficos();
                if (typeof atualizarKPI === "function") {
                    atualizarKPI();   // Busca o primeiro dado imediatamente
                    kpiInterval = setInterval(atualizarKPI, 300000);
                }
            }            

            // 3. Home / Dashboard (AQUI É ONDE ELE LIGA O dashboard.js)
            if (pageName === 'dashboard.html') {
                if (typeof inicializarDashboardHome === "function") {
                    inicializarDashboardHome();
                }
            }

            updateActiveMenu(pageName);
        })
        .catch(error => {
            contentDiv.innerHTML = "<p style='color:red'>Erro: " + error.message + "</p>";
            console.error(error);
        });
}

function updateActiveMenu(page) {
    const items = document.querySelectorAll('.sidebar li');
    items.forEach(item => {
        item.classList.remove('active');
        if(item.getAttribute('onclick') && item.getAttribute('onclick').includes(page)) {
            item.classList.add('active');
        }
    });
}

function atualizarRelogioESP() {
    fetch('/api/time')
        .then(response => {
            if (!response.ok) throw new Error("Erro na rede");
            return response.json();
        })
        .then(data => {
            if (data && data.hora_atual) {
                const clock = document.getElementById('esp-clock');
                if (clock) clock.innerText = data.hora_atual;
            }
        })
        .catch(err => {
            const clock = document.getElementById('esp-clock');
            if (clock) clock.innerText = "Desconectado";
        });
}