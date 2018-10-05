#include <stdio.h>
#include <gmp.h>
#include "mpi.h"

main (int argc, char *argv[]) {

    //double startTime, endTime, total;
    //startTime = MPI_Wtime();

    MPI_Init(&argc, &argv);

    int id,p,source,buffer,tag,flag,test;
    MPI_Status status;
    mpz_t end,size,my_start,my_end,mod_end,current_prime,prev_prime,gap,lgap,gap_upper,gap_lower,P,mpzId,mpzFlag;

    unsigned long lgap_ui, gap_lower_ui, gap_upper_ui, buffer1, buffer2, buffer3;

    double startTime, endTime, total;

    MPI_Barrier(MPI_COMM_WORLD);
    startTime = MPI_Wtime();

    // setup mpz variables
    mpz_init(mpzId);
    mpz_init(mpzFlag);
    mpz_init(P);
    mpz_init(current_prime);
    mpz_init(prev_prime);
    mpz_init(gap);
    mpz_init(lgap);
    mpz_init(gap_upper);
    mpz_init(gap_lower);
    mpz_init(end);
    mpz_init(size);
    mpz_init(my_start);
    mpz_init(my_end);
    mpz_init(mod_end);


    // init mpz variables with initial values
    //mpz_set_ui(P,atoi(argv[0]));
    mpz_set_ui(gap,0);
    mpz_set_ui(lgap,0);
    mpz_set_ui(gap_lower,0);
    mpz_set_ui(gap_upper,0);

//    end = argc[0];

    MPI_Comm_rank (MPI_COMM_WORLD, &id);
    MPI_Comm_size (MPI_COMM_WORLD, &p);
    mpz_set_ui(P,p);
    //divide the data into appropriate chunks - load balancing
    mpz_set_ui(mpzId,id);
    mpz_set_ui(mpzFlag,0);

    mpz_set_str(end,argv[1], 10);               //n

    mpz_mod(mod_end,end,P);                             //mod(n,P)
    if(mpz_cmp(mpzId,mod_end)<0){               //rank < mod(n,P)
        mpz_set_ui(mpzFlag,1);                  //conditional
    }
    mpz_fdiv_q(size,end,P);                             // n/P = size
 //size = mpz_fdiv_q(np,n,P);
    mpz_mul(my_start,mpzId,size);               // start = rank*size +min(rank, mod(n,P)
    if(mpz_cmp(P,mod_end)>0){
        mpz_add(my_start, my_start, mod_end);
    }
    else{
        mpz_add(my_start, my_start, P);
    }
    mpz_add(size, size, mpzFlag);               //size + conditional
/*    if(mpz_cmp(mod_end,P)>0){                 //start =
        mpz_mul(my_start,my_start,P);
    }
    else{
       mpz_mul(my_start,my_start,end);
    }
*/
    mpz_add(my_end,my_start,size);


    // requires my_start, my_end, end
    // result in lgap, gap_lower and gap_upper


    // find first prime in range
    mpz_sub_ui(current_prime, my_start, 1);
    mpz_nextprime(current_prime,current_prime);

    // continue while still in range
    while ( mpz_cmp (current_prime, my_end) <0){

        // set prev_prime to the previously found prime
        mpz_set(prev_prime, current_prime);
        // sent current_prime to the next prime after the prev prime
        mpz_nextprime(current_prime,current_prime);

        // calculate gap between prev prime and current prime
        mpz_sub(gap, current_prime, prev_prime);

        // check if current gap is larger than the previously recorded largest gap
        if (mpz_cmp(gap,lgap) >0 ){
            // record new largest gap info
            mpz_set(lgap, gap);
            mpz_set(gap_lower, prev_prime);
            mpz_set(gap_upper, current_prime);
        }

                // ignore gaps that contain values outside the main range
                if (mpz_cmp(current_prime, end)>0){
            break;
        }
    }

    lgap_ui = mpz_get_ui(lgap);
    gap_lower_ui = mpz_get_ui(gap_lower);
    gap_upper_ui = mpz_get_ui(gap_upper);


	if (id==0){

		for(source=1;source<p;source++){
			MPI_Recv(&buffer1,1,MPI_UNSIGNED_LONG,source,1,MPI_COMM_WORLD,&status);
			MPI_Recv(&buffer2,1,MPI_UNSIGNED_LONG,source,2,MPI_COMM_WORLD,&status);
			MPI_Recv(&buffer3,1,MPI_UNSIGNED_LONG,source,3,MPI_COMM_WORLD,&status);
			if (buffer1>lgap_ui){
				lgap_ui = buffer1;
				gap_lower_ui = buffer2;
				gap_upper_ui = buffer3;
			}
		}
		printf("The maximum gap is %lu, which is between %lu and %lu.\n",lgap_ui,gap_lower_ui, gap_upper_ui);
		MPI_Barrier(MPI_COMM_WORLD);
		endTime = MPI_Wtime();
		total = endTime - startTime;
		printf("The elapsed time for %i processes is %f seconds.", id,total);

	}else{

//        lgap_ui = mpz_get_ui(lgap);
//        gap_lower_ui = mpz_get_ui(gap_lower);
//        gap_upper_ui = mpz_get_ui(gap_upper);

		MPI_Send(&lgap_ui,1,MPI_UNSIGNED_LONG,0,1,MPI_COMM_WORLD);
		MPI_Send(&gap_lower_ui,1,MPI_UNSIGNED_LONG,0,2,MPI_COMM_WORLD);
		MPI_Send(&gap_upper_ui,1,MPI_UNSIGNED_LONG,0,3,MPI_COMM_WORLD);
	}
 
    //endTime = MPI_Wtime();
    //total = endTime - startTime;
    //printf("The elapsed time for %i processes is %f seconds",id,total);

    MPI_Finalize();
}

