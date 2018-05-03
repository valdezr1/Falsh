#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//Prompts user that they have entered the Falcon Shell
void prompt(){
	printf("\n");
	printf("*******************************************\n\n");
	
	printf("\tWelcome to the Falcon Shell\n\n");

	printf("*******************************************\n\n");
}

void promptHelp(){

	printf("\n");
	printf("*********************************************************************************************\n\n");
	
	printf("\t\t\tWelcome to the Falcon Shell Help File\n\n");

	printf("*********************************************************************************************\n\n");

	printf("\tAvailable Commands: \n");
	printf("---------------------------------------------------------------------------------------------\n");

	printf("\texit : Exit the program\n");
	printf("\tpwd : Print the current working directory\n");
	printf("\tcd [dir] : Change dirctory\n");
	printf("\tsetpath <dir> [dir] ... [dir] : Sets the user's path\n");
	printf("\thelp : List all built-in commands with short user-friendly descriptions\n");
	printf("\n");
	
	printf("*********************************************************************************************\n\n");

}

void pwdCommand(size_t nbytes, char* cstr){
	//getcwd gets path up until working directory
	//	Takes in c-str and size
	//	Saves path into passed c-str
	//	Returns NULL if path is larger than allotted size
	if(getcwd(cstr, nbytes) != NULL){;
		printf("%s\n", cstr);
	}
	else{
		printf("Path too large to print\n");
	}
}

int cdCommand(char* command, size_t nbytes){

	char* directoryName = (char*) malloc(nbytes);
	
	int ret = 1;
	strcpy(directoryName, command + 3); 	// from position 3 till end
						// 	'\0' included
	ret = chdir(directoryName);

	free(directoryName);

	return ret; //Directory not found
}

int setPathCommand(char* command, size_t nbytes){
	char* path = (char*)malloc(nbytes);
	char* curPath;
	int i;
	int ret = -1;
	//path = getenv("PATH"); //gets environment variable value specified
	

	// Function to setpath: int setEnv(const char* name, const char* value, int overwrite)
	//	Takes in name as the environment variable name (in this case it would be "PATH")
	//	And sets the environment variable specified with the value of the passed value.
	//	If it the environment variable exists (which "PATH" will), then the overwrite will
	//	Have to be non-zero to overwrite the existing environment variable.
	//	Returns 0 if successful or -1 if errored
	
	// Ideas:
	// 	* Could try to build a large cstring through recursion and returning new cstrings
	// 		over and over while concatinating
	//	* Could replace ' ' with a ';' or ':' which seems to be how the environment variable stores
	//		the path to multiple directories
	
	strcpy(path, command + 8);
	
	//Replace the ' ' with ':'
	if (strlen(path) == 0){
		return ret;
	}
	for(i = 0; i < strlen(path); i++){
		if(path[i] == ' '){
			path[i]= ':';
		}
	}

	ret = setenv("PATH", path, 1);
 
	curPath = getenv("PATH");

	printf("PATH: %s\n", path);
	printf("setPathCommand() ran\n");
	free(path);
	

	return ret;
}

int main(int argc, char** argv){
	
	char* userInput;
	char* workDir;
	char* checkCD;
	char* checkSetPath;
	char* checkWorkDir;
	char* runExec;
	char* curPath;

	char* childArgs[256];

	int rc;
	int read = 0;
	size_t nbytes = 256; //Can't be a const due to use in getline 
	

	//Check for [-h] 
	if (argc == 2) {
		//Check if 2nd arg is "-h"
		if (!strcmp(argv[1], "-h")) {
			promptHelp();
			exit(EXIT_SUCCESS);	
		}
		else {
			printf("Unrecognized Flag\n");
			exit(EXIT_FAILURE);	
		}
	}
	//Check for more than two arguments to invoke falsh
	else if(argc > 2){
		printf("Invalid arguments\n");
		exit(EXIT_FAILURE);
	}
	else {
		//allocates memory for userInput: MAX at 256 bytes
		userInput = (char*)malloc(nbytes);
		workDir = (char*)malloc(nbytes);

		//Prompt user
		prompt();
		while(1){
			getcwd(workDir, nbytes);
			printf("falsh>%s $ ", workDir);	
			read = getline(&userInput, &nbytes, stdin);
			
			//Check if user inputs pwd
			if(!strcmp(userInput, "pwd\n")){
				pwdCommand(nbytes, userInput);	
			}
			//Check if user inputs exit
			else if(!strcmp(userInput,"exit\n")){
				free(userInput); //deallocates memory
				printf("\nExiting Falcon Shell\n\n");
				exit(EXIT_SUCCESS);
			}
			//Cases in where user input is invalid 
			//	Or represents more than one argument 
			else {
				//Check if user inputs cd
				checkCD = (char*)malloc(nbytes);
				strncpy(checkCD, userInput, 3); // Increased to 3 to account for '\0'
				checkCD[2] = '\0';		// Changed 3rd element of checkCD to '\0'

				//Check if user inputs setpath
				checkSetPath = (char*)malloc(nbytes);
				strncpy(checkSetPath, userInput, 8);	//Increased to 8 to account for '\0'
				checkSetPath[7] = '\0';			//Changed 7th element of checkSetPath to '\0'

				//Gets rid of trailing '\n' in userInput from getline
				userInput[strcspn(userInput, "\n")] = 0; 

				if(!strcmp(checkCD, "cd")) {	
					if(cdCommand(userInput, nbytes) == -1){
						printf("Directory not Found\n"); //Directory not found
					}
				}
				else if(!strcmp(checkSetPath, "setpath")){
					setPathCommand(userInput, nbytes);
					printf("Setpath command invoked\n");
				}
				else{
					checkWorkDir = (char*)malloc(nbytes);
					strncpy(checkWorkDir, userInput, 3);
					checkWorkDir[2] = '\0';

					
					childArgs[0] = userInput;	//First argument will be the program the user inputs
					childArgs[1] = NULL;		//Last argument will be NULL to end the search
					if(!strcmp(checkWorkDir, "./")){

						rc = fork();
						if(rc < 0){ //Failed
							printf("Failed to fork.\n");
						}
						else if(rc == 0){
							printf("Child Process Occuring Now:\n");
							if(execvp(userInput, childArgs) == -1){ //exit if unable to be run
								exit(0);
							}
						}
						else{
							wait(NULL);
							printf("\nBack to Parent\n");
						}	
						//system(userInput);
					}	
					else {
						char* token;
						curPath = (char*)malloc(nbytes);
						strcpy(curPath, getenv("PATH"));
						printf("curPath: %s\n", curPath);
						int position = 0;
					
						token = strtok(curPath, ":");
						while(runExec != NULL){
								runExec = (char*)malloc(nbytes);
					
								strcpy(runExec, token);		
								strcat(runExec, "/");
								strcat(runExec, userInput);
								printf("runExec: %s\n", runExec);	
								
								
								rc = fork();
								if(rc < 0){ //Failed
									printf("Failed to fork\n");
								}
								else if (rc == 0){
									printf("Child Process Occuring Now:\n");
									if(execvp(runExec, childArgs) == -1){ //exit if unable to be run
										exit(0);
									}
								}
								else{
									wait(NULL);
									printf("\nBack to Parent\n");
								}
								
								token = strtok(NULL, ":");
								if(token == '\0'){
									break;
								}
								free(runExec);	
						}
						free(curPath);
					}

					free(checkWorkDir);
					//printf("Unrecognized Command\n");
				}
			
				free(checkCD);
				free(checkSetPath);

			}
		}
		free(userInput);
		free(workDir);
	}
	
	return 0;
}
