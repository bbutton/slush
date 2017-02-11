#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void read_command_line(char args[][256]) {
    size_t length;
    char * buf;
    size_t line_length = getline(&buf, &length, stdin);

    char * command = strtok(buf, " ");
    strncpy(args[0], command, 256);
    
    int i = 1;
    for(char * arg = strtok(NULL, " "), i = 1; arg != NULL; arg = strtok(NULL, " "), i++) {
      strncpy(args[i], arg, 256);
    }
}

int cd(char args[][256]) {
  printf("chdir to %s\n", args[1]);
  int ret = chdir(args[1]);
  char buf[256];
  printf("New directory now %s\n", getcwd(buf, 256));
  return ret;
}

int main(int argc, char** argv) {
  while(1) {
    printf("> ");

    char args[15][256];
    for(int i = 0; i < 15; i++) {
      memset(args[i], 0x00, 256);
    }
    
    read_command_line(args);

    printf("command is %s\n", args[0]);
    
    int ret = 0;
    if(strncmp(args[0], "cd", strlen("cd")) == 0) { 
      ret = cd(args);
    }

    if(ret != 0) perror("failed to run command");
  }

  return 0;
}
