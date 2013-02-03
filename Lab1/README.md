# CS2281 Lab 1: Code Generator (Graded)

In this assignment, you will write C programs which produce another C
program as the output. The example below illustrates what it means by 
producing another C program. As both your program and the output are 
legal C programs, 
we will call the program you write as "_yourprog_" and 
the output of your program as "_outputcode_".

For example, consider the task of writing a program which reads an 
integer N and produces another C program which will write a line 
containing the '\*' character for N times.

A possible solution is:

    #include <stdio.h>
    int main() {
      int N, i;
      scanf( "%d", &N );
      printf( "#include <stdio.h>\n" );
      printf( "int main() {\n" );
      for ( i = 0; i < N; i++ ) printf( "  printf(\"*\");\n" );
      printf( "  printf(\"\\n\");\n" );
      printf( "  return 0;\n" );
      printf( "}\n" );
      return 0;
    }
    

Given the following input:

    4

A possible output, the _outputcode_ is:

    #include <stdio.h>
    int main() {
      printf( "*" );
      printf( "*" );
      printf( "*" );
      printf( "*" );
      printf( "\n" );
      return 0;
    }

Compiling and running outputcode gives:

    ****

Any generated C code is accepted as long as it compiles without errors.
In addition, it **must** follow constraints given in the rules below. 
Different strategies or algorithms can be used, for example,
another possible outputcode is:

    #include <stdio.h>
    int main() {
      printf( "****\n" );
      return 0;
    }

Note that the outputcode shown above is quite simple, it has no input and has only performs output using printf(). 
In other parts of this lab, the outputcode may need input (e.g., using scanf).

You should end main() with
return 0 - we will explain why in later lectures, for now,
just follow this structure.
You can also read ahead in the notes, [C library (clib)][0], for
more on the input and output libraries.

The generated C code, _outputcode_, must obey the following rules.

*   The generated C code should be legal C.
*   It must be compilable by the gcc C compiler without any errors.
    Warnings messages from gcc are considered allowable.
*   The C code in _outputcode_ is
    **NOT** allowed
    to have any repetition constructs (for, while, do while, etc) and
    goto statement(s). Recursion is not allowed but function calls without
    recursion are legal.
*   The only functions not in your code which can be used are the following
    library functions:
    printf(), puts(), putchar() and scanf().
    (Note that you do not need to use puts() or putchar()
    and they are allowed just in case students want to also use those).
    

Essentially the rules mean that your program consists of straightline code
which can also have if statements and the only library functions
which can be used are for output, e.g. printf(), and for input,
e.g. scanf(). Thus, qsort() is not allowed.

You can think of _your program_ as a special kind of compiler which
generates a kind of optimized code, namely, the _outputcode_.
The _outputcode_ is a C program tailored for a specific problem instance,
e.g. sum 10 numbers rather than sum _n_ numbers.

### How to Test Your Generated C Code
You can test your program by redirecting the output to a C file, and then compile and run it.

    $ gcc code.c               # Assumes your program is code.c and generates a.out
    $ ./a.out > output.c       # Any output from a.out (your program compiled into an executable) is redirected to output.c
                               # You may be able to use a.out as well depending on your environment PATH variable
    4                          # This is input for a.out program (read from standard input)
                               # output.c is generated containing your output C code
    $ gcc output.c -o output   # Compile output.c
    $ ./output                 # Run it.
    ****                       # This is the output for the output C code.
    

Similarly, you can redirect input from a data file:

    $ ./output < test1.dat > result.txt
    

By default, redirecting output in the bash shell will not 
overwrite an existing file, giving an error.
To redirect and overwrite an existing file, use:

    $ ./output < test1.dat >| old-result.txt
    

