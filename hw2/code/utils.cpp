#include "header.h"

void load_data(const char* fn, vector<F>& frame){
    printf("Loading data....");
    FILE *fp = fopen(fn, "rb");
    if(fp == NULL){
        puts("NULL !");
        exit(0);
    }

    unsigned char Y[W*H], Cb[W*H/4], Cr[W*H/4];
    
    for(int i = 0 ; i < 300 ; i++){
        if(fread(Y, 1, W*H, fp) != W*H){
            puts("ERROR Y");
            exit(0);
        }
        if(fread(Cb, 1, W*H/4, fp) != W*H/4){
            puts("ERROR Cb");
            exit(0);
        }
        if(fread(Cr, 1, W*H/4, fp) != W*H/4){
            puts("ERROR Cr");
            exit(0);   
        }
        F tmp;
        int cnt = 0;
        for(int i = 0 ; i < W ; i++)
            for(int j = 0 ; j < H ; j++)
                tmp.Y[i][j] = Y[cnt++];
        frame.push_back(tmp);
    }
    fclose(fp);

    if(frame.size() != 300){
        puts("ERROR frame size");
        exit(0);
    }
    puts("Complete !");
}

double MAE(unsigned char img[W][H], unsigned char ref[W][H], const int& ix, const int& iy, const int& rx, const int& ry){
    double sum = 0;
    for(int i = 0 ; i < block_size ; i++)
        for(int j = 0 ; j < block_size ; j++){
            int c = img[ix+i][iy+j];
            int r = ref[rx+i][ry+j];
            sum += abs(c-r);
        }
    return sum / (block_size*block_size);
}

double MSE(unsigned char img[W][H], unsigned char ref[W][H], const int& ix, const int& iy, const int& rx, const int& ry){
    double sum = 0;
    for(int i = 0 ; i < block_size ; i++)
        for(int j = 0 ; j < block_size ; j++){
            int c = img[ix+i][iy+j];
            int r = ref[rx+i][ry+j];
            sum += (c-r)*(c-r);
        }
    return sum / (W*H);
}

double PSNR(const double& mse){
    return 20*log10(255/sqrt(mse));
}

void write_psnr(const vector<vector<double> >& PSNRs){
    static string filename[5] = {"TSS", "FS", "TDL", "BS", "BBGDS"};
    
    FILE *fp = fopen("out.csv", "w");
    for(int i = 0 ; i < 5 ; i++)
        fprintf(fp, "%s%c", filename[i].data(), ",\n"[i==4]);
    for(int f = 0 ; f < PSNRs[0].size() ; f++){
        for(int p = 0 ; p < 5 ; p++)
            fprintf(fp, "%f,", PSNRs[p][f]);
        fprintf(fp, "\n");
    }
    fclose(fp);
}