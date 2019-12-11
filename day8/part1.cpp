#include <iostream>
#include <fstream>
#include <vector>

std::vector<int> read_file(std::string address){
	std::string line;
	std::ifstream file;
	std::vector<int> input;

	file.open(address, std::ios::out);
	if (file.is_open())
	{
		while(std::getline(file, line))
		{
			for(const char &c: line){
				input.push_back(c-'0'); //converts char to int
			}
		}
		file.close();
	}
	return input;
}

struct Row{
	int idx;
	std::vector<int> data;

	Row(int i): data(){
		idx = i;
	};
};

struct Layer {
	int n_col;
	int n_row;
	std::vector<Row> row;

	Layer(int c, int r):row(){
		n_col = c;
		n_row = r;
	}

	void create(std::vector<int> input){
		int input_size = input.size();
		int i = 0;
		if(input_size != n_col*n_row){
			return;
		}
		int row_num = 0;
		while(i < input.size()){
			int col = 0;
			Row r = Row(row_num);
			while (col < n_col){
					r.data.push_back(input[i]);
					i += 1;
					col += 1;
				}
			row.push_back(r);
			row_num += 1;
		}
	}

	std::vector<int> & operator[](int i){
		return row[i].data;
	}

	int count_value(int v);
};

int Layer::count_value(int v){
	int count = 0;
	for(int i = 0; i < n_row; i++){
		for(int j = 0; j < n_col; j++){
			if(row[i].data[j]==v){
				count += 1;
			}
		}
	}
	return count;
}

std::vector< std::vector<int> > input_segments(int size, std::vector<int> in){
	std::vector< std::vector<int> > out;
	int idx = 0;
	while(idx < in.size()){
		std::vector<int> temp;
		int t = 0;
		while(t<size){
			temp.push_back(in[idx]);
			idx += 1;
			t += 1;
		}
		out.push_back(temp);
	}
	return out;
}

int fewest_zeros(std::vector< Layer > layers){
	int idx = -1;
	int n_zero = 10000;
	for(int n = 0; n < layers.size(); n++){
		Layer layer = layers[n];
		int t_zero = layer.count_value(0);
		if (t_zero < n_zero){
			idx = n;
			n_zero = t_zero;
		}
	}
	return idx;
}


int main(){
	std::vector<int> in = read_file("input.txt");
	std::vector< std::vector<int> > layer_pixels = input_segments(6*25, in);

	std::vector< Layer > layers;
	for(int i=0; i < layer_pixels.size(); i++){
		Layer l = Layer(25,6);
		l.create(layer_pixels[i]);
		layers.push_back(l);
	}
	int f_zero = fewest_zeros(layers);
	Layer l = layers[f_zero];

	std::cout<<"ANSWER: "<<l.count_value(1) * l.count_value(2)<<'\n';
}
