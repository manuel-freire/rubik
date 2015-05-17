#include <iostream>
#include <sstream>
#include <string>
#include "List.h"
#include "HashMap.h"
#include "Exceptions.h"
using namespace std;

struct Cubo {
	char cara[6];
	void swap(char v[], int a, int b, int c, int d) {
		int aux = v[a]; v[a] = v[b]; v[b] = v[c]; v[c] = v[d]; v[d] = aux;
	}
	void gira(char e1, char e2) {
		if (e1 == 'x' && e2 == 'y') swap(cara, 0, 2, 1, 3);
		if (e1 == 'y' && e2 == 'x') swap(cara, 0, 3, 1, 2);
		if (e1 == 'x' && e2 == 'z') swap(cara, 0, 4, 1, 5);
		if (e1 == 'z' && e2 == 'x') swap(cara, 0, 5, 1, 4);
		if (e1 == 'y' && e2 == 'z') swap(cara, 4, 3, 5, 2);
		if (e1 == 'z' && e2 == 'y') swap(cara, 4, 2, 5, 3);
	}
	void rellena(char a, char b, char c, char d, char e, char f) {
		cara[0] = a;
		cara[1] = b;
		cara[2] = c;
		cara[3] = d;
		cara[4] = e;
		cara[5] = f;

	}
	string toString() {
		string sol = "";
		sol = cara[0];
		sol = sol + cara[1] + cara[2] + cara[3] + cara[4] + cara[5];
		return sol;
	}
};

class Rubik {
	private:
		int tam;
		HashMap<int, Cubo> puzzle;
		void swapCubos(int a, int b, int c, int d) {
			Cubo cubo = puzzle[a];
			puzzle[a] = puzzle[b];
			puzzle[b] = puzzle[c];
			puzzle[c] = puzzle[d];
			puzzle[d] = cubo;
		}
	public:
		Rubik(int n, string s);
		string aCadena();
		void gira(char e1, char e2, int n);
		List<string> resuelve(int n);
		bool resuelto();
};

// Constructor
Rubik::Rubik(int n, string s) {
	tam = n;
	int numcubos = n*n*n;
	istringstream iss(s);
	string sub;
	for (int i = 0; i < numcubos; i++) {
		iss >> sub;
		Cubo c;
		c.rellena(sub[0], sub[1], sub[2], sub[3], sub[4], sub[5]);
		puzzle.insert(i, c);
	}
}

// Devuelve una cadena con el estdo del cubo
string Rubik::aCadena() {
	string sol = "";
	int num = tam*tam*tam;
	for (int i = 0; i < num; i++) {
		sol += puzzle[i].toString() + " ";
	}
	return sol;
}

// Gira la loncha correspondiente
void Rubik::gira(char e1, char e2, int n) {
	if ((e1 == 'x' && e2 == 'y') || (e1 == 'y' && e2 == 'x')) {
		int a, b, c, d;
		a = 0 + (n * 4);
		b = 1 + (n * 4);
		c = 3 + (n * 4);
		d = 2 + (n * 4);
		puzzle[a].gira(e1, e2);
		puzzle[b].gira(e1, e2);
		puzzle[c].gira(e1, e2);
		puzzle[d].gira(e1, e2);
		if(e1 == 'x') swapCubos(a, b, c, d);
		else swapCubos(a, d, c, b);
	}
	if ((e1 == 'x' && e2 == 'z') || (e1 == 'z' && e2 == 'x')) {
		int a, b, c, d;
		a = 0 + (n * 2);
		b = 1 + (n * 2);
		c = 5 + (n * 2);
		d = 4 + (n * 2);
		puzzle[a].gira(e1, e2);
		puzzle[b].gira(e1, e2);
		puzzle[c].gira(e1, e2);
		puzzle[d].gira(e1, e2);
		if (e1 == 'x') swapCubos(a, b, c, d);
		else swapCubos(a, d, c, b);
	}
	if ((e1 == 'y' && e2 == 'z') || (e1 == 'z' && e2 == 'y')) {
		int a, b, c, d;
		a = 2 + n;
		b = 0 + n;
		c = 4 + n;
		d = 6 + n;
		puzzle[a].gira(e1, e2);
		puzzle[b].gira(e1, e2);
		puzzle[c].gira(e1, e2);
		puzzle[d].gira(e1, e2);
		if(e1 == 'y') swapCubos(a, b, c, d);
		else swapCubos(a, d, c, b);
	}
}

// Lista con los movimientos para resolver el puzzle
List<string> Rubik::resuelve(int n) {
	List<string> s;
	return s;
}

// Comprueba que está resuelto el puzzle
bool Rubik::resuelto() {
	return false;
}

void main() {
	Rubik r = Rubik(2, ".d.e.f a..e.f .db..f a.b..f .d.ec. a..ec. .db.c. a.b.c.");
	cout << r.aCadena();
	int n;
	cin >> n;
}
