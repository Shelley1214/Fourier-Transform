1. Compile FFTW3_MPI
    * mpicxx {filename}.cpp -lfftw3_mpi -lfftw3 -o {filename}
    
2. Execute MPI compiled code
    * mpirun -np 4 ./{filename} 
