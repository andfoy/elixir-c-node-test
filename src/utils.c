#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ei.h"
#include "erl_test/utils.h"
#include "erl_test/uthash.h"
#include "erl_test/dispatchers.h"

int my_listen(int port)
{
  int listen_fd;
  struct sockaddr_in addr;
  int on = 1;

  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return (-1);

  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  memset((void *)&addr, 0, (size_t)sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    return (-1);

  listen(listen_fd, 5);
  return listen_fd;
}

void process_tuple(char *buff, int *index, int size, int fd)
{
  // ei_term term;
  // int res = ei_decode_ei_term(buff, index, &term);
  // if(res) {
  // for (int i = 0; i < size; i++)
  // {
  ei_term term;
  char *msg_type;
  int res = ei_decode_ei_term(buff, index, &term);
  fprintf(stderr, "Term decoding result: %d\n", res);
  fprintf(stderr, "Current index: %d\n", *index);

  char term_type = term.ei_type;
  switch (term_type)
  {
  case ERL_SMALL_TUPLE_EXT:
    process_tuple(buff, index, term.arity, fd);
    break;
  case ERL_SMALL_ATOM_UTF8_EXT:
    msg_type = term.value.atom_name;
    erlang_pid pid = term.value.pid;
    dispatch_message(msg_type, buff, index, size, pid, fd);
  default:
    break;
  }
  // }
  // }
}

void dispatch_message(char *msg_type, char *buff, int *index, int size,
                      erlang_pid pid, int fd)
{
  func_dispatcher *s;
  HASH_FIND_STR(functions, msg_type, s);
  if (s)
  {
    message_handler func = s->func;
    func(buff, index, pid, fd);
  }
}
