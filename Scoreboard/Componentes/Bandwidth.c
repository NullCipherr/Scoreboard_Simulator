#include <stdio.h>
#include "memoria.h"

// Estrutura do barramento
struct Barramento {
    struct Memory *memory ;
} ;

// Função para inicializar o barramento
void initBus(Barramento *bus, struct Memory *memory) 
{
    bus->memory = memory ;
}

// Função para ler um valor da memória via barramento
int readFromMemory(Barramento *bus, int pos) 
{
    if (pos >= 0 && pos < MEMORY_SIZE) 
    {
        return readMemory(bus->mem, pos) ;
    } 
    else 
    {
        printf("Error: Pos out of memory limits! \n") ;
        
        return -1 ; // Valor de erro
    }
}

// Função para escrever um valor na memória via barramento
void writeToMemory(Barramento *bus, int pos, int value) 
{
    if (pos >= 0 && pos < MEMORY_SIZE) 
    {
        writeMemory(bus->mem, pos, value) ;
    } 
    else 
    {
        printf("Error: Pos out of memory limits!\n") ;
    }
}

// Função para imprimir o conteúdo da memória via barramento
void printMemoryViaBus(Barramento *bus) 
{
    printMemory(bus->memory) ;
}
