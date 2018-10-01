


#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpi.h>


void min ( mpz_t the_min, mpz_t arg1, mpz_t arg2){
    
    int c = mpz_cmp( arg1,arg2);
    
    if (c > 0){
        mpz_set(the_min,arg1);
    }else{
        mpz_set(the_min,arg2);
    }

    
}


int main (int argc, char *argv[]) {

	MPI_Init(&argc, &argv);
	int id,p,source,tag;
    MPI_Status status;

	mpz_t end,size,my_start,my_end,mod_end,my_min, next_min, my_id, next_id, current_prime, prev_prime, gap, lgap,gap_upper, gap_lower;
	int flag;
    
    int buffer, result;
	

	MPI_Comm_rank (MPI_COMM_WORLD, &id);
	MPI_Comm_size (MPI_COMM_WORLD, &p);

	//divide
	//hard coding values
    
    

    
    mpz_init(end);
    mpz_init(size);
    mpz_init(my_start);
    mpz_init(my_end);
    mpz_init(mod_end);
    mpz_init(my_min);
    mpz_init(next_min);
    
    mpz_init(my_id);
    mpz_init(next_id);
    
   
    mpz_set_ui(my_id,id);
    mpz_set_ui(next_id, id+1);
    
    // i_start,p = id * (end // p) + min (p, mod(end,P))
    // i_start,p = id * size + min(p, mod_end)
    // i_start,p = id * size + min
    
	
    printf("id: %d p: %d \n", id, p);

	//mpz_set_str(end,argv[1], 10);
    
    mpz_set_str(end,"10000",10);
    
	mpz_mod_ui(mod_end,end,p);
    
    
   min (my_min, mod_end, my_id);
   min (next_min, mod_end, next_id);
	
    
	mpz_fdiv_q_ui(size,end,p);
	
    mpz_mul_ui(my_start,size, id);
    mpz_add(my_start, my_start,my_min);
    
    
    printf("id: %d p: %d  size: %d   my_id:  %d next_id: %d  mod_end: %d  my_min: %d, next_min: %d\n", id,p,mpz_get_ui(size),mpz_get_ui(my_id),mpz_get_ui(next_id), mpz_get_ui(mod_end),mpz_get_ui(my_min),mpz_get_ui(next_min));

    
    
    mpz_mul_ui(my_end,size, id+1);
    mpz_add(my_end, my_end,next_min);
    
    
    
    // requires my_start, my_end, end 
    // result in lgap, gap_lower and gap_upper
    
    // setup mpz variables
    mpz_init(current_prime);
    mpz_init(prev_prime);
    mpz_init(gap);
    mpz_init(lgap);
    mpz_init(gap_upper);
    mpz_init(gap_lower);
    
    // init mpz variables with initial values
	mpz_set_ui(gap,0);
    mpz_set_ui(lgap,0);
    mpz_set_ui(gap_lower,0);
    mpz_set_ui(gap_upper,0);
    
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
        
        // ignore gaps that contain values outside the main range
        if (mpz_cmp(current_prime, end)){
            break;
        }
        
        // check if current gap is larger than the previously recorded largest gap
        if (mpz_cmp(gap,lgap) >0 ){
            // record new largest gap info
            mpz_set(lgap, gap);
            mpz_set(gap_lower, prev_prime);
            mpz_set(gap_upper, current_prime);
            
        }
        
        
    }
   
    
    
    
    
    

    buffer = mpz_get_ui(lgap);
    result = mpz_get_ui(lgap);
    
    int i_start = mpz_get_ui(my_start);
    int i_end = mpz_get_ui(my_end);

    
    printf("id: %d p: %d  start: %d    end: %d\n", id,p,i_start, i_end);

    printf("id: %d p: %d  buffer: %d    result: %d\n", id,p,buffer, result);
    
     printf("id: %d p: %d    lower:%d upper:%d gap: %d  \n", id,p,mpz_get_ui(gap_lower), mpz_get_ui(gap_upper),mpz_get_ui(lgap));


	if (id==0){
		/*calculate the max difference for own set */
		for(source=1;source<p;source++){
			MPI_Recv(&buffer,1,MPI_INT,source,tag,MPI_COMM_WORLD,&status);
			result = result > buffer? result : buffer;
		}
		printf("The maximum gap is %d\n",result);
	}else{
		/*calculate the max difference for own set */
        printf("id:%d sending:%d\n",id, buffer);
		MPI_Send(&buffer,1,MPI_INT,0,tag,MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return 0;
}
