#include <iostream>
#include <sstream>
#include <cassert>   // para hacer pruebas

#include <chrono>    // para sacar tiempos (C++11)
#include <ratio>
#include <iomanip>

#include "List.h"
#include "HashMap.h"
#include "Deque.h"

using namespace std;

// Permite sacar tiempos precisos en multiplataforma; requiere C++11
class Timer {
	std::chrono::high_resolution_clock::time_point t1;
public:
	Timer() : t1(std::chrono::high_resolution_clock::now()) {}
	friend ostream& operator<<(ostream &out, Timer &t);
};
std::ostream& operator<<(std::ostream &out, Timer &t) {
	out << std::fixed << std::setprecision(4) <<
		std::chrono::duration_cast<std::chrono::duration<double> >(
		std::chrono::high_resolution_clock::now()-t.t1).count();
		return out;
}

// Clase solicitada en enunciado
class Rubik {
	// Los ejes, en notación interna
	enum Eje { X, Y, Z };
	Eje aEje(char c) {
		switch (c) {
			case 'x' : return X;
			case 'y' : return Y;
			default : return Z;
		}
	}
	
	// Uno de los cubitos que forman el cubo
	struct Cubito {
		static const int ladosCubo = 6;
		static const char caraOculta = '.';
		
		char caras[ladosCubo];
		
		Cubito() {
			for (int i=0; i<ladosCubo; i++) caras[i] = caraOculta;
		}
		Cubito(string cs) {			
			for (int i=0; i<ladosCubo; i++) caras[i] = cs[i];
		}
		void rota(int a, int b, int c, int d) {
			char aux = caras[a];
			caras[a] = caras[b];
			caras[b] = caras[c];
			caras[c] = caras[d];
			caras[d] = aux;
		}
		void rota(Eje a, Eje b) {
			if (a == X && b == Y) rota(2, 1, 3, 0);
			if (a == Y && b == X) rota(2, 0, 3, 1);
			if (a == X && b == Z) rota(5, 0, 4, 1);
			if (a == Z && b == X) rota(5, 1, 4, 0);
			if (a == Y && b == Z) rota(4, 3, 5, 2);
			if (a == Z && b == Y) rota(4, 2, 5, 3);
		}
		char cara(Eje e, bool positivo) const {
			switch (e) {
				case X:  return positivo? caras[0] : caras[1];
				case Y:  return positivo? caras[2] : caras[3];
				default: return positivo? caras[4] : caras[5];
			}
		}
		void concatena(string &s) const {
			for (int i=0; i<ladosCubo; i++) s.append(1, caras[i]);
		}
	};

	int dim;                         // cubitos de lado
	HashMap<string, Cubito> cubitos; // coordenada "xyz"
	
	// devuelve el cubito de la coordenada x y z
	Cubito &at(int x, int y, int z) {
		ostringstream coord;
		coord << x << ',' << y << ',' << z;
		return cubitos[coord.str()];
	}	
	// version const (usada automaticamente desde metodos const)
	const Cubito &at(int x, int y, int z) const {
		ostringstream coord;
		coord << x << ',' << y << ',' << z;
		return cubitos.at(coord.str());
	}
	// rota 4 cubitos entre si (y sobre si mismos); si no-positivo, los rota al reves
	void rota(Cubito &a, Cubito &b, Cubito &c, Cubito &d, bool positivo, Eje e1, Eje e2) {
		if (positivo) {
			Cubito aux = a; a = b; b = c; c = d ; d = aux;		
		} else {
			Cubito aux = a; a = d; d = c; c = b ; b = aux;		
		}
		a.rota(e1, e2); b.rota(e1, e2); c.rota(e1, e2); d.rota(e1, e2);
	}
	// usa vuelta atras para encontrar una solucion
	List<string> resuelve(List<string> &giros, HashMap<string, int> &vistos, int prof, int &expansiones) const {
		string cadena = aCadena();
		if (prof < 0) {
			return List<string>(); // no encontrado: profundidad maxima alcanzada
		} else if (vistos.contains(cadena) && vistos.at(cadena) > prof) {
			return List<string>(); // poda: esto lo hemos visto ya antes, y con mas tiempo		
		} else {
			if (resuelto()) {
				return giros;  // eureka!
			}
			vistos.insert(cadena, prof);
		}

		// ahora, buscamos en profundidad (limitada por 'prof')
		//cout << aCadena() << " -- expandiendo a profundidad " << prof << endl;
		expansiones ++;				
		Eje posiblesGiros[][2] = {{X, Y}, {Y, Z}, {Z, X}, {Y, X}, {Z, Y}, {X, Z}};
		for (int i=0; i<6; i++) {
			string giro = "Gira x x 0";
			giro[5] += posiblesGiros[i][0];
			giro[7] += posiblesGiros[i][1];
			for (int j=0; j<dim; j++) {
				giro[9] = '0' + j;
				giros.push_back(giro);
				Rubik r = *this;
				r.gira((Eje)posiblesGiros[i][0], (Eje)posiblesGiros[i][1], j);
				List<string> resultado = r.resuelve(giros, vistos, prof-1, expansiones);
				//cout << r.aCadena() << " tras " << giro << endl;
				if ( ! resultado.empty()) {
					return resultado;
				}
				giros.pop_back();
			}
		}
		return List<string>(); // no encontrado
	}
	
