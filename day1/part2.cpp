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
			int module_fuel = mass(v);
			total += module_fuel;
			std::cout << "Module needed " << module_fuel << " fuel \n";
			while(module_fuel > 0)
			{
				int temp = mass(module_fuel);
				if (temp < 0){
					temp = 0;
				}
				std::cout << "temp = " << temp;
				total+=temp;
				module_fuel = temp;
			}
		}
		file.close();
	}
	else std::cout<< "Unable to open file";
	std::cout << "Total Fuel Required =" << total;
	return 0;
}
