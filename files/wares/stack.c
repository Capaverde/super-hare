#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//needs -lm flag to compile, i.e, gcc stack.c -o stack -lm
//to do: make it read from stdin if no file argument is provided, and read from the specified file otherwise - this would solve the "*" expansion bug

//hash for switch statement
unsigned long hash(char * s){
	unsigned long acc = 0;
	unsigned long weight = 1;
	while (*s != '\0'){
		acc += (*s) * (weight);
		weight *= 256;
		s++;
	}
	return acc;
}

#define _add 6579297
#define _plussign 43	//+
#define _sub 6452595
#define _minussign 45	//-
#define _mul 7107949
#define _starsign 42	//*
#define _div 7760228
#define _divisionsign 47	//'/'
#define _expt 1953527909
#define _pow 7827312
#define _hatsign 94	//^
#define _sum 7173491
#define _prod 1685025392
#define _mod 6582125
#define _percentsign 37	//%
#define _neg 6776174
#define _abs 7561825
#define _floor 491495844966
#define _ceil 1818846563
#define _round 431349919602
#define _sqrt 1953657203
#define _sin 7235955
#define _cos 7565155
#define _tan 7233908
#define _csc 6517603
#define _sec 6514035
#define _cot 7630691
#define _pi 26992
#define _asin 1852404577
#define _acos 1936679777
#define _atan 1851880545
#define _acsc 1668506465
#define _asec 1667593057
#define _acot 1953456993
#define _ln 28268
#define _log 6778732
#define _log10 206987292524
#define _logb 1650945900
#define _log2 845639532
#define _e 101
#define _exp 7370853
#define _dup 7370084
#define _exch 1751349349
#define _swap 1885435763
#define _drop 1886351972
#define _pop 7368560
#define _clear 491260308579
#define _copy 2037411683
#define _eq 29029
#define _doubleequalsign 15677 //==
#define _neq 7431534
#define _shriekequalsign 15649 //!=
#define _gt 29799
#define _greaterthansign 62	//>
#define _lt 29804
#define _lessthansign 60	//<
#define _gte 6648935
#define _greaterthanequalsign 15678	//>=
#define _lte 6648940
#define _lessthanequalsign 15676	//<=
#define _or 29295
#define _and 6581857
#define _not 7630702
#define _ift 7628393
#define _if_then 31073746738308713	//if-then
#define _ifte 1702127209
#define _if_then_else 3273665478445065833	//if-then-else
#define _xor 7499640
#define _iff 6710889
#define _over 1919252079
#define _rot 7630706
#define _roll 1819045746
#define _index 517097156201
#define _stack 461228831859
#define _equalsign 61	//=
#define _count 500069396323

