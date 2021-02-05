#include "./util.h"
#include <stdio.h>

int main() {
  IPaddress ip;
  TCPsocket server;

  printf("client: starting\n");

  SDL_Init(0);
  SDLNet_Init();

  SDLNet_ResolveHost(&ip, "localhost", PORT);
  server = SDLNet_TCP_Open(&ip);

  printf("connected!\n");

  while(1) {
    SDL_Delay(1000);
  }

  printf("client: ending\n");
  return 0;
}
