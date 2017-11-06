// WordLadderSolver.cpp

#include "WordLadderSolver.h"
#include <string>		// originally here
#include <vector>		// originally here
#include <cctype>		// originally here
#include <algorithm>	// originally here
#include <queue>
#include <stack>

using namespace std;

const int HASH_TABLE_SIZE = 99991; // Closest prime to 100,000

void removeNonLetters(string& s);

// This is modified djb2 hash function, returning values ranging from 0 to HASH_TABLE_SIZE - 1
// Given the ~60,000 words list, this hash function yields ~45,000 used buckets (out of 100,000 available)
// That means there are ~15,000 collisions, or 1/4 of the vocabulary list.
unsigned long hashf(string s);

//**************************** WordLadderSolverImpl **********************************

struct Bucket{
	Bucket(string w) : word(w), next(nullptr), visited(false), root(nullptr) {}
	string word;
	bool visited;
	Bucket* next; //points to next bucket in hash table (for collision)
	Bucket* root; //points to the bucket that transforms into this
	vector<Bucket*> derivative; // contains all Buckets that can be transformed from this Bucket
};

class WordLadderSolverImpl{
public:
	WordLadderSolverImpl();
	~WordLadderSolverImpl();
	void createWordList(const vector<string>& words);
	int buildLadder(string start, string end, vector<string>& ladder);
	Bucket* lookupDict(string theWord); // returns the Bucket corresponding to theWord, nullptr if not found
	void addToDict(string); // adds a new word to dictionary, does nothing if word already there
private:
	// Functions
	void cleanUpDict(); // this will un-mark all the visited HashBucket for the next ladder build
	void generate(string, Bucket*);
	void generateHelper(string, Bucket*);
	void formLadder(Bucket* bucket, vector<string> &ladder);

	// Member data
	Bucket* dict[HASH_TABLE_SIZE]; // Open hash table with linked list impl, stores all valid words
	queue<Bucket*> q; //this stores to-be-processed words
};

WordLadderSolverImpl::WordLadderSolverImpl(){
	for (int i = 0; i < HASH_TABLE_SIZE; i++)
		dict[i] = nullptr;
}

WordLadderSolverImpl::~WordLadderSolverImpl(){
	Bucket *p;
	for (int i = 0; i < HASH_TABLE_SIZE; i++){
		while (dict[i] != nullptr){
			p = dict[i];
			dict[i] = dict[i]->next;
			delete p;
		}
	}
}

void WordLadderSolverImpl::formLadder(Bucket* bucket, vector<string> &ladder)
{
	stack<string> temp;
	Bucket* p = bucket;
	while (p != nullptr){
		temp.push(p->word); // this holds ladder in reverse order because we are going from end to start
		p = p->root;
	}

	string s;
	while (!temp.empty())	{
		s = temp.top();
		ladder.push_back(s);
		temp.pop();
	}
}

void WordLadderSolverImpl::generate(string root, Bucket* h){
	unsigned int i;
	char ch;

	// Transformation by letter replacement
	for (i = 0; i < root.size(); i++)
		for (ch = 'a'; ch <= 'z'; ch++)	{
			string s = root;
			s[i] = ch;
			generateHelper(s, h);
		}
	
	// Transformation by letter insertion
	for (i = 0; i <= root.size(); i++){
		for (ch = 'a'; ch <= 'z'; ch++)	{
			string s = root;
			s.insert(i, 1, ch);
			generateHelper(s, h);
		}
	}

	// Transformation by swapping adjacent letters
	for (i = 0; i < root.size() - 1; i++){
		string s = root;
		swap(s[i], s[i + 1]);
		generateHelper(s, h);
	}

	// Transformation by letter deletion
	for (i = 0; i < root.size(); i++){
		string s = root;
		s.erase(i, 1); 
		generateHelper(s, h);
	}
}

