LIBS = -lGL -lglfw -lGLEW -lIL
SONAME = libepic.so

all:
	cd Graphics; make
	cd Events; make
	cd Math; make
	
	g++ -shared -o $(SONAME) Graphics/epicGraphics.o Events/epicEvents.o Math/epicMath.o $(LIBS)

clean:
	cd Graphics; make clean
	cd Events; make clean
	cd Math; make clean
	
	rm $(SONAME)
