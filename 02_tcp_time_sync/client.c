#include "./util.h"
#include <stdio.h>

int main() {
  IPaddress ip;
  TCPsocket server = NULL;

  packet p;

  int simlag = 0;
  int done = 0;
  int base, now;
  unsigned int last;
  float lag;

  printf("client: starting\n");

  SDL_Init(0);
  SDLNet_Init();

  SDLNet_ResolveHost(&ip, "localhost", PORT);
  server = SDLNet_TCP_Open(&ip);

  printf("connected!\n");

  base = 0;
  now = SDL_GetTicks();
  while(!done) {
    last = now;
    recv_packet(server, &p);

    if(simlag)
      SDL_Delay(simlag);
    now = SDL_GetTicks();

    print_packet(&p);

    if(simlag)
      SDL_Delay(simlag);

    switch(p.type) {
      case 0:
        base = now - p.data.u.s;
        p.data.u.c = now;
        send_packet(server, &p);
        break;
      case 1:
        base -= (int)lag;
        p.data.u.c = SDL_GetTicks() - base;
        p.data.s.s = ((int)(now - base - p.data.s.s)) / 2;
        send_packet(server, &p);
        break;
      case 2:
        base = p.data.s.c;
        p.data.u.c = SDL_GetTicks() - base;
        p.data.s.s = ((int)(now - base - p.data.s.s + lag)) / 2;
        send_packet(server, &p);
      case 3:
        base -= p.data.s.c;
        printf("client: wait until %u\n", p.data.u.s);
        SDL_Delay(p.data.u.s - (SDL_GetTicks() - base));
        printf("TIME = %u\n", SDL_GetTicks() - base);
      default:
        done = 1;
        break;
    }   

    printf("(now - base = %u (lag=%.2f)\n", now - base, p.type ? lag : lag * 2);
    p.type = 10;
  }

  SDLNet_TCP_Close(server);
  printf("client: ending\n");
  SDLNet_Quit();
  SDL_Quit();
  return 0;
}
