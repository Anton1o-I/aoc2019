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
		int amp_out;
		int len;
		bool halt;
		int ticker;
		bool stop;

		int size(){
			return instructions.size();
		}
		std::vector<int> instructions;
		Directive current;
		IntCode(std::string a){
			idx = 0;
			ticker = 0;
			stop = false;
			halt = false;
			amp_out = 0;
			instructions = read_file(a);
			len = size();
		}
		void run_code(int i1);
		Directive get_directives(int code);
		void print_instructions();
		int get_value(int mode, int offset);
		void multiply();
		void add();
		void input(int i);
		int output();
		void jumptrue();
		void jumpfalse();
		void less();
		void equal();
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
		else if(d.opcode==1 || d.opcode==2||d.opcode==5||d.opcode==6||d.opcode==7||d.opcode==8){
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

void IntCode::input(int i){
	instructions[current.param1] = i;
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

int IntCode::output(){
	return instructions[current.param1];
}

void IntCode::run_code(int i1){
	halt = false;
	while (halt == false)
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
					if (ticker == 0){
						input(i1);
						ticker +=1;
					}
					else {
						input(amp_out);
					}
					break;
				case 4:
					amp_out = output();
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
				case 99:
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
					idx += 2;
					break;
				case 4:
					idx += 2;
					halt = true;
					break;
				case 5:
				case 6:
					idx += 0;
					break;
				case 99:
					stop = true;
					halt = true;
					break;
				default:
					idx = len;
					break;
			}
			if (idx >= len){
				halt = true;
			}
		}
}

int return_largest(std::vector<int> vec){
	int o = 0;
	for(int i = 0; i < vec.size(); i++){
		if (vec[i] > o){
			o = vec[i];
		}
	}
	return o;
}


int main(){

	int inp[5]={5,6,7,8,9};
	std::vector< int > amp_outputs;
	do{
		IntCode icA = IntCode("input.txt");
		IntCode icB = IntCode("input.txt");
		IntCode icC = IntCode("input.txt");
		IntCode icD = IntCode("input.txt");
		IntCode icE = IntCode("input.txt");
		int loop = 1;
		while(!icE.stop){
			std::cout<<"LOOP:"<< loop<<" BEFORE:"<<icA.amp_out<<'\n';

			icA.amp_out = icE.amp_out;
			icA.run_code(inp[0]);

			icB.amp_out = icA.amp_out;
			icB.run_code(inp[1]);

			icC.amp_out = icB.amp_out;
			icC.run_code(inp[2]);

			icD.amp_out = icC.amp_out;
			icD.run_code(inp[3]);

			icE.amp_out = icD.amp_out;
			icE.run_code(inp[4]);
		loop += 1;
		}
		amp_outputs.push_back(icE.amp_out);
	}
	while(
		std::next_permutation(inp,inp+5)
		);
	std::cout<<"SIZE VEC"<<amp_outputs.size();
	std::cout<<"LARGEST VALUE = "<<return_largest(amp_outputs)<<'\n';
}
