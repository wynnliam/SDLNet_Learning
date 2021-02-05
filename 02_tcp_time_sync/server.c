#include "./util.h"
#include <stdio.h>

int main() {
  IPaddress ip;
  TCPsocket server, client;

  printf("server: starting\n");

  SDL_Init(0);
  SDLNet_Init();

  SDLNet_ResolveHost(&ip, NULL, PORT);
  server = SDLNet_TCP_Open(&ip);

  while(1) {
    client = NULL;
    while(!client) {
      client = SDLNet_TCP_Accept(server);
      if(!client)
        SDL_Delay(100);
    }

    printf("We got the client!\n");
    while(1) {
      SDL_Delay(1000);
    }
  }

  printf("server: ending\n");
  return 0;
}
