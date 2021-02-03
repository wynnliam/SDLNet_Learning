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
  // Use a set of one to listen for activity from server.
  SDLNet_SocketSet set;
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

  set = SDLNet_AllocSocketSet(1);
  SDLNet_TCP_AddSocket(set, tcpsocket);

  printf("Connected!\nPlease enter a message\n");

  int num_ready;

  while(1) {
    num_ready = SDLNet_CheckSockets(set, 100);

    if(num_ready && SDLNet_SocketReady(tcpsocket)) {
      char recmessage[2048];
      int result = SDLNet_TCP_Recv(tcpsocket, recmessage, 2048);

      if(result >= 1)
        printf("%s\n", recmessage);
    }

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

