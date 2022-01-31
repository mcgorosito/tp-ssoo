#ifndef HMC_ARCHIVOS_HPP
#define HMC_ARCHIVOS_HPP

#include <ctype.h>
#include <vector>
#include <string>
//#include <pthread.h>

#include "HashMapConcurrente.hpp"

int cargarArchivo(
    HashMapConcurrente &hashMap,
    std::string filePath
);

void cargarMultiplesArchivos(
    HashMapConcurrente &hashMap,
    unsigned int cantThreads,
    std::vector<std::string> filePaths
);

void cargarMultiplesArchivosAux(HashMapConcurrente &hashMap, std::vector<std::string>& filePaths, std::atomic<unsigned int>& proximoArchivo);

#endif /* HMC_ARCHIVOS_HPP */
