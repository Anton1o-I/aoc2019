#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Pair {
	std::string first;
	std::string second;
};

struct Object {
	int idx;
	std::string name;
	int Direct;
	std::vector< int > Indirect;
	std::vector< int > Next;
	void get_indirects();

	void add_indirect(Object direct);
};


Pair parse_line(std::string str){
	std::stringstream ss(str);
	std::string token;
	int i = 0;
	Pair p;
	char delim = ')';
	while(std::getline(ss, token, delim)){
		if (i==0){
			p.first = token;
		}
		else if (i==1){
			p.second = token;
		}
		i += 1;
	}
	return p;
}

std::vector< Pair > read_file(std::string address){
	std::string line;
	std::ifstream file;
	std::vector<Pair> input;

	file.open(address, std::ios::out);
	if (file.is_open())
	{
		while(std::getline(file, line))
		{
			Pair p = parse_line(line);
			input.push_back(p);
		}
		file.close();
	}
	return input;
}

struct ObjList{
	std::vector< Object > list;

	int find_object(std::string n);
	void add_objects(Pair p);
	int count_orbits();
	int add_new(std::string n);
	void update_indirects(Object o);
};

int ObjList::find_object(std::string n){
	for (int i = 0; i < list.size(); i++){
		if (list[i].name == n){
			return i;
		}
	}
	return -1;
}

int ObjList::add_new(std::string n){
	int idx = list.size();
	Object o;
	o.idx = idx;
	o.name = n;
	o.Direct = -1;
	list.push_back(o);
	return idx;
}

void ObjList::update_indirects(Object o){
	for(int i = 0; i < o.Next.size(); i++){
		Object t = list[o.Next[i]];
		t.add_indirect(o);
		list[t.idx] = t;
		if (t.Next.size() > 0){
			update_indirects(t);
		}
	}
}

void ObjList::add_objects(Pair p){
	int loc1 = find_object(p.first);
	int loc2 = find_object(p.second);
	Object o1, o2;
	if (loc1 == -1) 
	{
		int i1 = add_new(p.first);
		o1 = list[i1];
	}
	else
	{
		o1 = list[loc1];
	}
	if (loc2 == -1)
	{
		int i2 = add_new(p.second);
		o2 = list[i2];
	}
	else 
	{
		o2 = list[loc2];
	}
	o2.Direct = o1.idx;
	o1.Next.push_back(o2.idx);
	o2.add_indirect(o1);
	if (loc2 != -1){
		update_indirects(o2);
	}
	list[o1.idx] = o1;
	list[o2.idx] = o2;
}

void Object::add_indirect(Object o){
	Indirect.clear();
	if (o.Direct >= 0){
		Indirect.push_back(o.Direct);
	}
	for (int i = 0; i < o.Indirect.size(); i++){
		Indirect.push_back(o.Indirect[i]);
	}
}

int ObjList::count_orbits(){
	int count = 0;
	for(int i=0; i<list.size(); i++){
		count += list[i].Indirect.size();
		if (list[i].Direct >= 0){
			count +=1;
		}
	}
	return count;
}

int main(){
	std::vector<Pair> p = read_file("input.txt");
	ObjList obj;
	for(int i = 0; i < p.size(); i++){
		obj.add_objects(p[i]);
	}
	std::cout<<"TOTAL"<<obj.count_orbits();
}
