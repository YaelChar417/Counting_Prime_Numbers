/*============================================================================*/
/*======================== Autor: Yael Charles Marin =========================*/
/*======================== Fecha de entrega: 12/06/2025 ======================*/
/*=========================== Matricula: A01711111 ===========================*/
/*=========================== Solucion: secuencial ===========================*/
/*============================================================================*/
#include <iostream>
#include <math.h>
#include <iomanip>
#include <chrono>

using namespace std;
using namespace std::chrono;

#define REPS 10 // numero de repeticiones del programa
const int SIZE = 5000000; // 5x10^6^

/*==============================================================================
*
* Función que recibe un número para determinar si es primo o no.
* Si el número es primo, devuelve true, si no, devuelve false.
*
* @param key -> int: número a verificar si es primo o no
* @return bool: true si el número es primo, false si no
* @complejidad_asintotica: O(sqrt(n))
*
==============================================================================*/
bool isPrime(int key){
    if(key < 2)
    {
        return false;
    }

    int limit = floor(sqrt(key));

    for(int i = 2; i <= limit; i++)
    {
        if(key % i == 0)
        {
            return false;
        }
    }

    return true;
}

/*==============================================================================
*
* Función que recibe un array y suma todos los números primos que contenga ese
* array, almacenando los resultados en un acumulador para después retornarlo
*
* @param *array -> int: array con los números a sumar
* @param size -> int: tamaño del arreglo
* @return acum -> int: suma de todos los números primos del array
* @complejidad_asintotica: O(n*(sqrt(n)))
*
==============================================================================*/
long long sum(int *array, const int &size){
    long long acum = 0;

    for(int i = 0; i < size; i++)
    {
        if(isPrime(array[i]))
        {
            acum += array[i];
        }
    }

    return acum;
}

/*==============================================================================
*
* Función que recibe un array y su tamaño para llenarlos de forma consecutiva
* desde 1 hasta el tamaño del array
*
* @param *array -> int: array con los números a sumar
* @param size -> int: tamaño del arreglo
* @return void 
* @complejidad_asintotica: O(n)
*
==============================================================================*/
void fillArray(int *array, int size){
    for(int i = 0; i < size; i++)
    {
        array[i] = i + 1;
    }
}

int main(){
    // Declarar variables
    int *array;
    long long result;

    // inicializar array con el tamaño
    array = new int[SIZE];

    // variables usadas para medir el tiempo
    high_resolution_clock::time_point startTime, endTime;
    double time;

    // llenar el array desde 1 hasta size
    fillArray(array, SIZE);

    cout << "Comenzando...\n";
    time = 0.0;

    // ejecutar el programa 10 veces y comenzar a medir
    for(int i = 0; i < REPS; i++)
    {
        startTime = high_resolution_clock::now();

        // almacenar el resultado de la suma en result
        result = sum(array, SIZE);

        // Fin de la medición
        endTime = high_resolution_clock::now();
        time += duration<double, std::milli>(endTime - startTime).count();
    }

    // imprimir el resultado y promedio del tiempo
    cout << "result = " << result << "\n";
    cout << "avg time = " << fixed << setprecision(3) 
         << (time / REPS) <<  " ms\n";

    // liberar memoria dinamica
    delete [] array;

    return 0;
}