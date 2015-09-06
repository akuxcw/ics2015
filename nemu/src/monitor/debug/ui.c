#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

bool flag;

void cpu_exec(uint32_t);
char last_command[30];

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;
	int i;
	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}
	if(strlen(line_read)==0) {
		for(i=0;i<strlen(last_command);++i) {
			line_read[i]=last_command[i];
		}
		line_read[i]='\0';
	}else {
		for(i=0;i<strlen(line_read);++i) {
			last_command[i]=line_read[i];
		}
		last_command[i]='\0';
	}
	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
	int x=0,i;
	if(args==NULL)x=1;else
	for(i=0;i<strlen(args);++i) {
		if(args[i]>'9'||args[i]<'0')flag=false;
		if(!flag)break;
		x=x*10+args[i]-48;
	}
	if(!flag)return 0;
//	printf("%d\n",x);
	if(x==0)x=1;
	cpu_exec(x);
	return 0;
}

static int cmd_info(char *args) {
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help          ", "Display informations about all supported commands", cmd_help },
	{ "c             ", "Continue the execution of the program", cmd_c },
	{ "q             ", "Exit NEMU", cmd_q },
	{ "si [N]        ", "Run the program by N command,default by one", cmd_si},
	{ "info SUBCMD   ", "SUBCMD=r print the value of register\n                    =w print the status of watch point", cmd_info},	
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strstr(cmd_table[i].name,args) == cmd_table[i].name) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);
		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strstr(cmd_table[i].name, cmd) == cmd_table[i].name) {
				flag=true;
				if(cmd_table[i].handler(args) < 0) { return; }
				if(flag)break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", str); }
	}
}