To try out what we discussed on the Wed 30/1 lecture, here are some exercises.
Compile [lab1-redirect.c][1] and I will assume
it is called a.out. For the moment, we have not covered the C library
yet but just assume that the line which says stdout has the output
going to the _standard output_ and similarly the line
with stderr has its output going to
_standard error_. 

    $ a.out
    $ a.out 1
    $ a.out 1 > file1
    $ a.out 1 2> file2
    $ a.out 1 &> file3
    

The reason for the above syntax will be clearer after we do Unix file I/O.
(_Hint: it is to do with file descriptors_).
Compare the different files. If you do not want bash to complain
about overwriting files. You can turn off the error with:

    set +o noclobber
    

This is a bash shell option - the noclobber refers to "not clobbering the file".

### Motivation

Students may find this lab to be somewhat unusual as you may not be familiar
with the idea of a program which itself generates another program.
However, all compilers perform some form of this idea.
The first C++ compiler also worked in a similar way - it
compiled from C++ source code to C source code.

From the perspective of CS2281, the purpose of Lab 1 is as follows:

*   Become familiar with compiling C
*   Become familiar with C syntax by generating C code (the outputcode)
*   Appreciate the function of the C compiler (or any other compiler)
    

Note that since you will be generating C code, you may find that more
work is needed to understand the output which is itself a C program.
To simplify things, you may want to output the code so that it is
properly formatted, i.e. rather than having it all on a single line.
Be careful that testing and debugging may be needed on both programs:
_yourprog_
and also _outputcode_.

## Part 1: Drawing a Triangle (20 points)

We start with a basic task.
Write a program which reads an integer, N, from standard input (stdin) 
and output a C program (output C code) to standard output (stdout) which
will draw a 
right angled triangle filled in by '\*' characters
whose two short sides have size N.
**Input Specification**  

The input is a line with an integer N.
**Output Specification**  

The output is a C program which draws a solid triangle using '\*'
whose vertical and horizontal side has N characters (see the example below).
**Constraints:**  

Your program should be able to handle N in range: 1 ≤ N ≤ 10\.
**Example Sample Input:**  

    3

**Example Output for Sample Input:**  

    #include <stdio.h>
    int main() {
      printf( "*\n" ); // puts( "*" ); /* also valid */
      printf( "**\n" );
      printf( "***\n" );
      return 0;
    }
    

Notice the shape of the triangle.

**Hints**  

*   Formatted output in C is usually through the printf() function.
    While we will cover the details later, the lecture notes examples as well
    as the ones here should already serve to illustrate simple usage of
    printf(). However, printf() can produce more complex
    formatted output.
*   If you know Java, printf() is almost the same as
    System.out.printf().
*   For Part 1, you can also use puts() as an alternative
    to printf() (see the man page, puts(3C) )
*   Note the use of backslash within the printf() format string
*   By default, the output of printf() goes to standard output (stdout)
    

## Part 2: Number Series (80 points)

First some background on number series.
An [arithmetic progression][2]
is a series of numbers where
the difference between consecutive terms in the series is a constant, e.g.

    1, 3, 5, 7, 9, ...
    

A [geometric progression][3]
is a series of numbers where
the ratio between consecutive terms in the series is a non-zero constant, e.g.

    3, 6, 12, 24, 48, ...
    

Write a program which will read an integer N from standard input (stdin) 
and generate a C program _outputcode_ to standard output (stdout) 
which will read N integers.
The N numbers correspond to either an arithmetic or geometric progression. 
Your outputcode has to determine which one and then print the next two terms 
in the series which is consistent with the input series.

**Input Specification**  

The input is a line with integer N.
**Output Specification**  

The output is C code which will read N integers from standard input and 
output the next **two** consecutive terms in the series which may be either arithmetic
or geometric. The choice of which output depends on whether numbers input
were part of an arithmetic or geometric progression.

**Constraints:**  

*   Your program should be able to handle input N in range: 3 ≤ N ≤ 10\.
*   Your _output C code_ should be able to handle signed integers in
    the input: -32768..32767
    

