// function togglePower(element) {
//     const isON = element.checked;
    
//     if (isON) {
//         console.log("Sistema: ON");
//         // Enviar comando para o ESP32
//         // fetch('/api/power?state=1');
//     } else {
//         console.log("Sistema: OFF");
//         // Enviar comando para o ESP32
//         // fetch('/api/power?state=0');
//     }
// }

// // Função para sincronizar o estado visual com o valor real (caso venha do ESP32)
// function updateSwitchState(stateFromServer) {
//     const sw = document.getElementById('powerSwitch');
//     sw.checked = (stateFromServer === 'ON');
// }


function togglePowerBtn() {
    const btn = document.getElementById('btnPower');
    
    if (btn.classList.contains('off')) {
        // Mudar para ON
        btn.classList.remove('off');
        btn.classList.add('on');
        console.log("Comando: LIGAR");
        // Exemplo de fetch para o seu WebServer no ESP32:
        // fetch('/control?output=0&state=1');
    } else {
        // Mudar para OFF
        btn.classList.remove('on');
        btn.classList.add('off');
        console.log("Comando: DESLIGAR");
        // fetch('/control?output=0&state=0');
    }
}