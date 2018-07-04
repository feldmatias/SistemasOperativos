#include "runcmd.h"

int status = 0;
struct cmd* parsed_command;

// runs the command in 'cmd'
int run_cmd(char* cmd) {
	
	pid_t pid;

	// if the "enter" key is pressed
	// just print the promt again
	if (cmd[0] == END_STRING)
		return 0;

	// cd built-in call
	if (cd(cmd))
		return 0;

	// exit built-in call
	if (exit_shell(cmd))
		return EXIT_SHELL;

	// pwd buil-in call
	if (pwd(cmd))
		return 0;

	// parses the command line
	parsed_command = parse_line(cmd);
	
	// forks and run the command
	pid = fork();
	if (pid < 0){
		perror("fork()");
		free_command(parsed_command);
		return 0;
	}

	if (pid == 0) {
		exec_cmd(parsed_command);
	}

	// store the pid of the process
	parsed_command->pid = pid;

	// background process special treatment
	// Hint:
	// - check if the process is 
	// 	going to be run in the 'back'
	// - print info about it with 
	// 	'print_back_info()'
	//
	if (parsed_command->type != BACK){
		// waits for the process to finish
		waitpid(pid, &status, 0);
		print_status_info(parsed_command);
		free_command(parsed_command);
		
	} else {
		last_back_command = parsed_command;
		print_back_info(parsed_command);
	}

	return 0;
}

