#pragma once
#include <string>
#include <iostream>
#include <sstream>

class Package{
public:
	virtual std::string to_string(){ return std::string("default"); }
};

class PlacePiecePackage : public Package{
public:
	int position_x;
	int position_y;
	int value;
	PlacePiecePackage(int x, int y, int v) : position_x(x), position_y(y), value(v){}
	std::string to_string(){
		std::stringstream ss;
        ss << "Position X: " << position_x << ", Position Y: " << position_y << ", Value: " << value;
        return ss.str();
	}
};





class DecodePackage{
public:
	DecodePackage(){};
	DecodePackage(void* data, int data_length){
		int* int_data = (int*) data;
		class_number = int_data[0];
		std::cout << int_data[1] << int_data[2] << int_data[3] << std::endl;
		content = (void*)(int_data+1);
	}
	int class_number;
	void* content;
};


Package* packageDecoder(void* data){
	int class_number( *(int*)data );
	char* content(((char*)data)+4);
	Package* test = (PlacePiecePackage*)content;
	test->to_string();
	if (class_number == 0){ return test; }
	else { return nullptr; }
}


class EncodedPackage{
public:
	int class_number;
	Package package;
	EncodedPackage(int class_number, Package package) : class_number(class_number), package(package){}
};

