#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {
  while(1) {
    printf("> ");

    size_t length;
    char * buf;
    size_t line_length = getline(&buf, &length, stdin);

    printf("exe: %s\n", strtok(buf, " "));
    for(char * arg = strtok(NULL, " "); arg != NULL; arg = strtok(NULL, " ")) {
      printf("arg: %s\n", arg);
    }
  }

  return 0;
}