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

int main(int argc, char** argv){
	
	
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
		//Prompt user
		prompt();
	}

	return 0;
}
