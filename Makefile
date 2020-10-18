VPATH = ../src
CFLAGS  += -c -I. -I../src -I/mingw64/x86_64-w64-mingw32/include -g
LDFLAGS += -L/mingw64/x86_64-w64-mingw32/lib -lgdiplus -lgdi32 -static -mwindows

default: world 

main: winMain.o plot.o main.o
	g++ -o $@ winMain.o plot.o main.o ${LDFLAGS} 

world: winMain.o plot.o world.o geometry.h
	g++ -o $@ winMain.o plot.o world.o ${LDFLAGS}

cube: winMain.o plot.o cube.o
	g++ -o cube.exe winMain.o plot.o cube.o ${LDFLAGS}

rotatingCube: winMain.o plot.o rotatingCube.o
	g++ -o rotatingCube.exe winMain.o plot.o rotatingCube.o ${LDFLAGS}

winMain.o: winMain.cc
	g++ $(CFLAGS) $< 

plot.o: plot.cc types.h
	g++ $(CFLAGS) $< 

main.o: main.cc
	g++ $(CFLAGS) $< 

world.o: world.cc 
	g++ $(CFLAGS) $< 

cube.o: cube.cc
	g++ $(CFLAGS) $< 

rotatingCube.o: rotatingCube.cc
	g++ $(CFLAGS) $< 

cubeTest.o: cubeTest.cc
	g++ $(CFLAGS) $< 

clean:
	rm -rf *.o *.exe

