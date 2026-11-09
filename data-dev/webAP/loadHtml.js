

let networks = [];

async function loadFragment(input, selector){
    try{
        let html = "";

        if(typeof input === "string" && input.trim().startsWith("<")){
            html = input;
        }

        else{
            const res = await fetch(input, {cache: 'no-cache'});
            if(!res.ok) throw new Error('Falha ao carregar ' + input);
            html = await res.text();

        }
        

        document.querySelector(selector).innerHTML += html;
        // Ajuste dinâmico de ano (se presente)
        const yEl = document.querySelector(selector + ' #year');
        if(yEl) yEl.textContent = new Date().getFullYear();

        return true;
    }
    catch(err){
        console.error(err);
        return false;
    }
}

function getSignalLevel(rssi) {
    if (rssi > -50) return 4;
    if (rssi > -70) return 3;
    if (rssi > -80) return 2;
    return 1;
}

async function loadNetworks(){

    // spin image
    const img = document.getElementById("refreshIcon");
    img.classList.add("spin");

    const listSelector = "#network-list";
    document.querySelector(listSelector).innerHTML = "";

    try{
        const res = await fetch("/network");
        const data = await res.json();
        networks = data.networks;

        let i = 0;
        const template = await fetch("/wifi-item.html").then(r => r.text());

        for(const net of data.networks){

            let fragment = template;

            let signal = getSignalLevel(net.rssi);

            fragment = fragment
                .replaceAll("{SSID}", net.ssid)
                .replaceAll("{ID}", String(i))
                .replaceAll("{WIFI_IMG}", signal);

            await loadFragment(fragment, listSelector);

            i++;
        }

        addEventListener();
    }
    catch (err){
        console.error("Erro:", err);
    }

    img.classList.remove("spin"); // Para de girar
}

function addEventListener(){

    const radios = document.querySelectorAll(".network-radio");
    const divisoes0 = document.querySelectorAll(".conect-division");
    const divisoes1 = document.querySelectorAll(".password-division");
    const passwords = document.querySelectorAll(".passwordInput");

    radios.forEach((radio, index) => {
        radio.addEventListener("change", () => {
            // esconde todas as divisões
            divisoes0.forEach(div => div.style.display = "none");
            divisoes1.forEach(div => div.style.display = "none");
            passwords.forEach(input => input.value = "");
        
            // mostra apenas a correspondente ao selecionado
            if(radio.checked){
                divisoes0[index].style.display = "flex";
            }

        });
    });

    //ENTER no campo de senha
    passwords.forEach((input, index) => {
        input.addEventListener("keydown", (event) => {
        
            if(event.key === "Enter"){
                event.preventDefault();
                connectClick(index);
            }
        });
    });
}

async function connectClick(index){
    const net = networks[index];
    const li = document.getElementById("network-list").children[index];
    const icon = li.querySelector(".connecting-img");
    
    const pass = li.querySelector('.passwordInput').value;
    const remember = li.querySelector('.remember-me').checked;
    let response;

    if(net.auth_mode !== 0 && pass === ""){
        //Desabilita a área de conexão
        li.querySelector(".conect-division").style.display = "none";
        
        //habilita a área de password
        li.querySelector(".password-division").style.display = "flex";
        
        
        li.querySelector(".remember-me")?.focus(); // <---

        return;
    }

    li.querySelector(".password-division").style.display = "none";
    icon.style.display = "flex";
    icon.classList.add("spin");
    

    try{
        response = await connect(net.ssid, pass, remember);
    }
    
    finally{
        icon.style.display = "none";
        li.querySelector(".password-division").style.display = "flex";
        icon.classList.remove("spin");
    }

    if(!response){
        console.log("Erro de resposta");
        return;
    }

    if(response.status == "ok"){

        await showPopup(response.ssid, response.ip);
        fetch("/kill");
    }


    switch (response.status){
        case "ok":
            console.log("Conectado com sucesso!");
            console.log(response);
            break;
        case "timeout":
            console.log("Tempo esgotado ao conectar.");
            break;
        case "wrong_password":
            console.log("Senha incorreta.");
            break;
        default:
            console.log("Erro ao conectar.");
    }

}

async function connect(ssid, pass, remember){

    const obj = {
        ssid: ssid,
        pass: pass,
        remember: remember
    };

    const response = await fetch("/connect", {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(obj)
    });

    
    if(!response.ok){
        return { status: "http_error", code: response.status };
    }

    return await response.json();

}

async function showPopup(ssid, ip){
    
    // Carrega o popup
    const placeholder = document.getElementById("popup-placeholder");
    const frag = await fetch("/popup.html").then(r => r.text());   
    placeholder.innerHTML = frag; // NÃO use +=
    
    
    const popup = document.getElementById("popup");
    
    const ssidPH = document.getElementById("popup-ssid");
    const ipPH   = document.getElementById("popup-ip");

    ssidPH.textContent = ssid;
    ipPH.textContent = ip;
    
    popup.classList.remove("hidden");

    document.getElementById("popupOk").onclick = () => {
        popup.classList.add("hidden");

        setTimeout(() => {
            window.location.href = "about:blank";
        }, 500);
    };
}


loadNetworks();


/*
  <script>
    // Fecha a aba automaticamente após 2s
  </script>
*/