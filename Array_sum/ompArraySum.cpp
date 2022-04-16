/* arraySum.cpp uses an array to sum the values in an input file,
 *  whose name is specified on the command-line.
 */
#include <iostream>
#include <fstream>
#include <omp.h> // OpenMP
#include <chrono>

using std::cout, std::endl;
using namespace std::chrono;

void readArray(char *fileName, double **a, int *n);
double sumArray(double *a, int numValues, const int REPS);

int main(int argc, char *argv[])
{
    const int num_threads = 8;

    for (int threads = 1; threads < num_threads +1; threads*=2){
        int howMany;
        double sum;
        double *a;

        if (argc != 2)
        {
            cout << endl << "*** Usage: arraySum <inputFile>" << endl << endl;
            exit(1);
        }

        cout << "Threads used: " << threads << endl;

        readArray(argv[1], &a, &howMany);

        auto start = high_resolution_clock::now();
        sum = sumArray(a, howMany, threads);
        auto stop = high_resolution_clock::now();
        nanoseconds duration = duration_cast<nanoseconds>(stop - start);

        // There's an error when using microseconds with Chrono
        duration /= 1000; // Therefore we'll be converting nanoseconds(billionth) to microseconds(millionth) manually

        cout << "Elasped time = " << duration.count() << " microseconds" << endl;
        cout << "The sum of the values in the input file '" << argv[1] << "' is " << sum << endl << endl;

        free(a);
    }

    return 0;
}

void readArray(char *fileName, double **a, int *n)
{
    int count, howMany, id;
    double *tempA;
    std::ifstream fin(fileName);
    if (!fin.is_open())
    {
        cout << endl << "*** Unable to open input file ''" << fileName << "'" << endl << endl;
        exit(1);
    }

    fin >> howMany;
    tempA = (double *)calloc(howMany, sizeof(double));
    if (tempA == NULL)
    {
        cout << endl << "*** Unable to allocate " << howMany << "-length array" << endl << endl;
        exit(1);
    }
    
        for (count = 0; count < howMany; count++)    
            fin >> tempA[count];

    fin.close();

    *n = howMany;
    *a = tempA;
}


double sumArray(double *a, int numValues, const int REPS)
{
    int i, id, count;
    double result = 0.0;

    const int part = numValues / REPS;

#pragma omp parallel for reduction (+:result) private(count, i)
    for (i = 0; i < REPS; i++){
        for (count = (i * part); count < (part * (i + 1)); count++) {
            result += a[count];
        }
    }
    return result;  
}
