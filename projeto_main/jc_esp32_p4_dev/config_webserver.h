#ifndef CONFIG_WEBSERVER_H
#define CONFIG_WEBSERVER_H

#include <WebServer.h>

extern WebServer server; // Isso permite que outros arquivos vejam o objeto server

void setupWebServer();
void handleWebServer(); 

#endif