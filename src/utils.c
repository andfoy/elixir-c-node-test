#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ei.h"
#include "erl_test/utils.h"

int foo(int x)
{
  return x + 1;
}

int bar(int y)
{
  return y * 2;
}


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

void process_tuple(char* buff, int *index, int size) {
    // ei_term term;
    // int res = ei_decode_ei_term(buff, index, &term);
    // if(res) {
    for(int i = 0; i < size; i++) {
        ei_term term;
        int res = ei_decode_ei_term(buff, index, &term);
        fprintf(stderr, "Term decoding result: %d\n", res);
    }
    // }
}
