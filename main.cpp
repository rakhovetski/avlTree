#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include "AVL.cpp"
using namespace std;

struct wordNode
{
	double key;
	int frequency;
};

void print(wordNode ss)
{
	cout << ss.key << " " << ss.frequency << endl;
}

string trim(string input)
{
	stringstream ss;
	for (int x = 0; x < (int)input.size(); x++)
	{
		if (isalpha(input[x]))
		{
			ss << input[x];
		}
	}

	if (ss.str().length() > 0)
		return ss.str();
	else
		return "";
}

int main()
{
	
	

	Tree<wordNode, string> j;
	wordNode k;
	
	for (size_t i = 0; i < 6; i++)
	{
		std::cin >> k.key;
		j._Insert(k);
	}

	j._Print();
	
	
	
}