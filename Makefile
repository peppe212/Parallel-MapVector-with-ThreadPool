# REALIZZATO DA GIUSEPPE MUSCHETTA
# LAUREANDO MAGISTRALE INFORMATICA

CXX			= g++
CXXFLAGS 	= -Wall -Wextra -Wpedantic -std=c++20 -O3

LDFLAGS 	= -pthread
OPTFLAGS	= -O3 -finline-functions -DNDEBUG

TARGET 		=   main

.PHONY: all clean cleanall
.SUFFIXES: .cpp


%: %.cpp
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) -o $@ $< $(LDFLAGS)

all			: $(TARGET)

clean		:
	rm -f $(TARGET)
cleanall	: clean
	\rm -f *.o *~