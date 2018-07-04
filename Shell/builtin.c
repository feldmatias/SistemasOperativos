#include "builtin.h"
#include "utils.h"

// returns true if the 'exit' call
// should be performed
int exit_shell(char* cmd) {
	return (strcmp(cmd, "exit") == 0);
}

// returns true if "chdir" was performed
// this means that if 'cmd' contains:
// 	$ cd directory (change to 'directory')
// 	$ cd (change to HOME)
// it has to be executed and then return true
int cd(char* cmd) {
	
	if (strncmp(cmd, "cd ", 3) != 0 && strcmp(cmd, "cd") != 0){
		return false;
	}

	char path[BUFLEN];
	char cwd[BUFLEN];
	char* directory = split_line(cmd, SPACE);

	if (directory[0] == END_STRING){
		// 	$ cd (change to HOME)
		snprintf(path, BUFLEN, "%s", getenv("HOME"));
	} else if (directory[0] == DIR_SEPARATOR){
		// 	$ cd /directory (change to '/directory')
		snprintf(path, BUFLEN, "%s", directory);
	} else {
		// 	$ cd directory (change to 'cwd/directory')
		getcwd(cwd, BUFLEN);
		snprintf(path, BUFLEN, "%s/%s", cwd, directory); //Concatena cwd y directory
	}

	if (chdir(path) < 0) {
		char error[BUFLEN];
		snprintf(error, BUFLEN, "Could not change directory to '%s'", path);
		perror(error);
	} else {
		//Actualizo prompt
		getcwd(cwd, BUFLEN);
		snprintf(promt, sizeof promt, "(%s)", cwd);
	}
	return true;
}

// returns true if 'pwd' was invoked
// in the command line
int pwd(char* cmd) {

	if (strcmp(cmd, "pwd") != 0){
		return false;
	}

	char cwd[BUFLEN];
	getcwd(cwd, BUFLEN);
	printf("%s\n", cwd);
	return true;
}
