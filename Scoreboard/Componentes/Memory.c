#include <stdio.h>
#include "memoria.h"

# define MEMORY_SIZE 1024 // 32 Bits 

struct memory{
	int data[MEMORY_SIZE] ;
} ;


// Função de inicialização de memória
void constructMemory(Memory *memory)
{
	for(int i=0; i < MEMORY_SIZE; i++)
	{
		memory->data[i] = 0 ;
	}
}

// Função de Escrita de Dados
int writeMemory(Memory *memory, int pos, int value)
{
	 if(pos >= 0 && MEMORY_SIZE)
	 {
	 	memory->data[pos] = value ;
	 }
	 else{
	 	printf("Error: Pos out of limits of memory!! \n") ;
	 }
	
	 return 0 ;
}

// Função de Leitura de Dados
int readMemory(Memory *memory, int pos)
{
	if(pos >= 0 && pos < MEMORY_SIZE)
	{
		return memory->data[pos] ;
	}	
	
	return 1 ;
}

