using namespace std;
#include <iostream>
#include <string>
#include "List.h"


const int tamInicial = 2;
const int prof = 3;
// GRUPO ==> prEDAtors                                        (Rafa no toques)


class Cubilete{
private:
	char caras[6];
public:
	Cubilete(){
		caras[0]= '.';
		caras[1]= '.';
		caras[2]= '.';
		caras[3]= '.';
		caras[4]= '.';
		caras[5]= '.';
	}
	



	char const getCara(int pos){//O(1)
		if ((pos<0)||(pos>5)) return 'z';
		return caras[pos];
	}

	void setCara(char c, int pos){//O(1)
		if ((pos<0)||(pos>5)) return;
		caras[pos]= c;
	}
	void girarXY(){
		char aux[6] = {caras[0],caras[1],caras[2],caras[3],caras[4],caras[5]};
		caras[0] = aux[3];
		caras[1] = aux[2];
		caras[2] = aux[0];
		caras[3] = aux[1];
		// caras 4 y 5 no cambian
	}
	void girarYX(){
		char aux[6] = {caras[0],caras[1],caras[2],caras[3],caras[4],caras[5]};
		caras[0] = aux[2];
		caras[1] = aux[3];
		caras[2] = aux[1];
		caras[3] = aux[0];
		// caras 4 y 5 no cambian
	}
	void girarXZ(){
		char aux[6] = {caras[0],caras[1],caras[2],caras[3],caras[4],caras[5]};
		caras[0] = aux[5];
		caras[1] = aux[4];
		// caras 2 y 3 no cambian
		caras[4] = aux[0];
		caras[5] = aux[1];	
	}
	void girarZX(){
		char aux[6] = {caras[0],caras[1],caras[2],caras[3],caras[4],caras[5]};
		caras[0] = aux[4];
		caras[1] = aux[5];
		// caras 2 y 3 no cambian
		caras[4] = aux[1];
		caras[5] = aux[0];	
	}
	void girarYZ(){
		char aux[6] = {caras[0],caras[1],caras[2],caras[3],caras[4],caras[5]};
		// caras 0 y 1 no cambian
		caras[2] = aux[5];
		caras[3] = aux[4];
		caras[4] = aux[2];
		caras[5] = aux[3];	
	}
	void girarZY(){
		char aux[6] = {caras[0],caras[1],caras[2],caras[3],caras[4],caras[5]};
		// caras 0 y 1 no cambian
		caras[2] = aux[4];
		caras[3] = aux[5];
		caras[4] = aux[3];
		caras[5] = aux[2];	
	}


};

class Rubik{

public:
	Rubik(int tam, string s){
		size = tam;
		int recorrer = 0;
		for (int k = 0; k<tam;k++){
			for (int j = 0; j<tam;j++){
				for (int i = 0; i<tam;i++){
					cubo[i][j][k] = Cubilete();
					for (int m = 0; m < 6;m++){
						ponCaraACubilete(i,j,k,s[recorrer],m);
						recorrer++;
					}
					recorrer++;
				}
			}
		}			
	}

	Cubilete const getCubilete(int i, int j, int k){
		return cubo[i][j][k];
	}
	void setCubilete(int i, int j, int k, Cubilete c){
		cubo[i][j][k] = c;
	}
	void ponCaraACubilete(int i, int j, int k, char c, int cara){
		cubo[i][j][k].setCara(c, cara);
	}
	char  const getCaraCubilete(int i, int j, int k, int cara){
		return cubo[i][j][k].getCara(cara);
	}

