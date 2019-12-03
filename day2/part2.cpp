#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

struct Parameters {
	int opcode;
	int in_pos1;
	int in_pos2;
	int out_pos;
	public:
		Parameters(int o, int i1, int i2, int op){
			opcode = o;
			in_pos1 = i1;
			in_pos2 = i2;
			out_pos = op;
	}
};

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

struct Intcode {
	private:
		int idx;
		std::vector<int> instructions;
		int len;

		int size (){
				return instructions.size();
			};

		Parameters get_params(){
				return Parameters(
							instructions[idx],
							instructions[idx+1],
							instructions[idx+2],
							instructions[idx+3]
							);
			};

		void add(Parameters p){
				instructions[p.out_pos] = instructions[p.in_pos1] + instructions[p.in_pos2] ;
			};

		void multiply(Parameters p){
				instructions[p.out_pos] = instructions[p.in_pos1] * instructions[p.in_pos2] ;
			};

	public:
		Intcode(std::string a){
			idx = 0;
			instructions = read_file(a);
			len = size();
		};

		void set_inputs(int f, int s){
			instructions[1] = f;
			instructions[2] = s;
		};

		void run_code(){
			while (idx < len)
				{
					Parameters params = get_params();
					switch (params.opcode)
					{
					case 1:
						add(params);
						break;
					case 2:
						multiply(params);
						break;
					case 99:
						idx = len;
						break;
					default:
						std::cout << "Unknown directive: " << params.opcode;
						idx = len;
						break;
				}
				idx += 4;
			}
		}
		int get_first(){
			return instructions[0];
		}
};

int main(){
	for (int i = 0; i<100; i++){
		for (int j=0; j<100; j++){
			Intcode program = Intcode("values.txt");
			program.set_inputs(i,j);
			program.run_code();
			if (program.get_first() == 19690720)
			{
				std::cout << "NOUN = " << i << "VERB = " << j;
				std::cout << "\n ANSWER =" << 100 * i + j;
				goto end_program;
			}
		}
	}
end_program:;
}
