

bool getParams(String json, String* ssid, String* pass, bool* remember){

  // --- Extrair SSID ---
  int s1 = json.indexOf("\"ssid\"");
  if (s1 < 0) return false;

  s1 = json.indexOf(":", s1) + 2;       // pula :"
  int s2 = json.indexOf("\"", s1);      // fecha "
  *ssid = json.substring(s1, s2);


  // --- Extrair PASS ---
  int p1 = json.indexOf("\"pass\"");
  if (p1 < 0) return false;

  p1 = json.indexOf(":", p1) + 2;       // pula :"
  int p2 = json.indexOf("\"", p1);      // fecha "
  *pass = json.substring(p1, p2);


  // --- Extrair REMEMBER ---
  int r1 = json.indexOf("\"remember\"");
  if (r1 < 0) return false;

  r1 = json.indexOf(":", r1) + 1;       // pula :
  String val = json.substring(r1, r1 + 5);  // pega "false" ou "true"

  val.trim();

  if (val.startsWith("true"))
    *remember = true;
  else
    *remember = false;

  return true;
}

String networkJsonList(){
  WifiNetworkList list;
  int len = scanWifiNetworks(list);
  
  String json = "{ \"networks\": [";

  for (int i = 0; i < len; i++) {
    json += "{";
    json += "\"ssid\": \"" + String(list[i].ssid) + "\",";
    json += "\"rssi\": \"" + String(list[i].rssi) + "\",";
    json += "\"bssid\": \"" + String(list[i].bssid) + "\",";
    json += "\"auth_mode\": \"" + String(list[i].authMode) + "\",";
    json += "\"channel\": \"" + String(list[i].channel) + "\"";
    json += "}";

    if (i < len - 1) {
      json += ",";
    }
  }

  json += "]}";
  return json;
}

String connectJson(WifiConnectResult status, String ssid, String ip){
  String json = "{\"status\":\"";
  
  switch(status){
    case WIFI_OK:
      json += "ok\",\"ssid\":\"";
      json += ssid;
      json += "\",\"ip\":\"";
      json += ip;
      break;

    case WIFI_WRONG_PASSWORD: json += "wrong_password"; break;
    case WIFI_NO_SSID: json += "no_ssid"; break;
    case WIFI_TIMEOUT: json += "timeout"; break;
  }

  json += "\"}";

  // Serial.printf("connectJson -> ssid: %s | ip: %s\n", ssid.c_str(), ip.c_str());

  return json;
}