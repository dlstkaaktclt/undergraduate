/*
 * M1522.000800 System Programming
 * Shell Lab
 *
 * tsh - A tiny shell program with job control
 *
 * Name: <KimBochang>
 * Student id: <2014-16757>
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

/* Misc manifest constants */
#define MAXLINE    1024   /* max line size */
#define MAXARGS     128   /* max args on a command line */
#define MAXJOBS      16   /* max jobs at any point in time */
#define MAXJID    1<<16   /* max job ID */

/* Job states */
#define UNDEF 0 /* undefined */
#define FG 1    /* running in foreground */
#define BG 2    /* running in background */
#define ST 3    /* stopped */

/*
 * Jobs states: FG (foreground), BG (background), ST (stopped)
 * Job state transitions and enabling actions:
 *     FG -> ST  : ctrl-z
 *     ST -> FG  : fg command
 *     ST -> BG  : bg command
 *     BG -> FG  : fg command
 * At most 1 job can be in the FG state.
 */

/* Global variables */
extern char **environ;      /* defined in libc */
char prompt[] = "tsh> ";    /* command line prompt (DO NOT CHANGE) */
int verbose = 0;            /* if true, print additional output */
int nextjid = 1;            /* next job ID to allocate */
char sbuf[MAXLINE];         /* for composing sprintf messages */

struct job_t {              /* The job struct */
  pid_t pid;              /* job PID */
  int jid;                /* job ID [1, 2, ...] */
  int state;              /* UNDEF, BG, FG, or ST */
  char cmdline[MAXLINE];  /* command line */
};
struct job_t jobs[MAXJOBS]; /* The job list */
/* End global variables */


/* Function prototypes */

/*----------------------------------------------------------------------------
 * Functions that you will implement
 */

void eval(char *cmdline);
int builtin_cmd(char **argv);
void do_bgfg(char **argv);
void waitfg(pid_t pid);

void sigchld_handler(int sig);
void sigint_handler(int sig);
void sigtstp_handler(int sig);

/*----------------------------------------------------------------------------*/

/* These functions are already implemented for your convenience */
int parseline(const char *cmdline, char **argv);
void sigquit_handler(int sig);

void clearjob(struct job_t *job);
void initjobs(struct job_t *jobs);
int maxjid(struct job_t *jobs);
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline);
int deletejob(struct job_t *jobs, pid_t pid);
pid_t fgpid(struct job_t *jobs);
struct job_t *getjobpid(struct job_t *jobs, pid_t pid);
struct job_t *getjobjid(struct job_t *jobs, int jid);
int pid2jid(pid_t pid);
void listjobs(struct job_t *jobs);

void usage(void);
void unix_error(char *msg);
void app_error(char *msg);
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);

/* my custom error-handling version functions.
 * all of custom error-handling functions get errmsg field to report where it's error occurs.
 * they first letter is Capital letter, so don't confuse by this.
 */
static int Sigemptyset(sigset_t *set, char *errmsg);
static int Sigfillset(sigset_t *set, char *errmsg);
static int Sigaddset(sigset_t *set,int signum, char *errmsg);
static int Sigprocmask(int how,const sigset_t *set, sigset_t *oldset, char *errmsg);

static int Kill(pid_t pid, int sig, char *errmsg);
static pid_t Fork(char *errmsg);
static int Setpgid(pid_t pid, pid_t pgid, char *errmsg);

static int Sigemptyset(sigset_t *set, char *errmsg)
{
    int res = sigemptyset(set);
    if(res == -1)
    {
        unix_error(errmsg);
        unix_error("sigemptyset error occured");
        exit(0);
    }
    return res;
}
static int Sigfillset(sigset_t *set, char *errmsg)
{
    int res = sigfillset(set);
    if(res == -1)
    {
        unix_error(errmsg);
        unix_error("sigfillset error occured");
        exit(0);
    }
    return res;
}
static int Sigaddset(sigset_t *set,int signum, char *errmsg)
{
    int res = sigaddset(set,signum);
    if(res == -1)
    {
        unix_error(errmsg);
        unix_error("sigaddset error occured");
        exit(0);
    }
    return res;
}
static int Sigprocmask(int how,const sigset_t *set, sigset_t *oldset, char *errmsg)
{
    int res = sigprocmask(how,set,oldset);
    if(res == -1)
    {
        unix_error(errmsg);
        unix_error("sigprocmask error occured");
        exit(0);
    }
    return res;
}

