#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define PORT	1234

#define MAX_CLIENTS	16

int main() {
  IPaddress ip;
  TCPsocket server_socket;

  printf("Starting server...\n");

  // Start SDL.
  if(SDL_Init(0) == -1) {
    printf("SDL_Init: %s\n", SDL_GetError());
    return -1;
  }

  // Start SDLNet
  if(SDLNet_Init() == -1) {
    printf("SDLNet_Init: %s\n", SDLNet_GetError());
    SDL_Quit();
    return -1;
  }

  // Gets the IP address of this server (used to get server socket).
  if(SDLNet_ResolveHost(&ip, NULL, PORT) == -1) {
    printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    return -1;
  }

  // Now create the server socket
  server_socket = SDLNet_TCP_Open(&ip);
  if(server_socket == NULL) {
    printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
    SDLNet_Quit();
    SDL_Quit();
    return -1;
  }

  printf("server: listening on port %d\n", PORT);

  while(1) {
    SDL_Delay(1000);
  }

  printf("server: shutting down\n");

  SDLNet_Quit();
  SDL_Quit();

  return 0;
}
