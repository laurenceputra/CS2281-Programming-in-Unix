# CS2281 Lab 6: Listing Processes and Users (Graded)

We have investigated 3 kinds of C programs (all of which are useful):

  * pure C programs (e.g. Lab 1); 
  * C programs which use Unix but could be modified suitably to work on other non-Unix systems (e.g. U-26-cp-fd.c and to a smaller extent, Lab 3); and 
  * C programs which are designed specifically for Unix (e.g. U-16-mysh1.c and this lab).  C programs in the third class, are usually what one might think of as _system programs_ and may need to be tailored for a particular version of Unix. They make use of Unix system calls and not just the C libraries. Lab 6 focuses on this kind of program. The objective is to show how the ps command works and **how you can write your own**. 

Please note that Lab 6 is less difficult than it seems. Start with Part 1 so
that you know how to deal with /proc. What you may find is that it seems
difficult because you will have to digest various complex man pages. The value
of Lab 6 is that it gives flavour of what a realistic system program entails.
If your Lab is fully working, then you will understand that a program of the
3rd time, a system program, requires various Unix and system details to be
sorted out (in the end may not be so difficult). On the other hand, if the Lab
is partly working, you will still get a better understanding of system
programming. In any case, **you should aim to get Part 1 working**.

This lab gets you to look carefully at some Unix man pages. You will also get
to make use of some of the example code given during lectures. In the lab, you
will also start encountering details of the real system, some of which may be
not so easy to understand, in such cases, experiments in the spirit of the
sample programs can be used to understand what it does and how to use it. **An
important takeaway lesson from CS2281 is to employ simple test programs to
explore something on the system. ** There may be details which are not very
precisely defined in the man pages. One approach is to read the system source
code but that may be a non-trivial exercise. It can be much easier instead to
do some tests on the system which can answer the question.

You might think that finding out information about processes is difficult
involve some "_kernel hacking_". Traditionally, programs like ps were
implemented that way. However, in modern Unix, one can access the information
needed for ps from the /proc filesystem. Take note that the details and
structure of /proc varies between Unix, e.g. Solaris is quite different from
Linux.

We already saw in Lecture 9 that the /proc filesystem in Solaris is a kind of
"virtual filesystem" which gives access to the state of each process. (_Linux
is similar but the details are completely different_). Every process in the
system has a directory in /proc. Look up the process ID (PID) of your shell
(using ps). Look at the files in /proc, it consists of numbered directories
where each directory corresponds to the pid of a process. Compare the files in
ls /proc with the PIDs from ps -e. Inside each directory, there are various
files which either give information about certain aspect of the process (or
can be used to control the process but we will not cover the control aspect).

For this lab, you will only need the file psinfo which contains the
information needed by ps. The relevant system include file which describes the
structures in /proc is /usr/include/procfs.h (to include in your C program: #include <procfs.h>). The details of /proc are described in proc(4) of the man
pages, i.e. man -s 4 proc. Read the _DESCRIPTION_ portion of the man page for
proc(4) and the _DIRECTORY STRUCTURE_ section. The main information you want
is the documentation for the psinfo file and struct. The psinfo file is a file
containing the contents of a psinfo struct, thus, the data in the file is in
binary, e.g. in the L-18-* examples, we see how to read and write a struct
stored in a file.

You may find that the /proc man page contains a lot of information which you
do not fully understand. The way to deal with information overload is to
mainly focus on the relevant information about psinfo. The motivation of the
Lab is to get some experience with writing a real system program. Usually
several (complex) man pages may need to be consulted but not all aspects of
the man page need to be understood at once, only the relevant portion. Again,
when in doubt, write some test programs to understand what is going on. This
is part of the learning process which you should get used to if you are going
to write real Unix programs (now or in the future). Finding and understanding
the system details is also important as there are points of differences
between various flavors of Unix.

If you have difficulty understanding /proc/_pid_/psinfo where _pid_ is an
existing process ID, I suggest you first write your own test program which
reads and prints out some fields in /proc/_pid_/psinfo for a given pid. This
will be more instructive than trying to understand the man page alone. (Note
the _italics_ above, if the PID of a process is 123 then /proc/123. There is
no file /proc/pid).

** General assumptions**:   
Assume for simplicity that there are at most 10000 users in the system. Note
that this does not mean that the numerical value of the UIDs are numbered
consecutively from 1. You can also assume that there are at most 50000
processes in total at any one time on sunfire.

## Part 1: a basic ps (40 marks)

Write a C program lab6-part1.c with the following usage

**Usage:** part1

part1 doesn't take any arguments. Part 1 will print the user name (the name of
the user owning the process, e.g. cs2281), process identifier (PID),
executable name (FNAME) and start time of the process (STIME). This is a
subset of what ps -ef gives and includes the UID, PID, part of CMD and STIME
columns from ps. For example, running ps -ef could give output which looks
something like:

>

>          UID   PID  PPID   C    STIME TTY         TIME CMD

>         root  3563  3563   0   Mar 09 ?           0:00 zsched

>       cs2281  4210  4149   0 23:04:19 pts/134     0:00 -bash

>

The result from ps tells you the following information aboout each process:

  * UID=root, PID=3563, FNAME=zsched 
  * UID=cs2281, PID=4210, FNAME=bash  The FNAME is the name of the executable which is usually the first argument displayed in CMD (however, sometimes the command has a dash in front to show the login shell as illustrated in the example). 

