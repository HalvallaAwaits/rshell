#rShell

##Contributors
Joseph Reimbold
[jreim001@ucr.edu](mailto:jreim001@ucr.edu)

Eric Marcelo
[emarc003@ucr.edu](mailto:emarc003@ucr.edu)

##What is it?

   This is a command shell named rShell that is able to read in commands from a 
command line and execute multiple commands at once.

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
1. A command can have up to 31 arguments (including the command itself), anything more will `break` it.
2. Typing exit at the end of a command following any parameters will cause it to `exit` without performing that particular command, but the preceding commands will be called.

