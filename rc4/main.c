/*
	main.c
 */

#include "rc4.h"

int main(int argc, char *argv[])
{
	void (*func[2])(int, char *) = {encrypt, decrypt};
	int menu = 0;

	if(argc != 2){
		fprintf(stderr,"Usage: %s <file_name>\n", argv[1]);
		exit(0);
	}

	int fd = open(argv[1], O_RDONLY);
	if(fd<0){
		perror("failed open file");
		exit(0);
	}
	puts("===================");
	puts(" 1. encrypt file");
	puts(" 2. decrypt file");
	puts(" 3. cancel      ");
	puts("===================");
	printf(">> ");
	

	scanf("%d", &menu);
	if(1<=menu && menu<=2){
		func[menu-1](fd, argv[1]);
	} else if(menu == 3){
		puts("program canceled");
	} else{
		puts("!! WRONG OPTION !!");
	}
	close(fd);

	return 0;
}
