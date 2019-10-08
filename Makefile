CXX_INCLUDES = -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g

OPENCV_FLAGS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs

MIS_FLAGS = -lpthread -lcurl

CFLAGS = -Wall -std=c++14 -Iinclude -c -o

all:
	g++ $(CFLAGS) objFiles/main.o src/main.cpp $(CXX_INCLUDES)
	g++ $(CFLAGS) objFiles/listings.o src/listings.cpp
	g++ -Wall -std=c++14 -o main objFiles/main.o objFiles/listings.o $(MIS_FLAGS) $(OPENCV_FLAGS)
	@echo "\nThe project has been successfully compiled"

clean:
	rm main
	rm objFiles/main.o
	rm objFiles/listings.o
