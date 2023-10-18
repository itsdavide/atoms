#ifndef __ATOM_H__
#define __ATOM_H__

#include <cstdlib>
#include <sstream>
#include <vector>

class Atom{
	std::vector<int> v;
	int size;
public:
	/* Public Constants */
	const static int DONTCARE = -1;
	const static int TRUE = 1;
	const static int FALSE = 0;
	/* Constructor */
	Atom(int size){
		Atom::v.resize(size); 
		for (std::vector<int>::iterator i = v.begin() ; i != v.end() ; i++)
			(*i) = 0;
		Atom::size = size;
	}
	void setTruthValue(int index, int value){v[index] = value;}
	int getSize(){return size;}
	void setSize(int size){Atom::size = size;}
	std::vector<int> &getVector(){return v;}
	void setVector(std::vector<int> v){Atom::v = v;}
	std::string toString(){
		std::stringstream s;
		for (std::vector<int>::iterator i = v.begin() ; i != v.end() ; i++)
			s << ((*i) == DONTCARE ? "X" : ((*i) == TRUE ? "1" : "0")) << "\t"; 
		return s.str();
	}
};

#endif
