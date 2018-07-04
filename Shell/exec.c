#include "exec.h"


// exits with error
// frees all the resources
static void exit_with_error() {
	if (parsed_command->type == PIPE){
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		while (wait(NULL) > 0){}
	}
	free_command(parsed_command);
	exit(EXIT_FAILURE);
}

// sets the "key" argument with the key part of
// the "arg" argument and null-terminates it
static void get_environ_key(char* arg, char* key) {

	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets the "value" argument with the value part of
// the "arg" argument and null-terminates it
static void get_environ_value(char* arg, char* value, int idx) {

	int i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

// sets the environment variables passed
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void set_environ_vars(char** eargv, int eargc) {

	for (int i = 0; i < eargc; i++){
		char name[BUFLEN];
		char value[BUFLEN];

		get_environ_key(eargv[i], name);
		int index = block_contains(eargv[i], '=');
		get_environ_value(eargv[i], value, index);

		setenv(name, value, true);
	}
} 

// opens the file in which the stdin/stdout or
// stderr flow will be redirected, and returns
// the file descriptor
// 
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?

//No, los file descriptors 0,1,2 se cierran solos en el exit.

//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int open_redir_fd(char* file, int action) {

	int fd;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

	if (action == READ){
		fd = open(file, O_RDONLY);
	} else if (action == WRITE){
		fd = open(file, O_CREAT|O_WRONLY|O_TRUNC , mode);
	} else {
		fd = open(file, O_CREAT|O_WRONLY|O_APPEND , mode);
	}
	if (fd < 0){

		char error[BUFLEN];
		snprintf(error, BUFLEN, "Could not open file '%s'", file);
		perror(error);

		exit_with_error();
	}

	return fd;
}

// redirects old fd to new fd
// old fd will be closed
static void redir_fd(int old_fd, int new_fd, bool close_old_fd) {

	int res = dup2(old_fd, new_fd);

	if (close_old_fd){
		close(old_fd);
	}

	if (res == -1){

		char error[BUFLEN];
		snprintf(error, BUFLEN, "Error in dup() from %d to %d", old_fd, new_fd);
		perror(error);

		exit_with_error();
	}
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
void exec_cmd(struct cmd* cmd) {

	struct execcmd* exec;
	struct backcmd* back;
	struct pipecmd* pipecmd;

	switch (cmd->type) {

		case BACK: {
			// runs a command in background
			back = (struct backcmd*) cmd;
			
			exec_cmd(back->c);
		}

		case REDIR: {
			// changes the input/output/stderr flow
			
			exec = (struct execcmd*) cmd;

			if (exec->in_file[0] != END_STRING){
				int fd = open_redir_fd(exec->in_file, READ);
				redir_fd(fd, STDIN_FILENO, true);
			}

			if (exec->out_file[0] != END_STRING){
				int fd = open_redir_fd(exec->out_file, exec->out_file_mode);
				redir_fd(fd, STDOUT_FILENO, true);
			}

			if (exec->err_file[0] != END_STRING){
				if (strcmp(exec->err_file, exec->out_file) == 0){
					//&>file
					//Se redirigio stdout y stderr al mismo archivo
					redir_fd(STDOUT_FILENO, STDERR_FILENO, false);
				} else {
					int fd = open_redir_fd(exec->err_file, exec->err_file_mode);
					redir_fd(fd, STDERR_FILENO, true);
				}
			}

			//No hago break, sigue ejecutando EXEC
		}

		case EXEC:
			// spawns a command

			exec = (struct execcmd*) cmd;
			set_environ_vars(exec->eargv, exec->eargc);

			execvp(exec->argv[0], exec->argv);


			//Si llega aca ocurrio un error en el exec()

			char error[BUFLEN];
			snprintf(error, BUFLEN, "Could not execute command '%s'", exec->argv[0]);
			perror(error);
			
			exit_with_error();
			break;
		
		case PIPE: {
			// pipes two commands
			
			pipecmd = (struct pipecmd*) cmd;
			int pipefd[2];

			if (pipe(pipefd) < 0) {
               perror("pipe");
               exit_with_error();
           }


			pid_t pid = fork();

			if (pid < 0){
				perror("fork()");
				exit_with_error();
			} else if(pid == 0){
				//proceso hijo, ejecuto leftcmd

				close(pipefd[READ]);
				redir_fd(pipefd[WRITE], STDOUT_FILENO, true);
				exec_cmd(pipecmd->leftcmd);
			} else {
				//proceso padre, ejecuto rightcmd
				close(pipefd[WRITE]);
				redir_fd(pipefd[READ], STDIN_FILENO, true);

				struct cmd* right_parsed;
				right_parsed = parse_line(pipecmd->rightcmd->scmd);

				free_command(pipecmd->rightcmd);
				pipecmd->rightcmd = right_parsed;

				exec_cmd(pipecmd->rightcmd);
			}

			break;
		}

	}

}

