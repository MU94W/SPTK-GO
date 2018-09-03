# SPTK-GO
SPTK-GO -- let's make it faster!

cmds:

  implemented
  
    mlpgGO -- ~55 times faster than SPTK/mlpg using a single thread!
      
      build command:
    
      bazel build --config=copt //bin/mlpg:mlpgGO
      
      run command:
      
      OMP_NUM_THREADS=1 bazel-bin/bin/mlpg/mlpgGO -m 59 in.bin > out.bin
