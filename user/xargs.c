#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void xarg(int argc, char *argv[], char *line) {
  /*
  fprintf(1, "\n All arguments before appending:\n");
  for (int i = 0; i < argc; i++) {
    fprintf(1, "%s; ", argv[i]);
  }
  fprintf(1, "\n");
  */

  int argc_a = argc;
  // char string[50] = "abc";

  char *argv_a[MAXARG];

  for (int i = 0; i < argc; i++) {
    argv_a[i] = argv[i];
  }

  argv_a[argc_a] = line;
  argc_a++;
  argv_a[argc_a] = (char *)0x0;

  if (fork() == 0) {
    exec(argv_a[1], argv_a + 1);
    printf("Error running %s with: argc %d argc_a %d args %s %s %s...\n",
           argv_a[1], argc, argc_a, argv_a[2], argv_a[3], argv_a[4]);
    exit(1);
  }

  wait(0);
  /*printf("running %s with: argc %d argc_a %d args %s %s %s...\n", argv_a[1],
         argc, argc_a, argv_a[2], argv_a[3], argv_a[4]);

  fprintf(1, "\n All arguments:\n");
  for (int i = 0; i < argc_a; i++) {
    fprintf(1, "%s; ", argv_a[i]);
  }
  fprintf(1, "\n");
  */

  return;
}

// NOTE: When I was trying to store extra arguments on argv, that made memory
// bugs happen. Maybe argv is allocated to have the size of argc instead of
// MAXARG
int main(int argc, char *argv[]) {

  if (argc < 2) {
    fprintf(2, "Usage: xargs command args...\n");
    exit(1);
  }

  // Read and execute lines from stdin
  char line[50];
  char *p = line;
  while (read(0, p, 1)) {
    // NOTE: Each line should have a single arg
    if (*p == '\n') {
      *p = '\0';
      //Run argument
      xarg(argc, argv, line);
      p = line;
    } else {
        p++;
    }
  }

  exit(0);
}
