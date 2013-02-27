# CS2281 Lab 3: Processing Text and Binary Files (Graded)

## Motivation

The main part (non-optional) part of the lab is to gain
practice with processing text files. It also shows you the
importance of making programs robust to the input - programs
should be able to deal with long lines. Furthermore,
a very common problem these days is that many programs are
susceptible to buffer overflow which may mean that they can
be exploited by malware.
A hypothetical example is to corrupt some data being read by a web server
using large input which the programmer might
have not have considered as a possibility.

Note that C programs do not need to be formatted in multiple lines.
In principle, the whole program can just be all on a single line.
Of course, this is not very legible, but it may be the output of some
kind of preprocessor (e.g. Lab 1). As such, the C compiler might
be expected to deal with long lines as well. I did not test this but
you may want to try it out on gcc.

After completing the lab, you should find that while palindrome checking 
is just a puzzle, portions of your program which deal with managing
large files will be useful for writing robust programs.
The following programs which process text files in
a line-oriented fashion were tested on Solaris:   
comm, cut, diff, fold gawk, grep, sed, sort, tail, uniq
  
They were tested to work with lines as large as 14M characters 
(even longer lines were not tried).
One program failed with long lines: awk.
It failed on lines as short as 1000 characters :-(
However, there is no need to use awk since
gawk (the GNU replacement
for awk) passes the test. 

## Preliminaries

This lab is also designed to expose students to programming with pointers.
It will prepare you for writing _real C programs_ and debugging
them in the future (both in this course and also when
you need to write real world code).
Students often debug using diagnostic output, e.g. printf style debugging.
You will find that this may be problematic with this lab due to large
files with many lines and long lines will lead to megabytes or even
gigabytes of diagnostics. 
Rather, I suggest that students learn how to use 
gdb as described in [Lab 0][0] and 
[Lab 2][1]. 
See also the links in the main page for gdb tutorials.

Please note that there is no simple route to writing C programs well.
Some people may find programming in C to be easy but
others may find it to be extremely difficult and frustrating :-(

In this lab, you will only be reading from standard input (stdin)
and writing to standard output (stdout).
Do not read and write other files.
Please make sure that you **do not have any debugging output**
in your submission - the program should only output what is specified below
and nothing else.

Please note that all files used in examples and labs can be found in
the directory ~cs2281/public\_html/Labs (samples in
~cs2281/public\_html/C).
Data files are not hyper-linked below but can be found in 
~cs2281/public\_html/Labs.
The **exact path name** is needed, e.g. 
~cs2281/public\_html/Labs/lab3-1k.dat

## Part 1: A Basic Palindrome Checker (20 points)

A palindrome is a string which is the same when reversed.
Some examples will make this clear: (the last palindrome is in Dutch
and Revilo is a real [person][2])

    aha
    abba
    step on no pets
    able was i ere i saw elba
    ablewasiereisawelba
    desserts I stressed
    amanaplanacanalpanama
    revilo p oliver
    Otto saw pup pup was ottO
    potstalmelkkoortspilstaalplaatslipstrookklemlatstop
    

Clearly, any string with repeated letters is a palindrome.
Similarly any string with one letter is a palindrome.
A blank line could be considered a palindrome **but for the purposes
of this lab**
blank lines are **not** palindromes.

An example of a line which is not a palindrome is
abab.
Sometimes, punctuation and letter case
are ignored for palindromes but to simplify the lab, 
we will only consider palindromes which match exactly using every character in the line.
Thus, the following lines are **not palindromes**:
(compare them with the punctuation removed versions above)

    A man, a plan, a canal, Panama!
    Revilo P. Oliver
    Otto saw pup ! Pup was Otto
    

**Task Overview:**   
Write a program which reads lines from standard input 
and prints to standard output all lines which are palindromes.
Your C program should be called _yourmatricno_-lab3-part1.c.

**Input Specification:**   
The input is a sequence of lines from standard input (stdin).
A line may be empty (just a newline by itself) or have
any number of non-newline characters
followed by a newline character ('\\n')).

**Output Specification:**   
Output the lines which are palindromes to standard output (stdout)
in the order they appear in the input.
The definition of palindrome is as above - a blank line is not a palindrome;
and punctuation, uppercase and lowercase letters are significant,
e.g. AbBa is not a palindrome.

For example, given the following input,

    a line
    aabbaa
    ..
    xyz
    AbBa
    aha
    

your program will output:

    aabbaa
    ..
    aha
    

**Assumptions:**   
You can assume in Part 1 that the maximum length of a line in the input,
not including the newline, is 60 characters.
You are allowed to use a fixed sized buffer, i.e. char line\[60+1\],
in your program to store a line.

**Notes:**   
All students should be able to successfully complete Part 1\.
The intention of Part 1 is for you to understand what is a palindrome
and how to recognize a palindrome.   
Hint: see the sample programs in the C Library section of the notes (clib).

If you already know how to do Part 2, it can also be used for Part 1\.
Note however that in Part 2
you are not allowed to use a fixed buffer.

Observe that if every line in the input is a palindrome, then, the result
is to copy the input to the output, e.g. behaves like the cat program.

## Part 2: Dealing with Arbitrary Line Lengths (80 points)

Write a program, _yourmatricno_-lab3-part2.c, which outputs the lines which
are palindromes as in Part 1 but taking into account the following
requirements: 

*   You **cannot** use a large buffer to store the
    line as there is no assumption of a pre-determined maximum line length.
*   The memory usage of your process should scale
    according to the length of the longest line encountered in the input.
    It should **not scale** according to the size of the input,
    the exception being when the input only consists of a single line.
*   The first step in your program is to
    call my\_memusage("INIT") at the start before anything else. 
    The prototype for my\_memusage() is: 
    
        void my_memusage(char *); 
        
    
    Before your program terminates, the last step is to call
    my\_memusage("END").
*   No temporary files are allowed. The only I/O allowed is reading from
    standard input and writing to standard output.
    

These constraints mean that process memory size is dependent on the
length of the longest line.
The skeleton of your main() function should be:

    // other declarations and function above
    
    void my_memusage(char *); // prototype for my_memusage
    
    int main()
    {
    	// various declarations
    	my_memusage("INIT");
    	
    	// your own code
    
    	my_memusage("END");
    	return 0;
    }
    

my\_memusage() has already been pre-compiled into the
following object file, lab3-memusage.o, which you should copy
to your directory from ~cs2281/public\_html/Labs/lab3-memusage.o.
Your program is compiled by linking in lab3-memusage.o.
If you have not yet tried out separate compilation and linking,
please see [lab0 preliminaries][0] and 
[lab 2][1]. 
For example,

    $ gcc lab3-part2.c lab3-memusage.o
    

You will find that my\_memusage() prints out some diagnostics
about time and memory usage to stderr.
It gives the _User Time_ (utime) which is the amount of time Unix ran the program
code; and the _System Time_ (stime) which is the amount of time spent inside the Unix
kernel (e.g. doing I/O).
The "_Delta_" is the
difference in memory usage between the start and end of your program
(the difference between "INIT" and "END" calls to my\_memusage().

You can also use the time command to do the timing (see time(1)). 

    $ time a.out
    

Try it and compare the times. Note that _Real Time_ is wall clock time which is
different from User Time - recall that Solaris is a multi-tasking and multi-user operating
system, so it is concurrently running many processes, and all of those take time.
The User Time is how much time was logically spent on running the program.

You **must** call my\_memusage() as shown above, otherwise,
your program will incur a penalty.
Please note that a pointer going _haywire_ may in the worst
case affect/corrupt the operation of my\_memusage()
(this is due to C not having the overhead of runtime safety).
Do run gcc -Wall and lint
to check your program for potential problems.

Your program should not take be too slow if there are long lines.
For example, an algorithm which takes quadratic complexity on the line
length may be too slow. Programs which take too long to execute may
incur a grading penalty as it is not feasible to wait more than
the expected amount of time.

Please ensure that the maximum process size does not exceed 16MB.
The maximum line length in test files for grading will not exceed
100000 characters per line.
Furthermore, when the input has short lines,
the memory usage should be small compared to when the input has long lines.
Another way of thinking about it is that the _Delta_ from my\_memusage() should
scale with the maximum line length.
It should also be small (taking into account any other memory overheads, e.g. from stdio)
when the input has short lines.

Grading will take into account the **memory usage** as well as the 
**running time** with 40 marks for correctness and 40 marks for performance
(memory + time).
Because the grading of Part 2 is based on performance, the final mark
will be **competive** and will, in part, depend on how well the submitted 
programs from the class perform.

### Notes and Hints

*   You will need to use I/O redirection provided by the bash
    shell to test your program.
    Please see the [Starting Unix][3] slides. 
    Some examples using bash:
    
        $ a.out <input.dat # standard input taken from file input.dat
        $ a.out <input.dat >output.dat # standard output also goes to file output.dat
        $ a.out 2>debug.dat # only stderr output goes to debug.dat
        $ a.out <input.dat >output.dat 2>debug.dat # stdin, stdout, stderr redirected
        
    
*   To allocate dynamic data structures, see malloc(3C). 
      
    Note the notation malloc(3C) is not a function call but a reference
    to the Unix man pages. 
    Thus, 3C refers
    to section 3C of the man, so man -s 3C malloc)
*   A dynamic data structure such as a linked list could be used to store
    a line as it is being read. Take note that a linked list such as
    
        struct letter {
        	char c; 
        	struct letter *next; 
        } 
        
    
    takes 8 bytes on sunfire (check sizeof(struct letter)).
    The memory overhead of one object of struct letter
    will be 8 bytes per letter! There are a number of more efficient and suitable 
    dynamic data structures.
*   In order to check your memory usage, see the diagnostics
    printed to stderr from my\_memusage().
    Take care to make sure that the submitted
    code has no diagnostic output to stdout.
*   The following sample input files have palindrome lines which are 
    _at least_ 32, 1K and 32K characters long respectively:   
    lab3-32.dat, lab3-1k.dat, lab3-32k.dat   
    You may need
    to write a program to make your own test data with long line lengths.
    Students are allowed to discuss and share code/programs for creating
    large files for testing. However, the actual program for the lab should
    be your own **individual work**.
    
    The sample files do not contain any interesting palindromes as they
    are only meant for testing how well your code deals with lines of various
    lengths.
    To do palindrome testing, you will need to create your own datasets.
    

## Part 3: Binary Forensics (Optional part, Bonus: 15 Marks) 

### Motivation

The rationale for this task is to understand how to process binary files,
play with hexadecimal formats, understand number formats and dealing with exact representations.

Students who do not have much of a background in computer architecture or are not interested
in "_hacking_" may want to skip this task.

### The Bonus Task

In this bonus task, you will be challenged with a digital forensics
problem. Here is the _imagined_ scenario:

> Last night, a secret agent successfully hacked into the network of a rogue bank
> and stole a file. He believes that this file contains a history of
> transactions from various account numbers. Unfortunately, this file is
> in binary format and he is not good with this kind of decoding problem.  
>   
> Your mission as the computer expert in [IMF][4] (_if you choose to accept it_) 
> is to decode the file into a readable and meaningful ASCII form.
> 

Your task is:

*   Write a program which reads a binary file and outputs the
    content to standard output (stdout) in ASCII format.
*   You can present the output in any style, but preferably it should
    be as similar to the contents and layout of binary file as possible.
*   Write some short comments in your source code describing the
    content/format of the binary file.
    

The file name will be given as argument to your program. For example,
let the binary file be "sample.bin" and your program is
"lab3-part3.c", then your program will be run using the
following commands:

    $ gcc lab3-part3.c -o lab3-part3
    $ lab3-part3 sample.bin
    

The binary file is not given to you, but you may use this sample file, 
lab3-sample.bin (640 bytes),
to figure out the format and prepare your program.
The original binary file will have a same format with the sample, but contain more data.

Here are some hints about the data:

*   Zhang Hao bank account's number is 529110386\.
*   On 1 Feb 2012, Ma Jungqi withdrew (debet) $1,000 from his account via ATM.
*   On 31 Jan 2013, Selena Koh deposited (credit) $5,500 cash into her account.
*   Julian Topor is a very rich man, he had 10 billion dollars in his account on 31 December 2012\.
    He is known to have deposited another 5 billion to his account on 14 Feb 2012\.
    

### Notes and Hints

*   This binary file was generated on an unknown machine (probably not sunfire).
*   It is known that the binary file contains **fixed length records**, i.e. it is a file of
    C struct.
*   Your program will be tested on sunfire.
*   Hexadecimal being an even number of bits (4 bits) is more useful than Octal (3 bits).
*   It will be useful to look at the binary file in hexadecimal as well as ASCII.
    Some programs you can use on sunfire are: od(1) and xxd(1).
    vim users may find this [link][5] to be useful.
    
    Note that you need not examine the file on sunfire. However your program must run on sunfire.
*   The material we covered in _c-part1_ and _c-part2_ notes will be useful. 
    Also lookahead (if we have not yet covered, when you start) to _clib_.
    This includes the sample code as well as the notes.
    

## Grading + Submission Summary

Submit the following in a single zip file with the filename _yourmatricno_**-lab3.zip**, e.g. if your matric number is U123X, then U123X-lab3.zip. Note the user of Uppercase in the matric number.
**You should add in a comment block to each file at the start**: 
Name, Matric, Email.

*   Part 1: _yourmatricno_-lab3-part1.c, 20 marks
*   Part 2: _yourmatricno_-lab3-part2.c, 40 (correctness) + 40 (performance) marks
*   Part 3: _yourmatricno_-lab3-part3.c, 15 bonus marks 
    



[0]: lab0.pdf
[1]: lab2.html
[2]: http://en.wikipedia.org/wiki/Revilo_P._Oliver
[3]: ../Notes/starting-unix.pdf
[4]: http://en.wikipedia.org/wiki/Impossible_Missions_Force
[5]: http://stackoverflow.com/questions/839227/how-to-edit-binary-file-on-the-unix-systems