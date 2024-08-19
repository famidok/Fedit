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
	raw.c_lflag &= ~(ECHO);

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

int main() 
{
	ENABLE_RAW_MODE();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
	return 0;
}

