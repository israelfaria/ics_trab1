#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <errno.h>
#include <argp.h>


#include "wav.h"
#include "oscil.h"

/*Versão do programa*/
const char * argp_program_version = "Trabalho 2 de ICS - Versão 0.1";
/*A quem reclamar se o programa der defeito...*/
const char * argp_program_bug_address = "<brunofigueiralourenco@gmail.com>";
/*Pequena descrição do programa*/
static char doc[] ="Trabalho 1 de Introdução à Computação Sônica --"
					" Oscilador Digital";
					
/*
 * Os argumentos obrigatórios que o programa recebe. No nosso caso, 
 * são 3
 */
static char args_doc[] = " [FREQUÊNCIA] [AMPLITUDE] [DURAÇÂO]";

/*!@brief Argumentos que o programa recebe*/
struct arguments{
	char *args[3];                
    char *output_file;
    interpolation_t inter_type;
    int32_t sample_rate;
    int32_t table_length;
};

static struct argp_option options[] = {
    {"output",			  'o',"FILE", 0, "Coloca a saída no arquivo FILE" },
	{"inter",'i',"Código "		, 0, "0 - Sem interpolação\n1 - Linear\n2 - Cúbica"},
	{"sample_rate",'s',"sr"		, 0, "Taxa de amostragem"},
	{"table_length",'t',"length", 0,"Tamanho da tabela de lookup"},
	{0}
};

/*!@brief Parser a ser utilizado internamente para os argumentos passados 
 * pela linha de comando
 * 
 */
static error_t parse_opt (int key, char *arg, struct argp_state *state){
	struct arguments * arguments = state->input;
	
	switch (key){
		case 'o':
			arguments->output_file = arg;	
			break;
		case 'i':{
			char *tail = "";
			interpolation_t inter_type;
			inter_type = strtol (arg, &tail, 0);

			/*
			 * Checa se a conversão foi bem-sucedida, isto é o usuário 
			 * realmente informou um número. Além disso, checa se 
			 * o código informado está na faixa de valores válida
			 */
			if ( strcmp(tail,"") != 0 || (inter_type > 2 || inter_type < 0 )){
				argp_error(state,"Tipo de interpolação inválida!\n");	
			}
			
			arguments->inter_type = atoi(arg);
			break;
		}
		case 's':{
			char *tail = "";
			arguments->sample_rate = strtol(arg,&tail,0);
			/*
			 * Checa se a conversão foi bem-sucedida, isto é o usuário 
			 * realmente informou um número. Além disso, checa se 
			 * a taxa de amostragem é maior que 0
			 */
			if ( strcmp(tail,"") != 0 || arguments->sample_rate < 0){
				argp_error(state,"Sample Rate negativo ou inválido!\n");	
			}
			break;	
		}	
		case 't':{
			char *tail;
			arguments->table_length = strtol(arg,&tail,0);
			/*
			 * Checa se a conversão foi bem-sucedida, isto é o usuário 
			 * realmente informou um número. Além disso, checa se 
			 * a taxa de amostragem é maior que 0
			 */
			if ( strcmp(tail,"") != 0 || arguments->table_length < 0){
				argp_error(state,"Tamanho da tabela negativo ou inválido!\n");	
			}
			break;		
		}
		case ARGP_KEY_ARG:
			/*Muitos argumentos!*/
			if (state->arg_num >= 3){
            	argp_usage (state);
			}
			arguments->args[state->arg_num] = arg;
			break;
		case ARGP_KEY_END:
           /*Poucos argumentos!*/
            if (state->arg_num < 3){
           		argp_state_help(state,stdout,ARGP_HELP_STD_HELP);
   			}       
        	break;

		default:
        	return ARGP_ERR_UNKNOWN;
	}
	
	return 0;
}

