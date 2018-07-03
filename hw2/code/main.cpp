# include "header.h"

int main(int argc, char* argv[]){

	vector<F> frame;
	vector<vector<double> > PSNRs;
	load_data(argv[1], frame);

	typedef double (*func)(unsigned char (*)[H], unsigned char (*)[H]);
	func searchFunctions[5] = {TSS_search, FS_search, TDL_search, BS_search, BBGDS_search};
	
	printf("Start search....");
	for(int fn = 0 ; fn < 5 ; fn++){
		vector<double> P;
		for(int f = 1 ; f < frame.size() ; f++){
			double psnr = searchFunctions[fn](frame[f].Y, frame[f-1].Y);
			P.push_back(psnr);
		}
		PSNRs.push_back(P);
	}
	write_psnr(PSNRs);
	puts("Complete !");
	return 0;
}