#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_REGISTERS 32


// ===================================================== REGISTRADORES =====================================================


// Estrutura para representar um Registrador
typedef struct {
	int valor ;
} Registrador ;

// Banco de Registradores
Registrador registradores[32] ;

// Rótulos para os registradores
const char *nomes_registradores[32] = {
    "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  "r8",  "r9",  "r10",
    "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19", "r20", "r21",
    "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"};

// Operações do processador
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
} ;

// Endereço do próximo comando a ser executado
int pc = 0 ;

// =========================================================================================================================

// Estrutura para representar a memória
struct Memoria {
  int dados[1024] ; // Memória fictícia com 1024 palavras
};

// Declaração de uma variável de memória
struct Memoria memoria ;

// Estrutura para representar uma entrada no scoreboard
typedef struct {
    Registrador reg ;
    bool busy ;
    int value ;
} ScoreboardEntry ;

// ===================================================== INSTRUÇÕES =====================================================

// Estrutura para representar uma instrução do MIPS
typedef struct {
    uint32_t opcode : 6 ;   // Campo de 6 bits para o opcode
    uint32_t rs : 5 ;       // Campo de 5 bits para o registrador de origem 1
    uint32_t rt : 5 ;       // Campo de 5 bits para o registrador de origem 2
    uint32_t rd : 5 ;       // Campo de 5 bits para o registrador de destino
    uint32_t shamt : 5 ;    // Campo de 5 bits para o deslocamento (shift amount)
    uint32_t funct : 6 ;    // Campo de 6 bits para a função (func)
    uint32_t immediate : 16 ; // Campo de 16 bits para o valor imediato (ou endereço)
} Instruction ;

// Tipos de instrução
enum InstrucaoTipo { R, I, J } ;

// =========================================================================================================================

// Pipeline com 5 estágios
Instruction pipeline[5] ;

// Scoreboard
ScoreboardEntry scoreboard[NUM_REGISTERS] ;

// ================================================= UNIDADES FUNCIONAIS  ================================================= 

// Função para imprimir o estado da unidade funcional de adição
void imprimir_Estado_Unidade_Funcional_ADD(Registrador r1, Registrador r2) 
{
    printf("==================== Unidade Funcional ADD =================== \n") ;
    printf("\nR1 --> %d", r1.valor) ;
    printf("\nR2 --> %d", r2.valor) ;
    
}

// Função para imprimir o estado da unidade funcional de subtração
void imprimir_Estado_Unidade_Funcional_SUB(Registrador r1, Registrador r2) 
{
    printf("==================== Unidade Funcional SUB ==================== \n") ;
    printf("\nR1 --> %d", r1.valor) ;
    printf("\nR2 --> %d", r2.valor) ;
    
}

// Função para imprimir o estado da unidade funcional de multiplicação
void imprimir_Estado_Unidade_Funcional_MUL(Registrador r1, Registrador r2) 
{
    printf("===================== Unidade Funcional MUL ==================== \n") ;
    printf("\nR1 --> %d", r1.valor) ;
    printf("\nR2 --> %d", r2.valor) ;
}

// Função para imprimir o estado da unidade funcional de divisão
void imprimir_Estado_Unidade_Funcional_DIV(Registrador r1, Registrador r2) 
{
    printf("==================== Unidade Funcional DIV ====================\n") ;
    printf("\nR1 --> %d", r1.valor) ;
    printf("\nR2 --> %d", r2.valor) ;    
}

// Função para executar a operação de adição
int unidade_Funcional_ADD(Registrador r1, Registrador r2) 
{
	    imprimir_Estado_Unidade_Funcional_ADD(r1, r2) ;
    	return r1.valor + r2.valor ;
}

// Função para executar a operaçãgistradores’ uno de subtração
int unidade_Funcional_SUB(Registrador r1, Registrador r2) 
{
	    imprimir_Estado_Unidade_Funcional_SUB(r1, r2) ;
    	return r1.valor - r2.valor ;
}

// Função para executar a operação de multiplicação
int unidade_Funcional_MUL(Registrador r1, Registrador r2) 
{
    imprimir_Estado_Unidade_Funcional_MUL(r1, r2) ;
    return r1.valor * r2.valor ;
}

// Função para executar a operação de divisão
int unidade_Funcional_DIV(Registrador r1, Registrador r2) 
{
    	if (r2.valor != 0) 
    	{
		    imprimir_Estado_Unidade_Funcional_DIV(r1, r2) ;
        	return r1.valor / r2.valor ;
    	} 
    	else 
    	{
        	// Lidar com divisão por zero, se necessário
        	return 0 ;
    	}
}

