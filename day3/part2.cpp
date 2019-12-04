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

struct CoordSum{
	int first;
	int second;
	int steps;

	public:
		CoordSum(int x, int y, int s = 0){
			first = x;
			second = y;
			steps = s;
		};
};

struct Directions{
	std::string wire1;
	std::string wire2;
};

struct Wire{
	std::vector< std::string > directions;
	CoordSum current;
	std::vector< CoordSum > locations;

	Wire(std::string s) : current(0,0) {
		directions = tokenize(s);
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
		current.steps += num;
		locations.push_back(current);
	}
};

struct Intersections{
	std::vector< CoordSum > points;

	public:
		int find_closest();
		int find_shortest();
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

bool on_segment(CoordSum comp, CoordSum a, CoordSum b){
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
		CoordSum a = wire1.locations[i];
		CoordSum b = wire1.locations[i+1];
		for(int j = 0; j<(wire2.locations.size()-1);j++){
			CoordSum c = wire2.locations[j];
			CoordSum d = wire2.locations[j+1];

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

				int steps_1 = a.steps + std::abs(a.first-x)+std::abs(a.second-y);
				int steps_2 = c.steps + std::abs(c.first-x)+std::abs(c.second-y);

				CoordSum new_pair = CoordSum(x,y, (steps_1+steps_2));
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

int Intersections::find_shortest(){
	int shortest = INT_MAX;
	for (int i=0; i<points.size(); i++){
		if (points[i].steps <shortest) {
			shortest = points[i].steps;
		}
	}
	return shortest;
}


int main(){
	Directions d = read_file("values.txt");
	Wire w1 = Wire(d.wire1);
	Wire w2 = Wire(d.wire2);
	w2.parse_directions();
	w1.parse_directions();
	Pair pair = {w1, w2};
	Intersections i = pair.find_intersections();
	int answer1 = i.find_closest();
	std::cout<<"ANSWER IS:"<<answer1;
	int answer2 = i.find_shortest();
	std::cout<<"ANSWER2 IS:"<<answer2;
}
