#include<stdio.h>
#include<string.h>
int main(){
	char string[50] = "/tmp/abc/x/y/hello.txt";
	char *token = strtok(string, "/");
	char sep[10][10];
	int l=0, i=1;
	//l = strlen(token);
	//token[l] = '\0';
	//strcpy(sep[0], token);
	while(token != NULL){
		printf("%s length = %ld\n", token, strlen(token));
		l = strlen(token);
		token[l] = '\0';
		strcpy(sep[i], token);
		printf("%s new length = %ld\n", sep[i], strlen(sep[i]));
		i++;
		token = strtok(NULL, "/");
	}

	return 0;
}