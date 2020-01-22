#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// #include "erl_interface.h"
#include "ei.h"
#include "erl_test/utils.h"

#define BUFSIZE 1000

int main(int argc, char **argv)
{
  (void)argc;

  const char *epmd_argv[64] = {"epmd", "-daemon", NULL}; /* EPMD args */

  // char *epmd_argv[] = { "epmd", "-daemon", NULL };

  struct in_addr addr; /* 32-bit IP number of host */
  int port;            /* Listen port number */
  int listen;          /* Listen socket */
  int fd;              /* fd to Erlang node */
  int n = 0;           /*  */
  ei_cnode ec;         /* C node information */
  ErlConnect conn;     /* Connection data */

  int loop = 1; /* Lopp flag */
  int got;      /* Result of receive */
  // unsigned char buf[BUFSIZE]; /* Buffer for incoming message */
  ei_x_buff buf;   /* Buffer for incoming message */
  erlang_msg emsg; /* Incoming message */

  // ETERM *fromp, *tuplep, *fnp, *argp, *resp;
  int res;

  port = atoi(argv[1]);

  ei_x_new_with_version(&buf);
  ei_init();

  // Starting EPMD before starting node
  int rc = exec_prog(epmd_argv);

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
    fprintf(stderr, "Waiting for a message to come?\n");
    got = ei_xreceive_msg(fd, &emsg, &buf);
    fprintf(stderr, "Got message %d\n", got);
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
        fprintf(stderr, "Some call goes here\n");
        int index = 1; // Set to 1 due to version byte
        ei_term term;
        ei_term term2;
        int size;
        int type;
        char *buff = buf.buff;
        res = ei_decode_ei_term(buff, &index, &term);
        char term_type = term.ei_type;
        switch (term_type)
        {
        case ERL_SMALL_TUPLE_EXT:
          process_tuple(buff, &index, term.arity);
          break;

        default:
          break;
        }
        // res = ei_decode_tuple_header(buff, &index, &size);
        // fprintf(stderr, "Decode result: %d\n", res);
        // res = ei_decode_ei_term(buff, &index, &term2);
        // fprintf(stderr, "Decode result: %d\n", res);
        // fprintf(stderr, "Decode result: %d\n", res);
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