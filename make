g++ -c sources/models.cpp -o bin/models.o
g++ -o bin/main sources/main.cpp bin/models.o

g++ -o bin/makeTest sources/makeTests.cpp
