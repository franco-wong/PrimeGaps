#import <mpi.h>
int main (int argc, char *argv[]) {

	MPI_Init(&argc, &argv);
	int id,p,source,result,buffer,tag,status;

	mpz_t end,size,my_start,my_end,mod_end;
	int id,P,flag;
	end = argc[0];

	MPI_Comm_rank (MPI_COMM_WORLD, &id);
	MPI_Comm_size (MPI_COMM_WORLD, &p);

	//divide
	//hard coding values
	P = 8;

	mpz_set_str(end,argv[1], 10);
	flag = 0;
	mod_end = mpz_mod(end,P);
	if(id<mod_end){
		flag = 1;
	}
	mpz_fdiv_q(size,n,P);
	mpz_add(size, size, flag);

	//size = mpz_fdiv_q(np,n,P);
	mpz_mul(my_start,id,size);
	if(mpz_cmp(mod_end,P)>0){
		mpz_mul(my_start,my_start,P);
	}
	else{
		mpz_mul(my_start,my_start,end);
	}
	mpz_add(my_end,my_start,size);


mpz_t lgap, gap_lower, gap_upper;
unsinged long lgap_ui, gap_lower_ui, gap_upper_ui. buffer1, buffer2, buffer3; 

if (id==0){
	/*calculate the max difference for own set */
	lgap_ui = mpz_get_ui(lgap);
	gap_lower_ui = mpz_get_ui(gap_lower);
	gap_upper_ui = mpz_get_ui(gap_upper);

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
	printf("The maximum gap is %lu, which is between %lu and %lu",lgap_ui,gap_lower_ui, gap_upper_ui);
}else{
	/*calculate the max difference for own set */

	lgap_ui = mpz_get_ui(lgap);
	gap_lower_ui = mpz_get_ui(gap_lower);
	gap_upper_ui = mpz_get_ui(gap_upper);

	MPI_Send(lgap_ui,1,MPI_UNSIGNED_LONG,0,1,MPI_COMM_WORLD);
	MPI_Send(gap_lower_ui,1,MPI_UNSIGNED_LONG,0,2,MPI_COMM_WORLD);
	MPI_Send(gap_upper_ui,1,MPI_UNSIGNED_LONG,0,3,MPI_COMM_WORLD);
}
MPI_Finalize();
