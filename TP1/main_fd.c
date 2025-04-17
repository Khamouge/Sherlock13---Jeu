#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void mycp(void){

	char original[100] = "./Reponses_TP1_OS_BOU_ALI_Aymen.txt";
	char copie[100] = "./TestdeMycp.txt";
	
	int fd_original = open(original, O_RDONLY);
	int fd_copie = open(copie, O_WRONLY);
	
	int length = 4096;
	char buffer[4096];
	int num_read;
	int num_write;
	
	while(num_read != 0){
		num_read = read(fd_original, buffer, length);
		num_write = write(fd_copie, buffer, length);
		
		while(num_read != num_write){
			num_read = read(fd_original, buffer, length);
		}
	}
	
	close(original);
	close(copie);
	return;
}

int main(){

	mycp();
	return 0;
}












