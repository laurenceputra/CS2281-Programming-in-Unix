# CS2281 Lab 5: Simple Shells and Software Testing (Graded)

### Motivation

In this assignment, you will learn how to create processes and make them run
particular executables. In particular, using the fork() and exec family of
system calls.

You will also see that both parts of the lab give you a certain kind of
specialized shell. Obviously its not bash but the main functions which are to
do with handling command lines and running processes are related.

## Preliminaries

For this lab, a _command line_ is a series of words on a line terminated by a
newline character. A word is any sequence of non-whitespace characters and
words can be separated by any number of whitespace characters. A whitespace
character is either a blank or a tab.

This definition of a command line is quite similar to what a shell like bash
uses. For example,

    
    
      ls      -al /tmp   .
    

There are 4 words, namely: "ls", "-al", "/tmp" and ".". Typically in a shell,
the first word of the command line, e.g. "ls", gives the name (pathname) of
the executable to run. Some side remarks, in bash, some command lines are not
commands to execute but rather special internal commands to bash, e.g. cd.
Also, not every command line starts with the name of a command to execute,
e.g. USER="Batman". (If you did not understand the remark, it is for when we
cover the bash shell).

You should also be familiar with how the usage syntax of programs are
typically described, i.e. the command line syntax for using the program. The
following example slightly modified from [Lab 4](lab.html) is used:

> fold [-bt] [-w _width_] [_file_...]

The minus denotes arguments which are options to the program. The square
brackets means that the argument is optional, i.e. may be given or not. If the
argument option is a single letter than the possible choices are given
together, so -bt covers the possibilities:

>

>     fold

>     fold -b

>     fold -t

>     fold -bt

>     fold -tb

>     fold -t -b

>     fold -b -t

>

Notice that the order is not significant - the option arguments can be given
in any order. Another example is:

> fold -w 10 -b test

The last argument with ... means that there can be many more arguments, e.g.
fold -w 10 -b file1 file2. There is an implicit ordering between the arguments
which are options to the program from those which are not. The arguments which
are options come first, thus, the following is not allowed:

> fold file2 file1 -w 10 -b

If you have difficulties with understanding how to processing arguments to the
program, please try [Lab 4](lab4.html). You will find that atoi(3C) to be
useful to convert strings to numbers. There are also many examples of using
arguments in the sample code. (_A general point about the sample code is that
to shorten the example, sometimes error handling is not done. Your code should
deal with potential errors. In general, there may be test cases to check if
you are dealing properly with errors_).

## Part 1: A Simple Logging Shell (40 points)

This part is to learn how to directly use the system calls for process
creation and executing binaries. It also shows you how to create a minimal
shell - it has almost no features except for running command lines.

**Task Overview:**   
Write a program which reads lines either from standard input or a specified
file _scriptfile_ and runs the command lines in the input. It should do this
by creating a new process to run the program specified in the first word of
the command line and supplying the remainder of the line as arguments to the
program.

The usage syntax is:

> lab5-part1 [-p _searchpath_] [-a] [-l _logfile_] [_scriptfile_]

The first word on the command line is either:

  1. An _absolute pathname_, i.e. begins with "/", for example, /usr/bin/ls; 
  2. or, a _relative pathname_ beginning with "./" or "../", for example, ./a.out; 
  3. or, the name of a program, e.g. ls.  Each command line should be run in a separate process. Without -p, the executable should be either the given absolute or relative pathname. With -p, it is as described in arguments (below), As the input may have many lines, each command line is to executed one at a time in sequence. So the next command line is only executed after the previous command finishes. (Bash is also similar). A blank line is ignored and so is any line beginning with '#' which is treated as a comment. 

A log of all command lines executed is written out together with the exit
status. By default, the log is written to lab5-part1.log. The format of each
line in the log corresponding to an executed command line is:

> _exitstatus_: _command line_

For example, if the lab5-part1 is run as, lab5-part1 test.script, and
test.script contains:

    
    
    # example 1
    
    
    /usr/bin/echo a test
    
    true
    false
    

The file lab5-part1.log contain the following which are the command lines
executed and their exit status:

    
    
    0: /usr/bin/echo a test
    0: true
    1: false
    

The output of lab5-part1 is simply whatever is the output of the commands
executed. For the above example, the output is simply the output from echo
since the other two commands have no output:

    
    
    a test
    

