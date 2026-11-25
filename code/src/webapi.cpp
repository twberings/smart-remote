#include "webapi.hpp"

WebAPI::WebAPI(WebServer &server, Remote &remote)
    : server(server), remote(remote) {
  server.on("/data", HTTP_GET, std::bind(&WebAPI::handleDataRequest, this));
  server.on("/command", HTTP_POST,
            std::bind(&WebAPI::handleCommandRequest, this));
}

void WebAPI::begin() { server.begin(); }
void WebAPI::update() { server.handleClient(); }

void WebAPI::handleDataRequest() {
  Serial.println("Data request received");
  auto &signals = remote.getSignals();
  String json = "[";
  for (size_t i = 0; i < signals.size(); ++i) {
    const auto &signal = signals[i];
    json += "{";
    json += "\"id\":" + String(signal.id) + ",";
    json += "\"batch\":" + String(signal.batch);
    json += "}";
    if (i < signals.size() - 1) {
      json += ",";
    }
  }
  json += "]";
  server.send(200, "application/json", json);
}

void WebAPI::handleCommandRequest() {
  if (server.hasArg("batch")) {
    uint8_t batch = server.arg("batch").toInt();
    remote.setBatch(batch);
    server.send(200, "text/plain", "Now recording to batch=" + String(batch));
  } else if (server.hasArg("id")) {
    uint16_t id = server.arg("id").toInt();
    if (this->remote.sendSignal(id)) {
      String response =
          "Sending signal id=" + String(id);
      server.send(200, "text/plain", response);
    } else {
      server.send(500, "text/plain", "Failed to send signal");
    }
  } else {
    server.send(400, "text/plain", "Missing 'batch' or 'id' parameter");
    return;
  }
}
