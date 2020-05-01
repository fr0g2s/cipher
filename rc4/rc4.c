/*
	rc4.c
*/

#include "rc4.h"

int S[256];	// keystream for xor 
char key[17];

void swap(int *a, int *b){ *a ^= *b ^= *a^= *b; } // xor algorithm	

void init_S()
{
	printf("[ key (16 bytes)]: ");
	fflush(stdout);
	read(0, key, 16);
	key[strlen(key)] = '\0';

	for(int i=0;i<256;i++){
		S[i] = i;
	}
	int j = 0;
	for(int i=0;i<256;i++){
		j = (j+S[i]+key[i%strlen(key)])%256;	// if same key, same pseudorandom number
		swap(&S[i], &S[j]);
	}
}

unsigned int getFileSize(int fd)
{
	int fileSize = (unsigned int)lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	return fileSize;
}

void xorCalc(int oldFd, int newFd, int length){
	int i, j, rand_idx, keystream;
	char c, p;
	
	i = j = 0;

	for(int idx=0;idx<length;idx++){
		read(oldFd, &p, 1);
		
		i = (i+1) % 256;
		j = (j+S[i]) % 256;
		
		swap(&S[i], &S[j]);
		rand_idx = (S[i]+S[j])%256;
		keystream = S[rand_idx];
		c = p ^ S[keystream];

		write(newFd, &c, 1);
	}
	puts("");
}

void encrypt(int plainFd, char *filename)
{
	char *encFileName;
	int length = strlen(filename);
	int encFd;

	encFileName = (char *)malloc(length+5);	// 'filename'.enc

	memcpy(encFileName, filename, length);
	memcpy(encFileName+length, ".enc\x00", 5);
	
	encFd = open(encFileName, O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if(encFd == -1){
		perror("failed create .enc file");
		exit(1);
	} else{
		printf("successed create\n");
	}

	init_S();
	
	unsigned int plaintextLength = getFileSize(plainFd);
	printf("file size: %u\n", plaintextLength);
	
	xorCalc(plainFd, encFd, plaintextLength);	// do encrypt

	close(encFd);
	free(encFileName);

	return;
}

void decrypt(int encFd, char *filename)
{

	char *decFileName;
	int length = strlen(filename);
	int plainFd;

	decFileName = (char *)malloc(length+5);	// 'filename'.dec
	
	memcpy(decFileName, filename, length);
	memcpy(decFileName+length, ".dec\x00", 5);

	plainFd = open(decFileName, O_CREAT|O_TRUNC|O_WRONLY, 0644);
	if(plainFd == -1){
		perror("failed create .dec file");
		exit(1);
	}
	
	init_S();
	unsigned int ciphertextLength = getFileSize(encFd);
	printf("file size: %u\n", ciphertextLength);
	
	xorCalc(encFd, plainFd, ciphertextLength);	// do decrypt
		
	close(plainFd);
	free(decFileName);

	return;
}