**Example Sample Input to** _yourprogr_:  

    4

**Example Sample Input to** _outputcode_:  

    1 3 5 7

**Example Output from running:** _outputcode_  

    9 11

Note that the output of _original program_ is not shown.
This is because it will also reveal the algorithm used in _your program_.

**Hints and Notes**  

*   You can assume the series is either a proper arithmetic or a proper geometric series.
*   A sample program which does input and output: 
    [lab1-IOeg.c][4]. 
    Notice the use of '&' which is
    is used to pass the address of the variable
    to scanf() (see lecture c-part2).
    If we have not finished covering the address operator, just use the example
    as a template, which should be straightforward even without understanding
    all the details. Be careful that omitting the ampersand is not only buggy
    but can lead to runtime exceptions, e.g. segmentation fault.
*   As mentioned before, the output C code must comply with the rules.
*   You may use any variable name in the output C code.
*   In order to output '%' in the printf() format
    string, it needs to be escaped, e.g. printf( "%% %%" )
    outputs "% %"
*   Arrays are not necessary but can be used. 
    Note that no loops are allowed in the _outputcode_ C program.
    

## Part 3: Finding the Median (Optional part, Bonus: 15 Marks)

This program is optional and it should only be submitted once you have Part 1
and 2 working. Note that part 1 and 2 already add up to 100%.
So the optional part is for students who like to do more programming.
Bonus marks can be used to compensate for any errors in the lab, e.g.
suppose part 1 and 2 was marked at 20 and 75 marks respectively while the
optional part 3 was marked at 8 marks. This means that the lab total is
100 marks with 3 leftover bonus marks to be used elsewhere.

The task here is to compute the [median][5] of N input numbers.
In part 3, we assume that N is an odd number (the even case can be handled
but with more work).
Roughly speaking, the median of N numbers can be obtained by sorting them
and picking the middle number. For example, consider the numbers

    5 7 2 3 10

The median is 5\.
You can then see that the median is the (N+1)/2 largest number in the input.
Take note while sorting is used, it is not essential and there are a
number of possible algorithms which do less work than sorting.

**Input Specification**  

The input is a single line contains an integer N which is
an odd number.
**Output Specification**  

The output is a C program, _outputcode_.
Outputcode will read N integers from standard input.
You can assume that the N integers read are distinct (do not have duplicates).
The outputcode program then outputs the median of the N integers.

**Constraints:**  

*   Your program should be able to handle input N in range: 1 ≤ N ≤ 9\.
*   Your output C code should be able to handle signed integers input: -2147483648...2147483647\.
    

**Example Input for Outputcode:**  
Assume the input size to _yourprog_ is 3\.

    7 -1 9

**Output for example input for the Outputcode:**  

    7
    

**Hint**  

*   Arrays are not necessary but can be used. 
    Note that no loops are allowed in the _outputcode_ C program.
*   Counting is one way of finding the k-th largest number.
    

## Grading + Submission Summary
Submit the following in a single zip file with the filename
yourmatricno-lab1.zip, e.g. if your matric number is U123X, then
U123X-lab1.zip. Note the user of Uppercase in the matric number.

*   Part 1: yourmatricno-lab1-part1.c, 20 marks
*   Part 2: yourmatricno-lab1-part2.c, 80 marks
*   Part 3: yourmatricno-lab1-part3.c, 15 bonus marks
    



[0]: http://www.comp.nus.edu.sg/%7Ecs2281/Notes/clib.pdf
[1]: http://www.comp.nus.edu.sg/%7Ecs2281/Labs/lab1-redirect.c
[2]: http://en.wikipedia.org/wiki/Arithmetic_progression
[3]: http://en.wikipedia.org/wiki/Geometric_progression
[4]: http://www.comp.nus.edu.sg/%7Ecs2281/Labs/lab1-IOeg.c
[5]: http://en.wikipedia.org/wiki/Median