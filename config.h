#define _BLOCKED_CHARS "|() *&^%$#@!~"
#define _SUP_STRING "bdev"
#define _SUP_STRING_BODY "~acrx"
#define _ARG_MAP(arg) (arg == 'e') ? OP_EDIT : (arg == '')
#define __DIR "tmp/cache"
// change this enum according to required parameters for the program
enum OP {
	OP_B,
	OP_D,
	OP_EDIT,
	OP_ARG,
	OP_READ,
	OP_BAR, // mandatory field
	OP_ADD,
	OP_COMMAND,
	OP_R,
	OP_CL
};

#define MAX_ARGN 20
