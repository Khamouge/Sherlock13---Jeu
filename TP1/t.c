#include <stdio.h>

int main(int argc,char **argv)
{
	char buffer[100];
	char nom[100];
	char prenom[100];
	int id;

	gets(buffer);
	sscanf(buffer,"%s %s %d",nom,prenom,&id);

	printf("%d %s %s\n",id,prenom,nom);
	exit(0);
}
