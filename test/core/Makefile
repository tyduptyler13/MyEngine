
FLAGS = --std=c++14 -g -I ../../include $(CFLAGS)
LIBS = -lGL -lSDL2 -Wl,-rpath ./ -L./ -lMyEngine

BasicExample: BasicExample.cpp ../../libMyEngine.so
	$(MAKE) -C ../../
	$(CXX) $(FLAGS) BasicExample.cpp $(LIBS) -o BasicExample

clean:
	$(MAKE) clean -C ../../
	rm BasicExample
