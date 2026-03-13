// VARIÁVEL GLOBAL: Guarda a referência do loop para podermos cancelá-lo
let kpiInterval = null; 

// Carrega a página inicial automaticamente quando o documento estiver pronto
document.addEventListener("DOMContentLoaded", () => {
    loadPage('home.html');
});

function loadPage(pageName) {
    console.log("Tentando carregar:", pageName);
    const contentDiv = document.getElementById('content');
    
    contentDiv.innerHTML = "Carregando...";

    // SEGURANÇA: Se o usuário mudar de página, cancela o loop do KPI
    if (kpiInterval !== null) {
        clearInterval(kpiInterval);
        kpiInterval = null;
    }

    fetch(pageName)
        .then(response => {
            if (!response.ok) throw new Error("Não foi possível encontrar o arquivo no SD.");
            return response.text();
        })
        .then(data => {
            contentDiv.innerHTML = data;
            
            // EXECUÇÃO DA CORREÇÃO:
            // Se a página for a de controle, chama a função que cria os 16 sliders
            if (pageName === 'control_pwm.html') {
                if (typeof inicializarPainelPWM === "function") {
                    inicializarPainelPWM();
                }
            }
            
            // Carregar Graficos.......
            if (pageName === 'charts.html') {
                inicializarGraficos();
                //inicializarKPI(); // Prepara o layout/canvas
                atualizarKPI();   // Busca o primeiro dado imediatamente
                
                // AQUI ENTRA O LOOP: Cria o ciclo de atualização a cada 5 minutos
                kpiInterval = setInterval(atualizarKPI, 300000);
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
    // Atenção: Se estiver testando pelo Live Server, troque por 'http://192.168.10.210/api/time'
    fetch('/api/time')
        .then(response => {
            if (!response.ok) throw new Error("Erro na rede");
            return response.json();
        })
        .then(data => {
            if (data && data.hora_atual) {
                document.getElementById('esp-clock').innerText = data.hora_atual;
            }
        })
        .catch(err => {
            // Silencia o erro no console para não floodar se a placa reiniciar
            document.getElementById('esp-clock').innerText = "Desconectado";
        });
}

// Inicia o loop do relógio assim que a página principal (index) carregar
document.addEventListener("DOMContentLoaded", () => {
    atualizarRelogioESP(); // Chama a primeira vez imediatamente
    setInterval(atualizarRelogioESP, 1000); // Atualiza a cada 1 segundo (1000ms)
});