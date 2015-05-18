#include <iostream>
#include <sstream>
#include "Hash.h"
#include "HashMap.h"


using namespace std;

const int N = 6;
const int TAM = 100; 
int const M = 4;

class Cubito{
	

	char cu[N];

	void swap(int a, int b, int c, int d){
		int aux = cu[a]; cu[a] = cu[b]; cu[b] = cu[c]; cu[c] = cu[d]; cu[d] = aux;
	}

public:
	Cubito(){}	
	
	bool contains(char x){
		for (int i = 0; i < N;i++) 
			if(cu[i]==x) 
				return true;
		
		return false;		
	}
	bool parecido(Cubito x){
		for(int i = 0; i < N; i++){
			if(x.contains('a') && cu.contains('a')) return true; // azul
			else if(x.contains('r') && cu.contains('r')) return true; //rojo
			else if(x.contains('v') && cu.contains('v')) return true; //verde
			else if(x.contains('n') && cu.contains('n')) return true; //naranja
			else if(x.contains('y') && cu.contains('y')) return true; //amarillo
			else if(x.contains('b') && cu.contains('b')) return true; // blanco
		}
		return false;
	}
	/*
	 * En funcion de los valores de los ejes llamaremos a swap con los parametros correspondientes
	 */
	void dir(char eje1, char eje2){
		if(eje1 == 'x' && eje2 == 'y') swap(0, 2, 1, 3);
		else if(eje1 == 'y' && eje2 == 'x') swap(3, 1, 2, 0);
		else if(eje1 == 'x' && eje2 == 'z') swap(0, 4, 1, 5);
		else if(eje1 == 'z' && eje2 == 'x') swap(5, 1, 4, 0);
		else if(eje1 == 'y' && eje2 == 'z') swap(2, 4, 3, 5);
		else if(eje1 == 'z' && eje2 == 'y') swap(5, 3, 4, 2);
	}

	void rellenaCubito(string s){
		for(int i= 0; i < N; i++){
			cu[i] = s[i];
		}
	}

	string aCadenaCubito(){
		string estado;
		for(int i = 0; i < N; i++) estado += cu[i];
		estado += ' ';
		return estado;
	}

};

struct Giro{
	char eje1, eje2;
	int nivel;

	Giro(char _eje1, char _eje2, int _nivel){
		eje1 = _eje1;
		eje2 = _eje2;
		nivel = _nivel;
	}

	Giro(){}

};

struct ListaGiro{
	
	Giro giros[TAM];
	int cont;

	ListaGiro(){
		cont = 0;
	}

	void inserta(Giro g, int i){
		giros[i] = g;
		cont++;
	} 

	void muestra(){
		for(int i = cont-1; i >= 0; i--) cout << giros[i].eje1 << ', ' << giros[i].eje2 << ', ' << giros[i].nivel << endl;
	}
};

class Rubik{
	
	//Cubito cub[M];
	HashMap<int,cubito> cub;
	
	int dim;

	void swap(int a, int b, int c, int d){
		Cubito aux = cub[a]; cub[a] = cub[b]; cub[b] = cub[c]; cub[c] = cub[d]; cub[d] = aux;
	}


	Giro eligeGiro(int m){
		Giro g;
		int n = 6 % m;
		if(n == 5){
			g.eje1 = 'x';
			g.eje2 = 'y';
		}else if(n == 4){
			g.eje1 = 'y';
			g.eje2 = 'x';
		}else if(n == 3){
			g.eje1 = 'x';
			g.eje2 = 'z';
		}else if(n == 2){
			g.eje1 = 'z';
			g.eje2 = 'x';
		}else if(n == 1){
			g.eje1 = 'y';
			g.eje2 = 'z';
		}else if(n == 0){
			g.eje1 = 'z';
			g.eje2 = 'y';
		}
		g.nivel = (m / 6) - 1;
		return g;
	}

	bool const resuelto(){
		for (int i = 0; i < M-1; i++){
			if(!cub[i].parecido(cub[i +1]) || !cub[i+M].parecido(cub[i+1+M])) return false;
		}
		return true;
	}

public:

	Rubik(int _dim, string s){ 
		dim = _dim;
		stringstream aux;
		aux << s;
		string cu;
		for(int i = 0; i < dim*dim*dim; i++){
			aux >> cu;
			cub[i].rellenaCubito(cu);
		}
	}
	
	void gira(char eje1, char eje2, int nivel){
		if(nivel == 0){
			if(eje1 == 'x' && eje2 == 'y') swap(0, 1, 3, 2);
			else if(eje1 == 'y' && eje2 == 'x') swap(2, 3, 1, 0);
			else if(eje1 == 'x' && eje2 == 'z') swap(0, 1, 5, 4);
			else if(eje1 == 'z' && eje2 == 'x') swap(4, 5, 1, 0);
			else if(eje1 == 'y' && eje2 == 'z') swap(0, 2, 6, 4);
			else if(eje1 == 'z' && eje2 == 'y') swap(4, 6, 2, 0);
		}else if(nivel == 1){
			if(eje1 == 'x' && eje2 == 'y') swap(4, 5, 7, 6);
			else if(eje1 == 'y' && eje2 == 'x') swap(6, 7, 5, 4);
			else if(eje1 == 'x' && eje2 == 'z') swap(2, 3, 7, 6);
			else if(eje1 == 'z' && eje2 == 'x') swap(6, 7, 3, 2);
			else if(eje1 == 'y' && eje2 == 'z') swap(1, 3, 7, 5);
			else if(eje1 == 'z' && eje2 == 'y') swap(5, 7, 3, 1);
		}
	}

	
	
	string const aCadena(){
		string estado;
		for(int i = 0; i < M; i++) estado += cub[i].aCadenaCubito();
		return estado;
	}

	void const resuelve(int k, int kLim, ListaGiro giros){
		if(resuelto() || k == 0) return;
		Giro g;
		for(int i = 0; i < 6*(dim+1); i++){
			g = eligeGiro(i);
			gira(g.eje1, g.eje2, g.nivel);

			if(resuelto()) giros.inserta(g, k);
			else if(giros.cont != 0) giros.inserta(g, k);
			else if(k == kLim);
			else resuelve(k+1, kLim,giros);
		}
	}

};


int main (){
	string s;
	int dim = 2;
	int profundidad;
	cin >> profundidad;
	getline(cin, s);
	Rubik r(dim, s);
	ListaGiro l;
	r.resuelve(0, profundidad, l);
	l.muestra();
	r.aCadena();
	return 0;
}