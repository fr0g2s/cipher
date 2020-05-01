#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

void init_S();
void swap(int *a, int *b);
void encrypt(int plainFd, char *filename);
void decrypt(int encFd, char *filename);
unsigned int getFileSize(int fd);
void xorCalc(int oldFd, int newFd, int length);
