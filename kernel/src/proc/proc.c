#include "defs.h"
#include "proc.h"
#include "spinlock.h"
#include "param.h"

struct ptable {
  spinlock_t lock;
  struct proc proc[NPROC];
} ptable;

void proc_init(void)
{
  init_lock(&ptable.lock, "ptable");
  memset(ptable.proc, 0, sizeof(ptable.proc));
}


