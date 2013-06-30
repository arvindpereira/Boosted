# Author: Arvind Pereira

INCLUDES = -I ./ -I/usr/local/include -I/usr/include
LIBS = -L/usr/lib/ -L/usr/local/lib -lm -pthread -lpthread

BOOST_MPI_LIBS = -L/usr/local/lib -lboost_mpi -lboost_serialization
BOOST_THREAD_LIBS = -L/usr/local/lib -lboost_thread -lboost_system -lboost_exception
JSON_SPIRIT_LIBS = -L/usr/local/lib -ljson_spirit

CXX_MPI = mpic++
CC = g++
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CFLAGS = -Wall -c 
PTHREAD_FLAGS = -D_POSIX_PTHREAD_SEMANTICS
SHARED_FLAGS = -fPIC -shared

OKCUPID_SRCS = ProcessOkCupid.cpp
OKCUPID_HDRS = 
OKCUPID_OBJS = $(OKCUPID_SRCS:.cpp=.o)

OKCUPID_EXE = ReadOkCupidJsonData

TOPLEVEL_FOLDER = JsonParseExample

all: $(OKCUPID_EXE)
	@echo "-------------------------Done------------------------------"
	@echo "Type: 'make docs' for documentation"
	@echo "Type: 'make tar'  for a code snapshot to be saved to tar.gz"

$(OKCUPID_EXE):	$(OKCUPID_OBJS) $(OKCUPID_HDRS)
	$(CC) -o $(OKCUPID_EXE) $(OKCUPID_OBJS) $(LFLAGS) $(JSON_SPIRIT_LIBS)
	
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

tar:
	make clean; cd ..; tar -czf $(TAR_FILE_NAME) $(TOPLEVEL_FOLDER)
	cd ..; mv *$(TOPLEVEL_FOLDER).tar.gz $(TOPLEVEL_FOLDER)/

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
	rm -rf docs
	
TAR_FILE_NAME = `date +'%Y%m%d_%H%M%S_'`$(TOPLEVEL_FOLDER).tar.gz

OBJ = $(OKCUPID_OBJS)
EXE = $(OKCUPID_EXE)

SOURCES = $(OKCUPID_SRCS)
HEADERS = $(OKCUPID_HDRS)

docs: $(SOURCES) $(HEADERS)
	if command -v doxygen; then doxygen Doxyfile; \
	else echo "Doxygen not found. Not making documentation."; fi

