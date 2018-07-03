#include "header.h"

double TSS_search(unsigned char img[W][H], unsigned char ref[W][H]){
	//printf("Start TSS search...");
	//MV motion_vector;
	double mse = 0;
    for(int i = 0 ; i <= W-block_size ; i += block_size){
		for(int j = 0 ; j <= H-block_size ; j += block_size){

			// current block index (i, j)
			int match_x = i, match_y = j, step_size = 8, x = i, y = j;
			double min_val = INT_MAX;

			for(int tss = 0 ; tss < 3 ; tss++){
				// (x, y) current center
				for(int k = -step_size ; k <= step_size ; k+=step_size){
					for(int l = -step_size ; l <= step_size ; l+=step_size){
						//ref block index (i+k, j+l)
						if(x+k < 0 || x+k > W-block_size || y+l < 0 || y+l > H-block_size)
							continue;
						double mae = MSE(img, ref, i, j, x+k, y+l);
						if(mae < min_val){
							match_x = x+k;
							match_y = y+l;
							min_val = mae;
						}
					}
				}
				x = match_x;
				y = match_y;
				step_size /= 2;
			}
			// cur + motion = ref
			//motion_vector.mae[i/8][j/8] = min_val;
			//motion_vector.x[i/8][j/8] = match_x - i;
			//motion_vector.y[i/8][j/8] = match_y - j;
			mse += min_val;
		}
	}
	//puts("complete !");
	return PSNR(mse);
	//return motion_vector;
}

double TDL_search(unsigned char img[W][H], unsigned char ref[W][H]){
	//printf("Start TDL search...");
	//MV motion_vector;
	double mse = 0;
    for(int i = 0 ; i <= W-block_size ; i += block_size){
		for(int j = 0 ; j <= H-block_size ; j += block_size){
			// current block index (i, j)
			//printf("%d %d\n", i, j);
			int match_x = i, match_y = j, step_size = 8, x = i, y = j;
			double min_val = INT_MAX;

			// direction for 5 points
			int dx[5] = {0, 0, 0, -1, 1};
			int dy[5] = {0, -1, 1, 0, 0};
			while(1){
				while(1){
					// step 1, (x, y) current center
					for(int k = 0 ; k < 5 ; k++){
						int tmp_x = x + dx[k]*step_size;
						int tmp_y = y + dy[k]*step_size;
						if(tmp_x < 0 || tmp_x > W-block_size || tmp_y < 0 || tmp_y > H-block_size)
							continue;
						double mae = MSE(img, ref, i, j, tmp_x, tmp_y);
						if(mae < min_val){
							match_x = tmp_x;
							match_y = tmp_y;
							min_val = mae;
						}
					}
					// step 2 : best match at center, else new center and repeat
					if(match_x == x && match_y == y){
						step_size /= 2;
						break;
					}
					else{
						x = match_x;
						y = match_y;
					}
				}
				// step 3 : if step size = 1, search nine blocks around
				if(step_size == 1){
					for(int k = -1 ; k <= 1 ; k++){
						for(int l = -1 ; l <= 1 ; l++){
							if(x+k < 0 || x+k > W-block_size || y+l < 0 || y+l > H-block_size)
								continue;
							double mae = MSE(img, ref, i, j, x+k, y+l);
							if(mae < min_val){
								match_x = x+k;
								match_y = y+l;
								min_val = mae;
							}
						}
					}
					break;
				}
			}
			// cur + motion = ref
			//motion_vector.mae[i/8][j/8] = min_val;
			//motion_vector.x[i/8][j/8] = match_x - i;
			//motion_vector.y[i/8][j/8] = match_y - j;	
			mse += min_val;
		}
	}
	//puts("complete !");
	return PSNR(mse);
}