double stack[1024];
int pointer = 0;
int main(int argc, char * argv[]){
	double aux = 0;
	double aux2 = 0;
	double aux3 = 0;
	for (int i = 1; i < argc; i++){
		 //+ - * /
		//testing switch
		switch (hash(argv[i])){
			case _add:
			case _plussign:
				if (pointer >= 2){
					stack[pointer-2] += stack[pointer-1];
					pointer -= 1;
				}
				break;
			case _sub:
			case _minussign:
				if (pointer >= 2){
					stack[pointer-2] -= stack[pointer-1];
					pointer -= 1;
				}
				break;
			case _mul:
			case _starsign:		//bash treats "*" as special character...
				if (pointer >= 2){
					stack[pointer-2] *= stack[pointer-1];
					pointer -= 1;
				}
				break;
			case _div: 
			case _divisionsign:
				if (pointer >= 2){
					stack[pointer-2] /= stack[pointer-1];
					pointer -= 1;
				}
				break;
			case _expt: 
			case _pow: 
			case _hatsign:
				if (pointer >= 2){
					stack[pointer-2] = pow(stack[pointer-2], stack[pointer-1]);
					pointer -= 1;
				}
				break;
			case _sum:	//sum everything in the stack
				aux = 0;	//accumulator
				for (int j = pointer-1; j>=0; j--){
					aux += stack[j];
				}
				stack[0] = aux;
				pointer = 1;
				break;
			case _prod:	//multiply everything in the stack
				aux = 1;
				for (int j = pointer-1; j>=0; j--){
					aux *= stack[j];
				}
				stack[0] = aux;
				pointer = 1;
				break;
			case _percentsign: 
			case _mod:
				if (pointer >= 2){
					stack[pointer-2] = fmod(stack[pointer-2], stack[pointer-1]);
				}
				break;
			case _neg:
				if (pointer >= 1){
					stack[pointer-1] = - stack[pointer-1];
				}
				break;
			case _abs:
				if (pointer >= 1){
					stack[pointer-1] = fabs(stack[pointer-1]);
				}
				break;
			case _floor:
				if (pointer >= 1){
					stack[pointer-1] = floor(stack[pointer-1]);
				}
				break;
			case _ceil:
				if (pointer >= 1){
					stack[pointer-1] = ceil(stack[pointer-1]);
				}
				break;
			case _round:
				if (pointer >= 1){
					stack[pointer-1] = round(stack[pointer-1]);
				}
				break;
			//sqrt is useful to have as a separate word
			case _sqrt:
				if (pointer >= 1){
					stack[pointer-1] = pow(stack[pointer-1], 0.5);
				}
				break;
			//trigonometric functions, sin, cos, tan, csc, sec, cot, pi and arc functions
			case _sin:
				if (pointer >= 1){
					stack[pointer-1] = sin(stack[pointer-1]);
				}
				break;
			case _cos:
				if (pointer >= 1){
					stack[pointer-1] = cos(stack[pointer-1]);
				}
				break;
			case _tan:
				if (pointer >= 1){
					stack[pointer-1] = tan(stack[pointer-1]);
				}
				break;
			case _csc:
				if (pointer >= 1){
					stack[pointer-1] = 1/sin(stack[pointer-1]);
				}
				break;
			case _sec:
				if (pointer >= 1){
					stack[pointer-1] = 1/cos(stack[pointer-1]);
				}
				break;
			case _cot:
				if (pointer >= 1){
					stack[pointer-1] = 1/tan(stack[pointer-1]);
				}
				break;
			case _pi:
				stack[pointer] = 3.1415926536;
				pointer += 1;
				break;
			case _asin:
				if (pointer >= 1){
					stack[pointer-1] = asin(stack[pointer-1]);
				}
				break;
			case _acos:
				if (pointer >= 1){
					stack[pointer-1] = acos(stack[pointer-1]);
				}
				break;
			case _atan:
				if (pointer >= 1){
					stack[pointer-1] = atan(stack[pointer-1]);
				}
				break;
			case _acsc:
				if (pointer >= 1){
					stack[pointer-1] = asin(1/stack[pointer-1]);
				}
				break;
			case _asec:
				if (pointer >= 1){
					stack[pointer-1] = acos(1/stack[pointer-1]);
				}
				break;
			case _acot:
				if (pointer >= 1){
					stack[pointer-1] = atan(1/stack[pointer-1]);
				}
				break;
			//log10 (alias log), ln, and log(a,b) (alias logb)
			case _ln:
				if (pointer >= 1){
					stack[pointer-1] = log(stack[pointer-1]);
				}
				break;
			case _log: 
			case _log10:
				if (pointer >= 1){
					stack[pointer-1] = log10(stack[pointer-1]);
				}
				break;
			case _logb:
				if (pointer >= 2){
					stack[pointer-2] = log(stack[pointer-2])/log(stack[pointer-1]);
					pointer -= 1;
				}
				break;
			case _log2:
				if (pointer >= 1){
					stack[pointer-1] = log(stack[pointer-1])/log(2);
				}
				break;
			case _e:
				stack[pointer] = 2.7182818285;
				pointer += 1;
				break;
			case _exp:	//exp usually supports complex numbers...
				if (pointer >= 1){
					stack[pointer-1] = pow(2.7182818285, stack[pointer-1]);
				}
				break;
			case _dup:
				if (pointer >= 1){
					stack[pointer] = stack[pointer-1];
					pointer += 1;
				}
				break;
			case _exch: 
			case _swap:
				if (pointer >= 2){
					aux = stack[pointer-2];
					stack[pointer-2] = stack[pointer-1];
					stack[pointer-1] = aux;
				}
				break;
			case _drop: 
			case _pop:
				if (pointer >= 1){
					pointer -= 1;
				}
				break;
			case _clear:
				pointer = 0;
				break;
			case _copy:
				//this operator adds a copy of the top n objects on the stack
				if (pointer >= 1){
					aux = floor(stack[pointer-1]);
					pointer -= 1;
					for (int j=pointer-aux;j < pointer;j++){
						stack[(int) (j+aux)] = stack[j];
					}
					pointer += aux;
				}
				break;
			//eq, neq, gt, lt, or, and, not
			//there are no anonymous functions currently, and the stack only accepts double
			//ift (if-then), ifte (if-then-else), without interpretation of code
		 	case _eq: 
			case _doubleequalsign:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] == stack[pointer-1]) ? 1 : 0;
					pointer -= 1;
				}
				break;
			case _neq: 
			case _shriekequalsign:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] != stack[pointer-1]) ? 1 : 0;
					pointer -= 1;
				}
				break;
			case _gt: 
			case _greaterthansign:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] > stack[pointer-1]) ? 1 : 0;
					pointer -= 1;
				}
				break;
			case _lt: 
			case _lessthansign:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] < stack[pointer-1]) ? 1 : 0;
					pointer -= 1;
				}
				break;
			case _gte: 
			case _greaterthanequalsign:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] >= stack[pointer-1]) ? 1 : 0;
					pointer -= 1;
				}
				break;
			case _lte: 
			case _lessthanequalsign:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] <= stack[pointer-1]) ? 1 : 0;
					pointer -= 1;
				}
				break;
			case _or:
				if (pointer >= 2){
					stack[pointer-2] = ((stack[pointer-2] != 0) || (stack[pointer-1] != 0)) ? 1 : 0;
					pointer -= 1;
				}
				break;
			case _and:
				if (pointer >= 2){
					stack[pointer-2] = ((stack[pointer-2] != 0) && (stack[pointer-1] != 0)) ? 1 : 0;
					pointer -= 1;
				}
				break;
			case _not:
				if (pointer >= 1){
					stack[pointer-1] = (stack[pointer-1] == 0) ? 1 : 0; 
				}
				break;
			case _ift: 
			case _if_then:
				if (pointer >= 2){
					aux = stack[pointer-2];
					stack[pointer-2] = stack[pointer-1];
					pointer -= (aux != 0) ? 1 : 2;
				}
				break;
			case _ifte: 
			case _if_then_else:
				if (pointer >= 3){
					stack[pointer-3] = (stack[pointer-3] != 0) ? stack[pointer-2] : stack[pointer-1];
					pointer -= 2;
				}
				break;
			//xor, iff (not bitwise, since these are doubles)
			case _xor:
				if (pointer >= 2){
					stack[pointer-2] = (((stack[pointer-2] != 0) && (stack[pointer-1] == 0)) || ((stack[pointer-2] == 0) && (stack[pointer-1] != 0))) 
? 1 : 0;
					pointer -= 1;
					//I think this could be done with a simple "!="
				}
				break;
			case _iff:
				if (pointer >= 2){
					stack[pointer-2] = (((stack[pointer-2] != 0) && (stack[pointer-1] != 0)) || ((stack[pointer-2] == 0) && (stack[pointer-1] == 0))) 
? 1 : 0;
					pointer -= 1;
				}
				break;
			//roll, rot, over, 
			case _over:
				if (pointer >= 2){
					stack[pointer] = stack[pointer-2];
					pointer += 1;
				}
				break;
			case _rot:
				//rot transforms the top three stack values from (a b c) to (b c a), i.e. shift left
				if (pointer >= 3){
					aux = stack[pointer-3];
					for (int j = pointer-3; j < pointer-1; j++){
						stack[j] = stack[j+1];
					}
					stack[pointer-1] = aux;
				}
				break;
			case _roll:
				//m n roll : rotate n times (shift right), but not the whole stack, only the top m
				if (pointer >= 2){
					aux = floor(fmax(fmin(stack[pointer-2],pointer),2));	//m
					aux2 = floor(stack[pointer-1]);	//n
					aux2 = (aux2 < 0) ? aux - fmod(abs(aux2), aux) : aux2;
					pointer -= 2;
					while (aux2 > 0){
						aux3 = stack[pointer-1];
						for (int j = pointer-1; j > pointer-aux; j--){
							stack[j] = stack[j-1];
						}
						stack[(int) (pointer-aux)] = aux3;
						aux2--;
					}
				}			
				break;
			case _index:
				//n index
				if (pointer >= 1){
					aux = floor(stack[pointer-1]);
					pointer -= 1;
					if (pointer >= aux){
						stack[pointer] = stack[(int) (pointer-aux)];
						pointer += 1;
					}
				}
				break;
			//might be able to implement map, reduce/fold and filter once I have blocks working
			case _stack:
				//print the whole stack, from top to bottom
				for (int j = pointer-1; j >= 0; j--){
					printf("%f\n", stack[j]);
				}
			case _equalsign:
				//remove the topmost object from the stack and print it
				if (pointer >= 1){
					printf("%f\n", stack[pointer-1]);
					pointer -= 1;
				}
				break;
			//count
			//e.g. count 0 eq
			//while? for? repeat? they all need blocks (which will later be implemented as lists on a stack of lists)
			case _count:
				stack[pointer] = pointer;
				pointer += 1;
				break; 
			default:
				stack[pointer] = (double) atof(argv[i]);
				pointer = pointer + 1;
				//if pointer > 1023 ...
		}
	}
	//print the stack
	for (int i = pointer-1; i >= 0; i--){
		printf("%f\n", stack[i]);
	}
	return 0;
}
