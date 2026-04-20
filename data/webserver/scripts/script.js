
let pageCache = {};


async function preloadPages(){

    const pages = {
        "home":     "/pages/home.html",
        "mqtt":     "/pages/mqtt.html",
        "net":      "/pages/net.html",
        "logs":     "/pages/logs.html",
        "monitor":  "/pages/monitor.html",
        "config":   "/pages/config.html"
    };

    for(const key in pages){
        try{
            const res = await fetch(pages[key], {cache: "no-cache"});
            if(res.ok){
                pageCache[key] = await res.text();
            } else {
                console.error("Erro ao carregar", pages[key]);
            }
        } catch(err){
            console.error("Fetch falhou:", pages[key], err);
        }
    }
}

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
        
        // document.querySelector(selector).innerHTML += html;
        document.querySelector(selector).insertAdjacentHTML("beforeend", html);

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

    await preloadPages();

    const headerCntt = await fetchSafe("/fragments/header.html");
    const sidebarCntt = await fetchSafe("/fragments/sidebar.html");

    await loadFragment(headerCntt, "#header-placeholder");
    await loadFragment(sidebarCntt, "#sidebar-placeholder");

    clearComponent("#content");
    await loadFragment(pageCache["home"], "#content");
    

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

    const link = event.target.closest("a");
    if(!link) return;
    
    event.preventDefault();

    const page = link.dataset.page;

    if(!page){
        console.warn("Link sem data-page");
        return;
    }

    if(!pageCache[page]){
        console.error("Página não carregada no cache:", page);
        return;
    }

    const contentPH = "#content";

    clearComponent(contentPH);
    await loadFragment(pageCache[page], contentPH);

    
    //close sidebar
    const sidebar = document.querySelector(".sidebar");
    if(sidebar){
        sidebar.classList.remove("show");
    }

}

async function fetchSafe(path, options = {}){

    try{
        const res = await fetch(path, {
            cache: "no-cache",
            ...options
        });

        if(!res.ok){
            console.error(`HTTP ${res.status} ao carregar: ${path}`);
            return "";
        }

        return await res.text();
    }
    catch(err){
        console.error(`Erro de rede ao carregar: ${path}`, err);
        return "";
    }
}

initialLoadFragments();