#
# Makefile for DFM
#
CXX				= g++
CFLAGS			= -Wall -O3 -std=c++11 
########################### 
# Set your own gurobi lib and headers. 
# These will be changed by TA
LFLAGS          = -L../lib
INCLUDES        = -I../include
############################ 
# Change -lgurobi81 to your version (IF you download the latest Gurobi version(8.1.1) it will be -lgurobi81)
LIBS			= -lgurobi_c++ -lgurobi81
SOURCES			= main.cpp
OBJECTS			= $(SOURCES:.cpp=.o)
EXECUTABLE		= choose

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo "===> Building $@ ..."
	$(CXX) $(CFLAGS) $(INCLUDES) $(OBJECTS) -o $@ $(LFLAGS) $(LIBS)
	@echo "===> $@ built successfully ..."
	@echo ""

%.o:  %.cpp
	@echo "===> Compiling $< ..."
	$(CXX) $(CFLAGS) $(INCLUDES) -c $< -o $@ 
	@echo "===> $< compiled successfully ..."
	@echo ""
	
clean:
	@echo "===> Deleting *.o $(EXECUTABLE) ..."
	rm -rf *.o $(EXECUTABLE)
	@echo "===> *.o $(EXECUTABLE) deleted successfully ..."
	@echo ""

run: $(EXECUTABLE)
	./$(EXECUTABLE) | tee log.txt
