#import <mpi.h>
int main (int argc, char *argv[]) {

	MPI_Init(&argc, &argv);
	int id,p,source,result,buffer,tag,status;
	MPI_Comm_rank (MPI_COMM_WORLD, &id);
	MPI_Comm_size (MPI_COMM_WORLD, &p);
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
