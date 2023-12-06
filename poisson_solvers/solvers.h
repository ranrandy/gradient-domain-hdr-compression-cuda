#ifndef POISSON_SOLVER_H_INCLUDED
#define POISSON_SOLVER_H_INCLUDED

#include <iostream>
#include <algorithm>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#define FULL_MASK 0xffffffff
#define WARP_SIZE 32
#define MAX_THREADS 1024
#define M_PI 3.14159265358979323846


typedef void (*methodFunction)(const int, const int, const float*, const float*, const dim3, const dim3, float*);

/*
    Integration of all the single-grid solvers:
    
    0. Jacobi iterative method:
        x_i^{k+1} = \frac{1}{4} (x_{i-W}^k + x_{i-1}^k + x_{i+1}^k + x_{i+W}^k - divG_i).
    
        0.8 Gauss-Seidel iterative method:
            x_i^{k+1} = \frac{1}{4} (x_{i-W}^{k+1} + x_{i-1}^{k+1} + x_{i+1}^k + x_{i+W}^k - divG_i).
    
    1. Gauss-Seidel iterative method with Red-Black Reordering:
        x_{2i}^{k+1} = \frac{1}{4} (x_{i-W}^k + x_{i-1}^k + x_{i+1}^k + x_{i+W}^k - divG_i),
        x_{2i+1}^{k+1} = \frac{1}{4} (x_{i-W}^{k+1} + x_{i-1}^{k+1} + x_{i+1}^{k+1} + x_{i+W}^{k+1} - divG_i).
    
    2. Gauss-Seidel iterative method with Red-Black Reordering and Overrelaxation (SOR):
        x_{2i}^{k+1} = \frac{1}{4} (x_{i-W}^k + x_{i-1}^k + x_{i+1}^k + x_{i+W}^k - divG_i) * w_opt + x_i^k * (1 - w_opt),
        x_{2i+1}^{k+1} = \frac{1}{4} (x_{i-W}^{k+1} + x_{i-1}^{k+1} + x_{i+1}^{k+1} + x_{i+W}^{k+1} - divG_i) * w_opt + x_i^{k+1} * (1 - w_opt),
        w_{opt} = \frac{2}{1 + \sqrt{1 - \cos{\pi/max(H, W)}^2}}.
*/
int solver(
    const int H, const int W, 
    const float* d_divG, const int method,
    const int iterations, const float tolerance, const int checkFrequency,
    float* d_I_log
);


/*
    Full Multigrid Poisson Solver with Gauss-Seidel smoothing iteration + Red-Black Reordering and Overrelaxation (SOR).
*/
void fullMultigridSolver(
    const int H, const int W, 
    const float* d_divG, 
    const int iterations, const float tolerance, const int checkFrequency,
    float* d_I_log);


#endif