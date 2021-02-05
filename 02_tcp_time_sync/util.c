#include "./util.h"

void send_packet(TCPsocket sock, packet* p) {
  char buf[8];

  SDLNet_Write32(p->data.u.s, buf);
  SDLNet_Write32(p->data.u.c, buf + 4);

  SDLNet_TCP_Send(sock, &(p->type), 1);
  SDLNet_TCP_Send(sock, buf, 8);
}

void recv_packet(TCPsocket sock, packet* p) {
  char buf[8];

  SDLNet_TCP_Recv(sock, &(p->type), 1);
  SDLNet_TCP_Recv(sock, buf, 8);

  p->data.u.s = SDLNet_Read32(buf);
  p->data.u.c = SDLNet_Read32(buf + 4);
}

void print_packet(packet* p) {
  printf("packet: %8p:\n", p);
  printf("\ttype=%d\n", p->type);
  printf("\tdata.u.s=%u\n", p->data.u.s);
  printf("\tdata.u.c=%u\n", p->data.u.c);
  printf("\tdata.d.s=%d\n", p->data.s.s);
  printf("\tdata.d.s=%d\n", p->data.s.c);
}
