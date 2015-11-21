#rShell

##Contributors
Joseph Reimbold
[jreim001@ucr.edu](mailto:jreim001@ucr.edu)

Eric Marcelo
[emarc003@ucr.edu](mailto:emarc003@ucr.edu)

##Design

   This is a command shell named rShell that is able to read in commands from a 
command line and execute multiple commands at once. It is intended for basic use
as it currently will only take pre-built commands found in /bin.

	A more robust feature set could be added and expanded functionality could include
commands such as the change directory command `cd`, etc.

###Functionality

* Placing a semicolon connector `;` between commands will ensure that after 
the first command is executed, the next command will follow. 

> $ ls -a; echo hello; mkdir test

* Placing a double ampersand connector `&&` between to commands will only execute the
second comand if the first one succeeds.

> $ ls echo hello && mkdir test

* If a command is followed by `||`, the second command will only execute is the first has 
failed.

> $ ls echo hello || echo world

* Using a `#` will cause any following text to become a comment.

> $ echo hello #just saying hi!

* Use parenthesis `( )` to set a precedence order to your commands. This causes your
commands to act as a group.

> $ (echo A && echo B) || (echo C && echo D)

* Use `test` to check for a file or directory with the flags `-e`, `-f`, or `-d`. You
can also use brackets `[ ]` and wrap the flag with the path as an alternative.

> $ test -e ./src/main.cpp
> $ [-e ./src/main.cpp]

* Use `exit` to exit out of rShell. Use it at the end of a chain of commands to
perform them before you exit!

##How to run
1. Clone the repository
> git clone https://github.com/HalvallaAwaits/rshell.git

2. Change directory into rshell
> cd rshell

3. Call `make`

4. Call `bin/rshell`

5. Clean up after yourself!
> make clean

##Known Bugs
Limitation: A command can have up to 31 arguments (including the command itself), anything more will `break` it.

1. Quotes do not currently function properly. The command echo "A # B" will echo '"A '
and then break out since it finds a comment.
