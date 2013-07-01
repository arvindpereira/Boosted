# Author: Arvind Pereira

# Modify the includes, sources, headers, executable and
# other flags accordingly!
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

STORE_SIM_SRCS = SimulateStore.cpp
STORE_SIM_HDRS = Store.h Inventory.h Item.h Cashier.h Customer.h PaymentProcessor.h
STORE_SIM_OBJS = $(STORE_SIM_SRCS:.cpp=.o)

#MPI2_SRCS = boost-mpi-test.cpp
#MPI2_HDRS = 
#MPI2_OBJS = $(MPI2_SRCS:.cpp=.o)

STORE_SIM = store-simulator
#MPI2_EXE = boost-mpi-test

TOPLEVEL_FOLDER = StoreSimulator

all: $(STORE_SIM)
	@echo "-------------------------Done------------------------------"
	@echo "Type: 'make docs' for documentation"
	@echo "Type: 'make tar'  for a code snapshot to be saved to tar.gz"

$(STORE_SIM):	$(STORE_SIM_OBJS) $(STORE_SIM_HDRS)
	$(CC) -o $(STORE_SIM) $(STORE_SIM_OBJS) $(LFLAGS) $(BOOST_THREAD_LIBS) $(JSON_SPIRIT_LIBS)
	
.cpp.o: $(INCLUDES)
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

tar:
	make clean; cd ..; tar -czf $(TAR_FILE_NAME) $(TOPLEVEL_FOLDER)
	cd ..; mv *$(TOPLEVEL_FOLDER).tar.gz $(TOPLEVEL_FOLDER)/

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
	rm -rf docs
	
TAR_FILE_NAME = `date +'%Y%m%d_%H%M%S_'`$(TOPLEVEL_FOLDER).tar.gz

OBJ = $(STORE_SIM_OBJS)
EXE = $(STORE_SIM)

SOURCES = $(STORE_SIM_SRCS)
HEADERS = $(STORE_SIM_HDRS)

docs: $(SOURCES) $(HEADERS)
	if command -v doxygen; then doxygen Doxyfile; \
	else echo "Doxygen not found. Not making documentation."; fi

