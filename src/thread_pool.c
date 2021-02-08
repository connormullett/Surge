
#include "../headers/thread_pool.h"

char *builtin_str[] = {"get", "set", "del", "dump", "help", "quit"};

char *(*builtin_func[])(table_t *t, request_t *request) = {
    &cli_get, &cli_set, &cli_del, &cli_dump, &cli_help, &cli_quit};

char *builtin_help_strings[] = {
    "get value by key",     "set a new key/value pair",
    "delete entry",         "dump table contents to stdout",
    "display help message", "exit the program"};

int cli_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

void *thread_work_job(void *arg) {
  table_t *table = (table_t *)arg;

  while (true) {
    int *pclient;
    pthread_mutex_lock(&queue_thread_lock);

    if ((pclient = dequeue()) == NULL) {
      pthread_cond_wait(&mutex_signal, &queue_thread_lock);

      pclient = dequeue();
    }
    pthread_mutex_unlock(&queue_thread_lock);

    if (pclient != NULL) {
      printf("Connection: handling job %d\n", *pclient);
      handle_connection(pclient, table);
    }
  }
}

void *handle_connection(void *p_client_socket, table_t *table) {
  int client_socket = *((int *)p_client_socket);
  free(p_client_socket);

  char buffer[BUFSIZE];
  size_t bytes_read;
  int msgsize = 0;
  char *value = NULL;

  while ((bytes_read = read(client_socket, buffer + msgsize,
                            sizeof(buffer) - msgsize)) > 0) {
    msgsize += bytes_read;
    if (msgsize > BUFSIZE - 1 || buffer[msgsize - 1] == '\n') break;
  }

  check(bytes_read, "read error");
  buffer[msgsize - 1] = 0;
  fflush(stdout);

  value = execute(table, buffer);

  if (value == NULL) {
    value = "OK";
  }

  write(client_socket, value, strlen(value));

  close(client_socket);
  return NULL;
}

char *execute(table_t *table, char *buffer) {
  char *value = NULL;
  request_t *request = parse_request_t(buffer);

  if (request == NULL) {
    puts("An error occured");
  }

  pthread_mutex_lock(&table_thread_lock);

  for (int i = 0; i < cli_num_builtins(); i++) {
    if (strcmp(request->operation, builtin_str[i]) == 0) {
      value = (*builtin_func[i])(table, request);
      break;
    }
  }

  pthread_mutex_unlock(&table_thread_lock);

  free(request);
  return value;
}

char *cli_get(table_t *t, request_t *request) {
  return table_t_get(t, request->key);
}

char *cli_set(table_t *t, request_t *request) {
  table_t_set(t, request->key, request->value);
  return NULL;
}

char *cli_del(table_t *t, request_t *request) {
  table_t_delete(t, request->key);
  return NULL;
}

char *cli_dump(table_t *t, request_t *request) {
  table_t_dump(t);
  return NULL;
}

#define MESSAGE_BUFFER_SIZE 1000
char *cli_help(table_t *t, request_t *request) {
  char *out = (char *)malloc(sizeof(char) * MESSAGE_BUFFER_SIZE);
  char *entry = (char *)malloc(sizeof(char) * MESSAGE_BUFFER_SIZE);

  for (int i = 0; i < cli_num_builtins(); i++) {
    sprintf(entry, "%s\t%s\n", builtin_str[i], builtin_help_strings[i]);
    strncat(out, entry, strlen(entry));
  }

  free(entry);
  return out;
}

char *cli_quit(table_t *t, request_t *request) {
  table_t_drop(t);
  exit(0);
}

request_t *init_request_t(void) {
  request_t *out = (request_t *)malloc(sizeof(request_t));
  out->operation = NULL;
  out->key = NULL;
  out->value = NULL;
  return out;
}

#define TOK_DELIM " \t"
request_t *parse_request_t(char *buffer) {
  // TODO: this is extremely hacky, rewrite

  size_t len = strlen(buffer);
  char *copy = (char *)malloc(sizeof(char) * len);
  memcpy(copy, buffer, len);

  request_t *request = init_request_t();

  // check if token is null
  char *op = strtok(copy, TOK_DELIM);
  request->operation = (char *)malloc(sizeof(char) * strlen(op));
  strcpy(request->operation, op);

  if (strcmp(op, "dump") == 0) return request;

  char *key = strtok(NULL, TOK_DELIM);
  if (key != NULL) {
    request->key = (char *)malloc(sizeof(char) * strlen(key));
    strcpy(request->key, key);
  }

  if (key != NULL && strcmp(key, "get") == 0) return request;

  char *val = strtok(NULL, TOK_DELIM);
  if (val != NULL) {
    request->value = (char *)malloc(sizeof(char) * strlen(val));
    strcpy(request->value, val);
  }

  free(copy);
  return request;
}

pthread_t *thread_pool_init(table_t *t) {
  pthread_t *thread_pool =
      (pthread_t *)malloc(sizeof(pthread_t) * THREAD_POOL_SIZE);
  for (int i = 0; i < THREAD_POOL_SIZE; i++) {
    pthread_create(&thread_pool[i], NULL, thread_work_job, t);
  }
  return thread_pool;
}

int check(int exp, const char *msg) {
  if (exp == SOCKETERROR) {
    perror(msg);
    exit(1);
  }
  return exp;
}
