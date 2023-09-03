#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int __read_line_sp(char **ptr, int *size, FILE *f) {
	int nl = 0, i = 1;
	char *tmp = malloc(sizeof(char) * 4095), *t = tmp;
	while(fread(t,1,1,f)) {
		if(*t == '\n') break;
		tmp = realloc(tmp,i);
		t = tmp + i;
		++i;
	}
	tmp[strlen(tmp) - 1] = '\0';
	*ptr = tmp;
	*size = i;
	if(0 == strcmp(tmp,"end")) return -1;
	return i - 1;
}

void readinput(FILE *f) {
	char *line = NULL;
	int i,isz=0,lsz=0;
	int len;
	for(i = 0;(len = __read_line_sp(&line,&lsz,f)) >= 0;++i) {
		printf("%d %s\n",len,line);
	}
} 
int main() {
	char *ptr = NULL;
	int size = 0;
 	FILE *f = convert_arg_fd("fuck\nyou\nfaggot\nend\n\0");
	readinput(f);
}
