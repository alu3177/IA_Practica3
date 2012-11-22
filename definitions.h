/*
 * Inteligencia Artificial - Práctica 3: Problema de empaquetado unidimensional
 *
 * AUTOR: Fernando González López-Peñalver (alu3177)
 * EMAIL: alu0100256543@ull.edu.es
 * FECHA DE ENTREGA: 22 - 11 -12
 *
 * Se incluye repositorio GIT con el desarrollo de la práctica.
 *
 * Enunciado del problema:
 *      Se dispone de una cantidad indeterminada de contenedores de
 *      capacidad C y n objetos de tamaños w1, w2, …, wn. Se trata de
 *      determinar el empaquetado de todos los objetos usando el menor
 *      número posible de contenedores m*. La suma de los tamaños de los
 *      objetos asignados a contenedor no puede sobrepasar la capacidad C.
 *
 *      Implementar los siguientes algoritmos:
 *          - ILS   (Busqueda Local Iterada)
 *          - SA    (Recocido Simulado)
 *          - VNS   (Busqueda por Entorno Variable, basica)
 *          - TS    (Busqueda Tabu)
 *          - GRASP (Procedimiento de Busqueda Adaptativa Aleatoria Voraz)
 *          - AG    (Algoritmo Genetico)
 *
 */

// Colores para std::cout
#define C_DEFAULT "\033[0;m"
#define C_RED     "\033[0;31m"
#define C_BLUE    "\033[0;34m"
#define C_GREEN   "\033[0;32m"
#define C_CYAN    "\033[0;36m"
#define C_MAGENTA "\033[0;35m"
#define C_BRED    "\033[1;31m"   // BOLD
#define C_BGREEN  "\033[1;32m"   // BOLD

// Modo VERBOSE: Si se activa se muestra mas informacion
// al mostrar por pantalla las instancias y las soluciones
#define VERBOSE     false
