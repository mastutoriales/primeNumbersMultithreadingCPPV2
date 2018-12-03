#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <cstdio>
#include <cstdint>
#include "ListaLigada.h"
#include "Bloqueador.h"
#include <thread>

using namespace std;


ListaLigada* primos;
ListaLigada primosN;
int64_t encontrar;
int64_t encontrados=0;
int64_t nthreads;
int nthread=0;
int64_t u;
int64_t maxValue;
Bloqueador semaforo1;
Bloqueador semaforo2;

int64_t maximo(int64_t numero){
    return (int64_t) ceil(sqrt(numero));
}
void run(){
	semaforo1.lock();
	int id=nthread;
	nthread++;
	semaforo1.unlock();
	Nodo *t;
    int64_t m;
    bool primo;
	int64_t comprobar;
	while(encontrados<encontrar){
		semaforo1.lock();
		comprobar=u;
		u+=2;
		semaforo1.unlock();
		primo=true;
		m = maximo(comprobar);
		t =primosN.head;
		while(t!=NULL&&t->dato<=m){
	    	if(comprobar%t->dato==0){
	        	primo=false;
	        	break;
	        }
	        t=t->sig;
    	}
		t=NULL;
        if(primo){
        	semaforo2.lock();
            encontrados++;
            semaforo2.unlock();
			primos[id].agregar(comprobar);
        }
	}
}


int limite(int cantidad, int64_t ultimo){
	return (int)(3.528*pow((int64_t)cantidad+(int64_t)(0.2*pow(ultimo,0.937)),0.5266))+1;
	
}

void generarPrimos(int limite){
	primosN = ListaLigada();
	primosN.agregar(2);
	primosN.agregar(3);
	primosN.agregar(5);
	int max;
	bool flag;
	for(int i = 7; i<=limite;i++){
		max= maximo(i);
		flag = true;
		Nodo* t = primosN.head;
		while(t->dato<=max && t != NULL){
			if(i%t->dato == 0){
				flag = false;
				break;
			}
			t=t->sig;
		}
		t=NULL;
		if(flag)primosN.agregar(i);
	}
}
bool estaVacia(){
	for(int64_t i=0;i<nthreads;i++){
		if(!primos[i].vacia()) return false;
	}
	return true;
}
void guardar(string salida){
	stringstream ss;
	int64_t min;
	int64_t pos;	
	while(!estaVacia()){
		min= 9223372036854775807;
		pos=-1;
		for(int64_t i=0;i<nthreads;i++){
			if(primos[i].head!=NULL&&primos[i].head->dato<min){
				min=primos[i].head->dato;
				pos=i;
			}
		}
		maxValue = primos[pos].eliminar();
		ss<<maxValue<<endl;
	}
	ofstream fichero;
	salida+=".txt";
	fichero.open(salida.c_str());
	fichero<<ss.str();
	fichero.close();
	
}
string tiempo(float segundos){
	float m=0,h=0,d=0;
	if(segundos>86400){
		float r = fmod(segundos,86400);
		d=(segundos-r)/86400;
			segundos-=(d*86400);
	}
	if(segundos>3600){
		float r = fmod(segundos,3600);
		h=(segundos-r)/3600;
		segundos-=(h*3600);
	}
	if(segundos>60){
		float r = fmod(segundos,60);
		m=(segundos-r)/60;
		segundos-=(m*60);
	}
	stringstream ss;
	if(d>0){
		if(d>1)ss<<d<<" Dias ";
		else ss<<d<<" Dia ";
	}
	if(h>0){
		if(h>1)ss<<h<<"  Horas ";
		else ss<<h<<"  Hora ";
	}
	if(m>0){
		if(m>1)ss<<m<<"  Minutos ";
		else ss<<m<<"  Minuto ";
	}
	if(segundos>0){
		if(segundos>1)ss<<segundos<<"  Segundos ";
		else ss<<segundos<<"  Segundo ";
	}
	return ss.str();
}
int64_t inicioN(int64_t inicio){
	if(inicio%2 == 0)inicio--;
	return inicio;
}
int main() {
	nthreads = std::thread::hardware_concurrency();
	primos = new ListaLigada[nthreads];
    string nombre;
    int64_t inicio;
    cout<<"Cuantos numeros primos desea encontrar?: ";
    cin>>encontrar;
    cout<<"Ingrese el nombre del fichero de salida: ";
    cin>>nombre;
    cout<<"Ingrese el numero de inicio: ";
	cin>>inicio;
	u = inicioN(inicio);
	generarPrimos(limite(encontrar,u));
	cout<<"Busqueda Iniciada"<<endl;
	clock_t busquedai = clock();
	thread* threadlist = new thread[nthreads];
	for(int64_t i=0;i<nthreads;i++){
		threadlist[i] = thread(run);
	}
	for(int64_t i = 0;i < nthreads;i++) {
		threadlist[i].join();
	}
	clock_t busquedaf = clock();
	cout<<"Busqueda Terminada"<<endl;
	cout<<"Iniciando Guardado"<<endl;
	clock_t guardari = clock();
	guardar(nombre);
	clock_t guardarf = clock();
	cout<<"Guardado Terminado"<<endl;
	cout<<"Tiempo Busqueda Primos: "<<tiempo((busquedaf - busquedai) / (float)CLOCKS_PER_SEC)<<endl;
    cout<<"Tiempo Guardado Primos: "<<tiempo((guardarf - guardari) / (float)CLOCKS_PER_SEC)<<endl;
    cout<<"Tiempo Total: "<<tiempo((guardarf - busquedai) / (float)CLOCKS_PER_SEC)<<endl;
    if(maximo(maxValue)<= primosN.tail->dato)cout<<"Comprobacion Valida"<<endl;
	else cout<<"Comprobacion Invalida"<<endl;
    cout << "Presione ENTER para Salir: ";
    cin.ignore().get();
}
