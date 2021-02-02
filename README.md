# SDLNet Learning
A series of demos demonstrating SDLNet. Can use for making networked games

## Compiling Code
Each demo has a server and a client. Suppose these are respectively called
`server.c` and `client.c`, then compilation will be in two steps:

`gcc -lSDL2 -lSDL2_net server.c -o server.o`

and

`gcc -lSDL2 -lSDL2_net client.c -o client.o`

## Running Code
To run a demo, you need two windows open. In the first one run:

`./server.out`

Now that the server is running, the client can connect:

`./client.out`

If any demo doesn't follow the above steps, specific instructions
can be found for that demo.
