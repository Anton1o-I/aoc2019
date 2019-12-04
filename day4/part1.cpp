#include <iostream>
#include <vector>

struct Password{
	int first;
	int second;
	int third;
	int fourth;
	int fifth;
	int sixth;

	Password(int v){
		first = v/100000;
		v -= first * 100000;
		second = v/10000;
		v -= second * 10000;
		third = v/1000;
		v -= third * 1000;
		fourth = v/100;
		v -= fourth * 100;
		fifth = v/10;
		v -= fifth * 10;
		sixth = v;
	};
	bool validate();
	bool increasing();
	bool adjacent();
};


struct Potentials{
	std::vector<int> list;

	int get_size(){
		return list.size();
	};
	Potentials generate(int lower, int upper);
};

Potentials generate(int lower, int upper){
	Potentials l;
	for(int i=lower; i<=upper; i++){
		Password p = Password(i);
		if (p.validate()){
			l.list.push_back(i);
		}
	}
	return l;
};

bool Password::increasing(){
	return first<=second && second<=third && third<=fourth && fourth<=fifth && fifth<=sixth;
};

bool Password::adjacent(){
	return first==second||second==third||third==fourth||fourth==fifth||fifth==sixth;
};


bool Password::validate(){
	return increasing() && adjacent();
}


int main(){
	Potentials p = generate(153517, 630395);
	int s = p.get_size();
	std::cout<<"ANSWER: "<<s<<'\n';
};
