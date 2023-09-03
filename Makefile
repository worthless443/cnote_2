include dmenu/config.mk
CC=gcc
OBJ=vargs.o sort.o
LIB=libvargs.a
MAIN=note
FLAGS=-I./ -O3  -ggdb -Wall 
PREFIX=/usr/bin
#SRC_DIR=examples

all: $(OBJ) $(LIB) $(MAIN)
$(OBJ): %.o : %.c
	$(CC) -c ${FLAGS} $^ -o $@
$(LIB): $(OBJ)
	ar rcs $@ $^
$(MAIN): % : %.c
	$(CC) $^ $(LIB) ${FLAGS} ${INCS} ${LIBS} -o $@
install:
	cp ${MAIN} ${PREFIX}/${MAIN}
	
clean:
	rm -rf ${OBJ} ${LIB} ${MAIN}
