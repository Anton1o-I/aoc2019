#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>


int main(){
	std::string line;
	std::ifstream file;
	std::vector<int> values;

	file.open("values.txt", std::ios::out);
	if (file.is_open())
	{
		while(std::getline(file, line, ','))
		{
			values.push_back(std::stoi(line));
		}
		file.close();
	}
	values[1]=12;
	values[2]=2;
	int idx = 0;
	int vec_len = values.size();
	while (idx < vec_len)
	{
		int dir = values[idx];
		int pos1 = values[idx+1];
		int pos2 = values[idx+2];
		int changed = values[idx+3];
		switch (dir)
		{
			case 1:
				values[changed] = values[pos1] + values[pos2];
				break;
			case 2:
				values[changed] = values[pos1] * values[pos2];
				break;
			case 99:
				idx = vec_len;
				break;
			default:
				std::cout << "Unknown directive: " << dir;
				idx = vec_len;
				break;
		}
		idx += 4;
	}
	std::cout << "POSITION 0 = " << values[0];
}
