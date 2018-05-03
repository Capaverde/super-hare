#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//only unsigned longs for both symbols and pointers. pointers point to heap. there is a dictionary which contains pairs of symbols and pointers
//instead of tagging, a type heap in a one-to-one correspondence with the heap
//insertsort into the dict and labels later
//(numbers also added)

//compile with: gcc heap.c -o heap

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


#define _opencurlybracket 123UL		//{
#define _closecurlybracket 125UL	//}
#define _forwardslash 47UL		// '/'
//#define _backslash 92UL			// '\'
#define _ampersand 38UL			//&
#define _def 6710628UL
#define _defun 474416047460UL		//like def, but (if type was 'p') forces type to be 'x' - executable
#define _eq 29029UL
#define _neq 7431534UL
#define _and 6581857UL
#define _or 29295UL
#define _not 7630702UL
#define _gt 29799UL
#define _lt 29804UL
#define _gteq 1902474343UL
#define _lteq 1902474348UL
#define _if 26217UL
#define _ifelse 111546414229097UL
#define _while 435610544247UL
#define _repeat 127961662514546UL
#define _for 7499622UL
#define _lit 7629164UL
#define _apply 521510350945UL
#define _head 1684104552UL
#define _tail 1818845556UL
#define _filter 125780070525286UL
#define _map 7364973UL
#define _reduce 111477845681522UL
#define _add 6579297UL
#define _mul 7107949UL
#define _sub 6452595UL
#define _mod 6582125UL
#define _lookup 123649615949676UL
#define _prettyprint 8246224458791350896UL
#define _type 1701869940UL
#define _deeptype 7309475736231175524UL
#define _null 1819047278UL
//#define deepmap
#define _dup 7370084UL
#define _swap 1885435763UL
#define _drop 1886351972UL


unsigned long primitives[] = {_opencurlybracket, _closecurlybracket,
				_def, _defun, _eq, _neq, _and, _or, _not, _gt, _lt, _gteq, _lteq, _if, _ifelse, _while, _repeat,
				_for, _lit, _apply, _head, _tail, _filter, _map, _reduce, _add, _mul, _sub, _mod, _lookup, _prettyprint, 
				_type, _deeptype, _dup, _swap};

typedef struct { unsigned long symbol; unsigned long meaning; char type; } entry;	//usually a pointer [index] to the heap, but can also be a number or a symbol
entry dict[128];	//symbol and meaning

typedef struct { unsigned long symbol; char string[10]; } label;
label labels[256];	//symbol and printable form

#define TYPE_NULL '\0'
#define TYPE_NUMBER 'n'
#define TYPE_SYMBOL 's'
#define TYPE_POINTER 'p'
#define TYPE_EXECUTABLE 'x'	//a pointer for code instead of data
#define TYPE_PRIMITIVE 'f'

unsigned long heap[512];
char typeheap[512];

unsigned long stack[1024];
char typestack[1024];

int pointer = 0;	//stack
int hpointer = 0;	//heap and typeheap
int dpointer = 0;	//dict
int lpointer = 0;	//labels

int binarysearch_labels(unsigned long symbol){	// label * labels, int lpointer, no need to pass them
	if (lpointer == 0){
		return -1;
	}
	int lowerbound = 0;
	int upperbound = lpointer - 1;
	int middle;
	if (labels[upperbound].symbol == symbol){
		return upperbound;
	}
	if (labels[lowerbound].symbol == symbol){
		return lowerbound;
	}
	while (1){
		middle = (lowerbound + upperbound)/2;
		if (labels[middle].symbol == symbol){
			return middle;
		} else if (labels[middle].symbol < symbol){
			lowerbound = middle;
		} else if (labels[middle].symbol > symbol){
			upperbound = middle;
		}
		if (upperbound - lowerbound <= 1){
			return -1;	//not found
		}
	}
}

void insertsort_labels(unsigned long symbol, char * string){	//label * labels, int lpointer, no need to pass them
	labels[lpointer].symbol = symbol;
	strcpy(labels[lpointer].string, string);
	unsigned long aux_s;
	char aux_str[10];
	for (int i = lpointer-1; i >= 0; i--){
		if (labels[i].symbol > labels[i+1].symbol){
			aux_s = labels[i].symbol;
			strcpy(aux_str, labels[i].string);
			labels[i].symbol = labels[i+1].symbol;
			strcpy(labels[i].string, labels[i+1].string);
			labels[i+1].symbol = aux_s;
			strcpy(labels[i+1].string, aux_str);
		} else {
			break;
		}
	}
	lpointer += 1;	//this is a side-effect outside the function
}

