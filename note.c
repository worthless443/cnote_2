#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

#include "config.h"
#include "def.h"
#include "vargs.h"

#define METAFILE "index.txt"
#define __DIR "/home/aissy/tmp/cache"

extern int PR_COMMAND,PR_ADD;
extern int arr[10];
extern int idx;
extern int ops[20];

struct MetaFile {
	int idx;
	char *name, *title;
};

int format_file(const char *str, char *buffer) {
	 sprintf(buffer,"## %s\n<text_begin>", str);
}
int write_file(const char *fn, char *content) {
	char format[strlen(content) + 10];
	format_file(content,format);
	char ffn[strlen(__DIR) + strlen(fn)];
	memset(ffn,'\0',strlen(__DIR) + strlen(fn));
	memcpy(ffn,__DIR,strlen(__DIR));
	strcat(ffn,"/");
	strcat(ffn, fn);
	printf("%s\n",ffn);
	FILE *f = fopen(ffn,"w");
	if(*(format + 3) >= 0x61) *(format + 3) -= 32;
	fprintf(f,format);
	fclose(f);
}

int format_filename(const char *str, char *buffer) {
	for(int i=0;i<strlen(str);++i) {
		if(str[i] == ' ') (buffer[i] = '_',++i);
		if(str[i] < 0x61) buffer[i] = str[i] + 32;
		else buffer[i] = str[i];
		for(int j=0;j<strlen(_BLOCKED_CHARS);++j)
			if(str[i] == _BLOCKED_CHARS[j])
				return 0 - i;
	}
	strcat(buffer,".md");
	return 1;
}

void handle_error_invalid_char(const char *str, int idx) {
	fprintf(stderr, "invalid string\n");
	fprintf(stderr, "%s\n", str);
	for(int i=idx;i<0;++i)
		printf("_");
	printf("^\n");
}

struct MetaFile *load_files(const char *dirname) {
	int i = 1;
	DIR *d = opendir(dirname);
	struct dirent *dir;
	struct MetaFile *files  = malloc(sizeof(struct MetaFile) * 4095);
	while((dir = readdir(d)) != NULL) {
		if(*dir->d_name != '.') {
			files = realloc(files,i*sizeof(struct MetaFile));
			files[i].idx = i;
			files[i].name = dir->d_name;
			i++;
		}
	}
	return files;
}

int open_files_dmenu(int flag) {
	int i;
	struct MetaFile *mf = load_files(__DIR);
	char *format = malloc(sizeof(char)*4095);
	memset(format,'\0',4095);
	for(i=1;mf[i].idx;++i) {
		strcat(format,mf[i].name);
		strcat(format,"\n");
	}
	strcat(format,"end\n\0");
	bring_menu_string(format, _NORM_MASK & flag);
	return 1;
}

int main(int argc, char **argv) {
	
	memset(arr,'\0',10);
	int op = 0;
	char **op_strings =  get_op_strings(100);
	setup(ops,op_strings, argc,argv);
	char *title = NULL;
	int ret;
	if(ops[OP_CL]) {
		system("rm -rf /home/aissy/tmp/cache/*");
	}
	if(ops[OP_R]) {
		open_files_dmenu(DELETE);
		return 0;
	}
	if(ops[OP_ADD]) {
		title = op_strings[OP_ADD];
		char buffer[strlen(title) + 10];
		memset(buffer,'\0',strlen(title) + 10);
		if((ret = format_filename(title, buffer)) <= 0) {
			handle_error_invalid_char(title,ret);
			return 1;
		}
		write_file(buffer, title);
	}
	if(ops[OP_EDIT])
		open_files_dmenu(WRITE);
	return 0;
}
