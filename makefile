tests: eda_file.o
	cd tests ; make

eda_file.o: eda_file.cc eda_file.h
	g++ -c eda_file.cc

clean:
	rm *.o ; cd tests ; rm *.o