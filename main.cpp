#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <set>
#include <utility>
#include <cmath>
#include "parser.h"
#include "bdd.h"
#include "atom.h"

using namespace std;

/* Global Varibles */
vector<Atom> atoms;
vector<Atom> expAtoms;

/* Function Prototypes */
void generate_atoms(char *varset, int size);
vector<Atom> expand_atoms(vector<Atom> atoms, int size);
void expand(Atom unexp, int i, Atom current, vector<Atom> &expAtoms);

int main(int argc, char **argv){
	if (argc != 2){
		cerr << "Usage: " << argv[0] << " <filename>" << endl;
		exit(1);
	}
	ifstream in(argv[1]);
	if (!in){
		cerr << endl << endl << "*** ERROR: impossible to read file! ***" << endl << endl;
		exit(1);
	}
	bdd_init(10000, 1000);
	Parser p(&in);
	bdd L;
	p.parse(L);
	//cout << "NUMBER OF ATOMS: " << bdd_satcount(L) << endl;
	//cout << "Atoms:" << endl;
	map<string, bdd> e = p.getEvents();
	for (map<string, bdd>::iterator i = e.begin() ; i != e.end() ; i++){
		cout << (*i).first << "\t";
	}
	cout << endl;
	bdd_allsat(L, generate_atoms);
	expAtoms = expand_atoms(atoms, e.size());
	for (vector<Atom>::iterator i = expAtoms.begin() ; i != expAtoms.end() ; i++){
		cout << (*i).toString() << endl;
	}
	bdd_done();
	return 0;
}

void generate_atoms(char *varset, int size){
    int v;
	Atom a(size);
     
    for(v = 0 ; v < size ; v++){
		if (varset[v] < 0)
			a.setTruthValue(v, Atom::DONTCARE);
		if (varset[v] == 0)
			a.setTruthValue(v, Atom::FALSE);
		if (varset[v] == 1)
			a.setTruthValue(v, Atom::TRUE);
    }
    atoms.push_back(a);
}

vector<Atom> expand_atoms(vector<Atom> atoms, int size){
	vector<Atom> expAtoms; 
	for (vector<Atom>::iterator i = atoms.begin() ; i != atoms.end() ; i++) {
		Atom empty(size);
		expand((*i), 0, empty, expAtoms);
	}
	return expAtoms;
}

void expand(Atom unexp, int i, Atom current, vector<Atom> &expAtoms){
	/* The expanded atom has been built */
	if (i == unexp.getSize())
		expAtoms.push_back(current);
	else {
		if ((unexp.getVector()).at(i) == Atom::DONTCARE){
			(current.getVector()).at(i) = Atom::FALSE;
			expand(unexp, i + 1, current, expAtoms);
			(current.getVector()).at(i) = Atom::TRUE;
			expand(unexp, i + 1, current, expAtoms);
		}
		else {
			(current.getVector()).at(i) = (unexp.getVector()).at(i);
			expand(unexp, i + 1, current, expAtoms);
		}
	}
}
