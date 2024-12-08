#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int p1[2];
    int p2[2];

    pipe(p1);
    pipe(p2);

    if (fork() == 0) {
      char* ping = 0;
      read(p1[0], &ping, 1);
      close(p1[0]);
      fprintf(1, "%d: received ping\n", getpid());

      char* pong = "b";
      write(p2[1], &pong, 1);
      close(p2[1]);

      exit(0);
    } else {
      char* ping = "a";
      write(p1[1], &ping, 1);
      close(p1[1]);

      char* pong = 0;
      read(p2[0], &pong, 1);
      close(p2[0]);
      fprintf(1, "%d: received pong\n", getpid());

    }

    exit(0);
}
