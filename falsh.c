#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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
//	if(strncpy(substr, userInput + 3, 2)) {
//		printf("%s\n", substr);
//		free(substr);
//	}
	char* directoryName = (char*) malloc(nbytes);
	strcpy(directoryName, command + 3); //from position 3 till end
	printf("%s", directoryName);
	free(directoryName);

	return 0; //Directory not found
}

int main(int argc, char** argv){
	
	char* userInput;
	char* substr;
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
		
		//Prompt user
		prompt();
		while(1){
			printf("falsh > $ ");	
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
				substr = (char*)malloc(nbytes);
				strncpy(substr, userInput, 2);
				if(!strcmp(substr, "cd")) {
					if(!cdCommand(userInput, nbytes)){
						printf("Directory not Found\n");//Directory not found
					}
				}
				else{
					printf("Unrecognized Command\n");
				}
			
				free(substr);

			}
		}
	}
	
	return 0;
}
