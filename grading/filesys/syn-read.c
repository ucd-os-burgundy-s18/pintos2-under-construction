#include <random.h>
#include <stdio.h>
#include <syscall.h>
#include "fslib.h"
#include "syn-read.h"

const char test_name[] = "syn-read";

static char buf[BUF_SIZE];

#define CHILD_CNT 10

void
test_main (void) 
{
  pid_t children[CHILD_CNT];
  int fd;
  int i;

  check (create (filename, sizeof buf), "create \"%s\"", filename);
  check ((fd = open (filename)) > 1, "open \"%s\"", filename);
  random_bytes (buf, sizeof buf);
  check (write (fd, buf, sizeof buf) > 0, "write \"%s\"", filename);
  msg ("close \"%s\"", filename);
  close (fd);

  for (i = 0; i < CHILD_CNT; i++) 
    {
      char cmd_line[128];
      snprintf (cmd_line, sizeof cmd_line, "child-syn-read %d", i);
      check ((children[i] = exec (cmd_line)) != PID_ERROR,
             "exec child %d of %d: \"%s\"", i + 1, (int) CHILD_CNT, cmd_line);
    }

  for (i = 0; i < CHILD_CNT; i++) 
    {
      int status = join (children[i]);
      check (status == i, "join child %d of %d", i + 1, (int) CHILD_CNT);
    }
}