The meaning of the options given to your program are the following:

  * -a: stop if the process executed from the command line gives an error exit status, i.e. the exit status is non-zero. If -a is not given, then the processing continues even if there is an error exit status. 
  * -l _logfile_: instead of the default filename for the log, the log is written to _logfile_. 
  * -p _searchpath_: If the first word of the command is an absolute or relative pathname, then it is tried first. If it fails to execute, i.e. the exec system fails, then the program name is tried against the directories in _searchpath_ in left to right order of the _searchpath_. The format of _searchpath_ is the same as the PATH environment variable. Note that PATH is not used in Part 1.  The exit status of lab5-part1 is the exit status of the last executed command or 0 if there are no commands. 

**Requirements and Assumptions**  
A special requirement (_given the pedagogical purpose of the lab_) is that you
**must** execute the command using one of the exec family of system calls.
While it is possible to use your line processing code from Lab 3, to simplify
this lab (furthermore, your Lab 3 code may have bugs), you can assume that the
maximum length of a line is 255 characters not counting the newline.

**Hints**  

  * An example of a simple shell is in the sample programs as U-16-mysh1.c. While the task is different, it shows you all the basic elements of a simple shell and those also apply here. 
  * In order to meet the sequential command line execution requirement, you need to wait until the process running the command line is finished which can be done using the wait family of system calls. 

## Part 2: A Software Testing Utility (60 points)

In Part 1, you developed a simple shell. Part 2 extends this idea and you also
learn the system(3C) library function.

A shell program, i.e. a shell script using bash, might be used for testing a
program. For example, you may have test cases which are automatically run and
checked to see if the output is correct by using a script to run the test
program on its test case inputs and also compare the output for correctness.
Often it will be useful to use I/O redirection of the shell to make it more
convenient to use different inputs and save the outputs. However, as we have
not covered I/O redirection yet, the system(3C) function gives a handy way of
using the features of the bash shell to run programs.

The syntax of lab5-part2 is:

> lab5-part2 [-c _testconfig_] [_prog_ ...]

lab5-part2 is controlled by a specified configuration file which we call
_testconfig_ and is either on the command line or from stdin. Testconfig gives
the tests to be run together with the inputs and expected outputs. The
remainder of the command line gives the executables of the programs to be
tested, e.g. _prog_.

> ** Programs to be tested expect their input from stdin and their output
which is checked for correctness by lab5-part2 goes to stdout. **

A summary of the test results are output by lab5-part2 to stdout (see below).
You can also think of lab5-part2 as being an auto-grading program for cs2281
submissions!

The format of the testconfig gives the total number of tests followed by a
sequence of tests to be done. The syntax of a _single test_ is given by the
following self-explanatory example

    
    
    # this is the base test
    test 1
    input <EOF
    line1
    line2
    EOF
    output <EOF
    output1
    output2
    output3
    EOF
    

A line starting with hash is a comment which is ignored. The "test" _i_ line
indicates the _i_-th test and the input for the test is the two lines, "line1"
and "line2", with expected output being "output1", "output2" and "output3".
The output from the test program matches the expected output only if it is
exactly the same. The "<EOF" and "EOF" lines are used as way to denote start
and end of input or output as follows:

  1. A line with "input <_pattern_" for an input test or "output <_pattern_" for an output test. The _pattern_ is just a word. 
  2. Some number of lines which are the actual input or output for the test. 
  3. The end of input or output is defined by a line containing just _pattern_.  Thus, in the example EOF can be any word which does not occur in the input (respectively, the output). For example, if the word "EOF" can occur in the output but not the word "THEEND", then one can use: 
    
    
    # this is the base test
    test 1
    # test 1 input - does not contain THEEND
    input <THEEND
    line1
    EOF
    line2
    THEEND
    # test 1 expected output - does not contain EOF
    output <EOF
    output1
    THEEND
    output3
    EOF
    

This syntax is purposely designed for the flexibility to easily delimit the
input/output lines. It is also similar to what is in bash (although bash uses
<< while we use a single <). The testconfig input then consists of a sequence
of these tests. The test number is simply for legibility and can be ignored so
there is no need to check whether the tests are numbered properly.

Suppose we want to test the wc program using the above test input. The
expected output will give the following test case:

    
    
    # wc testconfig
    # test input - 3 lines, 3 words, 16 chars
    input <THEEND
    line1
    EOF
    line2
    THEEND
    # expected output from wc
    output <EOF
           3       3      16
    EOF
    