	// usado en resuelve2
	struct Estado {
		string cadena;
		List<string> giros;
		int nivel;		
	};
	// usa busqueda en anchura para encontrar una solucion
	List<string> resuelve2(int prof, int &expansiones) const {
		HashMap<string, int> vistos;
		Deque<Estado> estados;
		estados.push_back({aCadena(), List<string>(), 0});
		while ( ! estados.empty()) {
			// extraemos el actual
			Estado estado = estados.front();
			estados.pop_front();
			
			// lo evaluamos
			Rubik r(dim, estado.cadena);
			if (estado.nivel > prof) {
				return List<string>(); // demasiado profundo
			} else if (vistos.contains(estado.cadena) && vistos.at(estado.cadena) < prof) {
				return List<string>(); // poda: esto lo hemos visto ya antes, y con mas tiempo		
			} else {
				if (r.resuelto()) {
					return estado.giros;  // eureka!
				}
				vistos.insert(estado.cadena, prof);
			}
			
			// expandimos el actual
			expansiones ++;
			Eje posiblesGiros[][2] = {{X, Y}, {Y, Z}, {Z, X}, {Y, X}, {Z, Y}, {X, Z}};
			for (int i=0; i<6; i++) {
				string giro = "Gira x x 0";
				giro[5] += posiblesGiros[i][0];
				giro[7] += posiblesGiros[i][1];
				for (int j=0; j<dim; j++) {
					giro[9] = '0' + j;
					Rubik r2 = r;
					r2.gira((Eje)posiblesGiros[i][0], (Eje)posiblesGiros[i][1], j);
					Estado nuevo = {r2.aCadena(), estado.giros, estado.nivel+1};
					nuevo.giros.push_back(giro);
					estados.push_back(nuevo);
				}
			}			
		}
		return List<string>(); // no resuelto
	}
	
	// O(dim^2) (por numero de cubitos en una cara) si cara externa
	// O(dim) (por perimetro de una loncha) en caso contrario
	void gira(Eje a, Eje b, int n) {
		if ((a == X && b == Y) || (a == Y && b == X)) {
			bool positivo = (a == X);
			// en una loncha de dim D hay D/2 coronas concentricas (c=0 la mas externa)
			for (int c=0; c<dim/2; c++) {
				// rota una corona: empezando por esquinas (i=0), luego adyacentes (i=1), ...
				for (int i=0; i<dim-1-2*c; i++) {
					rota(at(c+i,       c,         n), 
						 at(dim-1-c,   c+i,       n), 
						 at(dim-1-c-i, dim-1-c,   n), 
						 at(c,         dim-1-c-i, n), positivo, a, b);
				}
				// optimizacion: ignora cubitos internos sin caras visibles
				if (n != 0 && n != dim-1) break;
			}
		} else if ((a == X && b == Z) || (a == Z && b == X)) {
			bool positivo = (a == X);
			for (int c=0; c<dim/2; c++) {
				for (int i=0; i<dim-1-2*c; i++) {
					rota(at(c+i,       n, c        ), 
						 at(dim-1-c,   n, c+i      ), 
						 at(dim-1-c-i, n, dim-1-c  ), 
						 at(c,         n, dim-1-c-i), positivo, a, b);
				}
				if (n != 0 && n != dim-1) break;
			}
		} else if ((a == Y && b == Z) || (a == Z && b == Y)) {
			bool positivo = (a == Y);
			for (int c=0; c<dim/2; c++) {
				for (int i=0; i<dim-1-2*c; i++) {
					rota(at(n, c+i,       c        ), 
						 at(n, dim-1-c,   c+i      ), 
						 at(n, dim-1-c-i, dim-1-c  ), 
						 at(n, c,         dim-1-c-i), positivo, a, b);
				}
				if (n != 0 && n != dim-1) break;
			}
		} 
	}
	
public:
	
	// constructor con 1 argumento
	Rubik(int dimension) : dim(dimension) {
		for (int k=0; k<dim; k++) { // eje Z
			for (int j=0; j<dim; j++) { // eje Y
				for (int i=0; i<dim; i++) { // eje X
					Cubito &c = at(i, j, k);
					if (i == dim-1) c.caras[0] = 'a';
					if (j == dim-1) c.caras[2] = 'b';
					if (k == dim-1) c.caras[4] = 'c';
					if (i == 0)     c.caras[1] = 'd';
					if (j == 0)     c.caras[3] = 'e';
					if (k == 0)     c.caras[5] = 'f';
				}
			}
		}
	}
	
	// constructor con punto de partida
	Rubik(int dimension, string todasLasCaras) : dim(dimension) {
		istringstream lectorDeCaras(todasLasCaras);
		for (int k=0; k<dim; k++) { // eje Z
			for (int j=0; j<dim; j++) { // eje Y
				for (int i=0; i<dim; i++) { // eje X
					string caras;
					lectorDeCaras >> caras;
					at(i, j, k) = Cubito(caras); // sobreescribe con otro
				}
			}
		}
	}
		
