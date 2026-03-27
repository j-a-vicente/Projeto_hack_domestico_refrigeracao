



function togglePower(checkbox) {
    // Verifica o estado atual do checkbox
    const estado = checkbox.checked ? 'on' : 'off';
    
    // Faz uma requisição GET para o WebServer da JC-ESP32P4
    fetch(`/api/power?state=${estado}`)
        .then(response => {
            if (!response.ok) {
                throw new Error('Falha ao comunicar com o servidor');
            }
            return response.text();
        })
        .then(data => {
            console.log("Sucesso:", data);
        })
        .catch(error => {
            console.error("Erro no envio do comando:", error);
            // Reverte visualmente o botão caso haja falha na rede
            checkbox.checked = !checkbox.checked; 
            alert("Erro ao enviar comando para a placa. Verifique a conexão.");
        });
}