/*!@brief Struct com os elementos a serem passados para a função 
 * arg_parse. Contém as opções disponíveis, uma documentação breve e
 * o parser
 */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main(int argc, char *argv[]){
	oscil * osc;
	int16_t * samples;
	int32_t freq;
	int32_t sec;
	int32_t amp;
	char * tail = "";
	
	struct arguments arguments;
	/*Seta os valores padrões dos argumentos*/
	arguments.output_file = "teste.wav";
	arguments.inter_type = LINEAR;
	arguments.sample_rate = 44100;
	arguments.table_length = 1024;
	
	argp_parse(&argp,argc,argv,0,0,&arguments);
	
	/*
	printf ("ARG1 = %s\nARG2 = %s\nARG3 = %s\n"
          		"Output File = %s\nInter Type: %d\n"
          		"Sample Rate = %d\n"
          		"Table Length = %d\n",
               arguments.args[0], arguments.args[1],
               arguments.args[2],
               arguments.output_file,
               arguments.inter_type,
               arguments.sample_rate,
               arguments.table_length);
    */
    
    /*Checa se a frequência, amplitude e duração são válidos*/           
	freq = strtol(arguments.args[0],&tail,0);
	if (strcmp(tail,"") != 0 || freq < 0){
		printf("Frequência negativa ou inválida!\n");	
	}        
	amp = strtol(arguments.args[1],&tail,0);
	if (strcmp(tail,"") != 0 || amp < 0){
		printf("Amplitude negativa ou inválida!\n");	
	}  
	sec = strtol(arguments.args[2],&tail,0);
	if (strcmp(tail,"") != 0 || sec < 0){
		printf("Tempo de duração negativo ou inválido!\n");	
	}  
		
	/*Cria o oscilador*/
	osc = start_oscil(arguments.table_length,arguments.sample_rate,amp,arguments.inter_type);
	/*Gera as amostras*/
	samples = generate_sample(osc,freq,sec);
	/*Manda escrever no arquivo*/
	write_wave(arguments.output_file,arguments.sample_rate,samples,sec);

	/*Limpa a bagunça*/
	free_oscillator(&osc);
	free(samples);

	return 0;	
}

/*!@mainpage
 * @section Int Introdução
 * Esse programa implementa um oscilador digital. Há três opções de
 * osciladores: sem interpolação, com interpolação linear e 
 * interpolação cúbica. O usuário informa a frequência, amplitude e 
 * a duração do som e o programa escreve um arquivo chamado "teste.wav" 
 * contém o som com as características desejadsa
 * 
 * @section Com Compilação
 * Para compilar o programa digite make na raiz do diretório src. Para 
 * assegurar que a compilação seja bem-sucedida, use Linux.
 * 
 * @section exec Execução do programa
 * O programa recebe obrigatoriamente 3 argumentos na seguinte ordem: 
 * <i>freq</i> <i>amp</i> <i>dur</i>. Além disso opcionalmente o programa 
 * pode receber os seguintes argumentos:
 * 
 * - -o FILE : Especifica que a saída deve ser colocada no arquivo FILE
 * - -i Código: Indica o tipo de interpolação correspondente ao código. 
 * 0 - Sem interpolação. 1 - Interpolação Linear. 2 - Interpolação Cúbica
 * - -s sr : Seta o sample rate para sr
 * - -t length: Seta o tamanho da tabela de lookup para length
 * - -?: Mostra a listagem de comandos
 * 
 * Exemplos:
 * 
 * ./main 440 30000 10 -i 2 -o "A440.wav" : Gera uma amostra de 440 Hz, 
 * com amplitude 30000 durando 10 segundos usando a interpolação cúbica e 
 * coloca o resultado no arquivo A440.wav .
 * 
 * ./main 754 20000 5 -i 1 -s 88200 : Gera uma amostra de 754 Hz com amplitude
 * 20000 durante 5 segundos usando interpolação linear e com sample rate 
 * 88200. Quando não é especificado o nome do arquivo de saída o programa escreve 
 * um arquivo "teste.wav" como saída.
 */
