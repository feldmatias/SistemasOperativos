#ifndef EXEC_H
#define EXEC_H

#include "defs.h"
#include "types.h"
#include "utils.h"
#include "freecmd.h"
#include "parsing.h"

extern struct cmd* parsed_command;

void exec_cmd(struct cmd* c);

#endif // EXEC_H
