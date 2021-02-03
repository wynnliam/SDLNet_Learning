#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define PORT	1234

#define MAX_CLIENTS	16

typedef struct {
  TCPsocket socket;
  int active;
  int id;
} client;

IPaddress ip;
TCPsocket server_socket;

client clients[MAX_CLIENTS];
int num_clients = 0;
int next_client_id = 0;

// There will be MAX_CLIENTS + 1
// In this. The additional one is for the
// server_socket
SDLNet_SocketSet sockets;

int add_client(TCPsocket socket) {
  if(num_clients == MAX_CLIENTS) {
    printf("server: I'm full. Bye bye\n");
    SDLNet_TCP_Close(socket);
    return 0;
  }

  int i;
  for(i = 0; i < MAX_CLIENTS; i++) {
    if(!clients[i].active) {
      clients[i].socket = socket;
      clients[i].id = next_client_id;
      clients[i].active = 1;
      break;
    }   
  }

  num_clients++;
  next_client_id++;

  return 1;
}

int remove_client(int pos) {
  int i;
  for(i = 0; i < MAX_CLIENTS; i++) {
    if(pos == i) {
      SDLNet_TCP_DelSocket(sockets, clients[i].socket);
      SDLNet_TCP_Close(clients[i].socket);
      clients[i].active = 0;
      return 1;
    }
  }

  return 0;
}

int main() {
  printf("Starting server...\n");

  // Temporary socket for when user connects.
  TCPsocket socket;

  int i;
  for(i = 0; i < MAX_CLIENTS; i++) {
    clients[i].socket = NULL;
    clients[i].active = 0;
  }

  // Start SDL.
  if(SDL_Init(0) == -1) {
    printf("SDL_Init: %s\n", SDL_GetError());
    return -1;
  }

  // Start SDLNet
  if(SDLNet_Init() == -1) {
    printf("SDLNet_Init: %s\n", SDLNet_GetError());
    goto sdl_quit;
  }

  // Gets the IP address of this server (used to get server socket).
  if(SDLNet_ResolveHost(&ip, NULL, PORT) == -1) {
    printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
    goto sdlnet_quit;
  }

  // Now create the server socket
  server_socket = SDLNet_TCP_Open(&ip);
  if(server_socket == NULL) {
    printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
    goto sdlnet_quit;
  }

  // Now, create the socket set. + 1 for the server.
  sockets = SDLNet_AllocSocketSet(MAX_CLIENTS + 1);
  if(!sockets) {
    printf("SDLNet_AllocSocketSet: %s\n", SDLNet_GetError());
  }

  SDLNet_TCP_AddSocket(sockets, server_socket);

  printf("server: listening on port %d\n", PORT);

  int num_ready = 0;
  while(1) {
    num_ready = SDLNet_CheckSockets(sockets, -1);

    if(num_ready == -1) {
      printf("SDLNet_CheckSockets: %s\n", SDLNet_GetError());
      break;
    }

    if(num_ready == 0)
      continue;

    if(SDLNet_SocketReady(server_socket)) {
      num_ready--;
      printf("server: connection...\n");

      socket = SDLNet_TCP_Accept(server_socket);
      if(socket && add_client(socket)) {
        SDLNet_TCP_AddSocket(sockets, socket);
      }
    }

    for(i = 0; num_ready && i < MAX_CLIENTS; i++) {
      if(clients[i].active) {
        if(SDLNet_SocketReady(clients[i].socket)) {
          char message[1024];
          int result = SDLNet_TCP_Recv(clients[i].socket, message, 1024);

          if(result < 1)
            remove_client(i);
          else
            printf("client[%d]: %s\n", clients[i].id, message);
        }
      }
    }
  }

  printf("server: shutting down\n");

sdlnet_quit:
  SDLNet_Quit();
sdl_quit:
  SDL_Quit();

  return 0;
}
