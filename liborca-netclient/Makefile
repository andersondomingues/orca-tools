include Configuration.mk

LIBNAME = liborca-netclient.a
SRCDIR = src
BINDIR = bin
INCDIR = include
TSTDIR = test

CXX := g++
CXXFLAGS := -O3 -g -Wall -Wextra -Werror -I$(INCDIR) -std=c++17

TARGET_DEPS = \
	$(BINDIR)/UdpProto.o \
	$(BINDIR)/HermesProto.o \
	$(BINDIR)/HellfireProto.o \
	$(BINDIR)/Utils.o

#compile all classes (.cpp) into %.o files
$(BINDIR)/%.o: $(SRCDIR)/%.cpp $(INCDIR)/%.h 
	$(CXX) $(CXXFLAGS) $< -o $@ -c 

test: $(BINDIR)/__testapp.exe
	./$(BINDIR)/__testapp.exe

#make application test
$(BINDIR)/__testapp.exe: $(BINDIR)/$(LIBNAME) $(TSTDIR)/__testapp.cpp
	$(CXX) $(CXXFLAGS) $(TSTDIR)/__testapp.cpp -o $(BINDIR)/__testapp.exe -L$(BINDIR) -lorca-netclient 

#make application lib
$(BINDIR)/$(LIBNAME): $(TARGET_DEPS)
	ar rcs $(BINDIR)/$(LIBNAME) $(TARGET_DEPS)
	ar t $(BINDIR)/$(LIBNAME)

all: $(BINDIR)/$(LIBNAME)
	echo "done."

clean:
	rm -rf ./bin/*.exe ./bin/*.o ./bin/*.a
