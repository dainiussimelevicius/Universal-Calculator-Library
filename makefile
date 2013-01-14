calculator_tester: calculator_tester.o libuniversal_calculator.so
	gcc calculator_tester.o -Wl,-rpath,. -L. -luniversal_calculator -o calculator_tester

libuniversal_calculator.so: universal_calculator.o utils.o
	gcc -shared universal_calculator.o utils.o -lm -o libuniversal_calculator.so

calculator_tester.o: calculator_tester.c universal_calculator.h universal_bio_info.h
	gcc -c calculator_tester.c -o calculator_tester.o

universal_calculator.o: universal_calculator.c universal_calculator.h universal_bio_info.h constants.h
	gcc -c -fPIC universal_calculator.c -o universal_calculator.o

utils.o: utils.c utils.h
	gcc -c -fPIC utils.c -o utils.o

clean:
	rm libuniversal_calculator.so universal_calculator.o utils.o calculator_tester.o calculator_tester
