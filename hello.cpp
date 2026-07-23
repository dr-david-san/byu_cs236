#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
     using namespace std;
     int main(int argc, char* argv[]) {
       //cout << argc  << endl;
	//cout << "argv[0]: " << argv[0]  << endl;
	//cout << "argv[1]: " << argv[1]  << endl;
	string filename = argv[1];
	ifstream in;
	in.open(filename);
	stringstream ss;
	ss << in.rdbuf();
	string input = ss.str();
	in.close();

	//cout <<  "*** Start of Input ***" << endl;
	//cout <<  input << endl;
	//cout <<  "*** End of Input ***" << endl;

	int chars = 0;
	int lines = 0;
	int wspace = 0;
	int digits = 0;
	int letters = 0;
	
	for(char c : input) {
    	if (isdigit(c)){ 
			digits++;
		}
		if ('\n' == c){
			lines++;
		}
		if (isspace(c)){
			wspace++;
		}
		if (isalpha(c)) { 
			letters++;
		}
		if (c) {
			chars++;
		}
 		
	}
	cout << "chars: " << chars << endl;
	cout <<  "lines: " << lines << endl;
	cout <<  "wspace: " << wspace << endl;
	cout <<  "digits: " << digits << endl;
	cout <<  "letters: " << letters << endl;
     }
