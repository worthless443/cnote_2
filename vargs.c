#include<dirent.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

#include "config.h"

struct Arg {
	char *forward_arg;
	int size;
};

extern void mergesort(int*,int);

int PR_COMMAND = 0, PR_ADD = 0;
int arr[15] = {0};
int idx = 0;
int ops[15] = {0};
int og[20] = {3};
int gidx = 0;
int opt_invalid;
int arg_opt_invalid;
int arg_whole_len;
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
int start_idx_argv = 1;
static int check_arg_span_concat(char **argv, char **out) {
	int size = 0,i;
	int tmp_len = 0; //strlen(argv[start_idx_argv + 1]);
	char *tmp_arg = NULL;
	char *arg = NULL;;
	// checking if there is a next arg
	if(argv[start_idx_argv + 1] == NULL) {
		return 0;
	}
	//printf("%s\n", argv[start_idx_argv + 1]);
	for(i=start_idx_argv + 1, 
			arg = malloc(sizeof(char) * strlen(argv[i]));
			(tmp_arg = argv[i])!=NULL
			;++i) {
		if(*tmp_arg == '-' && strlen(arg) == 0)  {
			*out = NULL;
			return 0;
		}
		if(*tmp_arg == '-') {
			*out = arg;
			return size;
		}

		if(tmp_len)
			arg = realloc(arg, (tmp_len*(sizeof(char) + 1))); // insufficient storage (somehow :( )
		memcpy(arg + tmp_len, tmp_arg, strlen(tmp_arg));
	 	tmp_len += strlen(tmp_arg) + 1;
		if(argv[i + 1])
			memcpy(arg + tmp_len -1 ," ", 1);
		size+=1;
		arg_whole_len  = tmp_len;
			
	}
	*out = arg;
	return size;
}

// TODO: upsize heap char buffer and fit everything in it
// TODO: create a heap object unique to each set BUG: overwriting prev-
// argument 
// reapeating three times
int eval_arg_body_2(int *options,char **output, char **arg, int *i) {
	int j = -1;
	char *_arg = malloc(sizeof(char));
	char *tmp_arg = NULL;
	//printf("argv[i] = %s\n", arg[*i]);
	int arg_len = strlen(arg[*i] + global_i);
	int forward_len = check_arg_span_concat(arg,&tmp_arg);
	if(forward_len == 0 && arg_len == 0)  {
		free(_arg);
		free(tmp_arg);
		return 0;
	}

	int op_opt;
	if((_arg = realloc (_arg, (arg_len + arg_whole_len) 
					* sizeof(char))) == NULL)
			die("realloc() failed or no args given");


	memcpy(_arg, arg[*i] + global_i, arg_len);
	if (forward_len > 0) {
		_arg[arg_len] = ' ';
		memcpy(_arg + arg_len + 
				(!arg_len ? 0 : 1),
			tmp_arg,strlen(tmp_arg));

	}
	*i += forward_len;
	start_idx_argv = *i + 1;		
	//
	//printf("%s\n", _arg);

	// TODO: fix _sorting_ of `options`
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
	//printf("%d\n", *tmp_arg);
	memcpy(output[op_opt],_arg,strlen(_arg));
	if(_arg) {
		free(_arg);
	}
	if(tmp_arg)
		free(tmp_arg);
	// FIXME fix sorting
	//printf("(dd) %d\n", op_opt);
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
				die("expecting argument to %s", argv[i]);
}

// DEPRECATED
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