So in this example, this test uses the following fixed input to wc:

    
    
    line1
    EOF
    line2
    

and it expects that wc should give the output:

    
    
           3       3      16
    

So far, we have only described a single test case. The testconfig file can
have multiple test cases and multiple programs can be tested. Let's us expand
the single test case by adding one more case for wc into a full testconfig
file:

    
    
    # wc testconfig
    total 2
    # test 1 input - 3 lines, 3 words, 16 chars
    test 1
    input <THEEND
    line1
    EOF
    line2
    THEEND
    # expected output from wc
    output <EOF
           3       3      16
    EOF
    # test 2 input - 1 line, 2 words, 5 chars
    test 2
    input <EOF
    1 23
    EOF
    output <EOF
           1       2       5
    EOF
    

The line total 2 means there a total of two tests. Suppose we run as:

> $ lab5-part2 wc <testconfig

The output of lab5-part2 will be a summary of which tests are passed as
follows:

    
    
    wc: 1 1
    

The following example shows using multiple test programs. Suppose the command
line is:

> $ lab5-part2 -c testconfig ./prog1 ./prog2

The command line specifies that the executables to be tested are ./prog1 and
./prog2 using the tests defined in testconfig. You can assume that the
executables can be found in the PATH environment variable (unlike Part 1).
Suppose testconfig contains:

    
    
    total 2
    # this is the base test
    test 1
    input <EOF
    line1
    EOF
    output <EOF
    LINE1
    EOF
    # another test
    test 2
    input <EOF
    EOF
    output <EOF
    EOF
    

For test 1, we expect the output to be "LINE1" given input "line1", while for
test 2, we expect empty output given empty input. Suppose what happens is the
following:

  1. prog1 is run with the test 1 input and its output is checked against the expected output. Suppose the output is exactly the same as the expected output. 
  2. prog1 is run with the test 2 input and its output is checked against the expected output. Suppose the output differs from the expected output. 
  3. prog2 is run with the test 1 input and its output differs. 
  4. prog2 is run with the test 2 input and its output is as expected.  The output of lab5-part2 which is to stdout is a summary of the test results for each program. For the above example, the output is: 
    
    
    ./prog1: 1 0
    ./prog2: 0 1
    

This is because prog1 passed test 1 and failed on test 2 while prog2 was the
opposite. Notice that the filenames in the summary are the programs given to
lab5-part2 in the command line.

**Assumptions**  
You can assume that the testconfig input is syntactically correct following
the syntax in the examples above. The same line length assumption of Part 1
applies.

**Hints**  

  * You should use system(3C) (please see section 3C of the man). Actually you do not need to use system() but it will be much easier to do so, hence, the recommendation. 
  * Take note that input and output of lab5-part2 is different from the input and output of the program being tested, e.g. prog1. 
  * You may need temporary files, tmpfile(3C) can be used. 
  * Although the specification seems complex, you will find that it not as complex as you may think after some thought on the design of part 2. 

## Part 3: Timeout for Part 2 (Optional part, Bonus: 8 Marks)

This should only be attempted if Part 2 is working. Full marks will only be
given if Part 2 meets all our tests.

Part 3 extends Part 2 with a timeout feature. It adds a new option: -t timer.
The usage syntax is now:

> lab5-part3 [-t _timer_] [-c _testconfig_] _prog_ ...

The unit of _timer_ is in seconds.

The timeout option will let a process run for up to a time limit of _timer_
seconds. If the time limit is exceeded, the process is terminated and the
result of the test is recorded as -1. Otherwise, it is the same as in Part 2.

There are many ways to do this task and this optional part is really for
students who want to experiment further with Unix on their own, thus, there
are no hints given. Also the bonus is smaller for the same reason (_ideally
you should be exploring on your own anyway without any tie to grading_).

## Grading + Submission Summary

Submit the following in a single zip file with the filename _yourmatricno-
_lab5.zip, e.g. if your matric number is U123X, then U123X-lab5.zip. Note the
user of Uppercase in the matric number. **You should add in a comment block to
each file at the start**: Name, Matric, Email.

  * Part 1: _yourmatricno_-lab5-part1.c, 40 marks 
  * Part 2: _yourmatricno_-lab5-part2.c, 60 marks 
  * Part 3: _yourmatricno_-lab5-part3.c, bonus 8 marks 

