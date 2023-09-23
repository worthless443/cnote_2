#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

#include "config.h"

extern void mergesort(int*,int);

int PR_COMMAND = 0, PR_ADD = 0;
int arr[15] = {0};
int idx = 0;
int ops[15] = {0};
int og[20] = {3};
int gidx = 0;
int opt_invalid;
int arg_opt_invalid;
char *opt_invalid_opt = NULL;

int not_in_opt(char arg, char *string) {
 	for(int i=0;i<strlen(string);++i)	
		if(arg == string[i]) 
			return 0;
	return 1;
}

extern int die(const char *format, ...);

int global_i = 2;
int eval_arguments(char *raw_arg, int *output) {

	arg_opt_invalid = 0;
	opt_invalid = 0;
	int is_whole, invalid_one;
	char *arg = raw_arg  + 1;

	char catopt[strlen(_SUP_STRING) + strlen(_SUP_STRING_BODY) + 1];
	memset(catopt, '\0', strlen(_SUP_STRING) + strlen(_SUP_STRING_BODY) + 1);
  	strcat(catopt, _SUP_STRING);
	strcat(catopt, _SUP_STRING_BODY + 1); 

	if(*raw_arg != '-') return 2;
	if((is_whole = strlen(arg) > 1)) {
		global_i = 1;
		for(int i=0;i<strlen(arg);++i) {
			++global_i;
			if(not_in_opt(arg[i], catopt)) {
				opt_invalid = 1;
				opt_invalid_opt = malloc(sizeof(char));
				*opt_invalid_opt = arg[i];
			}
			
			for(int j=0;j<strlen(_SUP_STRING);++j) {
				if(arg[i] == _SUP_STRING[j]) {
					output[j] = 1;
				}
			}
			for(int j=1;j<strlen(_SUP_STRING_BODY);++j) {
				if(arg[i] == _SUP_STRING_BODY[j]) {
					output[OP_BAR + j] = 1;
					return 1;
				}
			}
		}
	}

	else {
		if(not_in_opt(*arg, catopt)) {
			opt_invalid = 1;	
			opt_invalid_opt = malloc(sizeof(char));
			opt_invalid_opt = (char*)(((long int)opt_invalid_opt + 3) & -4);
			*opt_invalid_opt = *arg;
		}
		for(int j=0;j<strlen(_SUP_STRING);++j) {
			if(*arg == _SUP_STRING[j]) 
				output[j] = 1;
		}

		for(int j=1;j<strlen(_SUP_STRING_BODY);++j) {
			if(*arg == _SUP_STRING_BODY[j]) {
				output[OP_BAR + j] = 1;
				return 1;
			}
		}
	}
	return 0;
}

int ii = 0;

int eval_arg_body_2(int *options,char **output, char **arg, int *i) {
	int j = -1;
	char *_arg = arg[*i] + global_i;
	//printf("%d\n", global_i);
	// NOT NULL terminated arg array
	int op_opt;
	if(strlen(_arg) == 0) {
		if(!(_arg = arg[++*i]))
			return 0;
	}
	//printf("%s\n", _arg);

	int _options[10] = {0};
#ifndef MAX_ARGN 
	for(int _i = 0; _i < 15; ++_i)   {
#else
	for(int _i = 0; _i < MAX_ARGN; ++_i)   {
#endif 
		if(options[_i])   {
			_options[++j] = _i;
		}
	}
	
	for(int _i = 0;_options[_i];++_i) {
		if(_options[_i] != og[_i]) {
			op_opt = _options[_i];
			break;
		}
		else op_opt = 0;
	}
	memcpy(output[op_opt],_arg,strlen(_arg));
	printf("(dd) %d\n", op_opt);
	//memcpy(og, _options, 15);

	// for(int _i=0;_i<j;++_i) {
	// 	if(options[op_opt] ) {
	// 		if(strlen(_arg) > 1)  {
	// 			for(int j=0;j<strlen(_arg);++j) {
	// 				output[op_opt][j] = _arg[j];
	// 			}
	// 		}
	// 		else  {
	// 			output[op_opt] = arg[++*i];
	// 		}
	// 		arr[op_opt]+=1;
	// 	 }
	// }	

	//printf("end of nigger\n");
	return 1;
}

char **get_op_strings(int size) {
	char **op_strings = malloc(sizeof(char*) * size);
	for(int i=0;i<20;++i)
		op_strings[i] = malloc(sizeof(char) * size);
	return op_strings;
}


void setup(int *ops, char **op_strings, int argc,char **argv) {
	for(int i=1,j = OP_BAR + 1;i<argc;++i,++j)  
		if(eval_arguments(argv[i], ops)) 
			if(!eval_arg_body_2(ops,op_strings,argv,&i))
				die("expecting argument to %s", argv[i - 1]);
}

void eval_arg_body(int *options,char **output, char **arg, int *i) {
	char *_arg = arg[*i] + global_i;
	if(options[OP_COMMAND] && !PR_COMMAND) {
		if(strlen(_arg) > 1)  {
			for(int j=0;j<strlen(_arg);++j) {
				output[OP_COMMAND][j] = _arg[j];
			}
		}
		else 
			output[OP_COMMAND] = arg[++*i];
		++PR_COMMAND;
	}
	if(options[OP_ADD] && !PR_ADD) {
		if(strlen(_arg) > 1)  {
			for(int j=0;j<strlen(_arg);++j) {
				output[OP_ADD][j] =  _arg[j];
			}
		}
		else {
			output[OP_ADD] = arg[++*i];
		}
		++PR_ADD;
	}
	
}


