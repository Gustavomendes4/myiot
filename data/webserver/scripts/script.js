
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

function clearComponent(id) {
  const cleanId = id.startsWith("#") ? id.slice(1) : id;
  const elemento = document.getElementById(cleanId);

  if (elemento) {
    elemento.innerHTML = "";
  }
}

async function initialLoadFragments(){

    const headerPath = "./fragments/header.html"
    const sidebarPath = "./fragments/sidebar.html"
    const headerPH  = "#header-placeholder";
    const sidebarPH = "#sidebar-placeholder";
    const headerContent = await fetch(headerPath).then(r => r.text());
    const sidebarContent = await fetch(sidebarPath).then(r => r.text());

    loadFragment(headerContent, headerPH);
    loadFragment(sidebarContent, sidebarPH);


    // Adiciona listner para abrir/fechar menu lateral
    const sidebarClass = ".sidebar";
    const burgerID = "burger-icon";
    const sidebar = document.querySelector(sidebarClass);

    document.getElementById(burgerID).addEventListener("click", () => {
        sidebar.classList.toggle("show");
    });


    // Adiciona listner para mudança de página no menu
    document.querySelector(".menu").addEventListener("click", listnerSidebarClick);
}

async function listnerSidebarClick(event){

    if(event.target.tagName !== "A"){
        return 0;
    }
    event.preventDefault();


    const listOfPages = {
        "home":     "./pages/home.html",
        "mqtt":     "./pages/mqtt.html",
        // "net":      "./pages/net.html",
        "net":      "../webAP/index.html",
        "logs":     "./pages/logs.html",
        "monitor":  "./pages/monitor.html",
        "config":   "./pages/config.html"
    }

    const contentPH = "#content";
    const page = event.target.dataset.page;
    let cntt = "";
    
    if(listOfPages[page]){
        cntt = await fetch(listOfPages[page]).then(r => r.text());
    }
    else{
        cntt = "";
        console.log(page + "não é uma página válida.");
        return 0;
    }

    clearComponent(contentPH);
    loadFragment(cntt, contentPH);
}

initialLoadFragments();