static int Kill(pid_t pid, int sig, char *errmsg)
{
    int res = kill(pid,sig);
    if(res == -1)
    {
        unix_error(errmsg);
        unix_error("kill error occured");
        exit(0);
    }
    return res;
}
static pid_t Fork(char *errmsg)
{
    pid_t res = fork();
    if(res == -1)
    {
        unix_error(errmsg);
        unix_error("fork error occured");
        exit(0);
    }
    return res;
}

static int Setpgid(pid_t pid, pid_t pgid, char *errmsg)
{
    int res = setpgid(pid,pgid);
    if(res == -1)
    {
        unix_error(errmsg);
        unix_error("setpgid error occured");
        exit(0);
    }
    return res;
}




/*
 * main - The shell's main routine
 */
int main(int argc, char **argv)
{
  char c;
  char cmdline[MAXLINE];
  int emit_prompt = 1; /* emit prompt (default) */

  /* Redirect stderr to stdout (so that driver will get all output
   * on the pipe connected to stdout) */
  dup2(1, 2);

  /* Parse the command line */
  while ((c = getopt(argc, argv, "hvp")) != EOF) {
    switch (c) {
      case 'h':             /* print help message */
        usage();
        break;
      case 'v':             /* emit additional diagnostic info */
        verbose = 1;
        break;
      case 'p':             /* don't print a prompt */
        emit_prompt = 0;  /* handy for automatic testing */
        break;
      default:
        usage();
    }
  }

  /* Install the signal handlers */

  /* These are the ones you will need to implement */
  Signal(SIGINT,  sigint_handler);   /* ctrl-c */
  Signal(SIGTSTP, sigtstp_handler);  /* ctrl-z */
  Signal(SIGCHLD, sigchld_handler);  /* Terminated or stopped child */

  /* This one provides a clean way to kill the shell */
  Signal(SIGQUIT, sigquit_handler);

  /* Initialize the job list */
  initjobs(jobs);

  /* Execute the shell's read/eval loop */
  while (1) {

    /* Read command line */
    if (emit_prompt) {
      printf("%s", prompt);
      fflush(stdout);
    }
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin))
      app_error("fgets error");
    if (feof(stdin)) { /* End of file (ctrl-d) */
      fflush(stdout);
      exit(0);
    }

    /* Evaluate the command line */
    eval(cmdline);
    fflush(stdout);
    fflush(stdout);
  }

  exit(0); /* control never reaches here */
}

/*
 * eval - Evaluate the command line that the user has just typed in
 *
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.
 */
void eval(char *cmdline)
{
  char *argv[MAXARGS]; /* argument list execve() */
  char buf[MAXLINE];   /* Holds modified command line */
  int bg;          /* sholud the job run in bg or fg? */
  pid_t pid;           /* process id */

  char *evalerrmsg = "at eval() function";      /* errormessage field for system call error handling */
  strcpy(buf,cmdline);
  bg = parseline(buf, argv);
  if(argv[0] == NULL) return;

  if (!builtin_cmd(argv))
  {
    sigset_t mask,prev_mask;
    Sigemptyset(&mask,evalerrmsg);
    Sigaddset(&mask,SIGCHLD,evalerrmsg);
    Sigprocmask(SIG_BLOCK,&mask,&prev_mask,evalerrmsg);     // all of these calls implements error handling.

    /* block SIGCHIDSIGNAL when addjob to joblist */
    pid = Fork(evalerrmsg);

    if (pid == 0){  /* child process */
        Setpgid(0,0,evalerrmsg); /* set pgid to child's own pid */
        Sigprocmask(SIG_SETMASK, &prev_mask, NULL,evalerrmsg); /* unblock SIGCHIDSIGNAL. child inherit sigmask, so it sholud be. */
        if(execve(argv[0],argv,environ)<0)
        {
            printf("%s: Command not found\n",argv[0]);    /* if program doesn't exist, print error message and exit child process */
            fflush(stdout);                             /* fflush for printf */
            exit(0);
        }
    }
    else /*parent process, add new job to joblist */
    {
      if(bg)
      {
          struct job_t *newjob;
          addjob(jobs,pid,BG,cmdline);
          newjob = getjobpid(jobs,pid);
          printf("[%d] (%d) %s",newjob->jid,pid,cmdline);
          fflush(stdout);
      }
      else addjob(jobs,pid,FG,cmdline);         /* add job to joblist */
      Sigprocmask(SIG_SETMASK, &prev_mask, NULL,evalerrmsg);/* after add new job, unblock SIGCHIDSIGNAL */
      if (!bg)
      {
         waitfg(pid);
      }
    }

    /* parent waits for foreground job to terminate */

  }
  return;
}

