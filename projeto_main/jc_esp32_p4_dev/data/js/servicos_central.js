function inicializarPainelPWM() {
    const listaCanais = document.getElementById('lista-canais');
    
    if (listaCanais) {
        listaCanais.innerHTML = ''; // Limpa antes de gerar
        
        for (let i = 0; i < 16; i++) {
            const item = document.createElement('div');
            item.style.marginBottom = "15px";
            item.style.padding = "10px";
            item.style.borderBottom = "1px solid #ddd";
            
            item.innerHTML = `
                <label style="font-weight:bold; display:block;">MOSFET Canal ${i}</label>
                <input type="range" min="0" max="4095" value="0" 
                       style="width: 100%; margin: 10px 0;" 
                       oninput="enviarPWM(${i}, this.value)">
            `;
            listaCanais.appendChild(item);
        }
        console.log("Interface PWM carregada dinamicamente.");
    }
}

function enviarPWM(canal, valor) {
    fetch(`/setPWM?ch=${canal}&val=${valor}`)
        .then(response => {
            if(!response.ok) console.error("Erro no comando PWM");
        })
        .catch(err => console.error("Erro de rede:", err));
}



