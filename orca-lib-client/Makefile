include ../../Configuration.mk

# issue an error if trying to compile with an incompatible platform.
# orca-mpsoc has NoC defines used by this software
ifneq ($(ORCA_PLATFORM), orca-mpsoc)
$(error ERROR: ORCA_PLATFORM = $(ORCA_PLATFORM) is not compatible with orca-lib-client)
endif

LIBNAME = liborca-client.a
SRCDIR  = ./src
BINDIR  = ./bin
INCDIR  = ./include

CPPFLAGS = -O3 -g -Wall -Wextra -Werror -I$(INCDIR) -c $(COMPLINE)

TRGTDEPS = \
	$(BINDIR)/orca-network-base.o \
	$(BINDIR)/orca-udp-client-server.o

#compile all classes (.cpp) into %.o files
$(BINDIR)/%.o: $(SRCDIR)/%.cpp 
	g++ $(CPPFLAGS) $< -o $@ -c 

#make application executable
$(BINDIR)/$(LIBNAME): $(TRGTDEPS)
	ar rcs $(BINDIR)/$(LIBNAME) $(TRGTDEPS)

all: $(BINDIR)/$(LIBNAME)
	echo "done."

clean:
	rm -rf ./bin/*.exe ./bin/*.o