void WordLadderSolverImpl::generateHelper(string str, Bucket* h){
	Bucket* n = lookupDict(str);
	if (n != nullptr && n != h)
		h->derivative.push_back(n);
	
}

void WordLadderSolverImpl::cleanUpDict() // O(N)
{
	for (int i = 0; i < HASH_TABLE_SIZE; i++){
		if (dict[i] != nullptr)	{
			Bucket* p = dict[i];
			while (p != nullptr){
				p->visited = false;
				p->root = nullptr;
				p = p->next;
			}
		}
	}
}

// The longest collision chain has 6 buckets. So the worst search takes 6 steps. 
Bucket* WordLadderSolverImpl::lookupDict(string theWord)
{
	int k = hashf(theWord);
	
	Bucket* p = dict[k];
	while (p != nullptr){
		if (p->word == theWord)
			break;
		p = p->next;
	}
	return p; //returns nullptr if theWord not found
}

void WordLadderSolverImpl::createWordList(const vector<string>& words)
{
	for (string word : words){
		removeNonLetters(word);
		if (word != "")
			addToDict(word);
	}

	// Map all words and their derivatives 
	for (int i = 0; i < HASH_TABLE_SIZE; i++){
		if (dict[i] != nullptr)	{
			Bucket* p = dict[i];
			while (p != nullptr){
				generate(p->word, p);
				p = p->next;
			}
		}
	}
}

void WordLadderSolverImpl::addToDict(string s){
	int k = hashf(s);
	if (dict[k] == nullptr)	{
		dict[k] = new Bucket(s);
		return;
	}

	Bucket* p = dict[k];
	while (p->next != nullptr){
		if (p->word == s)
			return;
		p = p->next;
	}
	p->next = new Bucket(s);
}

int WordLadderSolverImpl::buildLadder(string start, string end, vector<string>& ladder){
	ladder.clear();
	removeNonLetters(start);
	removeNonLetters(end);
	
	if (start == end)
		return 3;
	if (lookupDict(start) == nullptr)
		return 1;
	if (lookupDict(end) == nullptr)
		return 2;
	
	// ==================== Start building ladder if possible =====================
	// This uses a breadth-first search for a graph with the help of a queue
	Bucket* p = lookupDict(start);
	p->visited = true;
	q.push(p);
	while (!q.empty()) {
		if (q.front()->word == end){
			formLadder(q.front(), ladder);
			break;
		}
		// Push everything in derivative into queue q
		for (int i = 0; i < q.front()->derivative.size(); i++){
			if (!q.front()->derivative[i]->visited)	{ 
				q.push(q.front()->derivative[i]);
				q.front()->derivative[i]->visited = true;
				q.front()->derivative[i]->root = q.front();
			}
		}
		q.pop();
	}
	cleanUpDict();
	while (!q.empty())
		q.pop();
	if (!ladder.empty())
		return 0;

	//=============================================================================
	return -1;
}
	
void removeNonLetters(string& s){
	string::iterator to = s.begin();
	for (char ch : s){
		if (isalpha(ch)){
			*to = tolower(ch);
			to++;
		}
	}
	s.erase(to, s.end());  // chop off everything from "to" to end.
} 

unsigned long hashf(string s){
	char *str = &s[0];
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; // hash * 33 + c 

	return hash%HASH_TABLE_SIZE;
}

//******************** WordLadderSolver functions **********************************

// These functions simply delegate to WordLadderSolverImpl's functions.
// You probably don't want to change any of this code.

WordLadderSolver::WordLadderSolver()
	{ m_impl = new WordLadderSolverImpl; }

WordLadderSolver::~WordLadderSolver()
	{ delete m_impl; }

void WordLadderSolver::createWordList(const vector<string>& words)
	{ m_impl->createWordList(words); }

int WordLadderSolver::buildLadder(string start, string end, vector<string>& ladder)
	{ return m_impl->buildLadder(start, end, ladder); }

