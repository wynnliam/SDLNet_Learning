#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

int main() {
  printf("Starting basic client...\n");

  if(SDL_Init(0) == -1) {
    printf("SDL_Init: %s\n", SDL_GetError());
    return -1;
  }

  if(SDLNet_Init() == -1) {
    printf("SDLNet_Init: %s\n", SDLNet_GetError());
    return -1;
  }

  printf("SDLNet initialized!\n");

  printf("Connecting the client...\n");

  IPaddress ip;
  TCPsocket tcpsocket;
  unsigned short port = 1234;

  if(SDLNet_ResolveHost(&ip, "localhost", port) == -1) {
    printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    return -1;
  }

  tcpsocket = SDLNet_TCP_Open(&ip);
  if(!tcpsocket) {
    printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
    return -1;
  }

  printf("Connected!\nPlease enter a message\n> ");

  while(1) {
    char message[1024];
    int len;
    scanf("%s", message);
    while(getchar() != '\n');
    len = strlen(message);

    printf("Sending message...\n");
    int result = SDLNet_TCP_Send(tcpsocket, message, len);
    if(result < len)
      printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
  }

  SDLNet_TCP_Close(tcpsocket);

  printf("Stopping the client...\n");
  SDLNet_Quit();
  SDL_Quit();

  printf("Goodbye!\n");

  return 0;
}

