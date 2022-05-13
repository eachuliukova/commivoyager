all: Source.cpp
	g++ -o test.exe -std=c++20 Source.cpp 
clean:
	rm *.o