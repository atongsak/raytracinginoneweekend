CC=g++ -std=c++11 
exe_file=main
output_file=image.ppm

$(exe_file): main.cc 
	$(CC) main.cc -o $(exe_file)
	./$(exe_file) > $(output_file)

clean: 
	rm -f *.o $(exe_file) $(output_file)