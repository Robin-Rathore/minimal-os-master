CFLAGS := $(CFLAGS) -I../include/

all : shell.o

OBJ = shell.o

shell.o: $(OBJ)
		@$(LD) -r $(OBJ) -o $@
		@echo LD shell.o

%.o: %.c
		@${CC} -o $@ ${CFLAGS} -c $<
		@echo CC $<
