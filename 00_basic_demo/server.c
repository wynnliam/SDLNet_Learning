#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

// Defines the max number of clients that can be connected
#define MAX_CLIENTS	16

int main() {
  printf("Starting basic server...\n");

  if(SDL_Init(0) == -1) {
    printf("SDL_Init: %s\n", SDL_GetError());
    return -1;
  }

  if(SDLNet_Init() == -1) {
    printf("SDLNet_Init: %s\n", SDLNet_GetError());
    return -1;
  }

  printf("SDLNet initialized!\n");

  printf("Starting the server...\n");

  IPaddress ip;
  TCPsocket server_socket;
  unsigned short port = 1234;

  if(SDLNet_ResolveHost(&ip, NULL, port) == -1) {
    printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    return -1;
  }

  server_socket = SDLNet_TCP_Open(&ip);
  if(!server_socket) {
    printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
    return -1;
  }

  char next_message[1024];
  int message_len;
  TCPsocket client_socket;
  IPaddress* client_ipaddr;
  unsigned int ipaddr;

  while(1) {
    /* Attempt to accept an incoming client */
    client_socket = SDLNet_TCP_Accept(server_socket);
    if(client_socket == NULL) {
      printf("server: SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
      SDL_Delay(1000);
      continue;
    }

    client_ipaddr = SDLNet_TCP_GetPeerAddress(client_socket);
    if(!client_ipaddr) {
      printf("server: SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
      continue;
    }

    ipaddr = SDL_SwapBE32(client_ipaddr->host);
    printf("server: Accepted a connection from %d.%d.%d.%d port %hu\n",
			ipaddr>>24,
			(ipaddr>>16)&0xff,
			(ipaddr>>8)&0xff,
			ipaddr&0xff,
			client_ipaddr->port);

    message_len = SDLNet_TCP_Recv(client_socket, next_message, 1024);
    SDLNet_TCP_Close(client_socket);
    if(message_len < 1) {
      printf("server: SDLNet_TCP_Recv: %s\n", SDLNet_GetError());
      continue;
    }

    printf("server: recieved message: %s\n", next_message);
  }

  printf("Server started!\n");
  printf("Stopping server...\n");
  SDLNet_Quit();
  SDL_Quit();

  printf("Goodbye!\n");

  return 0;
}
