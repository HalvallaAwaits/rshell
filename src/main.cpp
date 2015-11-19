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
bool execute(char **);


int main(int argc, char * argv[]){

	while (true){
		//variables	
		string inputLine;										//holds the user input
		const int SIZE = 16;									//args size
		char * args[SIZE];									//holds the command
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
		boost::char_separator<char> sep(" \n", "();#");

		tokenizer toks(inputLine, sep);
		
		//copy tokens into vector for easier storage/manipulation 
		for (tokenizer::iterator tok_iter = toks.begin(); tok_iter != toks.end(); ++tok_iter){
			tokens.push_back(*tok_iter);
		}

		//----------------------------------------------------
		//Loop through the tokens and execute based on logic
		//tied to the connecter that precedes the command
		//----------------------------------------------------
		for (unsigned int i = 0; i < tokens.size(); i++){
			//if "exit" is found at any point in tokens
			if (tokens[i] == "exit"){
				if (prevCn == ";")
					exit(0);
				else if (prevCn == "&&" && success == true)
					exit(0);
				else if (prevCn == "||" && success == false)
					exit(0);
			}

			//if hastag (#) is found, execute for commented line based on prev connector
			//and take new command from user
			if (tokens[i] == "#"){
				if (prevCn == ";"){
					args[i] = 0;
					success = execute(args);				//execute stored command
					break;
				}

				if (prevCn == "&&"){
					if (success == true){
						args[i] = 0;
						success = execute(args);			//execute stored command
					}
					break;
				}

				if (prevCn == "||"){
					if (success == false){
						args[i] = 0;
						success = execute(args);			//execute stored command	
					}
					break;
				}

			}

			//if token is connector, operate based on previous connector
			if (tokens[i] == ";" || tokens[i] == "&&" || tokens[i] == "||" || tokens[i] == ")"){
				args[counter] = 0;
				
				//if previous connector was ;
				if (prevCn == ";"){
					success = execute(args);				//execute stored command	
				}

				//if previous connector was &&
				else if (prevCn == "&&"){
					if (success == true){
						success = execute(args);			//execute stored command
					}
				}

				//if previous connector was ||
				else if (prevCn == "||"){
					if (success == false){
						success = execute(args);			//execute stored command
					}
				}

				//clear args array, reset counter, and set prevCn for next command
				for (int j = 0; j < SIZE; j++){
					args[j] = 0;
				}

				prevCn = tokens[i];
				counter = 0;
			}

			//Precedence Logic
			else if (tokens[i] == "("){
				if ((prevCn == "&&" && success == false) || (prevCn == "||" && success == true)){
					while (tokens[i] != ")")
						i++;
				}
			}

			//------------------------------
			//if last command to run
			//------------------------------
			else if (i == tokens.size() - 1){
				args[counter] = const_cast<char *>(tokens[i].c_str());
				args[counter + 1] = 0;
				
				//if previous connector was ;
				if (prevCn == ";"){
					success = execute(args);				//execute stored command
				}

				//if previous connector was &&
				else if (prevCn == "&&"){
					if (success == true){
						success = execute(args);			//execute stored command
					}
				}

				//if previous connector was ||
				else if (prevCn == "||"){
					if (success == false){
						success = execute(args);			//execute stored command
					}
				}

				//clear args array, reset counter, and set prevCn for next user input 
				for (int j = 0; j < SIZE; j++){
					args[j] = 0;
				}

				prevCn = tokens[i];
				counter = 0;
			}

			
			//if not a connector
			else{
				args[counter] = const_cast<char *>(tokens[i].c_str());
				counter++;
			}
		}
	}
	return 0; 
}

bool execute(char **cmd){
	pid_t c_pid, pid;
	int status;

	c_pid = fork();

	if( c_pid < 0){
		perror("fork failed");
		exit(1);
	}
	
	else if (c_pid == 0){
		execvp( *cmd, cmd);
		perror("execve failed");
		exit(1);
	}
	
	else if (c_pid > 0){
		if( (pid = wait(&status)) < 0){
			perror("wait");
			exit(1);
		}
	}

	if (status !=0){
		return false;
	}

	return true;
}
