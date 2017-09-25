#ifndef WORDLADDERSOLVER_INCLUDED
#define WORDLADDERSOLVER_INCLUDED

#include <string>
#include <vector>

class WordLadderSolverImpl;

class WordLadderSolver
{
  public:
    WordLadderSolver();
    ~WordLadderSolver();
    void createWordList(const std::vector<std::string>& words);
    int buildLadder(std::string start, std::string end, std::vector<std::string>& ladder);
  private:
    WordLadderSolverImpl* m_impl;
      // WordLadderSolvers can not be copied or assigned.  We enforce this
      // by declaring the copy constructor and assignment operator private and
      // not implementing them.
    WordLadderSolver(const WordLadderSolver&);
    WordLadderSolver& operator=(const WordLadderSolver&);
};

#endif // WORDLADDERSOLVER_INCLUDED
