/*
 ============================================================================
 Name        : prime_gaps.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

int main(void) {
	char input_string[] = "1000";
	mpz_t n;
	mpz_t p;
	mpz_init(n);
	mpz_init(p);
	mpz_set_ui(n,50);
	mpz_set_ui(p,0);
	int flag;
	flag = mpz_set_str(n,input_string, 10);
	mpz_out_str(stdout,10,n);
	printf("\n");
	mpz_nextprime(p, n);
	mpz_out_str(stdout,10,p);
	printf("\n");

	//dividing up the work
	mpz_t np;
	mpz_init(np);
	//int P = 8;
	mpz_t P;
	mpz_init(P);
	mpz_init_set(P, 8);

	mpz_fdiv_q(np,n,P);

	mpz_t proc_work[P];
	int x;
	for(x=0;x<P;x++){
		if(x < mpz_mod(n,P)){
			mpz_add(proc_work[x],np,1);
		}
		else{
			mpz_init_set(proc_work[x],np);
		}
	}

	mpz_t first_part;

	mpz_t proc_start[P];
	for(x=0;x<P;x++){
		mpz_mul(first_part,np,x);
		if(mpz_cmp(p,mpz_mod(n,P))>0){
			mpz_add(proc_start[x],first_part,P);
		}
		else{
			mpz_add(proc_start[x],first_part,n);
		}

	}



}
