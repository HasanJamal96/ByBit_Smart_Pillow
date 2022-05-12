void ServerRoutes(){
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", HTML);
  });
  server.on("/_redirects", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", redirect);
  });
  server.on("/asset-manifest.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "application/json", asset_manifest);
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", favicon, 3870);
    request->send(response);
  });
  server.on("/manifest.json", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "application/json", manifest);
  });
  server.on("/robots.txt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", robots);
  });


/////////////////////////////css///////////////////////
  server.on("/static/css/main.f111b1bb.chunk.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/css", main_css);
  });
  server.on("/static/css/main.f111b1bb.chunk.css.map", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "application/unknown", main_css_map);
  });


/////////////////////////////js///////////////////////
  server.on("/static/js/2.7f4f1505.chunk.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/javascript", chunk_2_9);
  });
  server.on("/static/js/2.7f4f1505.chunk.js.LICENSE.txt", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", license);
  });
  server.on("/static/js/2.7f4f1505.chunk.js.map", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "application/unknown", chunk_2_9_map);
  });
  server.on("/static/js/3.1dc319fa.chunk.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/javascript", chunk_3_1);
  });
  server.on("/static/js/3.1dc319fa.chunk.js.map", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "application/unknown", chunk_3_1_map);
  });
  
  server.on("/static/js/main.8ddd0adb.chunk.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/javascript", chunk_main);
  });
  server.on("/static/js/main.8ddd0adb.chunk.js.map", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "application/unknown", chunk_main_map);
  });
  server.on("/static/js/runtime-main.758448bd.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/javascript", chunk_runtime_main);
  });
  server.on("/static/js/runtime-main.758448bd.js.map", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "application/unknown", chunk_runtime_main_map);
  });


/////////////////////////////media///////////////////////
  server.on("/static/media/add.646e4dd4.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_add);
  });
  server.on("/static/media/alert-octagon.72c45191.svgt", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_alert);
  });
  server.on("/static/media/cancel.f7e62130.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_cancel);
  });
  server.on("/static/media/celebration.9dfec100.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_celebration);
  });
  server.on("/static/media/chevron-left.26f445b8.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_chevron);
  });
  server.on("/static/media/connected-check.08d3cae2.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_connected);
  });
  server.on("/static/media/DINPro-Bold.d3c192a5.otf", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/DINPro-Bold.d3c192a5.otf");
  });
  server.on("/static/media/DINPro-Medium.2e7ded00.otf", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/DINPro-Medium.2e7ded00.otf");
  });
  server.on("/static/media/DINPro-Regular.cbb8d835.otf", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/DINPro-Regular.cbb8d835.otf");
  });
  server.on("/static/media/edit.9cb0934d.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_edit);
  });
  server.on("/static/media/legal.e34be80f.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_legel);
  });
  server.on("/static/media/monitoring-mode-graphic.d5f47959.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_monitoring);
  });
  server.on("/static/media/refresh.cd095505.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_refresh);
  });
  server.on("/static/media/sleep-graphic.5a9ea36e.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_sleep);
  });
  server.on("/static/media/tick.234d8f43.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_tick);
  });
  server.on("/static/media/trash.80f0226e.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_trash);
  });
  server.on("/static/media/vibration.aa1909aa.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_vibration_aa);
  });
  server.on("/static/media/vibration-graphic.e406d857.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_vibration);
  });
  server.on("/static/media/wifi.b463d022.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_wifib4);
  });
  server.on("/static/media/wifi-bg.fc63b34e.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_wifi_bg);
  });
  server.on("/static/media/wifi-connecting-graphic.9ecb689d.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_wifi);
  });
  server.on("/static/media/wifi-fg.7664f14f.svg", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "image/svg+xml", svg_wifi_fg);
  });
  

  
/////////////////////////////images///////////////////////
  server.on("/img/ADA.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", ADA, 2912);
    request->send(response);
  });
  server.on("/img/bg.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", bg, 136475);
    request->send(response);
  });
  server.on("/img/BIT.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", BIT, 1079);
    request->send(response);
  });
  server.on("/img/BNB.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", BNB, 2167);
    request->send(response);
  });
  server.on("/img/BTC.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", BTC, 2260);
    request->send(response);
  });
  server.on("/img/DOGE.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", Doge, 3120);
    request->send(response);
  });
  server.on("/img/DOT.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", DOT, 1663);
    request->send(response);
  });
  server.on("/img/ETH.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", ETH, 2449);
    request->send(response);
  });
  server.on("/img/LTC.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", LTC, 1530);
    request->send(response);
  });
  server.on("/img/SOL.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", SOL, 1352);
    request->send(response);
  });
  server.on("/img/XRP.png", HTTP_GET, [](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", XRP, 1933);
    request->send(response);
  });


