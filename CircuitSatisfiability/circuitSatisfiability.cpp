/* circuitSatifiability.cpp solves the Circuit Satisfiability
 *  Problem using a brute-force sequential solution.
 *
 *   The particular circuit being tested is "wired" into the
 *   logic of function 'checkCircuit'. All combinations of
 *   inputs that satisfy the circuit are printed.
 *
 *   16-bit version by Michael J. Quinn, Sept 2002.
 *   Extended to 32 bits by Joel C. Adams, Sept 2013.
 *   Adapted for HU-HPP by Huib Aldewereld, 2020.
 *   Adapted for C++ by Stephan van der Feest, 2022.
 */

#include <iostream>
#include <climits>
#include <mpi.h>
#include <math.h>

using std::cout, std::endl;

#define SIZE 32

/** EXTRACT_BIT is a macro that extracts the ith bit of number n.
 *
 * parameters: n, a number;
 *             i, the position of the bit we want to know.
 *
 * return: 1 if 'i'th bit of 'n' is 1; 0 otherwise
 */
#define EXTRACT_BIT(n, i) ((n & (1 << i)) ? 1 : 0)

int checkCircuit(int, bool *);

int main (int argc, char *argv[])
{
   unsigned int i, combination; // loop variable (32 bits)
    int id = -1, numProcesses = -1,
        start = -1, stop = -1;                // process id
   int count = 0, final_count = 0;        // number of solutions

   bool *v = (bool *)malloc(sizeof(bool) * SIZE); /* Each element is one of the 32 bits */

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &id);
   MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

   double startTime = 0.0, totalTime = 0.0;
   startTime = MPI_Wtime();

   // find chunk size for part of processes
   int chunkSize = (UINT_MAX / numProcesses);
   
   start = id * chunkSize;
   stop = start + chunkSize;

   for (combination = start; combination < stop; combination ++)
   {
      for (i = 0; i < SIZE; i++)
         v[i] = EXTRACT_BIT(combination, i);

      count += checkCircuit(id, v);
   }
   // source: https://www.youtube.com/watch?v=iA4uRZ8KT6o
   // takes the (int) count on each process and summs them up into "final_count" which is used by the root process (id 0)
   MPI_Reduce(&count, &final_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

   totalTime = MPI_Wtime() - startTime;
   
   if (id == 0){
      cout << "A total of " << final_count << " solutions were found." << endl << endl;
      cout << "Process " << id << " finished in " << totalTime << " seconds." << endl;
   }

   MPI_Finalize();
   return 0;   
}

/* checkCircuit() checks the circuit for a given input.
 * parameters: id, the id of the process checking;
 *             bits, the (long) rep. of the input being checked.
 *
 * output: the binary rep. of bits if the circuit outputs 1
 * return: 1 if the circuit outputs 1; 0 otherwise.
 */
int checkCircuit(int id, bool *v)
{
   if ( ( (v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
       && (!v[3] || !v[4]) && (v[4] || !v[5])
       && (v[5] || !v[6]) && (v[5] || v[6])
       && (v[6] || !v[15]) && (v[7] || !v[8])
       && (!v[7] || !v[13]) && (v[8] || v[9])
       && (v[8] || !v[9]) && (!v[9] || !v[10])
       && (v[9] || v[11]) && (v[10] || v[11])
       && (v[12] || v[13]) && (v[13] || !v[14])
       && (v[14] || v[15]) )
       &&
          ( (v[16] || v[17]) && (!v[17] || !v[19]) && (v[18] || v[19])
       && (!v[19] || !v[20]) && (v[20] || !v[21])
       && (v[21] || !v[22]) && (v[21] || v[22])
       && (v[22] || !v[31]) && (v[23] || !v[24])
       && (!v[23] || !v[29]) && (v[24] || v[25])
       && (v[24] || !v[25]) && (!v[25] || !v[26])
       && (v[25] || v[27]) && (v[26] || v[27])
       && (v[28] || v[29]) && (v[29] || !v[30])
       && (v[30] || v[31]) ) )
   {
      std::printf("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d \n", id,
             v[31], v[30], v[29], v[28], v[27], v[26], v[25], v[24], v[23], v[22],
             v[21], v[20], v[19], v[18], v[17], v[16], v[15], v[14], v[13], v[12],
             v[11], v[10], v[9], v[8], v[7], v[6], v[5], v[4], v[3], v[2], v[1], v[0]);
      fflush(stdout);
      return 1;
   } else {
      return 0;
   }
}
