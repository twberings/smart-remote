#include "WebServer.h"
#include "remote.hpp"

class WebAPI {
public:
  WebAPI(WebServer &server, Remote &remote);
  void begin();
  void update();

private:
  WebServer &server;
  Remote &remote;
  void handleDataRequest();
  void handleCommandRequest();
};
