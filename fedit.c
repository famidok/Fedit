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

void DIE(const char *s)
{
	perror(s);
	exit(1);
}

void DISABLE_RAW_MODE() 
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		DIE("Error at tcsetattr in disable raw mode.");
}

void ENABLE_RAW_MODE() 
{
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) DIE("Error at tcgetattr in enable raw mode.");
	atexit(DISABLE_RAW_MODE);

	struct termios raw = orig_termios;	
	raw.c_iflag &= ~(BRKINT | ICRNL | IXON | INPCK | ISTRIP);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) DIE("Error at tcsetattr in enable raw mode.");
}

char EDITOR_READ_KEY() 
{
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) 
	{
		if (nread == -1 && errno != EAGAIN) DIE("Error at read function.");
	}
	return c;
}

void EDITOR_PROCESS_KEYPRESS()
{
	char c = EDITOR_READ_KEY();

	switch (c)
	{
		case CTRL_KEY('q'):
			exit(0);
			break;
	}
}

/* == INIT == */

int main() 
{
	ENABLE_RAW_MODE();

	while(1)
	{
		EDITOR_PROCESS_KEYPRESS();
	}
	return 0;
}

