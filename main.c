#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MAX_ARGS 15
#define MAX_ARG_LENGTH 256
#define DELIMITERS " \n\t"

char ** read_command_line() {
  char ** args = malloc(sizeof(char*[15]));
  for(int i = 0; i < 15; i++) args[i] = NULL;

  char * buf = NULL;
  size_t length = 0;
  ssize_t line_length = getline(&buf, &length, stdin);

  if(line_length == -1) {
    return NULL;
  }
  
  char * command = strtok(buf, DELIMITERS);
  args[0] = strdup(command);
  
  int i = 1;
  for(char * arg = strtok(NULL, DELIMITERS), i = 1; arg != NULL; arg = strtok(NULL, DELIMITERS), i++) {
    args[i] = strdup(arg);
  }

  return args;
}

int cd(char ** args) { return chdir(args[1]); }

int pwd() {
  char * cwd = getcwd(NULL, 0);
  if(cwd == NULL) return -1;

  printf("%s\n", cwd);
  free(cwd);

  return 0;
}

void debug(char ** args) {
  for(int i = 0; i < MAX_ARGS; i++) {
    printf("%d: <%s>\n", i, args[i]);
  }
}

void free_data(char ** args) {
  for(int i = 0; i < MAX_ARGS; i++) if(args[i] != NULL) free(args[i]);
  free(args);
}

int main(int argc, char** argv) {
  signal(SIGINT, SIG_IGN);

  while(1) {
    printf("> ");

    char ** args = read_command_line();

    if(args == NULL) {
      break;
    }
    
    int ret = 0;
    char * res = NULL;
    
    if(strncmp(args[0], "cd", strlen("cd")) == 0) { 
      ret = cd(args);
    }
    else if(strncmp(args[0], "pwd", strlen("pwd")) == 0) {
      ret = pwd();
    }
    else if(strncmp(args[0], "debug", strlen("debug")) == 0) {
      debug(args);
    }

    if(ret != 0) perror(args[0]);

    free_data(args);
  }

  return 0;
}