	string const aCadena(){
		//O(n^2) con n = número de cubiletes por lado 
		string ret= "";
		for (int k = 0; k<tamInicial;k++){
			for (int j = 0; j<tamInicial;j++){
				for (int i = 0; i<tamInicial;i++){
					for (int m = 0; m < 6;m++){
						char c = cubo[i][j][k].getCara(m);
						ret += c;
					}
					ret+=" ";
				}
			}
		}
		return ret;
	}		
	void gira(char eje1, char eje2, int nivel){
		//O(n^2) con n = numero de cubiletes por lado
		if ((eje1=='x')&&(eje2=='y')){
			giraXY(nivel);
		}
		if ((eje1=='y')&&(eje2=='x')){
			giraYX(nivel);
		}
		if ((eje1=='x')&&(eje2=='z')){
			giraXZ(nivel);
		}
		if ((eje1=='z')&&(eje2=='x')){
			giraZX(nivel);
		}
		if ((eje1=='z')&&(eje2=='y')){
			giraZY(nivel);
		}
		if ((eje1=='y')&&(eje2=='z')){
			giraYZ(nivel);
		}
		return;
	}
	bool const resuelto(){
		//0(n^2) con n = numero de cubiletes por lado.
		return (cara0Resuelta() && cara1Resuelta() && cara2Resuelta() && cara3Resuelta() && cara4Resuelta() && cara5Resuelta());
	}
	List<string> const resuelve(string s){
		//O(6^n) con n = la profundidad de la búsqueda.
		List<string> movimientos;
		Rubik r(tamInicial,s);
		mejor = prof;
		//  for (int i = 0; i <= 6; i++)
		//	movimientos.push_front("solucion mala");
		//  movimientos.push_back("Iniciamos los movimientos ");
		return resuelveRec(r, movimientos); 
	}

private:
	int size;
	Cubilete cubo[tamInicial][tamInicial][tamInicial];
	int mejor;

	List<string> resuelveRec(Rubik r, List<string> movimientos){
	// prueba con cada hijo del nivel actual
    //if(/* si la ultima operacion no es yx, y no ha hecho 2 xy seguidos*/)
		List<string> ret, aux;
		//ret = movimientos;
		for (int i = 0; i <= prof; i++){
			ret.push_front("solucion mala");
		}
		if (r.resuelto()){
			//cout << "encuentra una solucion de profundidad " << movimientos.size() << endl;
			return movimientos;
			mejor = movimientos.size();
			

		}else if (movimientos.size()> mejor){
				//cout << "entra aqui con profundidad" << prof << endl;
				//return movimientos;
			}else{


		for (int i = 0 ; i < tamInicial; i++){     
			if (movimientos.empty()||(movimientos.back() != "Gira x y "+to_string(i)) && ((movimientos.back() != "Gira y x "+to_string(i)))){ 
				//cout << "probamos y x " << i <<" con profundidad " << prof; 
				//cout << endl;
				string s = "Gira y x ";
				s += to_string(i);
				//bool repetir = (movimientos.back() == s);
				r.gira('y','x',i);
				movimientos.push_back(s);
				aux = resuelveRec(r, movimientos);
				if (aux.size() < ret.size()) ret = aux;
				movimientos.pop_back();
				r.gira('x','y',i);
			}
		}

		for (int i = 0 ; i < tamInicial; i++){     
			if (movimientos.empty()||(movimientos.back() != "Gira y x "+to_string(i)) && ((movimientos.back() != "Gira x y "+to_string(i)))){ 
				//cout << "probamos x y " << i <<" con profundidad " << prof; 
				//cout << endl;
				r.gira('x','y',i);
				string s = "Gira x y ";
				s += to_string(i);
				//bool repetir = (movimientos.back() == s);
				movimientos.push_back(s);
				aux = resuelveRec(r, movimientos);
				if (aux.size() < ret.size()) ret = aux;
				movimientos.pop_back();
				r.gira('y','x',i);
		
			}
		}
		for (int i = 0 ; i < tamInicial; i++){     
			if (movimientos.empty()||(movimientos.back() != "Gira x z "+to_string(i)) && ((movimientos.back() != "Gira z x "+to_string(i)))){
				//cout << "probamos z x " << i <<" con profundidad " << prof; 
				//cout << endl;
				r.gira('z','x',i);
				string s = "Gira z x ";
				s += to_string(i);
				//bool repetir = (movimientos.back() == s);
				movimientos.push_back(s);
				aux = resuelveRec(r, movimientos);
				if (aux.size() < ret.size()) ret = aux;
				movimientos.pop_back();
				r.gira('x','z',i);			
		
			}
		}
		for (int i = 0 ; i < tamInicial; i++){     
			if (movimientos.empty()||(movimientos.back() != "Gira z x "+to_string(i)) && ((movimientos.back() != "Gira x z "+to_string(i)))){ 
				//cout << "probamos x z " << i <<" con profundidad " << prof; 
				//cout << endl;
				r.gira('x','z',i);
				string s = "Gira x z ";
				s += to_string(i);
				//bool repetir = (movimientos.back() == s); 
				movimientos.push_back(s);
				aux = resuelveRec(r, movimientos);
				if (aux.size() < ret.size()) ret = aux;
				r.gira('z','x',i);
				movimientos.pop_back();
			}
		}
		for (int i = 0 ; i < tamInicial; i++){     
			if (movimientos.empty()||(movimientos.back() != "Gira z y "+to_string(i)) && ((movimientos.back() != "Gira y z "+to_string(i)))){ 
				//cout << "probamos z y " << i <<" con profundidad " << prof; 
				//cout << endl;
				r.gira('y','z',i);
				string s = "Gira z y ";
				s += to_string(i);
				//bool repetir = (movimientos.back() == s);
				movimientos.push_back("Gira y z "+ i);
				aux = resuelveRec(r, movimientos);
				if (aux.size() < ret.size()) ret = aux;
				movimientos.pop_back();
				r.gira('y','z',i);
			}
		}
		for (int i = 0 ; i < tamInicial; i++){     
			if (movimientos.empty()||(movimientos.back() != "Gira y z "+to_string(i)) && ((movimientos.back() != "Gira z y "+to_string(i)))){ 
				//cout << "probamos y z " << i <<" con profundidad " << prof; 
				//cout << endl;
				r.gira('z','y',i);
				string s = "Gira z y ";
				s += to_string(i);
				//bool repetir = (movimientos.back() == s);
				movimientos.push_back(s);
				aux = resuelveRec(r, movimientos);
				if (aux.size() < ret.size()) ret = aux;
				movimientos.pop_back();
				r.gira('y','z',i);			
			}
		}		
		
		
		}
		return ret;
	
		
	}