// =========================================================================================================================





// ================================================= ESTÁGIOS DO PIPELINE  ================================================= 

// Função para buscar uma instrução da memória e atualizar o PC
void executar_Busca() 
{
    printf("Iniciando o estágio de --> Busca\n") ; 
    	
    // Verificar se a instrução é um salto incondicional
    if (pipeline[0].opcode == 'J' || pipeline[0].opcode == 'j') 
    	{
        	// Se for um salto incondicional, atualizar o PC imediatamente
        	int target_address = pipeline[0].immediate; // Supondo que immediate contenha o novo endereço
        	pc = target_address ;
    	} 
    	else // Caso contrário, apenas buscar a próxima instrução normalmente 
    	{
        	// Buscar a próxima instrução da memória de instruções e colocar no estágio IF do pipeline.
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
    	printf("Iniciando o estágio de --> Emissão \n") ;   

    	// Verificar se o estágio de busca contém uma instrução válida para emissão.
    	if (pipeline[0].opcode != ' ') 
    	{
        	// A instrução no estágio IF (busca) está pronta para emissão.
        
		// Emite esta instrução para o próximo estágio (etapa de emissão) do pipeline.
        	pipeline[1] = pipeline[0] ; // Move a instrução para o próximo estágio
        	pipeline[0].opcode = ' ' ; // Limpa o estágio de busca
    	} 
    	else 
    	{
		printf("Não há instruções prontas para emissão no momento !!") ;
		// Aguardar ou continuar buscando instruções.
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

    int source1_reg = pipeline[1].rs ;
    int source2_reg = pipeline[1].rt ;

    // Verificar se os registradores de origem estão ocupados
    if (!scoreboard[source1_reg].busy && !scoreboard[source2_reg].busy) 
    {
        // Os operandos estão disponíveis para leitura, podemos prosseguir com a leitura dos operandos da memória ou do banco de registradores
        int valor_source1 = registradores[source1_reg].valor ;
        int valor_source2 = registradores[source2_reg].valor ;

        // Atualizar os campos da instrução com os valores lidos
        pipeline[1].rs = valor_source1 ;
        pipeline[1].rt = valor_source2 ;
    } 
    else 
    {
        // Aguarde até que os operandos estejam prontos
        // Pode ser necessário implementar alguma lógica de espera ou controle de dependências aqui.
    }
}


// Função para executar o estágio de execução
void executar_Execucao(char opcode, Registrador rs, Registrador rt, Registrador rd, Registrador immediate) 
{
    printf("\nIniciando o estágio de --> Execucao\n") ;
    printf("\nOpcode --> %c \n", opcode) ;

     opcode = pipeline[2].opcode ;
     rs.valor = pipeline[2].rs ;
     rt.valor = pipeline[2].rt ;
     rd.valor = pipeline[2].rd ;
     immediate.valor = pipeline[2].immediate ;

    // Bloco switch para lidar com diferentes opcodes
    switch (opcode) 
    {
    	// Operação de adição.
        case ADD :
            printf("\nExecução --> ADD") ;
            pipeline[2].rd = unidade_Funcional_ADD(rs, rt) ;
            break ;
        
        // Operação de adição imediata.
        case ADDI :
            printf("\nExecução --> ADDI") ;
            pipeline[2].rt = unidade_Funcional_ADD(rs, immediate) ;
            break ;
 	
 	    // Operação de subtração
        case SUB:
            printf("\nExecução --> SUB") ;
	        pipeline[2].rd = unidade_Funcional_SUB(rs, rt) ;
            break ;
            
        case SUBI:
            printf("\nExecução --> SUBI") ;
	        pipeline[2].rt = unidade_Funcional_SUB(rs, immediate) ;
            break ;
	
	    // Operação de multiplicação
        case MUL:
            printf("\nExecução --> MUL") ;
            pipeline[2].rd = unidade_Funcional_MUL(rs, rt) ;
            break ;
	
	    // Operação de divisão
        case DIV:
            printf("\nExecução --> DIV") ;
            pipeline[2].rd = unidade_Funcional_DIV(rs, rt) ;
            break ;
       
        //
        case AND:
            printf("\nExecução --> AND") ;
   		    rd.valor = rs.valor & rt.valor ;
    		//printf("AND %s, %s, %s\n", nome_rd, nome_rs, nome_rt);
    		break;
    		
    	// 	
  	    case OR:
  	        printf("\nExecução --> OR") ;
    		rd.valor = rs.valor | rt.valor ;
    		//printf("OR %s, %s, %s\n", nome_rd, nome_rs, nome_rt);
    		break;
    		
    	// 	
  	    case NOT:
    		rd.valor = ~rs.valor ;
    		//printf("NOT %s, %s\n", nome_rd, nome_rs) ;
    		break;
    		
    	// 	
  	    case BLT:
    		if (rs.valor < rt.valor) 
    		{
      			//pc = &immediate ; // Salta para o endereço imediato
      			//printf("BLT %s, %s, %d (Salto para %d)\n", nome_rs, nome_rt, immediate, pc);
    		}
    		break;
    		
    	// 	
  	    case BGT:
    		if (rs.valor > rt.valor) 
    		{
      			//pc = &immediate ; // Salta para o endereço imediato
      			//printf("BGT %s, %s, %d (Salto para %d)\n", nome_rs, nome_rt, immediate, pc);
    		}
    		break;
    		
    	//
  	    case BEQ:
    		if (rs.valor == rt.valor) 
    		{
      			//pc = &immediate ; // Salta para o endereço imediato
      			//printf("BEQ %s, %s, %d (Salto para %d)\n", nome_rs, nome_rt, immediate, pc);
    		}
    		break;
    		
    	//
  	    case BNE:
    		if (rs.valor != rt.valor) 
    		{
      			//pc = &immediate ; // Salta para o endereço imediato
      			//printf("BNE %s, %s, %d (Salto para %d)\n", nome_rs, nome_rt, immediate, pc);
    		}
    		break;
  	
  	    // 
  	    case JUMP:
    		//pc = &immediate ; // Salto incondicional para o endereço imediato
    		printf("JUMP %d\n", immediate) ;
    		break ;
    		
    	// Carregamento de memória
  	    case LW:
    		if (rs.valor >= 0 && rs.valor < 32) 
    		{
      			// barramento_leitura = rs.valor ; // Conecta o barramento de leitura ao registrador rs
      			//int endereco_memoria = rs.valor + immediate.valor ;
      			//resultado = memoria.dados[endereco_memoria] ;
      			//barramento_leitura = -1; // Desconecta o barramento de leitura
      			//escrever_barramento(resultado); // Escreve o valor lido no barramento de escrita
      			// printf("LW %s, %s, %d (Valor lido: %d)\n", nome_rd, nome_rs, immediate) ; 
    		}
    		break;
  	
  	// Armazenamento na memória
  	case SW:
    		if (rs.valor >= 0 && rs.valor < 32) 
    		{
      			//barramento_leitura = rd.valor ; // Conecta o barramento de leitura ao registrador rd
      			//int endereco_memoria = rs.valor + immediate.valor ;
      			//int valor = ler_barramento(); // Use ler_barramento para ler o valor do
      			//escrever_memoria(endereco_memoria,valor); // Use escrever_memoria para escrever na memória
      			//barramento_leitura = -1; // Desconecta o barramento de leitura
      			//printf("SW %s, %s, %d (Valor escrito: %d)\n", nome_rd, nome_rs, imm,valor);
    		}
    		break;
  	
  	// Saída
  	case EXIT:
    		printf("Programa encerrado.\n") ;
    		pc = -1 ; // Termina o programa
    		break ;
  		
  	// Caso Base	
  	default:
    		printf("Opcode desconhecido\n") ;
    		break ;
    }
}


// Função para executar o estágio de escrita dos resultados (write back)
void executar_Escrita_Resultados(char opcode, int destination) 
{
    printf("Iniciando o estágio de --> Escrita dos Resultados\n") ;

    // Suponhamos que a instrução no estágio de execução (etapa anterior) já tenha seu resultado calculado.
    // Podemos escrever esse resultado no banco de registradores e liberar a entrada correspondente no scoreboard.

    opcode = pipeline[3].opcode ;
    destination = pipeline[3].rt ;

    // Escrever o resultado no banco de registradores (a menos que seja uma instrução de controle)
    if (opcode != JUMP && opcode != EXIT) 
    {
        registradores[destination].valor = pipeline[3].rt ;
    }

    // Liberar a entrada correspondente no scoreboard
    scoreboard[destination].busy = false ;
}

// =========================================================================================================================


// ===================================================== SCOREBOARD ========================================================

// Função para inicializar o scoreboard
void inicializar_Scoreboard() 
{
	printf("\nIniciando o Scoreboard ...\n") ;	
	
    	for (int i = 0; i < NUM_REGISTERS; i++) 
    	{
        	scoreboard[i].reg.valor = i ;
        	scoreboard[i].busy = false ; 
        	scoreboard[i].value = 0 ;
   	}
}

// Função para atualizar o scoreboard após a escrita de resultados
void atualizar_Scoreboard(int destination, int value) 
{
    if (destination >= 0 && destination < NUM_REGISTERS) 
    {
        scoreboard[destination].busy = false ;
        scoreboard[destination].value = value ;
    }
}

// Função para verificar se um registrador está ocupado
bool verificar_Registrador_Ocupado(int reg) 
{
    if (reg >= 0 && reg < NUM_REGISTERS) 
    {
        return scoreboard[reg].busy ;
    }
    
    return false ; // Registrador inválido
}

// Função para definir um registrador como ocupado
void definir_Registrador_Ocupado(int reg) 
{
    if (reg >= 0 && reg < NUM_REGISTERS) 
    {
        scoreboard[reg].busy = true ;
    }
}

// Função para obter o valor de um registrador do scoreboard
int obter_Valor_Registrador(int reg) 
{
    if (reg >= 0 && reg < NUM_REGISTERS) 
    {
        return scoreboard[reg].value ;
    }
    
    return 0 ; // Registrador inválido
}


// =========================================================================================================================






// ===================================================== MEMÓRIA =====================================================

void imprimir_memoria() 
{
  printf("Conteúdo da memória:\n");
  for (int i = 0; i < 1024; i++) 
  {
    printf("memoria[%d] = %d\n", i, memoria.dados[i]);
  }
}

// Função para ler dados da memória
int ler_memoria(int endereco) 
{
  if (endereco >= 0 && endereco < 1024) 
  {
    return memoria.dados[endereco]; // Corrigido para usar memoria.dados
  } 
  else 
  {
    // Tratar erro de acesso à memória fora dos limites
    printf("Erro: Acesso à memória fora dos limites.\n");

    return 0;
  }
}

// Função para escrever dados na memória
void escrever_memoria(int endereco, int dado) 
{
  if (endereco >= 0 && endereco < 1024) 
  {
    memoria.dados[endereco] = dado ; // Corrigido para usar memoria.dados
  } 
  else 
  {
    // Tratar erro de acesso à memória fora dos limites
    printf("Erro: Acesso à memória fora dos limites.\n") ;
  }
}

// =========================================================================================================================


// Declaração de barramentos
int barramento_leitura = -1; // Inicialmente, nenhum registrador está usando o barramento de leitura
int barramento_escrita = -1; // Inicialmente, nenhum registrador está usando o barramento de escrita

// Função para ler do barramento de leitura
int ler_barramento() 
{
    if (barramento_leitura != -1) 
    { 
  	    printf("Barramento de leitura conectado a %s\n",nomes_registradores[barramento_leitura]);
    	// return registradores[barramento_leitura];
    }
  
    return 0; // Se não estiver conectado, retorna 0
}

// Função para escrever no barramento de escrita
void escrever_barramento(int valor) 
{
  if (barramento_escrita != -1) 
  {
    	printf("Barramento de escrita conectado a %s\n",nomes_registradores[barramento_escrita]);
  	registradores[barramento_escrita].valor = valor;
  }
}






int main() 
{
	Registrador rd, rs, rt, imm ;

  	// Inicialização do pipeline e scoreboard
    for (int i = 0; i < 5; i++) 
    {
        pipeline[i].opcode = ' ' ;
        pipeline[i].rs = -1 ;
        pipeline[i].rd = -1 ;
        pipeline[i].rt = -1 ;
        pipeline[i].immediate = -1 ;
    }

  	for (int i = 0; i < 1024; i++) 
 	{
    		memoria.dados[i] = 0 ;
  	}

  	// Exemplo de instrução (opcode r1, r2)
  	enum OpCode opcode = SUBI ;
	rd.valor = 0 ;
	rs.valor = 0 ;
	rt.valor = 0 ;
	imm.valor = 10 ;

  	// Executa a instrução
  	executar_Execucao(opcode, rd, rs, rt, imm) ;

  	return 0;
}
