#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

//NOTE: This is not like Linux find. It just looks for a specific file inside a directory tree.
//NOTE: This is heavily based on xv6 ls implementation. Needs optimization.

char*
cut_name(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  *(buf+strlen(p)) = '\0';
  return buf;
}

void
find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    char* currentname = cut_name(path);

    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type){
    case T_FILE:
        if (!strcmp(filename, currentname)) {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        // one 1 for \ and other for \0
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf+strlen(buf);
        // Puts / after the current path in buf
        *p++ = '/';
        // Reads each file inside the directory
        while(read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0)
                continue;
            // Saves pathname for file inside directory in p
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;

            if(stat(buf, &st) < 0){
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            // Call find for files inside directory
            if (strcmp(de.name, ".") && strcmp(de.name, "..")) {
                find(buf, filename);
            }
        }
        break;
    }
    close(fd);
    return;
}

int
main(int argc, char *argv[])
{

  if(argc < 3){
    fprintf(2, "Usage: find directory filename\n");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);

}