/*
 * parseline - Parse the command line and build the argv array.
 *
 * Characters enclosed in single quotes are treated as a single
 * argument.  Return true if the user has requested a BG job, false if
 * the user has requested a FG job.
 */
int parseline(const char *cmdline, char **argv)
{
  static char array[MAXLINE]; /* holds local copy of command line */
  char *buf = array;          /* ptr that traverses command line */
  char *delim;                /* points to first space delimiter */
  int argc;                   /* number of args */
  int bg;                     /* background job? */

  strcpy(buf, cmdline);
  buf[strlen(buf)-1] = ' ';  /* replace trailing '\n' with space */
  while (*buf && (*buf == ' ')) /* ignore leading spaces */
    buf++;

  /* Build the argv list */
  argc = 0;
  if (*buf == '\'') {
    buf++;
    delim = strchr(buf, '\'');
  }
  else {
    delim = strchr(buf, ' ');
  }

  while (delim) {
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim + 1;
    while (*buf && (*buf == ' ')) /* ignore spaces */
      buf++;

    if (*buf == '\'') {
      buf++;
      delim = strchr(buf, '\'');
    }
    else {
      delim = strchr(buf, ' ');
    }
  }
  argv[argc] = NULL;

  if (argc == 0)  /* ignore blank line */
    return 1;

  /* should the job run in the background? */
  if ((bg = (*argv[argc-1] == '&')) != 0) {
    argv[--argc] = NULL;
  }
  return bg;
}

/*
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.
 */
int builtin_cmd(char **argv)
{
  if(strcmp(argv[0],"quit")==0)
  {
    exit(1);
  }
  if(strcmp(argv[0],"fg")==0)
  {
    do_bgfg(argv);
    return 1;
  }
  if(strcmp(argv[0],"bg")==0)
  {
    do_bgfg(argv);
    return 1;
  }
  if(strcmp(argv[0],"jobs")==0)
  {
    listjobs(jobs);
    return 1;
  }
  return 0;     /* not a builtin command */
}

