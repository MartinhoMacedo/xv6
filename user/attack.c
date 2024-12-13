#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)

  char *mem = sbrk(PGSIZE*32);

  // NOTE: For some reason it is in the page 16...
  //for (int i = 0; i < PGSIZE * 32; i += PGSIZE) {
    int i = 16 * PGSIZE;
    char *target = mem + i;
    char candidate[50];
    strcpy(candidate, target);
    candidate[31] = 0;
      printf("%s\n", target+32);
      write(2, target+32, 8);
      exit(0);
  //}


  exit(1);
}
