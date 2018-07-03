#ifndef __HEADER_H__
#define __HEADER_H__

#include <cstdio>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <string>
using namespace std;

#define W 352/2
#define H 288/2
#define block_size 8

typedef struct{
	unsigned char Y[W][H];
}F;

typedef struct{
	int x[W/block_size][H/block_size];
	int y[W/block_size][H/block_size];
	double mae[W/block_size][H/block_size];
}MV;

double TSS_search(unsigned char (*)[H], unsigned char (*)[H]);
double TDL_search(unsigned char (*)[H], unsigned char (*)[H]);
double BS_search(unsigned char (*)[H], unsigned char (*)[H]);
double FS_search(unsigned char (*)[H], unsigned char (*)[H]);
double BBGDS_search(unsigned char (*)[H], unsigned char (*)[H]);

double MAE(unsigned char (*)[H], unsigned char (*)[H], const int&, const int&, const int&, const int&);
double MSE(unsigned char (*)[H], unsigned char (*)[H], const int&, const int&, const int&, const int&);
double PSNR(const double&);
void load_data(const char*, vector<F>&);
void write_psnr(const vector<vector<double> >&);

#endif