Your program should print the UID, PID, FNAME and STIME information for every
processes. The following example shows the output for two processes:

>

>          UID   PID            FNAME STIME

>         root  3563           zsched Fri Mar  9 04:35:14 2012

>       cs2281  4210             bash Thu Mar 22 23:04:18 2012

>

The printf string format used in the output above is: **"%8s %5d %16s
%.24s"**. The time in the STIME column is in the time format given by
ctime(3C) (note that the STIME is output differently from the ps example). A
header is printed as the first line as shown above and is aligned with the
corresponding columns. Processes may be printed out in any order of the PIDs.

**Hints**  

  * While you can read the contents of a directory, e.g. reading /proc, it will be simpler to simply use the ls program. You can invoke it using the system(3C) library function, e.g. U-2-myscript.c. You can redirect the output of ls into a temporary file and read it from your code. 
  * You will find that it may be useful to have finished Lab 5 although it is possible to also do Lab 6 concurrently with Lab 5 
  * The file /proc/_pid_/psinfo can be read either using the stdio libraries or directly using system calls. I suggest it can be easier to use stdio but if you wish, you could read ahead in the slides and code examples and use system calls. Doing both ways is also instructive (though you should only submit one version). 
  * You will need the header file:   

> #include <procfs.h>

The actual file with the declarations is /usr/include/sys/procfs.h

  * You can get the program name from the field char pr_fname[PRFNSZ] in struct psinfo. Similarly the PID can be found there. The starting time is the pr_start field, its type is the same as as a struct timeval (the details can be found as struct timespec in /usr/include/sys/time_impl.h). You will need the field tv_sec which gives the number of seconds since the Unix Epoch begun (1 Jan 1970 UTC). 
  * You may need to #include various header files, e.g. <sys/types.h> and <time.h>. 
  * You can check your output by comparing with the information from ps. Note that as processes may be starting and terminating, the processes could be different when ps and part1 are run. 

## Part 2: summarizing the user and process information (60 marks + 10 bonus
marks)

Write a C program lab6-part2.c with the following usage

**Usage:** part2 [-p] [-n] [-u] 

Unlike Part 1, **only** processes which are **login shells** will be output.
Thus, zched from the Part 1 example is omitted.

The options refer to:

  * -p: print the PID column 
  * -u: print a unique set of UID. This option disables other options so that the other options are ignored (there is no error if there are other options, just that they are ignored) 
  * -n: (**bonus part**) print the N column which counts the total number of processes with the particular login shell as an ancestor. This option **only** needs to be implemented if you are aiming for the bonus marks.  The order of columns (if it is output) is UID, PID, N, FNAME, STIME where the format of UID, PID, FNAME and STIME is as in Part 1. The N and PID columns should only be printed if the corresponding option is specified on the command line. The format of the N column is "%3d". The -u option only outputs the UID column without any others. 

For example, if the command line is part2 -np, the output could look like:
(note that options can be combined in any order, so equivalent commands are:
part2 -p -n or part2 -pn)

>

>          UID   PID   N           FNAME STIME

>       cs2281  4210   2           bash Thu Mar 22 23:04:18 2012

>

In this example, cs2281 is using the bash shell and it happens to have one
child process, e.g. vim.

In order, to determine the login shell, the environment variable MYSHELLS
gives a list of shells separated by a colon, e.g. MYSHELLS=bash:sh will
recognize bash and sh as valid shells. If MYSHELLS is not defined, the default
is just tcsh:bash.

The -u option disables other options. If -u is given, a unique list of users
will be printed as the UID column (with header) and no other columns. This
means that a user logged in multiple times will only be printed once.

**Hints**  

  * A user can have multiple login shells. You can assume that a process is the login shell is if the first character of the command line is a dash and the FNAME is one of the approved shells from MYSHELLS (or the default). The command line can be found in the ps_psargs field in psinfo. 
  * Obviously, one way of testing is to login multiple times and start various numbers of processes. 
  * Any process whose ancestor is a login shell counts towards the number of processes with the -n option. Thus, the shell is not necessarily the immediate parent process. 
  * In bash, the following example shows how to create an environment variable which will be exported to any child process: 
    
    
    $ MYSHELLS=bash:sh:tcsh
    $ export MYSHELLS
    

This creates a shell variable MYSHELLS and the export command which is
internal to bash marks that variable as being exported as an environment
variable to any child process of bash.

  * You may find it useful to create your own arrays of appropriate C struct to store information about the processes, users, etc. 

## Grading + Submission Summary

**C files submitted must compile on Sunfire to be graded. You should add in a comment block to each file at the start**: Name, Matric, Email. 

Submit the following in a single zip file with the filename _yourmatricno-
_lab6.zip, e.g. if your matric number is U123X, then U123X-lab6.zip. Note the
user of Uppercase in the matric number. (Make sure your your zip file is in
correct ZIP format, i.e. test with unzip on Sunfire, and you should not submit
individual C files).

  * Part 1: _yourmatricno_-lab6-part1.c, 40 marks 
  * Part 2: _yourmatricno_-lab6-part2.c, 60 + 10 bonus marks 

