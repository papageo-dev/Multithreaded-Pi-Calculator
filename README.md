# Multithreaded Pi Calculator (C)

 Some multithreaded calculators for Pi, in C programming language.

# Code Information

 The code were created using "Code :: Blocks IDE" and I used "GNU Compiler for C / C ++".                                 
 Libraries: <stdio.h>, <string.h>,  <math.h>, <stdlib.h>, <unistd.h>, <pthread.h>                                                                 

# General Description

 These are some algorithms, which calculate the value of Pi, in various multithreaded ways.                               
 They divide the calculation into threads and the final result is the sum of the partial calculation of each thread.

# Analysis

 The piCalc1.c, piCalc2.c, piCalc3.c files show the solution to the problem of finding Pi(=3.14).
 Parallel computation using threads is applied to a shared memory system.

 • piCalc1.c:                                                                                                    
 In main () I make a loop of 0 to the maximum number of threads (#define ThreadsNum).                                 
 I want to create and initialize the thread structure variables (thread_data).                        
 In the same way (iterative loop) and using pthread_create() I call the Calculator() function.                         
 Calculator() calculates a part of Pi from each thread and stores it in the local variable my_sum.                        
 Next, I lock the shared variable my_data->sumPi of the thread_data structure, where the computations(my_sum)
 of all threads are added.                                                                                               
 Finally, in main() I call pthread_join() to wait for the thread to terminate each thread that counts                        
 and appears the end result(* (pArg-> sumPi)) and its exclusion(* (pArg-> sumPi) - PI25DT).                                             

 • piCalc2.c:                                                                                                            
 This solution works in the same way as piCalc1.c.                                                                       
 All that changes is the way each thread calculates part of Pi in the Calculator() function.                           
 
 • piCalc3.c:                                                                                                            
 In main() I make a loop from 0 to the maximum number of threads (#define ThreadsNum) that I want to                      
 create and using pthread_create() to call the Calculator() function with thread[rank].rank (thread number).               
 Calculator() calculates a part of Pi using Leibniz's algorithm and the result of each  thread is stored in                
 the threads thread[thread_rank].sumPi.                                                                                                                 
 Then, main() waits for the threads to be terminated using pthread_join().                                                 
 Finally, all the threads in the thread thread [thread_rank].sumPi are added to the pi variable and the final result(sumPi)
 and its exclusion(sumPi - PI25DT) are displayed.                                                                          
 (I didn't use lock here, because each thread stores its result in a table position and finally sums it to the SumPi variable.)
