#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<int> read_file(std::string address){
	std::string line;
	std::ifstream file;
	std::vector<int> input;

	file.open(address, std::ios::out);
	if (file.is_open())
	{
		while(std::getline(file, line, ','))
		{
			input.push_back(std::stoi(line));
		}
		file.close();
	}
	return input;
}


struct Directive {
	int mode_param1;
	int mode_param2;
	int mode_param3;
	int opcode;
	int param1;
	int param2;
	int param3;
};


struct IntCode{
		int idx;
		int len;
		int size(){
			return instructions.size();
		}
		std::vector<int> instructions;
		Directive current;
		IntCode(std::string a){
			idx = 0;
			instructions = read_file(a);
			len = size();
		}
		void run_code();
		Directive get_directives(int code);
		void print_instructions();
		int get_value(int mode, int offset);
		void multiply();
		void add();
		void input();
		void output();
};

void IntCode::print_instructions(){
	for(int i = 0; i<len; i++){
		std::cout<<instructions[i]<<',';
	}
};

Directive IntCode::get_directives(int idx){
		int code = instructions[idx];
		Directive d;
		d.mode_param3 = code/10000;
		code -= d.mode_param3*10000;
		d.mode_param2 = code/1000;
		code -= d.mode_param2*1000;
		d.mode_param1 = code/100;
		code -= d.mode_param1*100;
		d.opcode = code;

		if (d.opcode == 3)
		{
			d.param1 = instructions[idx+1];
		}
		else if (d.opcode == 4)
		{
			if (d.mode_param1 == 1){
				d.param1 = idx+1;
			}
			else{
				d.param1 = instructions[idx+1];
			}
		}
		else if(d.opcode==1 || d.opcode==2){
			d.param1 = get_value(d.mode_param1, 1);
			d.param2 = get_value(d.mode_param2, 2);
			d.param3 = instructions[idx+3];
		}
		return d;
}

int IntCode::get_value(int mode, int offset){
	if (mode==1)
	{
		return instructions[idx+offset];
	}
	else
	{
		int t = instructions[idx+offset];
		return instructions[t];
	}
}

void IntCode::multiply(){
	instructions[current.param3] = current.param1 * current.param2;
}

void IntCode::add(){
	instructions[current.param3] = current.param1 + current.param2;
}

void IntCode::input(){
	int user_input;
	std::cout<<"Input:";
	std::cin>>user_input;
	instructions[current.param1] = user_input;
}

void IntCode::output(){
	std::cout<<instructions[current.param1]<<'\n';
}

void IntCode::run_code(){
	while (idx<len)
		{
			current=get_directives(idx);
			switch (current.opcode)
			{
				case 1:
					add();
					break;
				case 2:
					multiply();
					break;
				case 3:
					input();
					break;
				case 4:
					output();
					break;
				case 99:
					break;
				default:
					std::cout<<"UNKNOWN DIRECTIVE:"<<current.opcode;
					break;
			}
			if (current.opcode==3||current.opcode==4){
				idx += 2;
			}
			else if (current.opcode==1||current.opcode==2){
				idx += 4;
			}
			else{
				idx = len;
			}
		}
}

int main(){
	IntCode ic = IntCode("input.txt");
	ic.run_code();
}
