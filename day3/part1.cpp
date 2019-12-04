#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

std::vector<std::string> tokenize(std::string s){
	std::vector<std::string> out;
	std::stringstream check1(s);
	std::string temp;

	while(getline(check1, temp, ',')){
		out.push_back(temp);
	}

	return out;
}

struct Directions{
	std::string wire1;
	std::string wire2;
};

struct Wire{
	std::vector< std::string > directions;
	std::pair<int, int> current;
	std::vector< std::pair<int,int> > locations;

	Wire(std::string s){
		directions = tokenize(s);
		current = std::make_pair(0,0);
	};
	void parse_directions();
};

void Wire::parse_directions(){
	for (int i = 0; i < directions.size() ; i++){
		std::string dir = directions[i];
		std::string move = dir.substr(1);
		int num = std::stoi(move);
		switch (dir[0])
		{
		case 'U':
			current.second += num;
			break;
		case 'D':
			current.second -= num;
			break;
		case 'R':
			current.first += num;
			break;
		case 'L':
			current.first -= num;
			break;
		default:
			std::cout << "UNKNOWN DIRECTION TYPE" << dir[0];
			break;
		}
		locations.push_back(current);
	}
};

struct Intersections{
	std::vector< std::pair<int,int> > points;

	public:
		int find_closest();
};

struct Pair{
	Wire wire1;
	Wire wire2;

	public:
		Intersections find_intersections();
};

Directions read_file(std::string address){
	std::string line;
	std::ifstream file;
	Directions d;
	int ticker = 0;

	file.open(address, std::ios::out);
	if (file.is_open())
	{
		while(std::getline(file, line))
		{
			if (ticker == 0){
				d.wire1 = line;
			}
			else d.wire2 = line;
			ticker += 1;
		}
		file.close();
	}
	return d;
}

bool on_segment(std::pair<int,int> comp, std::pair<int,int> a, std::pair<int,int> b){
	if(
			std::min(a.first, b.first)<=comp.first &&
			std::max(a.first, b.first)>=comp.first &&
			std::min(a.second, b.second)<=comp.second &&
			std::max(a.second, b.second)>=comp.second
	){
		return true;
	}
	return false;
}

Intersections Pair::find_intersections(){
	Intersections inter;
	for(int i = 0; i<(wire1.locations.size()-1);i++){
		std::pair<int, int> a = wire1.locations[i];
		std::pair<int, int> b = wire1.locations[i+1];
		for(int j = 0; j<(wire2.locations.size()-1);j++){
			std::pair<int, int> c = wire2.locations[j];
			std::pair<int, int> d = wire2.locations[j+1];

			int a1 = b.second - a.second;
			int b1 = a.first - b.first;
			int c1 = a1 * a.first + b1 * a.second;
			int a2 = d.second - c.second;
			int b2 = c.first - d.first;
			int c2 = a2 *c.first + b2*c.second;

			int determinant = (a1 * b2) - (a2 * b1);
			if (determinant == 0)
			{
				continue;
			}
			else
			{
				int x = (b2*c1 - b1*c2)/determinant;
				int y = (a1*c2 - a2*c1)/determinant;
				std::pair<int,int> new_pair = std::make_pair(x,y);
				if(on_segment(new_pair,a,b) && on_segment(new_pair, c,d))
				{
					inter.points.push_back(new_pair);
				}
			}
		}
	}
	return inter;
}

int Intersections::find_closest(){
	int smallest_dist = 100000000;
	for (int i=0; i<points.size(); i++){
		int dist = std::abs(points[i].first)+std::abs(points[i].second);
		if (dist < smallest_dist){
			smallest_dist = dist;

		}
	}
	return smallest_dist;
}


int main(){
	Directions d = read_file("values.txt");
	Wire w1 = Wire(d.wire1);
	Wire w2 = Wire(d.wire2);
	w2.parse_directions();
	w1.parse_directions();
	Pair pair = {w1, w2};
	Intersections i = pair.find_intersections();
	int answer = i.find_closest();
	std::cout<<"ANSWER IS:"<<answer;
}