/*
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv)
{
    char *bgfgerrmsg = "at do_bgfg() function";
    if(argv[1]==NULL)               /* if bg or fg command didn't get jid or pid */
    {
        printf("%s command requires PID or %%jobid argument\n",argv[0]);
        fflush(stdout);
        return;
    }
    if(argv[1][0] == '%')       /* if get jobid */
    {
        int jid;
        if(sscanf(argv[1],"%%%d",&jid)<=0)      /* if sscanf get no jobid */
        {
            printf("%s: argument must be a PID or %%jobid\n",argv[0]);
            fflush(stdout);
            return;
        }
        else
        {
            sigset_t mask,prev_mask;
            Sigemptyset(&mask,bgfgerrmsg);
            Sigaddset(&mask,SIGCHLD,bgfgerrmsg);
            Sigprocmask(SIG_BLOCK, &mask, &prev_mask,bgfgerrmsg);
            /* block SIGCHLD signal because these code change job list.*/
            struct job_t *job = getjobjid(jobs,jid);
            if(job==NULL)
            {
                printf("%%%d: No such job\n",jid);
                fflush(stdout);
                Sigprocmask(SIG_SETMASK, &prev_mask, NULL, bgfgerrmsg);
                return;
            }
            else
            {
                if(strcmp(argv[0],"fg")==0)
                {
                    if(job->state==ST)
                    {
                        pid_t jobpid = job->pid;   // to get exact pid, get value before unmask SIGCHLD signal
                        job->state=FG;
                        Kill(-(jobpid),SIGCONT,bgfgerrmsg);
                        Sigprocmask(SIG_SETMASK, &prev_mask, NULL, bgfgerrmsg);
                        waitfg(jobpid);    //if we use job-> pid instead of pid_t jobpid, job->pid get unvalid address after SIGCHLD handler
                        return;
                    }
                    else if(job->state == BG)
                    {
                        pid_t jobpid = job->pid;    // to get exact pid, get value before unmask SIGCHLD signal
                        job->state=FG;
                        Sigprocmask(SIG_SETMASK, &prev_mask, NULL, bgfgerrmsg);
                        waitfg(jobpid);            //if we use job-> pid instead of pid_t jobpid, job->pid get unvalid address after SIGCHLD handler
                        return;
                    }
                }
                else if(strcmp(argv[0],"bg")==0)
                {
                    if(job->state==ST)
                    {
                        job->state=BG;
                        Kill(-(job->pid),SIGCONT,bgfgerrmsg);
                        printf("[%d] (%d) %s",job->jid,job->pid,job->cmdline);
                        fflush(stdout);
                    }
                }
                Sigprocmask(SIG_SETMASK, &prev_mask, NULL, bgfgerrmsg);
                return;
            }
        }
    }
    else    /* if get pid */
    {
        unsigned int pid;
        if(sscanf(argv[1],"%u",&pid)<=0)        /* if sscanf dosen't find pid, print errormessage */
        {
            printf("%s: argument must be a PID or %%jobid\n",argv[0]);
            fflush(stdout);
            return;
        }
        else
        {
            sigset_t mask,prev_mask;
            Sigemptyset(&mask,bgfgerrmsg);
            Sigaddset(&mask,SIGCHLD,bgfgerrmsg);
            Sigprocmask(SIG_BLOCK, &mask, &prev_mask,bgfgerrmsg);

            /* block SIGCHLD signal because these code change job list.*/
            struct job_t *job = getjobpid(jobs,(pid_t) pid);
            if(job==NULL)
            {
                printf("(%u): No such process\n",pid);
                fflush(stdout);
                Sigprocmask(SIG_SETMASK, &prev_mask, NULL, bgfgerrmsg);
                return;
            }
            else
            {
                if(strcmp(argv[0],"fg")==0)
                {
                    if(job->state==ST)
                    {
                        pid_t jobpid = job->pid;    // to get exact pid, get value before unmask SIGCHLD signal
                        job->state=FG;
                        Kill(-(jobpid),SIGCONT,bgfgerrmsg);
                        Sigprocmask(SIG_SETMASK, &prev_mask, NULL, bgfgerrmsg);
                        waitfg(jobpid);     //if we use job-> pid instead of pid_t jobpid, job->pid get unvalid address after SIGCHLD handler
                        return;
                    }
                    else if(job->state == BG)
                    {
                        pid_t jobpid = job->pid;    // to get exact pid, get value before unmask SIGCHLD signal
                        job->state=FG;
                        Sigprocmask(SIG_SETMASK, &prev_mask, NULL, bgfgerrmsg);
                        waitfg(jobpid);     //if we use job-> pid instead of pid_t jobpid, job->pid get unvalid address after SIGCHLD handler
                        return;
                    }
                }
                else if(strcmp(argv[0],"bg")==0)
                {
                    if(job->state==ST)
                    {
                        job->state=BG;
                        Kill(-(job->pid),SIGCONT,bgfgerrmsg);
                        printf("[%d] (%d) %s",job->jid,job->pid,job->cmdline);
                        fflush(stdout);
                    }
                }
                Sigprocmask(SIG_SETMASK, &prev_mask, NULL, bgfgerrmsg);
                return;
            }
        }
        return;
    }
}

