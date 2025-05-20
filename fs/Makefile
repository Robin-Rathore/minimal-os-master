CFLAGS := $(CFLAGS) -I../include/

all : fs.o

OBJ = filesystem.o

fs.o: $(OBJ)
		@$(LD) -r $(OBJ) -o $@
		@echo LD fs.o

%.o: %.c
		@${CC} -o $@ ${CFLAGS} -c $<
		@echo CC $<
