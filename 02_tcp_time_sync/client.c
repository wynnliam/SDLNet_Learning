#include "./util.h"
#include <stdio.h>

int main() {
  IPaddress ip;
  TCPsocket server;

  packet p;

  printf("client: starting\n");

  SDL_Init(0);
  SDLNet_Init();

  SDLNet_ResolveHost(&ip, "localhost", PORT);
  server = SDLNet_TCP_Open(&ip);

  printf("connected!\n");

  while(1) {
    recv_packet(server, &p);
    print_packet(&p);

    SDL_Delay(1500);

    p.type=1;
    p.data.s.s=-4567;
    send_packet(server, &p);
    break;
  }

  printf("client: ending\n");
  return 0;
}
