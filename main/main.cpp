#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <tclap/CmdLine.h>
using namespace std;

//"a,b,c" to vector{a,b,c}
vector<string> split(std::string s, std::string delimiter){
	vector<string> result;

	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		result.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	result.push_back(s);
	return result;
}

//"1,2" to vector{1,2}
std::tuple<vector<vector<unsigned int>>, unsigned int> strings_to_int(std::vector<std::string> values){
	vector<vector<unsigned int>> result;
	unsigned int bigest_index = 0;
	for (string value: values){
		vector<unsigned int> value_res;
		for(string i: split(value,",")){
			unsigned long lresult = stoul(i, 0, 10);
			unsigned int value_int = lresult;
			value_res.push_back(value_int);
			if (value_int > bigest_index){
				bigest_index=value_int;
			}
		}
		result.push_back(value_res);
	}
	return {result, bigest_index};
}

//-keys "a1" "a2" -values "v1,v2" "v3,v4" -spliter "\t" -new_spliter ":" -files "asd.tx" "asd2.txt"
int main(int argc, char** argv)
{

	// Wrap everything in a try block.  Do this every time, 
	// because exceptions will be thrown for problems.
	try {  

	TCLAP::CmdLine cmd("Db spliter", ' ', "0.1");

	// Define a value argument and add it to the command line.
	// A value arg defines a flag and a type of value that it expects,
	TCLAP::ValueArg<std::string> spliter_arg("s","spliter","spliter of string",false,"\t","string");
    TCLAP::ValueArg<std::string> new_spliter_arg("n","new_spliter","new_spliter of string",false,":","string");
    TCLAP::MultiArg<std::string> keys_arg("k", "keys", "keys of indexes", true,"string" );
    TCLAP::MultiArg<std::string> values_arg("a", "values", "indexes to parse separated by ','. Example: -a '0,1' '3,4'", true,"string" );
    TCLAP::UnlabeledMultiArg<std::string> files_arg("files","file names",true,"path");

	// Add the argument keys to the CmdLine object. The CmdLine object
	// uses this Arg to parse the command line.
	cmd.add( spliter_arg );
    cmd.add( new_spliter_arg );
    cmd.add( keys_arg );
    cmd.add( values_arg );
    cmd.add( files_arg );

	// Parse the argv array.
	cmd.parse( argc, argv );
	
	std::string spliter = spliter_arg.getValue();
	std::string new_spliter = new_spliter_arg.getValue();
	std::vector<std::string> keys = keys_arg.getValue();
	std::vector<std::string> values = values_arg.getValue();
	std::vector<std::string> files = files_arg.getValue();

	cout << "spliter: " << spliter << "\n";
	cout << "new_spliter: " << new_spliter << "\n";
	cout << "keys: " << keys.size() << "\n";
	for(string i : keys) 
		cout << i << endl;

	cout << "values: " << values.size() << "\n";
	for(string i : values) 
		cout << i << endl;

	cout << "files: " << files.size() << "\n";
	for(string i : files) 
		cout << i << endl;

	std::vector<std::ofstream> outfiles;;
	auto [index_values, bigest_index] = strings_to_int(values);
	for (string file: keys){
		outfiles.push_back(ofstream(file+".txt",ios::app));
	}
	
	for (string file: files){
		ifstream file_opened (file, ios::in);
		cout << "file: "<< file <<"\n";
		
		int num_line=0;
		for( std::string line; getline( file_opened, line ); )
		{
			vector<string> splited_line = split(line,spliter);
			if (splited_line.size()>=bigest_index){
				for (size_t i = 0; i<index_values.size(); i++){
					string to_write;
					for (unsigned int index:index_values[i]){
						to_write+=splited_line[index];
						to_write+=new_spliter;
					}
					to_write.pop_back();
					outfiles[i] << to_write <<"\n";
				}
			}
			else{
				cout<< "LINE:"<< num_line <<"index<"<<bigest_index<<" "<<splited_line.size()<< " "<<splited_line[0]<<" \n";
			}
			num_line+=1;
		}
	}

	} catch (TCLAP::ArgException &e)  // catch exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }
}

