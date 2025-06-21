/*============================================================================*/
/*======================== Autor: Yael Charles Marin =========================*/
/*======================= Fecha de entrega: 12/06/2025 =======================*/
/*=========================== Matricula: A01711111 ===========================*/
/*========================= Solucion: multihilo CUDA =========================*/
/*============== Para compilar: !nvcc -arch=sm_75 -o app cuda.cu =============*/
/*========================== Speed Up: 51,541.77778 ==========================*/
/*============================ Eficiencia: 314.59% ===========================*/
/*============================================================================*/
#include <iostream>
#include <math.h>
#include <iomanip>
#include <chrono>
#include <cuda_runtime.h>

using namespace std;
using namespace std::chrono;

#define SIZE 5000000 // 5x10^6
#define REPS 10 // numero de repeticiones del programa
#define THREADS 512
#define BLOCKS min(32, ((SIZE / THREADS) + 1))

/*==============================================================================
*
* Función que recibe un número para determinar si es primo o no.
* Si el número es primo, devuelve true, si no, devuelve false.
* al ser __device__ puede ser llamada para ejecutarse en la GPU
*
* @param key -> int: número a verificar si es primo o no
* @return bool: true si el número es primo, false si no
*
==============================================================================*/
__device__ bool isPrime(int key){
    if(key < 2)
    {
        return false;
    }

    int limit = floor(sqrtf(key));

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
* Función que distribuye conocer los números primos y sumarlos del array a todos
* los hilos, para después guardar el resultado parcial en una memoria compartida
* y esperar a que los hilos terminen su tarea para sumar todos los subtotales
* de cada bloque y almacenarlo en results
*
* @param *array -> int: array con los números a sumar
* @param *result -> long long: array donde se almacenan los resultados parciales
* @return void
*
==============================================================================*/
__global__ void sum(int *array, long long *results){
    __shared__ long long cache[THREADS];
    int index = threadIdx.x + (blockIdx.x * blockDim.x);
    long long acum = 0;

    while(index < SIZE)
    {
        if(isPrime(array[index]))
        {
            acum += array[index];
        }
        index += blockDim.x * gridDim.x;
    }

    cache[threadIdx.x] = acum;
    __syncthreads();

    int i = blockDim.x / 2;
    while (i > 0) 
    {
        if (threadIdx.x < i) 
        {
            cache[threadIdx.x] += cache[threadIdx.x + i];
        }
        __syncthreads();
        i /= 2;
    }

    if (threadIdx.x == 0) 
    {
        results[blockIdx.x] = cache[0];
    }
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
    int *deviceArray;
    long long *deviceResults, result = 0;
    long long *results = new long long[BLOCKS];

    // Inicializar el array
    array = new int[SIZE];

    // variables usadas para medir el tiempo
    high_resolution_clock::time_point startTime, endTime;
    double time;

    // llenar el array del host desde 1 hasta size
    fillArray(array, SIZE);

    // reservar espacio de memoria en device para array y results
    cudaMalloc((void**)&deviceArray, SIZE * sizeof(int));
    cudaMalloc((void**)&deviceResults, BLOCKS * sizeof(long long));

    // Copiar del host a device el array con los numeros a sumar
    cudaMemcpy(deviceArray, array, SIZE * sizeof(int), cudaMemcpyHostToDevice);

    cout << "Comenzando...\n";
    time = 0.0;

    // ejecutar el programa 10 veces y comenzar a medir
    for(int j = 0; j < REPS; j++)
    {
        startTime = high_resolution_clock::now();

        // mandar a llamar la tarea global de sumar
        sum <<< BLOCKS, THREADS >>>(deviceArray, deviceResults);

        // Fin de la medición
        endTime = high_resolution_clock::now();
        time += duration<double, std::milli>(endTime - startTime).count();
    }

    // Pasar los resultados parciales del device al host
    cudaMemcpy(results, deviceResults, BLOCKS * sizeof(long long), 
                cudaMemcpyDeviceToHost);

    // sumar los resultados parciales para obtener el resultado final
    for (int i = 0; i < BLOCKS; i++) {
        result += results[i];
    }

    // imprimir el resultado y promedio del tiempo
    cout << "result = " << result << "\n";
    cout << "avg time = " << fixed << setprecision(3) 
         << (time / REPS) <<  " ms\n";

    // liberar memoria dinamica
    delete[] array;
    delete[] results;
    cudaFree(deviceArray);
    cudaFree(deviceResults);

    return 0;
}