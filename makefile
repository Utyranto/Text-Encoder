all:
	g++ preprocess.cpp -o preprocess
	g++ encode.cpp -o encode
	g++ decode.cpp -o decode

clean : 
	rm preprocess encode -v 