include Configuration.mk

LIBNAME = liborca-netclient.a
SRCDIR = src
BINDIR = bin
INCDIR = include

CXX := gcc
CXXFLAGS := -O3 -g -Wall -Wextra -Werror -I$(INCDIR) -std=c++17

TARGET_DEPS = \
	$(BINDIR)/UdpProto.o \
	$(BINDIR)/HermesProto.o \
	$(BINDIR)/HellfireProto.o

#compile all classes (.cpp) into %.o files
$(BINDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.h 
	$(CXX) $(CXXFLAGS) $< -o $@ -c 

#make application test
$(BINDIR)/testapp.exe: $(BINDIR)/$(LIBNAME) $(SRCDIR)/__testapp.cpp
	$(CXX) $(CXXFLAGS) $(SRCDIR)/__testapp.cpp -o $(BINDIR)/__testapp.exe -L$(BINDIR) -lorca-netclient 

#make application lib
$(BINDIR)/$(LIBNAME): $(TARGET_DEPS)
	ar rcs $(BINDIR)/$(LIBNAME) $(TARGET_DEPS)
	ar t $(BINDIR)/$(LIBNAME)

all: $(BINDIR)/$(LIBNAME)
	echo "done."

clean:
	rm -rf ./bin/*.exe ./bin/*.o ./bin/*.a
