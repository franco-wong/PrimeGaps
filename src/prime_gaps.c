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
}
