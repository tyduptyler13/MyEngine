
FLAGS = --std=c++14 -g -I ../../include $(CFLAGS)
LIBS = -lGL -lSDL2 -Wl,-rpath ./ -L./ -lMyEngine

BasicExample: BasicExample.cpp
	$(MAKE) -C ../../
	$(CXX) $(FLAGS) BasicExample.cpp $(LIBS) -o BasicExample

clean:
	rm BasicExample
