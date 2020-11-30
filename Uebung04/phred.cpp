#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<cassert>

#include "Fastq.h"
#include "Sequence.h"
#include "DNA.h"

using namespace Alphabet;

int main(int argc, char* argv[])
{
	if (argc != 3) {
		//Illegal number of Commandline arguments
		std::cerr << "Illeagl number of arguments" << argc << std::endl;
		return 1;
	}
	std::string argv1(argv[1]);
    std::string argv2(argv[2]);
	if (argv1.find(".fastq") != std::string::npos && argv2.find(".fastq") != std::string::npos) {
		std::ifstream in_file(argv[1]);
		std::ofstream out_file(argv[2]);
		if (!in_file || !out_file) {
			std::cerr << "Could not open file" << std::endl;
			return 1;
		} else {
			while (in_file.good()) {
                //einlesen ausgeben
            }
            in_file.close();
            out_file.close();
		}
	} else {
        std::cerr << "Illegal fileformat to read from " << std::endl;
        return 1;
    }
	return 0;
}

