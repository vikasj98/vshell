#include "vshell.h"

//The main function that starts the shell
int main(int argc, char **argv)
{
	//Enter an infinite loop and keep executing comands
	//Till exit is not called. Note : ctrl+c will kill the shell
	vsh_print_welcome_message();
	while(1)
	{
		vsh_execute();
	}
	return 0;
}
