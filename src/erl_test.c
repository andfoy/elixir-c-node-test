#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// #include "erl_interface.h"
#include "ei.h"

#define BUFSIZE 1000

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

int main(int argc, char **argv)
{
  struct in_addr addr; /* 32-bit IP number of host */
  int port;            /* Listen port number */
  int listen;          /* Listen socket */
  int fd;              /* fd to Erlang node */
  int n = 0;           /*  */
  ei_cnode ec;         /* C node information */
  ErlConnect conn;     /* Connection data */

  int loop = 1;               /* Lopp flag */
  int got;                    /* Result of receive */
  // unsigned char buf[BUFSIZE]; /* Buffer for incoming message */
  ei_x_buff buf;              /* Buffer for incoming message */
  erlang_msg emsg;        /* Incoming message */

  // ETERM *fromp, *tuplep, *fnp, *argp, *resp;
  int res;

  port = atoi(argv[1]);

  ei_init();

  addr.s_addr = inet_addr("127.0.0.1");
  // if (ei_connect_xinit("localhost", "testc", "test@127.0.0.1",
  //   &addr, "secretcookie", 0) == -1)
  //     erl_err_quit("erl_connect_xinit");

  if (ei_connect_xinit(&ec,
                       "localhost",
                       "testc",
                       "testc@127.0.0.1",
                       &addr,
                       "secretcookie",
                       n++) < 0)
  {
    fprintf(stderr, "ERROR when initializing: %d", erl_errno);
    exit(-1);
  }

  /* Make a listen socket */
  if ((listen = my_listen(port)) <= 0)
  {
    fprintf(stderr, "ERROR when initializing socket");
    exit(-1);
    // erl_err_quit("my_listen");
  }

  if (ei_publish(&ec, port) == -1)
  {
    fprintf(stderr, "ERROR when trying to publish on the port %d: %d", port, erl_errno);
    exit(-1);
  }
  // erl_err_quit("erl_publish");

  if ((fd = ei_accept(&ec, listen, &conn)) == ERL_ERROR)
  {
    fprintf(stderr, "ERROR when accepting a connection: %d", erl_errno);
    exit(-1);
    // erl_err_quit("erl_accept");
  }
  fprintf(stderr, "Connected to %s\n\r", conn.nodename);

  while (loop)
  {
    got = ei_xreceive_msg(fd, &emsg, &buf);
    printf("Got message %d", got)
    if (got == ERL_TICK)
    {
      /* ignore */
    }
    else if (got == ERL_ERROR)
    {
      loop = 0;
    }
    else
    {

      if (emsg.msgtype == ERL_REG_SEND)
      {
        printf("Some call goes here");
        // fromp = erl_element(2, emsg.msg);
        // tuplep = erl_element(3, emsg.msg);
        // fnp = erl_element(1, tuplep);
        // argp = erl_element(2, tuplep);

        // if (strncmp(ERL_ATOM_PTR(fnp), "foo", 3) == 0)
        // {
        //   res = foo(ERL_INT_VALUE(argp));
        // }
        // else if (strncmp(ERL_ATOM_PTR(fnp), "bar", 3) == 0)
        // {
        //   res = bar(ERL_INT_VALUE(argp));
        // }

        // resp = erl_format("{cnode, ~i}", res);
        // erl_send(fd, fromp, resp);

        // erl_free_term(emsg.from);
        // erl_free_term(emsg.msg);
        // erl_free_term(fromp);
        // erl_free_term(tuplep);
        // erl_free_term(fnp);
        // erl_free_term(argp);
        // erl_free_term(resp);
      }
    }
  } /* while */
}