	void giraXY(int nivel){
		Cubilete aux; 
		aux = getCubilete(0,0,nivel);
		Cubilete nuevo;
		
		nuevo = getCubilete(0,1,nivel);
		nuevo.girarXY();
		setCubilete(0,0,nivel, nuevo);
		
		nuevo = getCubilete(1,1,nivel);
		nuevo.girarXY();
		setCubilete(0,1,nivel, nuevo);

		nuevo = getCubilete(1,0,nivel);
		nuevo.girarXY();
		setCubilete(1,1,nivel, nuevo);

		nuevo = aux;
		nuevo.girarXY();
		setCubilete(1,0,nivel, nuevo);
	}	
	void giraYX(int nivel){
		Cubilete aux; 
		aux = getCubilete(0,0,nivel);
		Cubilete nuevo;
		
		nuevo = getCubilete(1,0,nivel);
		nuevo.girarYX();
		setCubilete(0,0,nivel, nuevo);
		
		nuevo = getCubilete(1,1,nivel);
		nuevo.girarYX();
		setCubilete(1,0,nivel, nuevo);

		nuevo = getCubilete(0,1,nivel);
		nuevo.girarYX();
		setCubilete(1,1,nivel, nuevo);

		nuevo = aux;
		nuevo.girarYX();
		setCubilete(0,1,nivel, nuevo);
	}		
	void giraXZ(int nivel){
		Cubilete aux; 
		aux = getCubilete(0,nivel,0);
		Cubilete nuevo;
		
		nuevo = getCubilete(0,nivel,1);
		nuevo.girarXZ();
		setCubilete(0,nivel,0, nuevo);
		
		nuevo = getCubilete(1,nivel,1);
		nuevo.girarXZ();
		setCubilete(0,nivel,1, nuevo);

		nuevo = getCubilete(1,nivel,0);
		nuevo.girarXZ();
		setCubilete(1,nivel,1, nuevo);

		nuevo = aux;
		nuevo.girarXZ();
		setCubilete(1,nivel,0, nuevo);
	}	
	void giraZX(int nivel){
		Cubilete aux; 
		aux = getCubilete(0,nivel,0);
		Cubilete nuevo;
		
		nuevo = getCubilete(1,nivel, 0);
		nuevo.girarZX();
		setCubilete(0,nivel,0, nuevo);
		
		nuevo = getCubilete(1,nivel,1);
		nuevo.girarZX();
		setCubilete(1,nivel,0, nuevo);

		nuevo = getCubilete(0,nivel,1);
		nuevo.girarZX();
		setCubilete(1,nivel,1, nuevo);

		nuevo = aux;
		nuevo.girarZX();
		setCubilete(0,nivel,1, nuevo);
	}		
	void giraYZ(int nivel){
		Cubilete aux; 
		aux = getCubilete(nivel,0,0);
		Cubilete nuevo;
		
		nuevo = getCubilete(nivel,0,1);
		nuevo.girarYZ();
		setCubilete(nivel,0,0, nuevo);
		
		nuevo = getCubilete(nivel,1,1);
		nuevo.girarYZ();
		setCubilete(nivel,0,1, nuevo);

		nuevo = getCubilete(nivel,1,0);
		nuevo.girarYZ();
		setCubilete(nivel,1,1, nuevo);

		nuevo = aux;
		nuevo.girarYZ();
		setCubilete(nivel,1,0, nuevo);
	}	
	void giraZY(int nivel){
		Cubilete aux; 
		aux = getCubilete(nivel,0,0);
		Cubilete nuevo;
		
		nuevo = getCubilete(nivel,1,0);
		nuevo.girarZY();
		setCubilete(nivel,0,0, nuevo);
		
		nuevo = getCubilete(nivel,1,1);
		nuevo.girarZY();
		setCubilete(nivel,1,0, nuevo);

		nuevo = getCubilete(nivel,0,1);
		nuevo.girarZY();
		setCubilete(nivel,1,1, nuevo);

		nuevo = aux;
		nuevo.girarZY();
		setCubilete(nivel,0,1, nuevo);
	}	

