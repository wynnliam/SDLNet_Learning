#include "./util.h"
#include <stdio.h>

int main() {
  IPaddress ip;
  TCPsocket server, client;

  packet p;
  int done;
  int minoff = 5; //+ or -
  unsigned int last, now;
  float lag;

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

    
    done = 0;
    lag = 0;

    p.type = 0;
    p.data.u.s = SDL_GetTicks();
    send_packet(client, &p);

    now = SDL_GetTicks();

    while(!done) {
      last = now;

      recv_packet(client, &p);

      now = SDL_GetTicks();
      lag = (now - last) / 2.0;

      print_packet(&p);

      done = 1;
    }

    break;
  }

  printf("server: ending\n");
  return 0;
}
