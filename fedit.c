#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void DISABLE_RAW_MODE() 
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void ENABLE_RAW_MODE() 
{
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(DISABLE_RAW_MODE);

	struct termios raw = orig_termios;	
	raw.c_iflag &= ~(ICRNL | IXON);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() 
{
	ENABLE_RAW_MODE();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
	{
		if (iscntrl(c)) {
			printf("%d\n", c);
		}
		else
		{
			printf("%d ('%c')\n", c, c);
		}
	}
	return 0;
}