	bool const cara0Resuelta(){
		char c = getCaraCubilete(1,0,0,0);
		bool ret = true;
			if (c != getCaraCubilete(1,1,0,0)) ret = false;
			if (c != getCaraCubilete(1,0,1,0)) ret = false;
			if (c != getCaraCubilete(1,1,1,0)) ret = false;
		//if (!ret) cout << "La cara 0 no esta resuelta" << endl;
		return ret;
	}
	bool const cara1Resuelta(){
		char c = getCaraCubilete(0,0,0,1);
		bool ret = true;
			if (c != getCaraCubilete(0,1,0,1)) ret = false;
			if (c != getCaraCubilete(0,0,1,1)) ret = false;
			if (c != getCaraCubilete(0,1,1,1)) ret = false;
		//if (!ret) cout << "La cara 1 no esta resuelta" << endl;
		return ret;
	}
	bool const cara2Resuelta(){
		char c = getCaraCubilete(0,1,0,2);
		bool ret = true;
			if (c != getCaraCubilete(1,1,0,2)) ret = false;
			if (c != getCaraCubilete(0,1,1,2)) ret = false;
			if (c != getCaraCubilete(1,1,1,2)) ret = false;
		//if (!ret) cout << "La cara 2 no esta resuelta" << endl;
		return ret;
	}
	bool const cara3Resuelta(){
		char c = getCaraCubilete(0,0,0,3);
		bool ret = true;
			if (c != getCaraCubilete(1,0,0,3)) ret = false;
			if (c != getCaraCubilete(0,0,1,3)) ret = false;
			if (c != getCaraCubilete(1,0,1,3)) ret = false;
		//if (!ret) cout << "La cara 3 no esta resuelta" << endl;
		return ret;
	}
	bool const cara4Resuelta(){
		char c = getCaraCubilete(0,0,1,4);
		bool ret = true;
			if (c != getCaraCubilete(1,0,1,4)) ret = false;
			if (c != getCaraCubilete(0,1,1,4)) ret = false;
			if (c != getCaraCubilete(1,1,1,4)) ret = false;
		//if (!ret) cout << "La cara 4 no esta resuelta" << endl;
		return ret;
	}
	bool const cara5Resuelta(){
		char c = getCaraCubilete(0,0,0,5);
		bool ret = true;
			if (c != getCaraCubilete(1,0,0,5)) ret = false;
			if (c != getCaraCubilete(0,1,0,5)) ret = false;
			if (c != getCaraCubilete(1,1,0,5)) ret = false;
		//if (!ret) cout << "La cara 5 no esta resuelta" << endl;
		return ret;
	}




};


int main(){
	Rubik r = Rubik(2,".d.e.f a..e.f .db..f a.b..f .d.ec. a..ec. .db.c. a.b.c.");
	
	string sprueba = r.aCadena();
	cout << sprueba << endl;
	
	if(r.resuelto()) cout << "RESUELTO" << endl; else cout << "NO RESUELTO" << endl; 
	//r.gira('x','y',0);
	//r.gira('x','z',1); 
	r.gira('y','z',0); 
	r.gira('x','z',0); 
	List<string> solu = r.resuelve(r.aCadena());
	//cout << "Sale de resuelve" << endl; 
	cout << "Ha encontrado una solucion con numero de movimientos: " << solu.size() << endl; 
	List<string>::ConstIterator it = solu.cbegin();
	while (it!=solu.cend()){
		cout << it.elem() << endl;
		it.next();
	}

	int n;
	cin >> n; 
	return n;
}

