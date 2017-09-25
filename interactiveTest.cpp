#include "WordLadderSolver.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
using namespace std;

const char* filename = "wordlist.txt";
  // A Windows user might have the path be "C:/CS32/P4/wordlist.txt"
  // A Mac user might have it be "/Users/fred/CS32/P4/wordlist.txt"

int main()
{
	ifstream wordlistfile(filename);
	if (!wordlistfile)
	{
		cerr << "Cannot open " << filename << "!" << endl;
		return 1;
	}

	//cout << "Loading word list. This should take about 10 minutes ..." << endl;
	vector<string> words;
	string w;
	while (wordlistfile >> w)
		words.push_back(w);
	wordlistfile.close();
		
	WordLadderSolver wls;
	wls.createWordList(words);
	cout << "This program will (if possible) build a chain of words from START to END." << endl;
	cout << "Each word is generated from the previous using one of these transformations:" << endl;
	cout << "1) Letter replacement (\"dog\" -> \"dig\")." << endl;
	cout << "2) Letter insertion (\"table\" -> \"stable\")." << endl;
	cout << "3) Letter deletion (\"tide\" -> \"tie\")." << endl; 
	cout << "4) Swapping adjacent letters (\"rat\" -> \"art\")." << endl;
	cout << "Given a word list, the chain is guaranteed to be the shortest possible chain." << endl;
	cout << "Enter nothing to exit." << endl << endl;
	cout << "=============================================================================" << endl << endl;
	for (;;)
	{
		cout << "Enter START word: ";
		string start;
		getline(cin, start);
		if (start.empty())
			break;
		cout << "Enter END word: ";
		string end;
		getline(cin, end);

		vector<string> result;
		int status = wls.buildLadder(start, end, result);

		switch (status)
		{
		case 3:
			cout << "They are the same word!" << endl << endl;
			break;
		case 2:
			cout << "END word not found in word list!" << endl << endl;
			break;
		case 1:
			cout << "START word not found in word list" << endl << endl;
			break;
		case -1:
			cout << "Cannot build a ladder with this word list!" << endl << endl;
			break;
		case 0:
			cout << "Successfully built ladder:" << endl;
			for (string w : result)
				cout << "- " << w << endl;
			cout << endl;
			break;
		}
	}
}
