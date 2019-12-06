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
	std::string name;
	Object *Direct;
	std::vector< Object *> Indirect;
	std::vector< Object *> Next;
	void get_indirects();

	Object():Indirect(),Next(){};
};

Object new_object(std::string obj, Object * direct = NULL){
	Object o;
	if (direct == NULL){
		o.name = obj;
	}
	else {
		o.name = obj;
		o.Direct = direct;
		o.Indirect.push_back(direct->Direct);
		for(int i=0; i<direct->Indirect.size(); i++){
			o.Indirect.push_back(direct->Indirect[i]);
		};
	}
	return o;
}

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
	std::vector<Object> list;

	int find_object(std::string n);
	void add_objects(Pair p);
	int count_orbits();
};

int ObjList::find_object(std::string n){
	for (int i = 0; i < list.size(); i++){
		if (list[i].name == n){
			return i;
		}
	}
	return -1;
}

void update_nexts(Object * o){
	for(int i = 0; i < o->Next.size(); i++){
		if(o->Next[i]->Next.size() != 0){
			update_nexts(o->Next[i]);
		}
		else
		{
			o->Next[i]->Next.push_back(o->Direct);
		}
	}
}

void ObjList::add_objects(Pair p){
	int loc1 = find_object(p.first);
	int loc2 = find_object(p.second);
	Object o1;
	Object o2;
	if (loc1 == -1 && loc2 == -1)
	{
		o1.name = p.first;
		o1.Next.push_back(&o2);
		o2.name = p.second;
		o2.Direct = &o1;
		list.push_back(o1);
		list.push_back(o2);
	}
	else if (loc1 != -1 && loc2 == -1)
	{
		o1 = list[loc1];
		o1.Next.push_back(&o2);
		o2.name = p.second;
		o2.Direct = &o1;
		o2.Indirect.push_back(o1.Direct);
		for (int j = 0; j<o1.Indirect.size(); j++){
			o2.Indirect.push_back(o1.Indirect[j]);
		}
		list.push_back(o2);
	}
	else if(loc1 != -1 && loc2 != -1)
	{
		o1 = list[loc1];
		o2 = list[loc2];
		o2.Direct = &o1;
		std::cout<<"SEGFAULT in 1";
		if(o2.Next.size() > 0){
			update_nexts(&o2);
		}
	}
	else if(loc1 ==-1 && loc2 != -1)
	{
		o1.name = p.first;
		o2 = list[loc2] ;
		o2.Direct = &o1;
		o1.Next.push_back(&o2);
		std::cout<<"SEGFAULT in 11";
		if (o2.Next.size() > 0){
			update_nexts(&o2);
		}
	}
}

int ObjList::count_orbits(){
	int count = 0;
	for(int i=0; i<list.size(); i++){
		count += list[i].Indirect.size();
		if (list[i].Direct != NULL){
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
