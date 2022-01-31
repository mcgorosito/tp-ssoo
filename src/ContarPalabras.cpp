#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "HashMapConcurrente.hpp"
#include "CargarArchivos.hpp"
#include <chrono>


int main(int argc, char **argv) {
    if (argc < 4) {
        std::cout << "Error: faltan argumentos." << std::endl;
        std::cout << std::endl;
        std::cout << "Modo de uso: " << argv[0] << " <threads_lectura> <hacerMaximo> <threads_maximo>" << std::endl;
        std::cout << "    " << "<archivo1> [<archivo2>...]" << std::endl;
        std::cout << std::endl;
        std::cout << "    threads_lectura: "
            << "Cantidad de threads a usar para leer archivos." << std::endl;
        std::cout << "    hacerMaximo: "
            << "'true' si requiere hacer el maximo" << std::endl;
        std::cout << "    threads_maximo: "
            << "Cantidad de threads a usar para computar máximo. No indicar de no ser necesario" << std::endl;
        std::cout << "    archivo1, archivo2...: "
            << "Archivos a procesar." << std::endl;
        return 1;
    }

    std::string hacerMaximo = argv[2];

    int cantThreads = std::stoi(argv[1]);
    
    std::vector<std::string> filePaths = {};
    for (int i = 3; i < argc; i++) {
        filePaths.push_back(argv[i]);
    }

    HashMapConcurrente hashMap{}; // = HashMapConcurrente();
    auto start_multiples_archivos = std::chrono::steady_clock::now();
    cargarMultiplesArchivos(hashMap, cantThreads, filePaths);
    auto end_multiples_archivos = std::chrono::steady_clock::now();
    double tiempo_multiples_archivos = std::chrono::duration<double, std::milli>(end_multiples_archivos -  start_multiples_archivos).count();
    std::clog << tiempo_multiples_archivos << std::endl;
    if(hacerMaximo == "true"){
        auto start_maximo_paralelo = std::chrono::steady_clock::now();
        auto maximo = hashMap.maximoParalelo(cantThreads);
        auto end_maximo_paralelo = std::chrono::steady_clock::now();
        double tiempo_maximo_paralelo = std::chrono::duration<double, std::milli>(end_maximo_paralelo - start_maximo_paralelo).count();
        std::cout << tiempo_maximo_paralelo << std::endl;
    }

    return 0;
}
