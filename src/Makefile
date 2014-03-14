# --------------------------------------------------
# if there is no Makefile.in then use the template
# --------------------------------------------------
ifneq ($(strip $(MAKEFILE_IN)),)
# use value of MAKEFILE_IN if provided on the command line
else ifeq ($(shell test -e Makefile.in && echo 1), 1)
MAKEFILE_IN = Makefile.in
else
MAKEFILE_IN = Makefile.in.template
endif
include $(MAKEFILE_IN)

LUA = $(LUA_HOME)/bin/lua
B2C = ../share/bin2c.lua

INC += -I$(LUA_HOME)/include
LIB += -L$(LUA_HOME)/lib -llua

ifeq ($(strip $(HAVE_HDF5)), 1)
INC += -I$(HDF5_HOME)/include
LIB += -L$(HDF5_HOME)/lib -lhdf5
endif

ZULU_INC = $(addprefix ../include/, $(wildcard *.h))
ZULU_LIB = ../lib/libzulu.a
ZULU_EXE = ../bin/zulu
ZULU_OBJ = \
	30log.o \
	ansicolors.o \
	argparse.o \
	array.o \
	buffer.o \
	class.o \
	serpent.o \
	struct.o \
	H5.o \
	hdf5.o \
	json.o \
	zulu.o

install : $(ZULU_EXE) $(ZULU_LIB) $(ZULU_INC)

show :
	@echo "MAKEFILE_IN=$(MAKEFILE_IN)"
	@echo "HAVE_MPI=$(HAVE_MPI)"
	@echo "HAVE_HDF5=$(HAVE_HDF5)"
	@echo "ZULU_INC=$(ZULU_INC)"
	@echo "ZULU_LIB=$(ZULU_LIB)"
	@echo "ZULU_EXE=$(ZULU_EXE)"
	@echo "INC=$(INC)"
	@echo "LIB=$(LIB)"

$(ZULU_LIB) : $(ZULU_OBJ)
	$(AR) $@ $?
	$(RANLIB) $@

$(ZULU_EXE) : main.o $(ZULU_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

../include/%.h : %.h
	cp $< $@

%.c : %.lua
	$(LUA) $(B2C) +$< > $@

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $< $(INC) -c

clean :
	$(RM) $(ZULU_INC) $(ZULU_LIB) $(ZULU_EXE) $(ZULU_OBJ)

struct.o : struct.h

.PHONY: install
