#include <iostream>
#include <cmath>
#include <fstream>
#include <string>


int mass(int m){
	return floor(m/3)-2;
}


int main(){
	std::string line;
	std::ifstream file;
	int total=0;
	file.open("values.txt", std::ios::out);
	if (file.is_open())
	{
		while(std::getline(file, line))
		{
			int v = std::stoi(line);
			total += floor(v/3)-2;
		}
		file.close();
	}
	else std::cout<< "Unable to open file";
	std::cout<<total;
	return 0;
}
