#ifndef LISTALIGADA_H
#define LISTALIGADA_H
#include "Nodo.h"
#include <cstdlib>
#include <iostream>

class ListaLigada
{
	private:
		Nodo*  head;
		Nodo* tail;
		Nodo* pos;
		Nodo* t;
	public:
		friend void run();
		friend void guardar(std::string);
		friend void generarPrimos(int);
		friend int64_t maxCarga();
		friend int main();
		ListaLigada();
		~ListaLigada();
		void agregar (int64_t);
		int64_t eliminar();
		bool vacia();
		int64_t valor();
		void reiniciar();
		
};

#endif
