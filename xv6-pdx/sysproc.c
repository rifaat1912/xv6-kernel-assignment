#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#ifdef PDX_XV6
#include "pdx-kernel.h"
#endif // PDX_XV6
#ifdef CS333_P2
  #include "uproc.h"
#endif  // CS333_P2

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_date ( void )
{
  struct rtcdate *d ;
  if (argptr ( 0 ,( void*)&d , sizeof ( struct rtcdate)) < 0)
    return -1;
  cmostime(d);
  return 0;
 }
}
#ifdef CS333_P2
int
sys_getuid(void)
{
  struct proc *curproc = myproc();
  return curproc->uid;
}

int
sys_getgid(void)
{
  struct proc *curproc = myproc();
  return curproc->gid;
}

int
sys_getppid(void)
{
  struct proc *curproc = myproc();
  struct proc *parent = curproc->parent;
  return parent != NULL ? parent->pid : 0;
}

int sys_setuid(void)
{
  uint uid;
  struct proc *curproc = myproc();

  if(argint(0, (int*)&uid) >= 0) {
    if(uid >= 0 && uid <= 32767) {
      curproc->uid = uid;
      return 0;
    }
  }

  return -1;
}

int sys_setgid(void)
{
  uint gid;
  struct proc *curproc = myproc();

  if(argint(0, (int*)&gid) >= 0) {
    if(gid >= 0 && gid <= 32767) {
     curproc->gid = gid;
      return 0;
    }
  }

  return -1;
}

int sys_getprocs(void)
{
  uint max;
  struct uproc* proc;

  if (argint(0,(int*)&max) >= 0) {
    if (max == 1 || max == 16 || max == 64 || max == 72) {
      if (argptr(1, (void*)&proc, sizeof(struct uproc)) >= 0) {
        return getprocs(max, proc);
      }
   }
  }

  return -1;
}
+#endif //CS333_P2

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      return -1;
    }
    sleep(&ticks, (struct spinlock *)0);
  }
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  xticks = ticks;
  return xticks;
}

#ifdef PDX_XV6
// shutdown QEMU
int
sys_halt(void)
{
  do_shutdown();  // never returns
  return 0;
}
#endif // PDX_XV6
