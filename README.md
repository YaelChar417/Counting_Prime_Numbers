## Descripcion del problema.
Se desea conocer la suma de los números primos que hay del 1 al 5 millones, sin embargo implementar esto a mano o usando excel puede ser muy tardado y complicado.

Por ello se utilizará c++ para desarrollar una solución, pero adicionalmente a esto se explorará implementar soluciones multi-hilo y usando la tecnologia de CUDA.

## Para compilar.
- Secuencial: g++ sec.cpp -o app | app.exe
- Paralelo / multi.hilo: g++ paralelo.cpp | app.exe
- CUDA: !nvcc -arch=sm_75 -o app cuda.cu | !./app
