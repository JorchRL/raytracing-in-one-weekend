
ImageRender: main.cpp
	g++ -o ./build/ImageRender main.cpp

render: 
	./build/ImageRender > ./build/img.ppm

clean: 
	rm -f ./build/ImageRender
	rm -f ./build/img.ppm

