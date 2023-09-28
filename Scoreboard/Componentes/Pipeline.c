#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_REGISTERS 32

// Estrutura para representar uma instrução
typedef struct {
    char opcode ;
    int source1 ;
    int source2 ;
    int destination ;
} Instruction ;

// Estrutura para representar uma entrada no scoreboard
typedef struct {
    int reg ;
    bool busy ;
    int value ;
} ScoreboardEntry ;

// Enumeração para representar as operações do processador
enum OpCode {
    ADD = 0x00,
    ADDI = 0x01,
    SUB = 0x02,
    SUBI = 0x03,
    MUL = 0x04,
    DIV = 0x05,
    AND = 0x06,
    OR = 0x07,
    NOT = 0x08,
    BLT = 0x09,
    BGT = 0x0A,
    BEQ = 0x0B,
    BNE = 0x0C,
    JUMP = 0x0D,
    LW = 0x0E,
    SW = 0x0F,
    EXIT = 0x10,
    // Adicione outras operações aqui conforme necessário
} ;

// Pipeline com 5 estágios
Instruction pipeline[5] ;
ScoreboardEntry scoreboard[NUM_REGISTERS] ;
// Contador de programa
int PC = 0 ; 			
// Banco de registradores		
int registradores[NUM_REGISTERS] ;



// ================================================= UNIDADES FUNCIONAIS  ================================================= 

// Função para executar a operação de adição
int unidade_Funcional_ADD(int source1, int source2) 
{
    return source1 + source2 ;
}

// Função para executar a operação de subtração
int unidade_Funcional_SUB(int source1, int source2) 
{
    return source1 - source2 ;
}

// Função para executar a operação de multiplicação
int unidade_Funcional_MUL(int source1, int source2) 
{
    return source1 * source2 ;
}

// Função para executar a operação de divisão
int unidade_Funcional_DIV(int source1, int source2) 
{
    if (source2 != 0) 
    {
        return source1 / source2 ;
    } 
    else 
    {
        // Lidar com divisão por zero, se necessário
        return 0 ;
    }
}

// Função para imprimir o estado da unidade funcional de adição
void imprimir_Estado_Unidade_Funcional_ADD() 
{
    printf("Unidade Funcional ADD:\n") ;
    
}

// Função para imprimir o estado da unidade funcional de subtração
void imprimir_Estado_Unidade_Funcional_SUB() 
{
    printf("Unidade Funcional SUB:\n") ;
    
}

// Função para imprimir o estado da unidade funcional de multiplicação
void imprimir_Estado_Unidade_Funcional_MUL() 
{
    printf("Unidade Funcional MUL:\n") ;
    
}

// Função para imprimir o estado da unidade funcional de divisão
void imprimir_Estado_Unidade_Funcional_DIV() 
{
    printf("Unidade Funcional DIV:\n") ;
    
}

// =========================================================================================================================





// ================================================= ESTÁGIOS DO PIPELINE  ================================================= 

// Função para buscar uma instrução da memória e atualizar o PC
void executar_Busca() 
{
	printf("Iniciando o estágio de --> Busca\n") ; 
    	// Simulação do estágio de busca 
    	
    	// Verificar se a instrução é um salto incondicional
    	if (pipeline[0].opcode == 'J' || pipeline[0].opcode == 'j') 
    	{
        	// Se for um salto incondicional, atualizar o PC imediatamente
        	int target_address = pipeline[0].destination ; // Supondo que destination contenha o novo endereço
        	PC = target_address ;
    	} 
    	else // Caso contrário, apenas buscar a próxima instrução normalmente 
    	{
        	// Aqui você buscaria a próxima instrução da memória de instruções
        	// e a colocaria no estágio IF do pipeline.
        	// Suponha que as instruções estejam armazenadas em um array de instruções.
        	// Por exemplo:
        	// pipeline[0] = próxima instrução buscada;
        	// pipeline[1] = pipeline[0];
        	// ...
    	}
}


// Função para executar o estágio de emissão
void executar_Emissao() 
{
    printf("Iniciando o estágio de --> Emissao\n") ;
    
    // Aqui você pode verificar se há instruções a serem emitidas no pipeline
    // Por exemplo, você pode verificar se o estágio de busca contém uma instrução válida para emissão.
    if (pipeline[0].opcode != ' ') 
    {
        // A instrução no estágio IF (busca) está pronta para emissão.
        // Você pode emitir esta instrução para o próximo estágio (etapa de emissão) do pipeline.
        pipeline[1] = pipeline[0] ; // Move a instrução para o próximo estágio
        pipeline[0].opcode = ' ' ; // Limpa o estágio de busca
        // Você também pode fazer outras verificações ou operações relacionadas à emissão de instruções aqui.
    } 
    else 
    {
        // Não há instruções prontas para emissão no momento.
        // Você pode decidir o que fazer nesse caso, como aguardar ou continuar buscando instruções.
    }
}


