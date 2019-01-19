
DEPENDS = .depend

CXX = g++
CXXFLAGS = -O2 -Wall -MMD

# What we're generating

#SOURCE = game.cpp player.cpp main.cpp

OBJECTS = game.o player.o main.o

MAINFILE = main

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

# the stuff


$(MAINFILE):	$(OBJECTS)
		$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LIBRARIES)

.PHONY: clean
clean:
	rm -f $(OBJECTS) \
	touch $(DEPENDS)

.PHONY: distclean
distclean:
	rm -f $(OBJECTS) $(MAINFILE) \
	touch $(DEPENDS)
