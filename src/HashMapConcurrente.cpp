#ifndef CHM_CPP
#define CHM_CPP

#include <thread>
// alternativamente #include <pthread.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "HashMapConcurrente.hpp"

HashMapConcurrente::HashMapConcurrente() {
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        tabla[i] = new ListaAtomica<hashMapPair>();
        // Arrancamos los semaforos desbloqueados
        _semaforos[i] = new std::mutex();
    }
}

HashMapConcurrente::~HashMapConcurrente(){
    for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
        delete tabla[i];
        delete _semaforos[i];
    }
}

unsigned int HashMapConcurrente::hashIndex(std::string clave) {
    return (unsigned int)(clave[0] - 'a');
}

void HashMapConcurrente::incrementar(std::string clave) {
    unsigned int idx = hashIndex(clave);

    _semaforos[idx]->lock();

    unsigned int valor = this->valor(clave);

    if (valor == 0){
        hashMapPair nuevoPar = make_pair(clave, 1);
        tabla[idx]->insertar(nuevoPar);
    } else {
        ListaAtomica<hashMapPair>::iterator it_clave = tabla[idx]->buscar(make_pair(clave,valor));
        *it_clave = make_pair(clave,valor+1);
    }

    _semaforos[idx]->unlock();
}

std::vector<std::string> HashMapConcurrente::claves() {
    std::vector<std::string> res(0);
    for (int i = 0; i < cantLetras; i++){
        for (ListaAtomica<hashMapPair>::iterator it_actual = tabla[i]->begin(); it_actual != tabla[i]->end(); it_actual++){
            res.push_back((*it_actual).first);
        }
    }
    return res;
}

unsigned int HashMapConcurrente::valor(std::string clave) {
    ListaAtomica<hashMapPair> * listaDicc = tabla[hashIndex(clave)];
    for (ListaAtomica<hashMapPair>::iterator it = listaDicc->begin(); it != listaDicc->end(); it++){
        if((*it).first == clave){
            return (*it).second;
        }
    }
    return 0;
}

hashMapPair HashMapConcurrente::maximo() {
    hashMapPair res{"",0};

    for (unsigned int index = 0; index < HashMapConcurrente::cantLetras; index++) {
        this->_semaforos[index]->lock();
        for (auto &p : *tabla[index]) {
            if (p.second > res.second) {
                res = p;
            }
        }
    }
    for(auto s:_semaforos){
        s->unlock();
    }
    return res;
}



hashMapPair HashMapConcurrente::maximoParalelo(unsigned int cant_threads) {
    if(cant_threads > cantLetras)throw("invalid input");

    std::thread* hilos[cant_threads];

    std::atomic<int> proximaFila(0);    
    std::vector<hashMapPair> maxFila(cantLetras);

    for(unsigned int i = 0; i < cant_threads; i++){
        hilos[i] = new std::thread(&HashMapConcurrente::maximoAux,this,std::ref(proximaFila), std::ref(maxFila));
    }

    for(unsigned int i = 0; i < cant_threads; i++){
        hilos[i]->join();
        delete hilos[i];        
    }

    hashMapPair res = maxFila[0];
    for(int i = 0; i < cantLetras; i ++){
        if (maxFila[i].second > res.second) {
                res = maxFila[i];
        }
    }

    return res;

}
void HashMapConcurrente::maximoAux(std::atomic<int>& proximaFila, std::vector<hashMapPair>& maxFila){
    while(1){
        int filaActual = proximaFila.fetch_add(1);
        if(filaActual >= cantLetras) break;
        hashMapPair *max = new hashMapPair();
        max->second = 0;
        for (auto &p : *(this->tabla[filaActual])) {
            if (p.second > max->second) {
                max->first = p.first;
                max->second = p.second;
            }
        }
        maxFila[filaActual] = *max;
        delete max;
    }

}

#endif
