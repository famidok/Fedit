/* == INCLUDES == */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

/* == DEFINES == */

#define CTRL_KEY(k) ((k) & 0x1f)

/* == DATA == */

struct termios orig_termios;

/* == TERMINAL == */

void die(const char *s)
{
	perror(s);
	exit(1);
}

void DISABLE_RAW_MODE() 
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		die("Error at tcsetattr in disable raw mode.");
}

void ENABLE_RAW_MODE() 
{
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("Error at tcgetattr in enable raw mode.");
	atexit(DISABLE_RAW_MODE);

	struct termios raw = orig_termios;	
	raw.c_iflag &= ~(BRKINT | ICRNL | IXON | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("Error at tcsetattr in enable raw mode.");
}

/* == INIT == */

int main() 
{
	ENABLE_RAW_MODE();

	while(1)
	{
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("Error at main read function.");
		if (iscntrl(c))
		{
			printf("%d\r\n", c);
		}
		else
		{
			printf("%d ('%c')\r\n", c, c);
		}
		if (c == CTRL_KEY('q')) break;
	}
	return 0;
}

