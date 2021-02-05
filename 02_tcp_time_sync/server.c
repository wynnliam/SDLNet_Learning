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

      switch(p.type) {
        case 0:
          p.type = 1;
          p.data.u.s = SDL_GetTicks() + (unsigned int)lag;
          send_packet(client, &p);
          break;

        case 1:
          p.type = 2;
        case 2:
          p.data.s.c = now - (unsigned int)lag - p.data.u.c;
          if(p.data.s.c < minoff && p.data.s.c >-minoff) {
            p.type = 3;
            p.data.u.s = SDL_GetTicks() + (unsigned int)lag + minoff * 100;
          } else {
            p.data.u.s = SDL_GetTicks() + (unsigned int)lag;
          }

          send_packet(client, &p);

          if(p.type == 3) {
            printf("server: wait until %u\n", p.data.u.s);
            SDL_Delay(p.data.u.s - SDL_GetTicks());
            printf("server: TIME = %u\n", SDL_GetTicks());
            done = 1;
          }

          break;
        default:
          done = 1;
          break;
      }

      printf("now=%u (lag=%.2f)\n", now, lag);
      p.type = 10;
    }
  }

  printf("server: ending\n");

  SDLNet_Quit();
  SDL_Quit();
  return 0;
}
