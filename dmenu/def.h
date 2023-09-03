#define RDWR 1
#define WRITE 3
#define READ 4
#define DELETE 5
#define LOGOUT 8
#define ALL 7
#define EXIT 10
#define DO_NULL 255

#define _NORM_MASK (((1 << 1 | 1) << 1 | 1) << 1 | 1)
#define _MIN_MASK (((1 << 1 | 1) << 1 | 1) << 1 | 1) - 2
#define _REP_MASK 0b101010

int bring_menu_string(const char *,int);
