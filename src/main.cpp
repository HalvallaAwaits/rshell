#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <string.h>
#include <boost/tokenizer.hpp>
#include <vector>
using namespace std;

//function prototypes
void execute(char **);

int main(int argc, char * argv[]){

	while (true){
		//variables	
		string inputLine;										//holds the user input
		char * args[128];										//holds the command
		vector<string> tokens;								//holds tokens
		char hostname[100];									//holds hostname
		char * username;										//holds username

		string prevCn = ";";									//holds previous connector
		int counter = 0;										//used to loop through vector
		bool success = true;									//passed back from execvp

		//----------------------
		//shell prompt display with user and host names
		//----------------------
		if (-1 == (gethostname(hostname, 100))){
			perror("gethostname failed");
			exit(1);
		}

		if (NULL == (username = getlogin())){
			perror("no username");
			exit(1);
		}
			  
		cout << "[" << username << "@" << hostname << "] $ ";
		
		//get input from user
		getline(cin, inputLine);
		
		//check to see if exit was typed
		if (strcmp(inputLine.c_str(), "exit") == 0){
			exit(0);
		}
		
		//parse input in order to execute	
		typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
		boost::char_separator<char> sep(" \n", ";");
		tokenizer toks(inputLine, sep);
		
		//copy tokens into vector for easier storage/manipulation 
		for (tokenizer::iterator tok_iter = toks.begin(); tok_iter != toks.end(); ++tok_iter){
			tokens.push_back(*tok_iter);
		}

		//-------------------------------------------------
		//THIS ONLY WORKS FOR SINGLE COMMAND!!!
		//This is where we will loop through the entire vector.
		//When you hit a connector, you will execute the
		//command that was just stored prior to hitting it
		//based on the logic attached to the connector.
		//Start with ; and then && followed by ||.
		//-------------------------------------------------
	



		/*
		//convert strings into char * for storage in args array
		for (unsigned int i = 0; i < tokens.size(); i++){
			args[i] = const_cast<char *>(tokens[i].c_str());
		}

		args[tokens.size()] = 0;
		 
		//execute
		execute(args);
		*/


		//NEW CODE BLOCK FOR HANDLING MULTIPLE COMMANDS
		//loop through the tokens vector
		for (unsigned int i = 0; i <tokens.size(); i++){
			//if connector is found we determine which and 
			//operate according to the previous connector found
			if (tokens[i] == ";" || tokens[i] == "&&" || tokens[i] == "||" || i == tokens.size() - 1){
				args[counter] = 0;
				
				//if previous connector was ;
				if (prevCn == ";"){
					execute(args);							//will update success to t/f

					//clear args array, reset counter, and set prevCn for next command
					for (unsigned int j = 0; j < 128; j++){
						args[j] = 0;
					}

					prevCn = tokens[i];
					counter = 0;
				}

				//if previous connector was &&
				if (prevCn == "&&"){
					if (success == true){
						execute(args);						//will update success to t/f
					}

					//clear args array, reset counter, and set prevCn for next command
					for (unsigned int j = 0; j < 128; j++){
						args[j] = 0;
					}

					prevCn = tokens[i];
					counter = 0;
				}

				//if previous connector was ||
				if (prevCn == "||"){
					if (success == false){
						execute(args);						//will update success to t/f
					}

					//clear args array, reset counter, and set prevCn for next command
					for (unsigned int j = 0; j < 128; j++){
						args[j] = 0;
					}

					prevCn = tokens[i];
					counter = 0;
				}
			}

			//if not a connector, store into args array
			args[counter] = const_cast<char *>(tokens[counter].c_str());
			counter++;
		}

	}
	return 0; 
}

void execute(char **cmd){
	pid_t c_pid, pid;
	int status;

	c_pid = fork();

	if( c_pid < 0){
		perror("fork failed");
		exit(1);
	}

	else if (c_pid == 0){
		printf("Child: executing\n");
		execvp( *cmd, cmd);
		perror("execve failed");
	}
	
	else if (c_pid > 0){
		if( (pid = wait(&status)) < 0){
			perror("wait");
			exit(1);
		}
		printf("Parent: finished\n");
	}
}