double BS_search(unsigned char img[W][H], unsigned char ref[W][H]){
	//printf("Start BS search...");
	//MV motion_vector;
	double mse = 0;
    for(int i = 0 ; i <= W-block_size ; i += block_size){
		for(int j = 0 ; j <= H-block_size ; j += block_size){
			
			// current block index (i, j)
			int match_x = i, match_y = j, search_window_size = 8, x = i, y = j;
			double min_val = INT_MAX;

			
			// (x, y) current center
			for(int k = -search_window_size ; k <= search_window_size ; k+=search_window_size){
				for(int l = -search_window_size ; l <= search_window_size ; l+=search_window_size){
					//ref block index (i+k, j+l)
					if(x+k < 0 || x+k > W-block_size || y+l < 0 || y+l > H-block_size)
						continue;
					double mae = MSE(img, ref, i, j, x+k, y+l);
					if(mae < min_val){
						match_x = x+k;
						match_y = y+l;
						min_val = mae;
					}
				}
			}
			int xl, xu, yl, yu;
			if(match_x < i){
				xl == match_x;
				xu = match_x + search_window_size/2;
			}
			else if(match_x == i){
				xl = match_x - search_window_size/2;
				xu = match_x + search_window_size/2;
			}
			else if(match_x > i){
				xl = match_x - search_window_size/2;
				xu = match_x;
			}

			if(match_y < j){
				yl == match_y;
				yu = match_y + search_window_size/2;
			}
			else if(match_y == j){
				yl = match_y - search_window_size/2;
				yu = match_y + search_window_size/2;
			}
			else if(match_y > j){
				yl = match_y - search_window_size/2;
				yu = match_y;
			}

			for(int k = xl ; k <= xu ; k++)
				for(int l = yl ; l <= yu ; l++){
					if(k < 0 || k > W-block_size || l < 0 || l > H-block_size)
						continue;
					double mae = MSE(img, ref, i, j, k, l);
					if(mae < min_val){
						match_x = k;
						match_y = l;
						min_val = mae;
					}
				}
			mse += min_val;
			// cur + motion = ref
			//motion_vector.mae[i/8][j/8] = min_val;
			//motion_vector.x[i/8][j/8] = match_x - i;
			//motion_vector.y[i/8][j/8] = match_y - j;
		}
	}
	//puts("complete !");
	return PSNR(mse);
	//return motion_vector;
}

double FS_search(unsigned char img[W][H], unsigned char ref[W][H]){
	//printf("Start FS search...");
	//MV motion_vector;
	double mse = 0;
    for(int i = 0 ; i <= W-block_size ; i += block_size){
		for(int j = 0 ; j <= H-block_size ; j += block_size){
			//printf("%d %d\n", i, j);
			// current block index (i, j)
			int match_x = i, match_y = j, search_window_size = 8, x = i, y = j;
			double min_val = INT_MAX;

			// (x, y) current center
			for(int k = -search_window_size ; k <= search_window_size ; k++){
				for(int l = -search_window_size ; l <= search_window_size ; l++){
					//ref block index (i+k, j+l)
					if(x+k < 0 || x+k > W-block_size || y+l < 0 || y+l > H-block_size)
						continue;
					double mae = MSE(img, ref, i, j, x+k, y+l);
					if(mae < min_val){
						match_x = x+k;
						match_y = y+l;
						min_val = mae;
					}
				}
			}
			// cur + motion = ref
			//motion_vector.mae[i/8][j/8] = min_val;
			//motion_vector.x[i/8][j/8] = match_x - i;
			//motion_vector.y[i/8][j/8] = match_y - j;
			mse += min_val;
		}
	}
	//puts("complete !");
	if(mse <= 0)
		printf("%f\n", mse);
	return PSNR(mse);
	//return motion_vector;
}

double BBGDS_search(unsigned char img[W][H], unsigned char ref[W][H]){
	//printf("Start BBGDS search...");
	//MV motion_vector;
	double mse = 0;
    for(int i = 0 ; i <= W-block_size ; i += block_size){
		for(int j = 0 ; j <= H-block_size ; j += block_size){
			//printf("%d %d\n", i, j);
			// current block index (i, j)
			int match_x = i, match_y = j, x = i, y = j;
			double min_val = INT_MAX;

			// (x, y) current center
			while(1){
				for(int k = -1 ; k <= 1 ; k+=1){
					for(int l = -1 ; l <= 1 ; l+=1){
						//ref block index (i+k, j+l)
						if(x+k < 0 || x+k > W-block_size || y+l < 0 || y+l > H-block_size)
							continue;
						double mae = MSE(img, ref, i, j, x+k, y+l);
						if(mae < min_val){
							match_x = x+k;
							match_y = y+l;
							min_val = mae;
						}
					}
				}
				if(match_x == x && match_y == y)
					break;
				else{
					x = match_x;
					y = match_y;
				}
			}
			//motion_vector.mae[i/8][j/8] = min_val;
			//motion_vector.x[i/8][j/8] = match_x - i;
			//motion_vector.y[i/8][j/8] = match_y - j;
			mse += min_val;
			// cur + motion = ref
		}
	}
	//puts("complete !");
	return PSNR(mse);
	//return motion_vector;
}