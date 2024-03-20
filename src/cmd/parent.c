#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include "error.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t run(const char *path, char *const *argv, char *const *envp) {
  pid_t pid = fork();
  if (pid == 0) {
    if (execve(path, argv, envp) == -1)
      error(CHILD_FAILED, "execve(...) failed, errno: %d", errno);
  }
  return pid;
}

bool forkNew() {}

bool killAll() {}

bool killLast() {}

bool list() {}

bool silent() {}

bool grant() {}

bool silentAll() {}

bool grantAll() {}

bool priority() {}

bool quit() {}

bool unknown(const char *command) {
  printf("Unknown command: %s\n", command);
  return true;
}

bool handleCommand(const char *command) {
  if (strcmp(command, "+")) return forkNew();
  if (strcmp(command, "-")) return killLast();
  if (strcmp(command, "l")) return list();
  if (strcmp(command, "k")) return killAll();
  if (strcmp(command, "s")) return silentAll();
  if (strcmp(command, "g")) return grantAll();
  if (strcmp(command, "q")) return quit();

  int id = 0;
  if (sscanf(command, "s%d", &id)) return silent(id);
  if (sscanf(command, "g%d", &id)) return grant(id);
  if (sscanf(command, "p%d", &id)) return priority(id);
  return unknown(command);
}

int main(int argc) {
  while (true) {
    char command[256];
    scanf("%255s", command);
    if (!handleCommand(command)) break;
  }
}
