#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

static volatile int exitflag = 0;

int handler(int sig)
{
	exitflag = 1;
	return 0;
}

int main(int argc, char **argv)
{
	struct termios oflags, nflags;
	char password[64];

	/* disabling echo */
	tcgetattr(fileno(stdin), &oflags);
	nflags = oflags;
	nflags.c_lflag &= ~ECHO;
	nflags.c_lflag |= ECHONL;

	if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0) {
		perror("tcsetattr");
		return 1;
	}

	printf("First, type the password: ");
	fgets(password, sizeof(password), stdin);
	password[strlen(password) - 1] = 0;
	printf("Password saved.\n");

	while(!exitflag) {
		char tmp_pass[64];
		printf("Practice: ");
		fgets(tmp_pass, sizeof(tmp_pass), stdin);
		tmp_pass[strlen(tmp_pass) - 1] = 0;
		if(strcmp(password, tmp_pass) == 0) {
			printf(KGRN "Correct\n" KNRM);
		} else {
			printf(KRED "Incorrect\n" KNRM);
		}
	}

	/* restore terminal */
	if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0) {
		perror("tcsetattr");
		return 1;
	}

	return 0;
}
