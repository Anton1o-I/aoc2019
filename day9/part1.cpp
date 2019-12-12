#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<long long int> read_file(std::string address){
	std::string line;
	std::ifstream file;
	std::vector<long long int> input;

	file.open(address, std::ios::out);
	if (file.is_open())
	{
		while(std::getline(file, line, ','))
		{
			input.push_back(std::stoll(line));
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
	long long int param1;
	long long int param2;
	int param3;
};


struct IntCode{
		int idx;
		int len;
		int relative_base;
		int size(){
			return instructions.size();
		}
		bool halt;
		std::vector<long long int> instructions;
		Directive current;
		IntCode(std::string a){
			idx = 0;
			instructions = read_file(a);
			std::vector<long long int> temp;
			temp.assign(10000000, 0);
			instructions.insert(std::end(instructions), std::begin(temp), std::end(temp));
			len = size();
			relative_base = 0;
			halt = false;
		}
		void run_code();
		Directive get_directives(int code);
		void print_instructions();
		long long int get_value(int mode, int offset);
		void multiply();
		void add();
		void input();
		void output();
		void jumptrue();
		void jumpfalse();
		void less();
		void equal();
		void adjust_relative();
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
			if (d.mode_param1 == 0){
				d.param1 = instructions[idx+1];
			}
			else if (d.mode_param1 == 2){
				d.param1 = relative_base + instructions[idx+1];
			}
			else if (d.mode_param1 == 1){
				d.param1 = idx+1;
			}
		}
		else if (d.opcode == 4)
		{
			if (d.mode_param1 == 1){
				d.param1 = idx+1;
			}
			else if(d.mode_param1 == 2){
				d.param1 = relative_base + instructions[idx+1];
			}
			else if(d.mode_param1 == 0 ){
				d.param1 = instructions[idx+1];
			}
		}
		else if (d.opcode == 9)
		{
			d.param1 = get_value(d.mode_param1, 1);
		}
		else if(d.opcode==1 || d.opcode==2||d.opcode==5||d.opcode==6||d.opcode==7||d.opcode==8){
			d.param1 = get_value(d.mode_param1, 1);
			d.param2 = get_value(d.mode_param2, 2);
			d.param3 = get_value(d.mode_param3, 3);
		}
		return d;
}

long long int IntCode::get_value(int mode, int offset){
	if ( mode == 1 )
	{
		return instructions[idx+offset];
	}
	else if( mode == 2)
	{
		int i = relative_base + instructions[idx+offset];
		return instructions[i];
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
	std::cout<<"PARAM TO UPDATE"<<current.param1;
	instructions[current.param1] = user_input;
}

void IntCode::jumptrue(){
	if(current.param1!=0){
		idx=current.param2;
	}
	else
	{
		idx += 3;
	}
}

void IntCode::jumpfalse(){
	if(current.param1==0){
		idx = current.param2;
	}
	else
	{
		idx += 3;
	}
}

void IntCode::less(){
	if(current.param1<current.param2){
		instructions[current.param3]=1;
	}
	else
	{
		instructions[current.param3]=0;
	}
}

void IntCode::equal(){
	if(current.param1==current.param2){
		instructions[current.param3]=1;
	}
	else
	{
		instructions[current.param3]=0;
	}
}

void IntCode::output(){
	std::cout<<instructions[current.param1]<<'\n';
}

void IntCode::adjust_relative(){
	std::cout<<"ONE SHOULD = 1015"<<current.param1<<'\n';
	relative_base += current.param1;
	std::cout<<"RELATIVE BASE =="<<relative_base;
}

void IntCode::run_code(){
	while (!halt)
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
				case 5:
					jumptrue();
					break;
				case 6:
					jumpfalse();
					break;
				case 7:
					less();
					break;
				case 8:
					equal();
					break;
				case 9:
					adjust_relative();
					break;
				case 99:
					std::cout<<"HALTING PROGRAM ON PURPOSE\n";
					halt = true;
					break;
				default:
					break;
			}
			switch (current.opcode)
			{
				case 1:
				case 2:
				case 7:
				case 8:
					idx += 4;
					break;
				case 3:
				case 4:
				case 9:
					idx += 2;
					break;
				case 5:
				case 6:
					idx += 0;
					break;
				case 99:
					break;
				default:
					std::cout<<"HALTING NOT ON PURPOSE";
					halt = true;
					break;
			}
		}
}

int main(){
	IntCode ic = IntCode("input.txt");

	std::cout<<"INDEX 100   "<<ic.instructions[100]<<'\n';
	ic.run_code();
}