int binarysearch_dict(unsigned long symbol){	//entry * dict, int dpointer, no need to pass them
	if (dpointer == 0){
		return -1;
	}
	int lowerbound = 0;
	int upperbound = dpointer - 1;
	int middle;
	if (dict[upperbound].symbol == symbol){
		return upperbound;
	}
	if (dict[lowerbound].symbol == symbol){
		return lowerbound;
	}
	while (1){
		middle = (lowerbound + upperbound)/2;
		if (dict[middle].symbol == symbol){
			return middle;
		} else if (dict[middle].symbol < symbol){
			lowerbound = middle;
		} else if (dict[middle].symbol > symbol){
			upperbound = middle;
		}
		if (upperbound - lowerbound <= 1){
			return -1;	//not found
		}
	}
}

void insertsort_dict(unsigned long symbol, unsigned long meaning, char type){	//entry * dict, int dpointer, no need to pass them
	dict[dpointer].symbol = symbol;
	dict[dpointer].meaning = meaning;
	dict[dpointer].type = type;
	unsigned long aux_s;
	unsigned long aux_m;
	char aux_t;
	for (int i = dpointer-1; i >= 0; i--){
		if (dict[i].symbol > dict[i+1].symbol){
			aux_s = dict[i].symbol;
			aux_m = dict[i].meaning;
			aux_t = dict[i].type;
			dict[i].symbol = dict[i+1].symbol;
			dict[i].meaning = dict[i+1].meaning;
			dict[i].type = dict[i+1].type;
			dict[i+1].symbol = aux_s;
			dict[i+1].meaning = aux_m;
			dict[i+1].type = aux_t;
		} else {
			break;
		}
	}
	dpointer += 1;	//this is a side-effect outside the function
}


