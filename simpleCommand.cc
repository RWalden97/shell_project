#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <regex.h>
#include <pwd.h>

#include "simpleCommand.hh"

SimpleCommand::SimpleCommand()
{
	// Creat available space for 5 arguments
	_numberOfAvailableArguments = 5;
	_numberOfArguments = 0;
	_arguments = (char **) malloc( _numberOfAvailableArguments * sizeof( char * ) );
}

void
SimpleCommand::insertArgument( char * argument )
{
	if ( _numberOfAvailableArguments == _numberOfArguments  + 1 ) {
		// Double the available space
		_numberOfAvailableArguments *= 2;
		_arguments = (char **) realloc( _arguments,
				  _numberOfAvailableArguments * sizeof( char * ) );
	}
	
	
	if(argument[0] == '~'){
		if(argument[1] == '\0'){
			//printf("%d\n", getuid());
			struct passwd *pw = getpwuid(getuid());
			const char * directory = pw->pw_dir;
			strcpy(argument,directory);
		}
		else{
			char * tmp = argument;
			int len = strlen(argument);
			char * subdir = (char*)calloc(sizeof(char), (len+20));
			char * back = subdir;
			tmp++;
			while(*tmp != '\0'){
				if(*tmp == '/'){
					break;
				}
				*subdir = *tmp;
				subdir++;
				tmp++;
			}
			--subdir = '\0';
			subdir = back;
			if(*tmp == '\0'){
				struct passwd *pw = getpwnam(subdir);
				const char * directory = pw->pw_dir;
				argument = strdup(directory);
			}else{
				char * t = strcat(getpwnam(subdir)->pw_dir,tmp);
				argument = strdup(t); 
			}
			free(subdir);
		}
	}
	

	char * newArgument = (char*)malloc(sizeof(char) * (strlen(argument) + 200));
	char * globVar = (char*)malloc(sizeof(char) * 50);
	char * globVarVal = (char*)malloc(sizeof(char) * 150);

	int newArgCounter = 0;
	int oldArgCounter = 0;

	while(argument[oldArgCounter] != '\0'){
		if(argument[oldArgCounter] == '\\' && argument[oldArgCounter+1] == '$'){
			oldArgCounter++;
			while(argument[oldArgCounter] != '}'){
				newArgument[newArgCounter] = argument[oldArgCounter];
				newArgCounter++;
				oldArgCounter++;
			}
		}
		else if (argument[oldArgCounter] == '$' && argument[oldArgCounter+1] == '{'){
			oldArgCounter+=2;
			int counter = 0;
			while(argument[oldArgCounter] != '}'){
				globVar[counter] = argument[oldArgCounter];
				counter+=1;
				oldArgCounter+=1;
			}
			oldArgCounter+=1;
			globVar[counter] = '\0';
			globVarVal = getenv(globVar);	
			for(int i = 0; i < strlen(globVarVal); i++){
				newArgument[newArgCounter] = globVarVal[i];
				newArgCounter++;
			}
		}else{
			newArgument[newArgCounter] = argument[oldArgCounter];
			newArgCounter+=1;
			oldArgCounter+=1;
			
		}
	}
	newArgument[newArgCounter] = '\0'; 

	_arguments[ _numberOfArguments ] = newArgument;
	
	// Add NULL argument at the end
	_arguments[ _numberOfArguments + 1] = NULL;
	
	_numberOfArguments++;
}