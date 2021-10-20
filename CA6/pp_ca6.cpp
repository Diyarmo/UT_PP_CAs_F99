#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <string>
#include <chrono> 
#include <iomanip> 



#define N  12

#define THREAD_NUM 6
int solutions = 0;
int solutions_P = 0;
int thread_solutions[THREAD_NUM];

int put(int Queens[], int row, int column)
{
 int i;
 for (i = 0; i < row; i++) {
  if (Queens[i] == column || abs(Queens[i] - column) == (row - i))
   return -1;
 }
 Queens[row] = column;
 if (row == N - 1) {
  solutions++;
 }
 else {
  for (i = 0; i < N; i++) { // increment row
   put(Queens, row + 1, i);
  }
 }
 return 0;
}


void solve(int Queens[]) {
 int i;
 for (i = 0; i < N; i++) {
  put(Queens, 0, i);
 }
}


#include <iostream>
#include <cstdlib>


using namespace std;
const int correctSolution[16] = { 0,     1,      0,       0, //  0 -  3
2,    10,      4,      40, //  4 -  7
92,   352,    724,    2680, //  8 - 11
14200, 73712, 365596, 2279184  // 12 - 15
};


void put_parallel(int queens[], int row, int col, int tid) {

 for (int i = 0; i < row; i++) {
  if ((queens[i] == col) || (abs(queens[i] - col) == (row - i))) {
   return;
  }
 }

 queens[row] = col;
 if (row == (N - 1)) {
#pragma omp atomic

  solutions_P++;
 }
 else {
  for (int i = 0; i < N; i++) {
   put_parallel(queens, row + 1, i, tid);
  }
 }
}

void solve_parallel() {
 int tid;
 #pragma omp parallel for private(tid)
 for (int i = 0; i < N; i++) {
  tid = omp_get_thread_num();
  int* queens = new int[N]; // Array of queens on the chess board.

  put_parallel(queens, 0, i, tid);

  delete[] queens;
 }
 for (int i = 0; i < THREAD_NUM; i++)
  solutions_P += thread_solutions[i];
}


int main() {
 std::cout << "Rasta Tadayon - 810196436\nDiyar Mohammadi - 810196553" << std::endl;
 using namespace std::chrono;
 std::chrono::time_point<std::chrono::system_clock> start_c, end_c;

 int Queens[N];

 ////serial 
 start_c = std::chrono::system_clock::now();
 solve(Queens);
 end_c = std::chrono::system_clock::now();
 std::chrono::duration<double> elapsed_seconds_serial = end_c - start_c;
 std::cout << std::setprecision(3) << std::fixed;

 std::cout << "serial result : " << solutions << std::endl;
 std::cout << "serial time = " << elapsed_seconds_serial.count() * 1000 << " ms" << std::endl;

 solutions = 0;

 //parallel
 omp_set_num_threads(THREAD_NUM);
 start_c = std::chrono::system_clock::now();
 for (int i = 0; i < THREAD_NUM; i++)
  thread_solutions[i] = 0;
 solve_parallel();
 end_c = std::chrono::system_clock::now();
 std::chrono::duration<double> elapsed_seconds_parallel = end_c - start_c;

 std::cout << "parallel result : " << solutions_P << std::endl;
 std::cout << "parallel time = " << elapsed_seconds_parallel.count() * 1000 << " ms" << std::endl;

 std::cout << "\nspeedup: " << elapsed_seconds_serial.count() / elapsed_seconds_parallel.count() << std::endl;
 float a;
 std::cin >> a;
 return 0;

}
