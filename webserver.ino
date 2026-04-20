#include <WiFi.h>
#include <WebServer.h>

#include <DNSServer.h>
#include <ESPmDNS.h>


#include "FS.h"
#include "LittleFS.h"
// #include "prompt.h"

#define DNS_PORT 53
DNSServer dnsServer;

// WebServer server(80);
WebServer* server;
bool killAp = false;

void webServerAPInit(){

  if(server){
    server->stop();
    delete server;
    server = nullptr;
  }

  // WiFi.disconnect(true, true);
  // WiFi.mode(WIFI_OFF);
  // delay(200);
  
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.disconnect(true);
  
  WiFi.softAP(MY_SSID, MY_PASS);
  
  server = new WebServer(80);
  
  Serial.print("AP iniciado. IP: ");
  Serial.println(WiFi.softAPIP());

  //dns
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  server->onNotFound([](){
    server->sendHeader("Location", "/", true);
    server->send(302, "text/plain", "");
  });

  // assets
  server->serveStatic("/assets/favicon.ico", LittleFS, "/assets/favicon.ico");
  server->serveStatic("/assets/padlockIcon.svg", LittleFS, "/assets/padlockIcon.svg");
  server->serveStatic("/assets/refreshIcon.svg", LittleFS, "/assets/refreshIcon.svg");
  server->serveStatic("/assets/wifiIcon_1.svg", LittleFS, "/assets/wifiIcon_1.svg");
  server->serveStatic("/assets/wifiIcon_2.svg", LittleFS, "/assets/wifiIcon_2.svg");
  server->serveStatic("/assets/wifiIcon_3.svg", LittleFS, "/assets/wifiIcon_3.svg");
  server->serveStatic("/assets/wifiIcon_4.svg", LittleFS, "/assets/wifiIcon_4.svg");
  server->serveStatic("/assets/connecting.svg", LittleFS, "/assets/connecting.svg");

  // script and template
  server->serveStatic("/loadHtml.js", LittleFS, "/webAP/loadHtml.js");
  server->serveStatic("/wifi-item.html", LittleFS, "/webAP/wifi-item.html");
  server->serveStatic("/popup.html", LittleFS, "/webAP/popup.html");

  // api
  server->on("/network", [](){
    server->send(200, "application/json", networkJsonList());
  });

  server->on("/connect", HTTP_POST, handleConnect);

  server->on("/kill", [](){
    killAp = true;
  });

  //main
  server->on("/", [](){
    server->send(200, "text/html", readFile("/webAP/index.html"));}
  );

  server->begin();
}

void webServerAPstop(){

  if(!server) return;

  server->stop();
  delete server;
  server = nullptr;

  dnsServer.stop();

  WiFi.softAPdisconnect(true);
  delay(150);

  // WiFi.disconnect(true, true);
  // WiFi.mode(WIFI_OFF);
  // delay(150);

  // WiFi.mode(WIFI_STA);
}

void serverLoop(){
  if(server)
    server->handleClient();

  // if(!WIFIconnected)
  if(WiFi.getMode() & WIFI_AP)
    dnsServer.processNextRequest();
  

  if(killAp){
    killAp = false;
    delay(10000);
    webServerAPstop(); // Ideal aqui é esperar a resposta do /kill
    webServerInit();
    //initMqtt();
  }

}

void handleConnect(){
  
  String body = server->arg("plain");
  String ssid;
  String pass;
  String ip;

  bool rememb;

  bool ok = getParams(body, &ssid, &pass, &rememb);

  if( !ok ){
    server->send(400, "application/json", "{\"status\":\"missing_params\"}");
    return;
  }

  WifiConnectResult sts = connectWifi(ssid, pass, &ip, true);

  String json = connectJson(sts, ssid, ip);

  server->send(200, "application/json", json);

  if( sts == WIFI_OK ){
    // delay(15000);
    // webServerAPstop(); // < -------- Aqui precisa ser fora do handle, pra permitir que o navegador receba a resposta

    if(rememb){
      saveNetworkFS(ssid, pass);
    }
  }
}

// =======  WEB server ==========

void webServerRoutes(){

  // assets
  server->serveStatic("/assets/favicon.ico", LittleFS, "/assets/favicon.ico");
  server->serveStatic("/assets/burgerIcon.svg", LittleFS, "/assets/burgerIcon.svg");
  server->serveStatic("/assets/userIcon.svg", LittleFS, "/assets/userIcon.svg");
  server->serveStatic("/assets/logo.png", LittleFS, "/assets/logo.png");
  // server->serveStatic("/assets/padlockIcon.svg", LittleFS, "/assets/padlockIcon.svg");
  // server->serveStatic("/assets/refreshIcon.svg", LittleFS, "/assets/refreshIcon.svg");
  // server->serveStatic("/assets/wifiIcon_1.svg", LittleFS, "/assets/wifiIcon_1.svg");
  // server->serveStatic("/assets/wifiIcon_2.svg", LittleFS, "/assets/wifiIcon_2.svg");
  // server->serveStatic("/assets/wifiIcon_3.svg", LittleFS, "/assets/wifiIcon_3.svg");
  // server->serveStatic("/assets/wifiIcon_4.svg", LittleFS, "/assets/wifiIcon_4.svg");
  // server->serveStatic("/assets/connecting.svg", LittleFS, "/assets/connecting.svg");

  // script and template
  server->serveStatic("/scripts/script.js", LittleFS, "/webserver/scripts/script.js");
  server->serveStatic("/style.css", LittleFS, "/webserver/style.css");

  server->serveStatic("/fragments/header.html", LittleFS, "/webserver/fragments/header.html");
  server->serveStatic("/fragments/sidebar.html", LittleFS, "/webserver/fragments/sidebar.html");

  server->serveStatic("/pages/home.html", LittleFS, "/webserver/pages/home.html");
  server->serveStatic("/pages/config.html", LittleFS, "/webserver/pages/config.html");
  server->serveStatic("/pages/logs.html", LittleFS, "/webserver/pages/logs.html");
  server->serveStatic("/pages/monitor.html", LittleFS, "/webserver/pages/monitor.html");
  server->serveStatic("/pages/mqtt.html", LittleFS, "/webserver/pages/mqtt.html");
  server->serveStatic("/pages/net.html", LittleFS, "/webserver/pages/net.html");

  // server->serveStatic("/popup.html", LittleFS, "/webAP/popup.html");

    // api
  // server->on("/network", [](){
    // server->send(200, "application/json", networkJsonList());
  // });

  // server->on("/connect", HTTP_POST, handleConnect);

  //main
  server->on("/", [](){
    server->send(200, "text/html", readFile("/webserver/index.html"));
  });


}

void webServerInit(){

  if(server){
    server->stop();
    delete server;
    server = nullptr;
  }

  WiFi.mode(WIFI_STA);
  delay(100);

  server = new WebServer(80);

  webServerRoutes();

  Serial.print("WBS iniciado\n");

  configDNS();

  server->begin();
}

void configDNS(){

  if(!MDNS.begin("myiot")){
    Serial.println("Erro na config de DNS");
    return;
  }

  MDNS.addService("http", "tcp", 80);
}
