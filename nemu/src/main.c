#include<stdio.h>
void init_monitor(int, char *[]);
void reg_test();
void restart();
void ui_mainloop();

int main(int argc, char *argv[]) {

	/* Initialize the monitor. */
	printf("%d\n",argc);
	init_monitor(argc, argv);

	/* Test the implementation of the ``CPU_state'' structure. */
	reg_test();

	/* Initialize the virtual computer system. */
	restart();

	/* Receive commands from user. */
	ui_mainloop();

	return 0;
}