void apply(unsigned long symbol, char * item, int quoting){
	unsigned long aux;
	unsigned long aux2;
	unsigned long aux3;
	unsigned long aux4;
	int auxi;
	char auxt;
	char auxt2;
	int i;
	char * end;
	char auxstr[10];
	if (quoting == 1 && symbol != _opencurlybracket && symbol != _closecurlybracket){
		switch(symbol){
			case _def:
			case _defun:
			case _eq:
			case _neq:
			case _and:
			case _or:
			case _not:
			case _gt:
			case _lt:
			case _gteq:
			case _lteq:
			case _apply:
			case _if:
			case _ifelse:
			case _while:
			case _repeat:
			case _for:
			case _head:
			case _tail:
			case _filter:
			case _map:
			case _reduce:
			case _add:
			case _mul:
			case _sub:
			case _mod:		//common case of bug is forgetting to add the command here - should need to only add in one place instead
			case _lookup:
			case _prettyprint:
			case _type:
			case _deeptype:
			case _null:
			case _dup:
			case _swap:
			case _drop:
				stack[pointer] = symbol;
				typestack[pointer] = 'f';
				pointer += 1;				
				break;
			default:
				stack[pointer] = (unsigned long) strtoul(item, &end, 10);
				if (*end == '\0'){ 
					typestack[pointer] = 'n';
					pointer += 1;
					return;
				}
				//interpret it as a symbol
				if (item[0] == '/'){	//quote
					auxi = binarysearch_dict(symbol);
					aux = hash(&item[1]);
					if (auxi == -1){
						insertsort_dict(symbol, aux, 's');
					}
					auxi = binarysearch_labels(aux);
					if (auxi == -1){
						insertsort_labels(aux, &item[1]);
					}
				}
				stack[pointer] = symbol;
				typestack[pointer] = 's';
				pointer += 1;
				auxi = binarysearch_labels(symbol);
				if (auxi == -1){
					insertsort_labels(symbol, item);
				}		
		}
	} else {
		switch (symbol){	//moved outside main for recursion
			//	block symbols are { and } (after PostScript)
			case _opencurlybracket:
				stack[pointer] = _opencurlybracket;
				typestack[pointer] = 's';
				pointer += 1;
				break;
			case _closecurlybracket:
				stack[pointer] = _closecurlybracket;
				typestack[pointer] = 's';
				pointer += 1;
				for (i = pointer-1; i >= 0; i--){
					if (stack[i] == _opencurlybracket){
						break;
					}
				}
				if (i < 0){
					pointer = 0;	//no open bracket found, nothing to be done, clear the stack and move on
					printf("Unmatched bracket\n");
					break;
				}					
				pointer = i;
				aux = hpointer;
				while (1){
					i += 1;
					if (stack[i] != _closecurlybracket){
						heap[hpointer] = stack[i];
						typeheap[hpointer] = typestack[i];		//s, p, f
						hpointer += 1;
					} else {
						break;
					}
				}
				heap[hpointer] = 0;	//termination
				typeheap[hpointer] = TYPE_NULL;
				hpointer += 1;
				//push the pointer to the start of the block to the stack
				stack[pointer] = aux;
				typestack[pointer] = 'p';
				pointer += 1;
				break;
			case _def:
				if (pointer >= 2){
					//stack[pointer-1] is the symbol, stack[pointer-2] is the pointer (or number or another symbol)
					auxi = binarysearch_dict(stack[pointer-1]);
					if (auxi == -1){
						insertsort_dict(stack[pointer-1], stack[pointer-2], typestack[pointer-2]);
					} else {
						dict[auxi].symbol = stack[pointer-1];
						dict[auxi].meaning = stack[pointer-2];
						dict[auxi].type = typestack[pointer-2];
					}
					pointer -= 2;
				}
				break;
			case _defun:
				if (pointer >= 2){
					//p s defun
					auxi = binarysearch_dict(stack[pointer-1]);
					auxt = (typestack[pointer-2] == 'p') ? 'x' : typestack[pointer-2];
					if (auxi == -1){
						insertsort_dict(stack[pointer-1], stack[pointer-2], auxt);
						if (auxt == 'x'){
							insertsort_dict(_ampersand + stack[pointer-1]*256, stack[pointer-2], 'p');
						}
					} else {
						dict[auxi].symbol = stack[pointer-1];
						dict[auxi].meaning = stack[pointer-2];
						dict[auxi].type = auxt;
						if (auxt == 'x'){
							auxi = binarysearch_dict(_ampersand + stack[pointer-1]*256);
							if (auxi == -1){
								insertsort_dict(_ampersand + stack[pointer-1]*256, stack[pointer-2], 'p');
							} else {
								dict[auxi].symbol = _ampersand + stack[pointer-1]*256;
								dict[auxi].meaning = stack[pointer-2];
								dict[auxi].type = 'p';
							}
						}
					}
					pointer -= 2;
				}
				break;
			case _eq:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-1] == stack[pointer-2]) ? 1 : 0;
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _neq:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-1] != stack[pointer-2]) ? 1 : 0;				
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _and:
				if (pointer >= 2){
					stack[pointer-2] = ((stack[pointer-1] != 0) && (stack[pointer-2] != 0)) ? 1 : 0;
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _or:
				if (pointer >= 2){
					stack[pointer-2] = ((stack[pointer-1] != 0) || (stack[pointer-2] != 0)) ? 1 : 0;
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _not:
				if (pointer >= 1){
					stack[pointer-1] = (stack[pointer-1] == 0) ? 1 : 0;
					typestack[pointer-1] = 'n';
				}
				break;
			case _gt:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] > stack[pointer-1]) ? 1 : 0;
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _lt:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] < stack[pointer-1]) ? 1 : 0;
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _gteq:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] >= stack[pointer-1]) ? 1 : 0;
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _lteq:
				if (pointer >= 2){
					stack[pointer-2] = (stack[pointer-2] <= stack[pointer-1]) ? 1 : 0;
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _apply:
				//... f apply
				//lookup heap, interpret it up til null is found
				if (pointer >= 1){
					aux = stack[pointer-1];	//a pointer
					auxt = typestack[pointer-1];
					if (auxt != 'p' && auxt != 'f' && auxt != 'x'){
						return;
					}				
					pointer -= 1; 
					if (auxt == 'f'){
						apply(aux, NULL, 0);
						return;
					}
					//iterate from heap[aux], aux++, until heap[aux] == null
					//check its type at typeheap[aux] to know how to interpret it, it can be s, n, p, or f
					while (typeheap[aux] != TYPE_NULL){
						switch (typeheap[aux]){
							case 's':
								stack[pointer] = heap[aux];
								typestack[pointer] = 's';
								pointer += 1;
								apply(_lookup, NULL, 0);	//dictionary lookup is automatic, use '/' to quote (e.g. 10 /n def)
								if (typestack[pointer-1] == 'x'){
									apply(_apply, NULL, 0);
								}
								break;
							case 'n':
								stack[pointer] = heap[aux];
								typestack[pointer] = 'n';
								pointer += 1;
								break;
							case 'f':
								apply(heap[aux], NULL, 0);
								break;
							case 'p':
								stack[pointer] = heap[aux];
								typestack[pointer] = 'p';
								pointer += 1;
								//apply(_apply, NULL, 0);	//apply must be explicit for pointers 'p', while it is automatic for pointers 'x' is code or data
								break;
							/*case 'x':
								stack[pointer] = heap[aux];
								typestack[pointer] = 'x';
								pointer += 1;
								apply(_apply, NULL, 0);
								break;*/
							//no default, these are the only possible ones
						}
						aux += 1;
					}
				}
				break;
			case _if:
				//cond f if
				if (pointer >= 2){
					if (stack[pointer-2] != 0){
						aux = stack[pointer-1];
						auxt = typestack[pointer-1];
						pointer -= 2;
						//apply aux here
						stack[pointer] = aux;
						typestack[pointer] = auxt;
						pointer += 1;
						apply(_apply, NULL, 0);
					} else {
						pointer -= 2;
					}
				}
				break;
			case _ifelse:
				//cond f1 f2 ifelse
				if (pointer >= 3){
					aux = (stack[pointer-3] != 0) ? stack[pointer-2] : stack[pointer-1];
					auxt = (stack[pointer-3] != 0) ? typestack[pointer-2] : typestack[pointer-1];
					pointer -= 3;
					//apply aux here
					stack[pointer] = aux;
					typestack[pointer] = auxt;
					pointer += 1;
					apply(_apply, NULL, 0);
				}
				break;
			case _while:
				//cond f while, but cond is an expression
				if (pointer >= 2){
					aux = stack[pointer-2];
					auxt = typestack[pointer-2];
					aux2 = stack[pointer-1];
					pointer -= 2;
					while (1){
						stack[pointer] = aux;
						typestack[pointer] = auxt;	//could also be 'n', no? 1 f while - though there's no exit statement yet
						pointer += 1;
						apply(_apply, NULL, 0);	//eval cond
						if (stack[pointer-1] != 0){
							pointer -= 1;
							stack[pointer] = aux2;
							typestack[pointer] = 'p';
							pointer += 1;
							apply(_apply, NULL, 0);
						} else {
							pointer -= 1;
							break;
						}
					}
				}
				break;
			case _repeat:
				//f n repeat	//repeat f (i.e apply) n times
				if (pointer >= 2){
					aux = stack[pointer-2];
					aux2 = stack[pointer-1];
					pointer -= 2;
					for (i = 0; i < aux2; i++){
						stack[pointer] = aux;
						typestack[pointer] = 'p';
						pointer += 1;
						apply(_apply, NULL, 0);
					}
				}
				break;
			case _for:
				//start end step f for
				if (pointer >= 4){
					aux = stack[pointer-4];
					aux2 = stack[pointer-3];
					aux3 = stack[pointer-2];
					aux4 = stack[pointer-1];
					pointer -= 4;
					for (i = aux; i < aux2; i += aux3){
						//i f a bunch of times
						stack[pointer] = i;
						typestack[pointer] = 'n';
						pointer += 1;
						stack[pointer] = aux4;
						typestack[pointer] = 'p';
						pointer += 1;
						apply(_apply, NULL, 0);
					}
				}
				break;
			case _head:
				if (pointer >= 1){
					aux = stack[pointer-1];
					pointer -= 1;
					stack[pointer] = heap[aux];
					typestack[pointer] = typeheap[aux];
					pointer += 1;
				}
				break;
			case _tail:
				if (pointer >= 1){
					aux = stack[pointer-1];
					pointer -= 1;
					stack[pointer] = aux + 1;
					typestack[pointer] = 'p';
					pointer += 1;
				}
				break;
			case _filter:
				//list f filter
				if (pointer >= 2){
					aux = stack[pointer-2];
					aux2 = stack[pointer-1];
					auxt = typestack[pointer-1];	//'p' or 'f'
					pointer -= 2;
					aux3 = hpointer;
					while (typeheap[aux] != TYPE_NULL){
						stack[pointer] = heap[aux];	// list head
						typestack[pointer] = typeheap[aux];
						pointer += 1;
						stack[pointer] = aux2;
						typestack[pointer] = auxt;
						pointer += 1;
						apply(_apply, NULL, 0);
						if (stack[pointer-1] != 0) {
							heap[hpointer] = heap[aux];
							typeheap[hpointer] = typeheap[aux]; 
							hpointer += 1;
							pointer -= 1;
						} else {
							pointer -= 1;
						}
						aux += 1;
					}
					heap[hpointer] = 0;
					typeheap[hpointer] = TYPE_NULL;
					hpointer += 1;
					stack[pointer] = aux3;
					typestack[pointer] = 'p';
					pointer += 1; 
				}
				break;
			case _map:
				//list f map
				if (pointer >= 2){
					aux = stack[pointer-2];
					aux2 = stack[pointer-1];
					auxt = typestack[pointer-1];
					pointer -= 2;
					aux3 = hpointer;
					while (typeheap[aux] != TYPE_NULL){
						stack[pointer] = heap[aux];	// list head
						typestack[pointer] = typeheap[aux];
						pointer += 1;
						stack[pointer] = aux2;
						typestack[pointer] = auxt;
						pointer += 1;
						apply(_apply, NULL, 0);
						heap[hpointer] = stack[pointer-1];
						typeheap[hpointer] = typestack[pointer-1]; 
						hpointer += 1;
						pointer -= 1;
						aux += 1;
					}
					heap[hpointer] = 0;
					typeheap[hpointer] = TYPE_NULL;
					hpointer += 1;
					stack[pointer] = aux3;
					typestack[pointer] = 'p';
					pointer += 1;
				}
				break;
			case _reduce:
				//list acc f reduce, e.g. { 1 2 3 4 } 0 { add } reduce
				if (pointer >= 3){
					aux = stack[pointer-3];
					aux2 = stack[pointer-2];
					auxt = typestack[pointer-2];
					aux3 = stack[pointer-1];
					auxt2 = typestack[pointer-1];
					pointer -= 3;
					//first place acc on stack
					stack[pointer] = aux2;
					typestack[pointer] = auxt;
					pointer += 1;
					//now just put list head on stack, f on stack, apply, increment aux
					while (typeheap[aux] != TYPE_NULL){
						stack[pointer] = heap[aux];
						typestack[pointer] = typeheap[aux];
						pointer += 1;
						stack[pointer] = aux3;
						typestack[pointer] = auxt2;
						pointer += 1;
						apply(_apply, NULL, 0);
						aux += 1;
					}
				}
				break;
			case _add:
				if (pointer >= 2){
					stack[pointer-2] = stack[pointer-2] + stack[pointer-1];
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _mul:
				if (pointer >= 2){
					stack[pointer-2] = stack[pointer-2] * stack[pointer-1];
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _sub:
				if (pointer >= 2){
					stack[pointer-2] = stack[pointer-2] - stack[pointer-1];
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _mod:
				if (pointer >= 2){
					stack[pointer-2] = stack[pointer-2] % stack[pointer-1];
					typestack[pointer-2] = 'n';
					pointer -= 1;
				}
				break;
			case _lookup:	//explicit dictionary lookup
				if (pointer >= 1){
					auxi = binarysearch_dict(stack[pointer-1]);
					if (auxi == -1){
						//nothing
					} else {
						stack[pointer-1] = dict[auxi].meaning;
						typestack[pointer-1] = dict[auxi].type;
					}
				}
				break;
			//need a way to pretty print lists
			case _prettyprint:	//in fact truncated to 'prettypr' - unsigned long only takes 8 bytes (could this differ between machines?)
				if (pointer >= 1){
					aux = stack[pointer-1];
					auxt = typestack[pointer-1];
					pointer -= 1;
					//if auxt == 'p', recursively print all content
					switch (auxt)	{
						case 'n':
							printf("%lu\n", aux);
							break;
						case 's':
							printf("%s\n", labels[binarysearch_labels(aux)].string);	//assuming found because I'm hurried
							break;
						case TYPE_NULL:
							printf("%lu (null)\n", aux);
							break;
						case 'f':
							printf("f %lu ", aux);		//primitives aren't in labels - gonna need to create a table for them later
							break;
						case 'p':
						case 'x':
							printf("{\n");
							while (typeheap[aux] != TYPE_NULL){
								stack[pointer] = heap[aux];
								typestack[pointer] = typeheap[aux];
								pointer += 1;
								apply(_prettyprint, NULL, 0);
								aux += 1;
							}
							printf("}\n");
							break;
						default:
							printf("%lu %c\n", aux, auxt);
					}
				}
				break;
			case _type:
				//x type
				if (pointer >= 1){
					//need to create a symbol from the typestack value
					stack[pointer-1] = (unsigned long) typestack[pointer-1];
					typestack[pointer-1] = 's';
					//I would do a ... = hash(typestack[pointer-1]), but it's all chars, and chars have the same value as their hashes
					auxi = binarysearch_labels(stack[pointer-1]);
					if (auxi == -1){
						auxstr[0] = (char) stack[pointer-1];
						auxstr[1] = '\0';
						insertsort_labels(stack[pointer-1], auxstr);
					}
				}
				break;
			case _deeptype:
				if (pointer >= 1){	//this would do best to return a string, not a symbol, because deeptypes can be larger than 8 chars
					//stack[pointer-1] = (unsigned long) typestack[pointer-1];
					//typestack[pointer-1] = 's';
					if (typestack[pointer-1] == 'p'){
						
					} else {

					} 
				}
				break;
			case _null:
				//{ type 0 eq } /null defun
				if (pointer >= 1){
					stack[pointer-1] = (typestack[pointer-1] == TYPE_NULL) ? 1 : 0;
					typestack[pointer-1] = 'n';
				}
				break;
			case _dup:
				if (pointer >= 1){
					stack[pointer] = stack[pointer-1];
					typestack[pointer] = typestack[pointer-1];
					pointer += 1;
				}
				break;
			case _swap:
				//a b swap
				if (pointer >= 2){
					aux = stack[pointer-2];
					auxt = typestack[pointer-2];
					stack[pointer-2] = stack[pointer-1];
					typestack[pointer-2] = typestack[pointer-1];
					stack[pointer-1] = aux;
					typestack[pointer-1] = auxt;
				}
				break;
			case _drop:
				if (pointer >= 1){
					pointer -= 1;
				}
				break;
			//default:
				/* stack[pointer] = (unsigned long) strtoul(item, &end, 10);	//non functional, pretty much
				if (*end == '\0'){ 
					typestack[pointer] = 'n';
					pointer += 1;
					return;
				}
				//interpret it as a symbol
				if (item[0] == '/'){	//quote
					auxi = binarysearch_dict(symbol);
					if (auxi == -1){
						//insertsort_dict(symbol, hash(&item[1]), 's');
					}
				}
				stack[pointer] = symbol;
				typestack[pointer] = 's';
				pointer += 1;
				auxi = binarysearch_labels(symbol);
				if (auxi == -1){
					insertsort_labels(symbol, item);
				}*/
		}
	}
}



int main(int argc, char * argv[]){
	FILE * file;
	char item[20];
	char status;
	int ret;
	int auxi;
	for (int i = 0; i < sizeof(primitives)/sizeof(unsigned long); i++){
		insertsort_dict(_ampersand + primitives[i]*256, primitives[i], 'f');
	}
	if (argc > 1){	//file argument
		file = fopen(argv[1], "r");
		if (file == NULL){
			printf("No such file: %s\n", argv[1]);
			exit(1);
		}
	} else {
		file = stdin;
	}
	apply(_opencurlybracket, NULL, 1);
	while (1){
		ret = fscanf(file, "%19s", item, &status);
		if (ret == EOF){
			break;
		}
		apply(hash(item), item, 1);
	}
	apply(_closecurlybracket, NULL, 1);
	apply(_apply, NULL, 0);
	//print the stack
	for (int i = pointer-1; i >= 0; i--){		//either symbols, numbers or pointers
		switch (typestack[i]){
			case 'n':
				printf("%lu\n", stack[i]);
				break;
			case 'p':
				printf("_%lu\n", stack[i]);	//'_' means pointer
				break;
			case 's':
				auxi = binarysearch_labels(stack[i]);
				if (auxi != -1){
					strcpy(item, labels[auxi].string);
					printf("%s\n", item);
				} else {
					printf("s%lu\n", stack[i]);
				}
				break;
			case TYPE_NULL:
				printf("%lu(null)", stack[i]);
				break;
			default:
				printf("%c%lu\n", typestack[i], stack[i]);
		}
	}
	return 0;
}
