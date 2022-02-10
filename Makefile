
.PHONY: all
all: directories bin makelib makebins

LDFLAGS += -L./lib
include /usr/local/etc/Makefile-Base

SHOWLIBNAME=show${MACAPPEND}

LIBNAME=gittool${MACAPPEND}
LIB=lib/libgittool${MACAPPEND}.a
LIB_ARGS= -cvrU
ifeq ($(UNAME), Darwin)
	LIB_ARGS= -cvr
endif

#--------------------------------------------------
# What goes into making the library.
#--------------------------------------------------
LIB_NOSORT := $(wildcard ${SRCDIR}/*.cpp)
LIB_SRC := $(sort ${LIB_NOSORT})
LIB_OBJ := $(patsubst ${SRCDIR}/%.cpp,${OBJDIR}/%.o,${LIB_SRC})

VPATH := ${SRCDIR}:programs
INCLUDES += -I. -I./src

LIBS += -l${LIBNAME}
LIBS += -l${SHOWLIBNAME}
LIBS += -L/usr/local/opt/openssl/lib
LIBS += -lcurlpp -lcurl -lssh2 -lssl -lz -lssh2 -lcrypto
LIBS += -lpthread -lstdc++ -lm -ldl
ifeq ($(UNAME), Darwin)
	LIBS += -framework CoreFoundation -framework Security
endif

LDFLAGS += ${LIBS}

# Where we'll install things.
INSTALL_BASE=/usr/local

echo:
	@echo LIB_SRC: ${LIB_SRC}

#======================================================================
# Top-level targets.
#======================================================================
Makefile: ;

# Clean the contents of the subdirs.
.PHONY: clean
clean:
	rm -f ${DEPDIR}/* ${OBJDIR}/* ${LIB}

directories: ${DEPDIR}

#======================================================================
# Making the library
#======================================================================
makelib:
	@$(MAKE) ${THREADING_ARG} --output-sync=target --no-print-directory lib

lib: ${LIB}

${LIB}: ${LIB_OBJ}
	@mkdir -p lib
	ar ${LIB_ARGS} ${LIB} ${LIB_OBJ}
	ranlib ${LIB}


#======================================================================
# I have a small number of programs.
#======================================================================
makebins:
	@$(MAKE) ${THREADING_ARG} --output-sync=target --no-print-directory bins

bins: ${BINDIR}/GitTool

${BINDIR}/GitTool: ${OBJDIR}/GitTool.o ${LIB}
	$(CXX) ${OBJDIR}/GitTool.o ${LDFLAGS} $(OUTPUT_OPTION)

#======================================================================
# Installation.
#======================================================================
.PHONY: install
install: ${LIB} install_includes /usr/local/bin/GitTool
	cp -p ${LIB} ${INSTALL_BASE}/lib

.PHONY: install_includes
install_includes:
	@mkdir -p ${INSTALL_BASE}/include/gittools
	cp -p ${SRCDIR}/*.h ${INSTALL_BASE}/include/gittools

/usr/local/bin/GitTool: ${BINDIR}/GitTool
	cp ${BINDIR}/GitTool /usr/local/bin
