EXECUTE MULTIPLE COMMANDS AT ONCE WITH RSHELL

   This is a command shell named rshell that is able to read in commands from the
command line and execute multiple commands at once. Connectors are used to run multiple
commands. Placing a semicolon connector (;) between commands will ensure that after 
the first command is executed, the next command will follow. 

$ ls -a; echo hello; mkdir test

Placing a double ampersand connector (&&) between to commands will only execute the
second comand if the first one succeeds.

$ ls echo hello && mkdir test

If a command is followed by ||, the second command will only execute is the first has 
failed.

$ ls echo hello || echo world

There is no limit to the number of commands that can be chained together
with these operators.

$ ls -a; echo hello && mkdir test || echo world; git status


