#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

#define PORT	1234

typedef struct {
  unsigned char type;
  union {
    struct {
      unsigned int s;
      unsigned int c;
    } u;

    struct {
      int s;
      int c;
    } s;
  } data;
} packet;

void send_packet(TCPsocket sock, packet* p);
void recv_packet(TCPsocket sock, packet* p);
void print_packet(packet* p);
