#include "defs.h"
#include "types.h"
#include "readline.h"
#include "runcmd.h"

char promt[PRMTLEN] = {0};
struct cmd* last_back_command;

// runs a shell command
static void run_shell() {

	char* cmd;

	while ((cmd = read_line(promt)) != NULL){
		if (run_cmd(cmd) == EXIT_SHELL){
			return;
		}

		//Compruebo los hijos en segundo plano
		pid_t pid;
		if ((pid = waitpid(-1, NULL, WNOHANG)) > 0){
			printf("==> terminado: PID=%d ( %s)\n", pid, last_back_command->scmd);
			free_command(last_back_command);
		}
	}
}

// initialize the shell
// with the "HOME" directory
static void init_shell() {

	char buf[BUFLEN] = {0};
	char* home = getenv("HOME");

	if (chdir(home) < 0) {
		snprintf(buf, sizeof buf, "cannot cd to %s ", home);
		perror(buf);
	} else {
		snprintf(promt, sizeof promt, "(%s)", home);
	}
}

int main(void) {

	init_shell();

	run_shell();

	return 0;
}