/*
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid)
{
    volatile pid_t fpid;
    while(pid == (fpid = fgpid(jobs)))      // wait fgjob is not current fgjob
    {
        sleep(1);
    }
    return;
}

/*****************
 * Signal handlers
 *****************/

/*
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.
 */
void sigchld_handler(int sig)
{
    int olderrno = errno;
    char *cldhdrerrmsg = "at sigchld_handler() function";
    sigset_t mask,prev_mask;
    Sigfillset(&mask,cldhdrerrmsg);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask,cldhdrerrmsg);
    // block all of signal during handler

    volatile int waitstat;
    int status;

    while((waitstat = waitpid(-1,&status,WNOHANG|WUNTRACED)) > 0)       //reap all terminated childs or process stopped childs
    {
       struct job_t *job = getjobpid(jobs,waitstat);
       if(WIFSTOPPED(status))
       {
            if(job!=NULL) job->state = ST;
            printf("Job [%d] (%d) stopped by signal %d\n",job->jid,waitstat,WSTOPSIG(status));
            fflush(stdout);
       }
       else
       {
           if(WIFSIGNALED(status))
           {
                printf("Job [%d] (%d) terminated by signal %d\n",job->jid,waitstat,WTERMSIG(status));
                fflush(stdout);
           }
           deletejob(jobs,waitstat);
       }
    }
    if(waitstat == -1)      //end of loop
    {
       if(errno!=ECHILD)        //if cause of ending loop is not empty child process, set error message
       {
        unix_error("sigchld_handler() error : waitpid error occured");
        exit(0);
       }
    }
    Sigprocmask(SIG_SETMASK, &prev_mask, NULL, cldhdrerrmsg);
    //unblock signal
    errno = olderrno;
    return;
}

/*
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.
 */
void sigint_handler(int sig)
{
    int olderrno = errno;
    char *inthdrerrmsg = "at sigint_handler() function";
    sigset_t mask,prev_mask;
    Sigfillset(&mask,inthdrerrmsg);
    Sigprocmask(SIG_BLOCK, &mask, &prev_mask,inthdrerrmsg);
    // block signal

    pid_t pid = fgpid(jobs);
    if(pid!=0)
    {
        Kill(-pid,SIGINT,inthdrerrmsg);
    }

    Sigprocmask(SIG_SETMASK,&prev_mask,NULL,inthdrerrmsg);
    //unblock signal
    errno = olderrno;
    return;
}

/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.
 */
void sigtstp_handler(int sig)
{
    int olderrno = errno;
    char *tstphdrerrmsg = "at sigtstp_handler function()";
    sigset_t mask,prev_mask;
    Sigfillset(&mask,tstphdrerrmsg);
    Sigprocmask(SIG_BLOCK,&mask,&prev_mask,tstphdrerrmsg);
    // block signal

    pid_t pid = fgpid(jobs);

    if(pid!=0)
    {
        Kill(-pid,SIGTSTP,tstphdrerrmsg);
    }
    Sigprocmask(SIG_SETMASK,&prev_mask,NULL,tstphdrerrmsg);

    //unblock signal
    errno = olderrno;
    return;
}

/*********************
 * End signal handlers
 *********************/

/***********************************************
 * Helper routines that manipulate the job list
 **********************************************/

/* clearjob - Clear the entries in a job struct */
void clearjob(struct job_t *job) {
  job->pid = 0;
  job->jid = 0;
  job->state = UNDEF;
  job->cmdline[0] = '\0';
}

/* initjobs - Initialize the job list */
void initjobs(struct job_t *jobs) {
  int i;

  for (i = 0; i < MAXJOBS; i++)
    clearjob(&jobs[i]);
}

/* maxjid - Returns largest allocated job ID */
int maxjid(struct job_t *jobs)
{
  int i, max=0;

  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].jid > max)
      max = jobs[i].jid;
  return max;
}