// Função para executar o estágio de leitura dos operandos
void executar_Leitura_Operandos() 
{
    printf("Iniciando o estágio de --> Leitura dos Operandos\n") ;

    // Simulação do estágio de leitura dos operandos
    // Aqui você pode verificar se os operandos necessários estão disponíveis
    // e aguardar até que os operandos estejam prontos

    // Suponhamos que a instrução no estágio de emissão (etapa anterior) esteja pronta para leitura dos operandos.
    // Você pode verificar se os registradores de origem (source1 e source2) estão ocupados no scoreboard.

    int source1_reg = pipeline[1].source1 ;
    int source2_reg = pipeline[1].source2 ;

    // Verificar se os registradores de origem estão ocupados
    if (!scoreboard[source1_reg].busy && !scoreboard[source2_reg].busy) 
    {
        // Os operandos estão disponíveis para leitura
        // Você pode prosseguir com a leitura dos operandos da memória ou do banco de registradores
        int valor_source1 = registradores[source1_reg] ;
        int valor_source2 = registradores[source2_reg] ;

        // Faça as operações de leitura dos operandos aqui, se necessário

        // Atualizar os campos da instrução com os valores lidos
        pipeline[1].source1 = valor_source1 ;
        pipeline[1].source2 = valor_source2 ;

        // Você também pode fazer outras verificações ou operações relacionadas à leitura dos operandos aqui.
    } 
    else 
    {
        // Aguarde até que os operandos estejam prontos
        // Pode ser necessário implementar alguma lógica de espera ou controle de dependências aqui.
    }
}


// Função para executar o estágio de execução
void executar_Execucao() 
{
    printf("Iniciando o estágio de --> Execucao\n") ;

    // Aqui você pode executar a lógica da instrução

    // Você pode realizar a execução real da instrução aqui, com base no opcode e nos operandos.

    char opcode = pipeline[2].opcode ;
    int source1 = pipeline[2].source1 ;
    int source2 = pipeline[2].source2 ;
    int destination = pipeline[2].destination ;

    // Bloco switch para lidar com diferentes opcodes
    switch (opcode) 
    {
    	// Operação de adição
        case ADD :
            pipeline[2].destination = unidade_Funcional_ADD(source1, source2) ;
            break ;
 	
 	// Operação de subtração
        case SUB:
	    pipeline[2].destination = unidade_Funcional_SUB(source1, source2) ;
            break ;
	
	// Operação de multiplicação
        case MUL:
            pipeline[2].destination = unidade_Funcional_MUL(source1, source2) ;
            break ;
	
	// Operação de divisão
        case DIV:
            pipeline[2].destination = unidade_Funcional_DIV(source1, source2) ;
            break ;

        default:
            // Opcode desconhecido, lidar com erro ou exceção
            // Por exemplo, gerar uma exceção ou tratamento de erro
            break ;
    }
}


// Função para executar o estágio de escrita dos resultados (write back)
void executar_Escrita_Resultados() 
{
    printf("Iniciando o estágio de --> Escrita dos Resultados\n") ;

    // Escrever o resultado no banco de registradores e liberar a entrada correspondente no scoreboard

    // Suponhamos que a instrução no estágio de execução (etapa anterior) já tenha seu resultado calculado.
    // Você pode escrever esse resultado no banco de registradores e liberar a entrada correspondente no scoreboard.

    char opcode = pipeline[3].opcode ;
    int destination = pipeline[3].destination ;

    // Escrever o resultado no banco de registradores (a menos que seja uma instrução de controle)
    if (opcode != JUMP && opcode != EXIT) 
    {
        registradores[destination] = pipeline[3].destination ;
    }

    // Liberar a entrada correspondente no scoreboard
    scoreboard[destination].busy = false ;
}

// =========================================================================================================================



int main() 
{
	int ciclo_de_clock = 0 ; // Variável para contar o ciclo de clock
	
    	// Inicialização do pipeline e scoreboard
    	for (int i = 0; i < 5; i++) 
    	{
        	pipeline[i].opcode = ' ' ;
        	pipeline[i].source1 = -1 ;
        	pipeline[i].source2 = -1 ;
        	pipeline[i].destination = -1 ;
    	}

    	for (int i = 0; i < NUM_REGISTERS; i++) 
    	{
        	scoreboard[i].busy = false ;
    	}
    	
    	printf("Iniciando a Simulação ...\n") ; 

	// Laço da Simulação
    	while (true) 
    	{ 
    		ciclo_de_clock++ ; // Incrementa o ciclo de clock a cada iteração
    		// Imprime o ciclo de clock atual
        	printf("\nCiclo de Clock Atual: %d\n", ciclo_de_clock) ;
    		
        	executar_Busca() ;
        	executar_Emissao() ; 
        	executar_Leitura_Operandos() ;
        	executar_Execucao() ;
        	executar_Escrita_Resultados() ;

        	// Imprime o estado das unidades funcionais
        	imprimir_Estado_Unidade_Funcional_ADD() ;
        	imprimir_Estado_Unidade_Funcional_SUB() ;
        	imprimir_Estado_Unidade_Funcional_MUL() ;
        	imprimir_Estado_Unidade_Funcional_DIV() ;
        	// ...
        	

        	printf("\nPressione Enter para avançar para o próximo ciclo de clock...") ;
        	getchar() ; // Pausa e aguarda que o usuário pressione Enter
    	}

    	return 0 ;
}
