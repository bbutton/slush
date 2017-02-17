#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MAX_ARGS 17
#define MAX_ARG_LENGTH 256
#define DELIMITERS " \n\t"

char ** read_command_line(char * command_line) {
  char ** args = malloc(sizeof(char*[MAX_ARGS]));
  for(int i = 0; i < MAX_ARGS; i++) args[i] = NULL;
  
  int arg_counter = 0;
  char * arg = NULL;
  
  while((arg = strsep(&command_line, DELIMITERS)) != NULL) {
    if(strlen(arg) > 0)
      args[arg_counter++] = strdup(arg);
  }

  return args;
}

char *** read_all_commands() {
  char * buf = NULL;
  size_t length = 0;
  ssize_t line_length = getline(&buf, &length, stdin);

  if(line_length == -1) {
    return NULL;
  }
  
  int separator_count = 0;
  for(char * tmp = buf; *tmp; tmp++) {
    separator_count += *tmp == '(';
  }
  
  char *** all_commands = malloc(sizeof(char***[separator_count + 2]));
  for(int i = 0; i < separator_count + 2; i++) all_commands[i] = NULL;
  
  char * command_line = NULL;
  int command_index = 0;
  
  while((command_line = strsep(&buf, "(\n")) != NULL) {
    while(*command_line && *command_line == ' ') command_line++;
    if(strlen(command_line) > 0) {
      all_commands[command_index] = read_command_line(command_line);
    }
    command_index++;
  }

  return all_commands;
}  

int cd(char ** args) { return chdir(args[1]); }

int pwd() {
  char * cwd = getcwd(NULL, 0);
  if(cwd == NULL) return -1;

  printf("%s\n", cwd);
  free(cwd);

  return 0;
}

void debug_internal(char ** args, int length) {
  for(int i = 0; i < length; i++) {
    printf("%d: <%s>\n", i, args[i]);
  }
}  

void debug(char ** args) {
  debug_internal(args, MAX_ARGS);
}

void free_data(char ** args) {
  for(int i = 0; i < MAX_ARGS; i++) if(args[i] != NULL) free(args[i]);
  free(args);
}

int exec_child(char ** args) {
  pid_t new_pid = fork();
  if(new_pid > 0) {
    int stat_loc = 0;
    pid_t dead_child = wait(&stat_loc);
  } else if (new_pid == 0) {
    char * exe_name = args[0];

    int exec_ret = execvp(args[0], args);
    if(exec_ret == -1) {
      perror(args[0]);
      exit(exec_ret);
    }
  } else {
    return -1;
  }

  return 0;
}

void handle_interrupt(int signal_num) {
}


int main(int argc, char** argv) {
  signal(SIGINT, handle_interrupt);
  
  while(1) {
    printf("> ");

    char *** all_commands = read_all_commands();
    if(all_commands == NULL) {
      break;
    }
      
    
    for(int counter = 0; all_commands[counter]; counter++) {
      char ** args = all_commands[counter];

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
      else {
	ret = exec_child(args);
      }
      
      if(ret != 0) perror(args[0]);
      
      free_data(args);
    }
  }

  return 0;
}