/////////////////// Extras configuration routes /////////////////////
  server.on("/wifistatus",HTTP_GET, [](AsyncWebServerRequest *request){
    String payload = "{\"connected\":";
    if(WiFiconnected){
      payload = payload + "true,\"ssid\":\"";
      payload = payload + wifi_ssid + "\",\"reason\":\"";
      payload = payload + Reason + "\"}";
    }
    else{
      payload = payload + "false,\"ssid\":\"";
      payload = payload + wifi_ssid + "\",\"reason\":\"";
      payload = payload + Reason + "\"}";
    }
    request->send(200, "text/plain", payload);
  });

  server.on("/coins",HTTP_GET,[](AsyncWebServerRequest * request){
    request->send(200, "text/plain", COINS_LIST);
  });


  server.on("/log", HTTP_GET, [](AsyncWebServerRequest *request){
    if(SPIFFS.exists("/log.txt")){
      request->send(SPIFFS, "/log.txt");
    }
    else
      request->send(200, "text/plain", "Log file doesnot exist");
  });
  server.on("/deletelogfile", HTTP_GET, [](AsyncWebServerRequest *request){
    SPIFFS.remove("/log.txt");
    request->send(200, "text/plain", "Ok");
  });
  

  server.on("/triggers",HTTP_GET,[](AsyncWebServerRequest * request){
    String at3 = "\":{\"is3\":true,\"is5\":true},\"";
    String at2 = "\":{\"is3\":false,\"is5\":true},\"";
    String at1 = "\":{\"is3\":true,\"is5\":false},\"";
    String rec = "{\"" ;
    for(int i=0; i<MAX; i++){
      if(Coins[i] != ""){
        rec = rec + Coins[i];
        if(Triggers[i] == 3)
          rec = rec + at3;
        else if(Triggers[i] == 1)
          rec = rec + at1;
        else if(Triggers[i] == 2)
          rec = rec + at2;
      }
    }
    int ln = rec.length()-2;
    if(ln>5){
      rec = rec.substring(0,ln) + "}";
      request->send(200, "text/plain", rec);
    }
    else
      request->send(200, "text/plain", "{}");
  });
  
  
  server.on("/wificreds",HTTP_POST,[](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total){
    WiFi.disconnect(true);
    WiFiconnected = isWiFi = false;
    Reason = "";
    String rec = "";
    for(int i=0; i<len; i++)
      rec += (char)data[i];
    deserializeJson(doc, rec);
    JsonObject obj = doc.as<JsonObject>();
    wifi_ssid = obj["ssid"].as<String>();
    wifi_password = obj["pass"].as<String>();
    doConnect = true;
    request->send(200, "text/plain", "ok");
  });

  server.on("/addcoins",HTTP_POST,[](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total){
    String rec = "";
    for(int i=0; i<len; i++)
      rec += (char)data[i];
    deserializeJson(doc, rec);
    JsonObject obj = doc.as<JsonObject>();
    
    for(int i=0; i<MAX; i++){
      rec = obj[String(i)].as<String>();
      if(rec != "null")
        Coins[i] = rec;
      else
        Coins[i] = "";
    }
    doSaveCoins = true;
    request->send(200, "text/plain", "ok");
  });

  server.on("/addtriggers",HTTP_POST,[](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total){
    String rec = "";
    for(int i=0; i<len; i++)
      rec += (char)data[i];
    doSaveTrigs = true;
    deserializeJson(doc, rec);
    JsonObject obj = doc.as<JsonObject>();
    for(int i=0; i<MAX; i++)
      Triggers[i] = obj["T"+String(i)].as<int>();
    request->send(200, "text/plain", "ok");
  });

  server.on("/scanresults",HTTP_GET,[](AsyncWebServerRequest * request){
    String json = "[";
    int n = WiFi.scanComplete();
    if(n == -2)
      WiFi.scanNetworks(true);
    else{
      if(n){
        for (int i = 0; i < n; ++i)
          json += "\""+WiFi.SSID(i)+"\",";
        WiFi.scanDelete();
        if(WiFi.scanComplete() == -2)
          WiFi.scanNetworks(true);
      }
    }
    if(json.length() > 3)
      json = json.substring(0,json.length()-1);
    json += "]";
    request->send(200, "application/json", json);
  });

  server.on("/vibration",HTTP_POST,[](AsyncWebServerRequest * request){
    request->send(200, "text/plain", "ok");
    doVibrate = true;
  });

  server.on("/getstatus",HTTP_GET,[](AsyncWebServerRequest * request){
    String rec = "{\"vibration\":";
    if(VibrationChecked == "true")
      rec += "true,";
    else
      rec += "false,";

    rec += "\"landing\":";
    if(landing == "true")
      rec += "true}";
    else
      rec += "false}";
    request->send(200, "text/plain", rec);
  });
  server.on("/poststatus",HTTP_POST,[](AsyncWebServerRequest * request){},NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total){
    String rec = "";
    for(int i=0; i<len; i++)
      rec += (char)data[i];
    deserializeJson(doc, rec);
    JsonObject obj = doc.as<JsonObject>();

    VibrationChecked = obj["vibration"].as<String>();
    landing = obj["landing"].as<String>();
    UpdateStatus = true;
    request->send(200, "text/plain", "ok");
  });

  server.on("/HardResetSmartPillow", HTTP_GET, [](AsyncWebServerRequest *request){
    doReset = true;
    request->send(200, "text/plain", "Ok");
  });

  
  server.onNotFound([](AsyncWebServerRequest *request){
    #ifdef DEBUG
      Serial.print("Route does not exist: ");
      Serial.println(request->url());
    #endif
    request->send(200, "text/plain", "Route deos not exist");
  });
}
