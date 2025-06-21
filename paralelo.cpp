/*============================================================================*/
/*======================== Autor: Yael Charles Marin =========================*/
/*======================== Fecha de entrega: 12/06/2025 ======================*/
/*=========================== Matricula: A01711111 ===========================*/
/*========================= Solucion: multihilo CPU ==========================*/
/*========================== Speed Up: 7.096485994 ===========================*/
/*============================ Eficiencia: 44.35% ============================*/
/*============================================================================*/
#include <iostream>
#include <math.h>
#include <iomanip>
#include <chrono>
#include <thread>

using namespace std;
using namespace std::chrono;

#define SIZE 5000000 // 5x10^6^
#define REPS 10 // numero de repeticiones del programa
const int THREADS = std::thread::hardware_concurrency(); // hilos del procesador

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
* Función que recibe el valor de inicio y final sobre los cuales podra 
* interactuar cada hilo para sumar los numeros primos en ese rango, y almacenar
* el resultado parcial en una posición del vector results
*
* @param start -> int: inicio del rango
* @param end -> int: final del rango
* @param *array -> int: array con los números a sumar
* @param result -> long long: resultado parcial de la suma
* @return void
*
==============================================================================*/
void sum(int start, int end, int *array, long long &result){
    long long acum = 0;

    for(int i = start; i < end; i++)
    {
        if(isPrime(array[i]))
        {
            acum += array[i];
        }
    }
    
    result = acum;
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
    // Declaración de variables
    int *array;
    long long result = 0;
    int start, end, remainder, blockSize;

    // Inicializar array, hilos y arreglo de resultados
    array = new int[SIZE];
    thread tds[THREADS];
    long long results[THREADS];

    // inicializar tamaño de paso para controlar el rango de los hilos
    blockSize = SIZE / THREADS;
    remainder = SIZE % THREADS;
    
    // variables usadas para medir el tiempo
    high_resolution_clock::time_point startTime, endTime;
    double time;

    // llenar el array desde 1 hasta size
    fillArray(array, SIZE);

    cout << "Comenzando...\n";
    time = 0.0;

    // ejecutar el programa 10 veces y comenzar a medir
    for(int j = 0; j < REPS; j++)
    {
        startTime = high_resolution_clock::now();
        // reiniciar valores de start y result para cada iteracion
        start = 0;
        result = 0;

        // crear hilos y asignarles la tarea con su rango a cada uno
        for(int i = 0; i < THREADS; i++)
        {   
            // si es el ultimo bloque, asignarle uno mas
            end = start + blockSize + ((i < remainder)? 1 : 0);
            tds[i] = thread(sum, start, end, array, ref(results[i]));
            start = end;
        }

        // Esperar a que los hilos completen su tarea
        for (int i = 0; i < THREADS; i++) 
        {
            tds[i].join();
        }

        // Sumar los resultados parciales de results
        for (int i = 0; i < THREADS; i++) 
        {
            result += results[i];
        }

        // Fin de la medición
        endTime = high_resolution_clock::now();
        time += duration<double, std::milli>(endTime - startTime).count();
    }

    // imprimir el resultado y promedio del tiempo
    cout << "Num. de hilos = " << THREADS << "\n";
    cout << "result = " << result << "\n";
    cout << "avg time = " << fixed << setprecision(3) 
         << (time / REPS) <<  " ms\n";

    // liberar memoria dinamica
    delete [] array;

    return 0;
}