/* addjob - Add a job to the job list */
int addjob(struct job_t *jobs, pid_t pid, int state, char *cmdline)
{
  int i;

  if (pid < 1)
    return 0;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == 0) {
      jobs[i].pid = pid;
      jobs[i].state = state;
      jobs[i].jid = nextjid++;
      if (nextjid > MAXJOBS)
        nextjid = 1;
      strcpy(jobs[i].cmdline, cmdline);
      if(verbose){
        printf("Added job [%d] %d %s\n", jobs[i].jid, jobs[i].pid, jobs[i].cmdline);
      }
      return 1;
    }
  }
  printf("Tried to create too many jobs\n");
  return 0;
}

/* deletejob - Delete a job whose PID=pid from the job list */
int deletejob(struct job_t *jobs, pid_t pid)
{
  int i;

  if (pid < 1)
    return 0;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid == pid) {
      clearjob(&jobs[i]);
      nextjid = maxjid(jobs)+1;
      return 1;
    }
  }
  return 0;
}

/* fgpid - Return PID of current foreground job, 0 if no such job */
pid_t fgpid(struct job_t *jobs) {
  int i;

  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].state == FG)
      return jobs[i].pid;
  return 0;
}

/* getjobpid  - Find a job (by PID) on the job list */
struct job_t *getjobpid(struct job_t *jobs, pid_t pid) {
  int i;

  if (pid < 1)
    return NULL;
  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].pid == pid)
      return &jobs[i];
  return NULL;
}

/* getjobjid  - Find a job (by JID) on the job list */
struct job_t *getjobjid(struct job_t *jobs, int jid)
{
  int i;

  if (jid < 1)
    return NULL;
  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].jid == jid)
      return &jobs[i];
  return NULL;
}

/* pid2jid - Map process ID to job ID */
int pid2jid(pid_t pid)
{
  int i;

  if (pid < 1)
    return 0;
  for (i = 0; i < MAXJOBS; i++)
    if (jobs[i].pid == pid) {
      return jobs[i].jid;
    }
  return 0;
}

/* listjobs - Print the job list */
void listjobs(struct job_t *jobs)
{
  int i;

  for (i = 0; i < MAXJOBS; i++) {
    if (jobs[i].pid != 0) {
      printf("[%d] (%d) ", jobs[i].jid, jobs[i].pid);
      switch (jobs[i].state) {
        case BG:
          printf("Running ");
          break;
        case FG:
          printf("Foreground ");
          break;
        case ST:
          printf("Stopped ");
          break;
        default:
          printf("listjobs: Internal error: job[%d].state=%d ",
              i, jobs[i].state);
      }
      printf("%s", jobs[i].cmdline);
    }
  }
}
/******************************
 * end job list helper routines
 ******************************/


/***********************
 * Other helper routines
 ***********************/

/*
 * usage - print a help message
 */
void usage(void)
{
  printf("Usage: shell [-hvp]\n");
  printf("   -h   print this message\n");
  printf("   -v   print additional diagnostic information\n");
  printf("   -p   do not emit a command prompt\n");
  exit(1);
}

/*
 * unix_error - unix-style error routine
 */
void unix_error(char *msg)
{
  fprintf(stdout, "%s: %s\n", msg, strerror(errno));
  exit(1);
}

/*
 * app_error - application-style error routine
 */
void app_error(char *msg)
{
  fprintf(stdout, "%s\n", msg);
  exit(1);
}

/*
 * Signal - wrapper for the sigaction function
 */
handler_t *Signal(int signum, handler_t *handler)
{
  struct sigaction action, old_action;

  action.sa_handler = handler;
  sigemptyset(&action.sa_mask); /* block sigs of type being handled */
  action.sa_flags = SA_RESTART; /* restart syscalls if possible */

  if (sigaction(signum, &action, &old_action) < 0)
    unix_error("Signal error");
  return (old_action.sa_handler);
}

/*
 * sigquit_handler - The driver program can gracefully terminate the
 *    child shell by sending it a SIGQUIT signal.
 */
void sigquit_handler(int sig)
{
  printf("Terminating after receipt of SIGQUIT signal\n");
  exit(1);
}



