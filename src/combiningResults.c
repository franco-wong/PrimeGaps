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







	if (id==0){
		/*calculate the max difference for own set */
		for(source=1;source<p;source++){
			MPI_Recv(&buffer,1,MPI_INT,source,tag,MPI_COMM_WORLD,&status);
			result=max(result,buffer);
		}
		printf("The maximum gap is %d",result);
	}else{
		/*calculate the max difference for own set */
		MPI_Send(&buffer,1,MPI_INT,0,tag,MPI_COMM_WORLD);
	}
	MPI_Finalize();
	return result;
}
