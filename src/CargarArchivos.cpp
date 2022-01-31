#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
//#include <pthread.h>
#include <thread>

#include "CargarArchivos.hpp"

int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
) {
    std::fstream file;
    int cant = 0;
    std::string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo '" << filePath << "'" << std::endl;
        return -1;
    }
    
    while (file >> palabraActual) {
        hashMap.incrementar(palabraActual);
        cant++;
    }
    // Cierro el archivo.
    
    if (!file.eof()) {
        std::cerr << "Error al leer el archivo" << std::endl;
        file.close();
        return -1;
    }
    
    file.close();
    return cant;
}


void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
) {
    // Completar (Ejercicio 4)
    //if(cantThreads > filePaths.size())throw("invalid input");

    std::thread* hilos[cantThreads];
    std::atomic<unsigned int> proximoArchivo(0);
    
    for(unsigned int i = 0; i < cantThreads; i++){
        hilos[i] = new std::thread(cargarMultiplesArchivosAux,std::ref(hashMap),std::ref(filePaths),std::ref(proximoArchivo));
    }

    for(unsigned int i = 0; i < cantThreads; i++){
        hilos[i]->join();
        delete hilos[i];        
    }
}

/*void cargarMultiplesArchivosAux(HashMapConcurrente &hashMap, std::vector<std::string>& filePaths, std::atomic<int>& proximoArchivo){
    int archivoActual = 0;
    while(proximoArchivo.load() < filePaths.size()){
        archivoActual = proximoArchivo.fetch_add(1);
        cargarArchivo(hashMap,filePaths[proximoArchivo]);
    }s
}*/

void cargarMultiplesArchivosAux(HashMapConcurrente& hashMap, std::vector<std::string>& filePaths, std::atomic<unsigned int>& proximoArchivo){
    while(1){
        unsigned int archivoActual = proximoArchivo.fetch_add(1);
        if(archivoActual >= filePaths.size())break;
        cargarArchivo(hashMap,filePaths[archivoActual]);
    }
}



#endif