	// O(dim^2) (por doble bucle anidado; o, geometricamente, superficie del cubo)
	bool resuelto() const {
		// miramos dos esquinas para tener un punto de partida para las caras
		char caras[] = {
			at(0,0,0).cara(X,false), at(dim-1, dim-1, dim-1).cara(X,true),
			at(0,0,0).cara(Y,false), at(dim-1, dim-1, dim-1).cara(Y,true),
			at(0,0,0).cara(Z,false), at(dim-1, dim-1, dim-1).cara(Z,true),
		};
		// comprobamos, para todos los cubos laterales, que las caras coinciden
		for (int i=0; i<dim; i++) {
			for (int j=0; j<dim; j++) {
				if (at(0,     i, j).cara(X, false) != caras[0]) return false;
				if (at(dim-1, i, j).cara(X, true)  != caras[1]) return false;
				if (at(i, 0,     j).cara(Y, false) != caras[2]) return false;
				if (at(i, dim-1, j).cara(Y, true)  != caras[3]) return false;
				if (at(i, j,     0).cara(Z, false) != caras[4]) return false;
				if (at(i, j, dim-1).cara(Z, true)  != caras[5]) return false;
			}
		}
		return true;
	}
	
	// O(dim^2) (por area de un lado; ver tambien implementacion interna)
	void gira(char e1, char e2, int n) {
		gira(aEje(e1), aEje(e2), n);
	}
	
	// O((6*dim)^n), aunque muy mejorado por podas
	List<string> resuelve(int n) const {
		List<string> resultado;
		int expansiones = 0;
		Timer t;
		for (int i=0; i<=n; i++) {
			HashMap<string, int> vistos;
			List<string> giros;
			Rubik r = *this; // hacer una copia me permite ser 'const'
			resultado = r.resuelve(giros, vistos, i, expansiones);
			if ( ! resultado.empty()) {
				break;
			}
		}
		cout << "en " << t << " segundos, tras " << expansiones << " expansiones - " << endl;
		return resultado;
	}
	
	// misma complejidad asintotica, pero algo mas eficiente
	List<string> resuelve2(int n) const {
		Timer t;
		int expansiones = 0;
		List<string> resultado = resuelve2(n, expansiones);
		cout << "en " << t << " segundos, tras " << expansiones << " expansiones - " << endl;
		return resultado;
	}
	
	// O(dim^3) (por numero de cubitos en un cubo)
	string aCadena() const {
		string s;
		for (int k=0; k<dim; k++) {
			for (int j=0; j<dim; j++) {
				for (int i=0; i<dim; i++) {
					at(i, j, k).concatena(s);
                    s.append(1, ' ');
				}
			}
		}
		return s;
	}
};

void baraja(Rubik &r, int dim, int n) {
	char posiblesGiros[][2] = {
		{'x', 'y'}, {'y', 'z'}, {'z', 'x'}, {'y', 'x'}, {'z', 'y'}, {'x', 'z'}};
		for (int i=0; i<n; i++) {
			int loncha = (rand() % dim);
			int giro = (rand() % 6); // 6 posibles giros
			r.gira(posiblesGiros[giro][0], posiblesGiros[giro][1], loncha);
			cout << "\t" << (i+1) << ". Gira " 
				 << posiblesGiros[giro][0] << " " << posiblesGiros[giro][1]
				 << " " << loncha << endl;
		}
}

void muestraSolucion(const List<string> &l) {
	if (l.empty()) {
		cout << "\t[Vacia]";
	} else {
		cout << "\tLista(" << l.size() << ") "; 
		for (List<string>::ConstIterator li=l.cbegin(); li!=l.cend(); li++) {
			cout << " -> " << *li;
		}
	}
	cout << endl;
}

int main() {
	srand(time(0)); // para que cada vez los giros aleatorios sean distintos
	int dimMin = 2;
	int dimMax = 5;
	int numRepeticionesCubo = 2;
	int maxGirosAleatorios = 3;
	int maxProfundidad = 4;
	
	for (int i=dimMin; i<=dimMax; i++) {
		Rubik r(i);
		cout << " ==== Con cubo de dimension " << i << ": " << endl << r.aCadena() << endl;		
		for (int j=0; j<numRepeticionesCubo; j++) {
			int nGiros = (rand() % maxGirosAleatorios) + 1;
			cout << "Girado, aleatoriamente, " << nGiros << " veces..." << endl;
			Rubik r2(r);
			baraja(r2, i, nGiros);
			cout << r2.aCadena() << (r2.resuelto()?"(RESUELTO!)":"(no-resuelto)") << endl;
			for (int k=0; k<maxProfundidad; k++) {
				cout << "---- Ahora con limite " << k << ":" << endl;
				cout << "     y Vuelta Atras por olas: ";
				muestraSolucion(r2.resuelve(k));
				cout << "     y Busqueda en Anchura:   ";
				muestraSolucion(r2.resuelve2(k));
			}
		}
	}
	return 0;
}
