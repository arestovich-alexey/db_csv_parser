#include <iostream>
#include <fstream>
#include <tclap/CmdLine.h>
using namespace std;

string VERSION = "0.1.1";

std::string unescape(const std::string& s)
{
    std::stringstream  ss{""};

    for(size_t i = 0; i < s.length(); i++)
    {
        if (s.at(i) == '\\')
        {
            switch(s.at(i + 1))
            {
                case 'a':  ss << "\a"; i++; break;
                case 'b':  ss << "\b"; i++; break;
                case 't':  ss << "\t"; i++; break;
                case 'n':  ss << "\n"; i++; break;
                case 'v':  ss << "\v"; i++; break;
                case 'f':  ss << "\f"; i++; break;
                case 'r':  ss << "\r"; i++; break;
                case '"':  ss << "\""; i++; break;
                case '\'':  ss << "'"; i++; break;
                case '?':  ss << "?"; i++; break;
                default:   ss << "\\";      break;
            }       
        }
        else 
        {
            ss << s.at(i);
        }
    }

    return ss.str();
}

//"a,b,c" to vector{a,b,c}
vector<string> split(std::string s, std::string delimiter)
{
	vector<string> result;

	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		token = s.substr(0, pos);
		result.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	result.push_back(s);
	return result;
}

//"1,2" to vector{1,2}
std::tuple<vector<vector<unsigned int>>, unsigned int> strings_to_int(std::vector<std::string> values)
{
	vector<vector<unsigned int>> result;
	unsigned int bigest_index = 0;
	for (string value : values)
	{
		vector<unsigned int> value_res;
		for (string i : split(value, ","))
		{
			unsigned long lresult = stoul(i, 0, 10);
			unsigned int value_int = lresult;
			value_res.push_back(value_int);
			if (value_int > bigest_index)
			{
				bigest_index = value_int;
			}
		}
		result.push_back(value_res);
	}
	return {result, bigest_index};
}

class ArgumentParse
{
public:
	string spliter;								  // spliter for files to parse
	string new_spliter;							  // spliter for new files
	bool unsecape_spliter;
	vector<string> files;						  // files to parse
	vector<ofstream> outfiles;					  // files to output (opened to append)
	unsigned int bigest_index;					  // bigest index of values (for exception)
	unsigned int bigest_size;					  // bigest index of values (for exception)
	vector<vector<unsigned int>> key_and_indexes; // {"key":{[0,1],[2,3]},"key2":{[2,4],[4,1]}};

	vector<string> keys;
	vector<string> values;

	ArgumentParse(int argc, char **argv)//string spliter_str, string new_spliter_str, vector<string> keys_str, vector<string> values_str, vector<string> files_str
	{
		TCLAP::CmdLine cmd("Db parser", ' ', VERSION);

		// Define a value argument and add it to the command line.
		// A value arg defines a flag and a type of value that it expects,
		TCLAP::ValueArg<std::string> spliter_arg("s", "spliter", "spliter of string", false, "\t", "string");
		TCLAP::ValueArg<std::string> new_spliter_arg("n", "new_spliter", "new_spliter of string", false, ":", "string");
		TCLAP::MultiArg<std::string> keys_arg("k", "keys", "keys of indexes. Example: -k key1 -k key2", true, "string");
		TCLAP::MultiArg<std::string> values_arg("a", "values", "indexes to parse separated by ','. Example: -a '0,1' -a '3,4'", true, "string");
		TCLAP::UnlabeledMultiArg<std::string> files_arg("files", "file names", true, "path");
		TCLAP::SwitchArg unsecapeSwitch("u","unescape","Unescape spliters. To work with '\\t', etc.", false);

		// Add the argument keys to the CmdLine object. The CmdLine object
		// uses this Arg to parse the command line.
		cmd.add(spliter_arg);
		cmd.add(new_spliter_arg);
		cmd.add(unsecapeSwitch);
		cmd.add(keys_arg);
		cmd.add(values_arg);
		cmd.add(files_arg);

		cmd.parse(argc, argv);

		spliter = spliter_arg.getValue();
		new_spliter = new_spliter_arg.getValue();
		unsecape_spliter = unsecapeSwitch.getValue();
		keys = keys_arg.getValue();
		values = values_arg.getValue();
		files = files_arg.getValue();

		if (unsecape_spliter==true){
			spliter=unescape(spliter);
			new_spliter=unescape(new_spliter);
		}

		auto [key_and_indexes_dict, bigest_index_int] = strings_to_int(values);
		key_and_indexes = key_and_indexes_dict;
		bigest_index = bigest_index_int;
		bigest_size = bigest_index + 1;

		for (string file : keys)
		{
			outfiles.push_back(ofstream(file + ".txt", ios::app));
		}
	}

	void print()
	{
		cout << "spliter: " << spliter << "\n";
		cout << "new_spliter: " << new_spliter << "\n";
		cout << "keys(len): " << keys.size() << "\n";
		for (string i : keys)
			cout << i << endl;

		cout << "values(len): " << values.size() << ", bigest_index: " << bigest_index << "\n";
		for (string i : values)
			cout << i << endl;

		cout << "files(len): " << files.size() << "\n";
		for (string i : files)
			cout << i << endl;

		cout << "out_files(len): " << outfiles.size() << "\n";
	}
};

//-keys "a1" "a2" -values "v1,v2" "v3,v4" -spliter "\t" -new_spliter ":" -files "asd.tx" "asd2.txt"
int main(int argc, char **argv)
{

	// Wrap everything in a try block.  Do this every time,
	// because exceptions will be thrown for problems.
	try
	{
		ArgumentParse args(argc, argv);//spliter_arg.getValue(), new_spliter_arg.getValue(), keys_arg.getValue(), values_arg.getValue(), files_arg.getValue()
		args.print();

		for (string file : args.files)
		{
			ifstream file_opened(file, ios::in);
			cout << "file: " << file << "\n";

			int num_line = 0;
			for (std::string line; getline(file_opened, line);)
			{
				vector<string> splited_line = split(line, args.spliter);
				if (splited_line.size() >= args.bigest_size)
				{
					for (size_t i = 0; i < args.key_and_indexes.size(); i++)
					{
						string to_write;
						for (unsigned int index : args.key_and_indexes[i])// i - its key from dict
						{
							to_write += splited_line[index];
							to_write += args.new_spliter;
						}
						to_write.pop_back();
						args.outfiles[i] << to_write << "\n";
					}
				}
				else
				{
					cout << "LINE: " << num_line << " index<" << args.bigest_index << " \n";
				}
				num_line += 1;
			}
		}
	}
	catch (TCLAP::ArgException &e) // catch exceptions
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
	}
}
