
#include "../headers/cli.h"

#include <stdbool.h>
#include <string.h>

#include "../headers/table.h"
#include "../headers/thread_pool.h"

void *command_line(void *arg) {
  table_t *table = (table_t *)arg;
  int input;
  bool error_flag = false;

  char buffer[MAX_COMMAND_SIZE];
  const char *prompt = "> ";
  int sz = sizeof(buffer);

  // TODO: graceful exits

  while (1) {
    memset(buffer, 0, MAX_COMMAND_SIZE);

    input = get_line(prompt, buffer, sz);

    if (input == NO_INPUT) {
      puts("no input");
      error_flag = true;
    }

    if (input == TOO_LONG) {
      puts("too long");
      error_flag = true;
    }

    char *value = execute(table, buffer);

    if (value != NULL) printf("%s\n", value);

  }  // while
}

int get_line(const char *prompt, char *buff, size_t sz) {
  int ch, extra;

  if (prompt != NULL) {
    printf("%s ", prompt);
    fflush(stdout);
  }

  if (fgets(buff, sz, stdin) == NULL) return NO_INPUT;

  if (buff[strlen(buff) - 1] != '\n') {
    extra = 0;
    while (((ch = getchar()) != '\n') && (ch != EOF)) extra = 1;
    return (extra == 1) ? TOO_LONG : OK;
  }

  buff[strlen(buff) - 1] = '\0';
  return OK;
}
