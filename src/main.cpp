#include <iostream>
#include <vector>
#include <math.h>
#define RECURSIVITY 4

std::vector<std::vector<char> > board;

//int RECURSIVITY = 5;
int row_count = 8;
int col_count = 8;
enum direction{
	VERTICAL = 'V',
	HORIZONTAL = 'H'
};
struct killingMove{
	int i;
	int j;
	direction dir;
	int rate;
};
std::vector<killingMove*> movesRegistered;
std::vector<std::vector<int> > history_movesRegistered;

void draw_game();
void input(int &command, const char* command_name);
bool place_item(int row, int col, direction dir, bool silent);
void remove_item(int row, int col, direction dir);
int get_possibilities(direction dir);
void computer();
int max(int recursivity, direction dir, int &ri, int &rj);
int min(int recursivity, direction dir);
int alphabetakillerhistory(int recursivity, direction dir, int &ri, int &rj, int alpha, int beta);
void alphabetakillerhistory();
int alphabetakiller(int recursivity, direction dir, int &ri, int &rj, int alpha, int beta);
void alphabetakiller();
void minimax();
void alphabeta();
int alphabeta(int recursivity, direction dir, int &ri, int &rj, int alpha, int beta);
void negamax();
int negamax(int recursivity, direction dir, int &ri, int &rj);


void draw_game(){
	std::cout << " _";
	for(int i = 0; i < col_count; i++){
		std::cout << "_" << i+1;
	}
	std::cout << std::endl;
	for(int i = 0; i < row_count; i++){
		std::cout << i+1 << "|";
		for(int j = 0; j < col_count; j++){
			std::cout << " " << board[i][j];
		}
		std::cout << std::endl;
	}
}

void input(int &command, const char* command_name){
	std::cout << "Press " << command_name << ". Use 0 to exit." << std::endl;
	std::cin >> command;
}

bool place_item(int row, int col, direction dir, bool silent){
	int col_m = 0;
	int row_m = 0;
	if(dir == VERTICAL){
		row_m = 1;
	}else{
		col_m = 1;
	}

	if(row+row_m >= row_count || col+col_m >= col_count || board[row+row_m][col] != '0' || board[row][col+col_m] != '0'){
		if(!silent)
			std::cout << "Please check your input and try again." << std::endl;
		return false;
	}else{
		board[row][col] = dir;
		board[row+row_m][col+col_m] = dir;
	}

	return true;
}

void remove_item(int row, int col, direction dir){
	if(dir == VERTICAL){
		board[row][col] = '0';
		board[row+1][col] = '0';
	}else{
		board[row][col] = '0';
		board[row][col+1] = '0';
	}
}

int get_possibilities(direction dir){
	int sum = 0;
	int row_m = 0;
	int col_m = 0;
	if(dir == VERTICAL){
		row_m = 1;
	}else{
		col_m = 1;
	}

	for(int i = 0; i < row_count-row_m; i++){
		for(int j = 0; j < col_count-col_m; j++){
			if(board[i][j] == '0' && board[i+row_m][j+col_m] == '0'){
				sum += 1;
			}
		}
	}

	return sum;
}

int alphabetakillerhistory(int recursivity, direction dir, int &ri, int &rj, int alpha, int beta){
	if(recursivity == 0)
		return get_possibilities(dir) - ((dir==HORIZONTAL)?get_possibilities(VERTICAL):get_possibilities(HORIZONTAL));


	int fj, fi = 0;
	if(history_movesRegistered.size() == 0){
		history_movesRegistered.resize(row_count);
		for(int i = 0; i < col_count; i++){
			history_movesRegistered[i].resize(col_count);
			history_movesRegistered[i].assign(col_count, 0);
		}
	}else{
		std::vector<killingMove> history;
		for(int i = 0; i < row_count; i++){
			for(int j = 0; j < col_count; j++){
				if(history_movesRegistered[i][j] != 0){
					history.push_back({i, j, dir, history_movesRegistered[i][j]});
				}
			}
		}

		for(unsigned int i = 0; i < history.size(); i++){
			for(unsigned int j = 0; j < history.size(); j++){
				if(history[i].rate > history[j].rate){
					killingMove tmp = history[i];
					history[i] = history[j];
					history[j] = tmp;
				}
			}
		}

		for(unsigned int i = 0; i < history.size(); i++){
			if(place_item(history[i].i, history[i].j, dir, true)){
				int l = -alphabetakiller(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), fi, fj, -beta, -alpha);
				history_movesRegistered[history[i].i][history[i].j] += 4;
				remove_item(history[i].i, history[i].j, dir);
				if(l > alpha){
					alpha = l;
					ri = history[i].i;
					rj = history[i].j;
					if(alpha >= beta){
						history_movesRegistered[history[i].i][history[i].j] += pow(4, recursivity);
						return beta;
					}
				}
			}
		}
		return alpha;
	}

	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int l = -alphabetakiller(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), fi, fj, -beta, -alpha);
				history_movesRegistered[i][j] += 4;
				remove_item(i, j, dir);
				if(l > alpha){
					alpha = l;
					ri = i;
					rj = j;
					history_movesRegistered[i][j] += pow(4, recursivity);
					if(alpha >= beta){
						return beta;
					}
				}
			}
		}
	}
	return alpha;
}

int alphabetakiller(int recursivity, direction dir, int &ri, int &rj, int alpha, int beta){
	if(recursivity == 0)
		return get_possibilities(dir) - ((dir==HORIZONTAL)?get_possibilities(VERTICAL):get_possibilities(HORIZONTAL));


	int fi = 0;
	int fj = 0;
	if(movesRegistered.size() == 0){
		movesRegistered.resize(recursivity);
		movesRegistered.assign(recursivity, NULL);
	}else{
		if(movesRegistered[recursivity-1] != NULL){
			if(place_item(movesRegistered[recursivity-1]->i, movesRegistered[recursivity-1]->j, dir, true)){
				int l = -alphabetakiller(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), fi, fj, -beta, -alpha);
				remove_item(movesRegistered[recursivity-1]->i, movesRegistered[recursivity-1]->j, dir);
				if(l > alpha){
					alpha = l;
					ri = movesRegistered[recursivity-1]->i;
					rj = movesRegistered[recursivity-1]->j;
					if(alpha >= beta){
						return beta;
					}
				}
			}
		}
	}

	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int l = -alphabetakiller(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), fi, fj, -beta, -alpha);
				remove_item(i, j, dir);
				if(l > alpha){
					alpha = l;
					ri = i;
					rj = j;
					delete movesRegistered[recursivity-1];
					movesRegistered[recursivity-1] = new killingMove{ri, rj, dir, 0};
					if(alpha >= beta){
						return beta;
					}
				}
			}
		}
	}
	return alpha;
}

int alphabeta(int recursivity, direction dir, int &ri, int &rj, int alpha, int beta){
	if(recursivity == 0)
		return get_possibilities(dir) - ((dir==HORIZONTAL)?get_possibilities(VERTICAL):get_possibilities(HORIZONTAL));

	int fi = 0;
	int fj = 0;
	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int e = -alphabeta(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), fi, fj, -beta, -alpha);
				remove_item(i, j, dir);
				if(e > alpha){
					alpha = e;
					ri = i;
					rj = j;
					if(alpha >= beta){
						return beta;
					}
				}
			}
		}
	}
	return alpha;
}

int negamax(int recursivity, direction dir, int &ri, int &rj){
	if(recursivity == 0)
		return get_possibilities(dir) - ((dir==HORIZONTAL)?get_possibilities(VERTICAL):get_possibilities(HORIZONTAL));

	int eval = -row_count*col_count;
	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int l = -negamax(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), ri, rj);
				remove_item(i, j, dir);
				if(l > eval){
					eval = l;
					ri = i;
					rj = j;
				}
			}
		}
	}
	return eval;
}

int min(int recursivity, direction dir){
	if(recursivity == 0)
		return get_possibilities(HORIZONTAL) - get_possibilities(VERTICAL);

	int eval = row_count*col_count;
	int ri;
	int rj;
	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int l = max(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL), ri, rj);
				remove_item(i, j, dir);
				if(l < eval)
					eval = l;
			}
		}
	}
	return eval;
}

int max(int recursivity, direction dir, int &ri, int &rj){
	if(recursivity == 0)
		return get_possibilities(HORIZONTAL) - get_possibilities(VERTICAL);

	int eval = -col_count * row_count;
	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, dir, true)){
				int l = min(recursivity-1, ((dir==HORIZONTAL)?VERTICAL:HORIZONTAL));
				remove_item(i, j, dir);
				if(l > eval){
					eval = l;
					ri = i;
					rj = j;
				}
			}
		}
	}
	return eval;
}

void alphabetakillerhistory(){
	int i = 0;
	int j = 0;
	std::cout << "Computer turn using alphabetakiller history..." << std::endl;
	alphabetakillerhistory(RECURSIVITY, HORIZONTAL, i, j, -row_count*col_count, row_count*col_count);
	std::cout << "Placed item on column " << j+1 << ", row " << i+1 << std::endl;
	place_item(i, j, HORIZONTAL, true);
}

void alphabetakiller(){
	int i = 0;
	int j = 0;
	std::cout << "Computer turn using alphabetakiller..." << std::endl;
	alphabetakiller(RECURSIVITY, HORIZONTAL, i, j, -row_count*col_count, row_count*col_count);
	std::cout << "Placed item on column " << j+1 << ", row " << i+1 << std::endl;
	place_item(i, j, HORIZONTAL, true);
}

void negamax(){
	int i = 0;
	int j = 0;
	std::cout << "Computer turn using negamax..." << std::endl;
	negamax(RECURSIVITY, HORIZONTAL, i, j);
	std::cout << "Placed item on column " << j+1 << ", row " << i+1 << std::endl;
	place_item(i, j, HORIZONTAL, true);
}

void alphabeta(){
	int i = 0;
	int j = 0;
	std::cout << "Computer turn using alphabeta..." << std::endl;
	alphabeta(RECURSIVITY, HORIZONTAL, i, j, -row_count*col_count, row_count*col_count);
	std::cout << "Placed item on column " << j+1 << ", row " << i+1 << std::endl;
	place_item(i, j, HORIZONTAL, true);
}

void minimax(){
	int i = 0;
	int j = 0;
	std::cout << "Computer turn using minimax..." << std::endl;
	max(RECURSIVITY, HORIZONTAL, i, j);
	std::cout << "Placed item on column " << j+1 << ", row " << i+1 << std::endl;
	place_item(i, j, HORIZONTAL, true);
}

void computer(){
	std::cout << "Computer turn..." << std::endl;

	int maxI, maxJ = 0;
	int max = -1;

	for(int i = 0; i < row_count; i ++){
		for(int j = 0; j < col_count; j ++){
			if(place_item(i, j, HORIZONTAL, true)){
				int sP = get_possibilities(HORIZONTAL) - get_possibilities(VERTICAL);
				remove_item(i, j, HORIZONTAL);
				if(sP > max){
					max = sP;
					maxI = i;
					maxJ = j;
				}
			}
		}
	}

	std::cout << "Placed item on column " << maxJ+1 << ", row " << maxI+1 << std::endl;
	place_item(maxI, maxJ, HORIZONTAL, true);
}

int main(){//(int argc, char** argv){
	std::cout << "Domineering !" << std::endl;
	std::cout << "You will play with verical pieces" << std::endl;
	board.resize(row_count);
	for(int i = 0; i < row_count; i++){
		board[i].assign(col_count, '0');
	}
	int type = 0;
	std::cout<<"Choose your IA type : "<<std::endl;
	std::cout<<"1 - MINMAX"<<std::endl;
	std::cout<<"2 - NEGAMAX"<<std::endl;
	std::cout<<"3 - ALPHABETA"<<std::endl;
	std::cout<<"4 - ALPHABETAKILLER"<<std::endl;
	std::cout<<"5 - ALPHABETAKILLERHISTORY"<<std::endl;
	std::cout<<"0 - BASIC AI"<<std::endl;
	std::cin >> type;
	int command = -1;
	bool validated = false;
	while(command != 0 || !validated){
		draw_game();
		std::cout << "Player turn..." << std::endl;
		if(get_possibilities(VERTICAL) == 0){
			std::cout << "You lost." << std::endl;
			break;
		}
		input(command, "column to place your first box");
		if(command > 0 && command <= row_count){
			int col = command;
			input(command, "row to place your first box");
			if(command > 0 && command <= col_count){
				validated = place_item(command-1, col-1, VERTICAL, false);
				if(validated){
					if(get_possibilities(HORIZONTAL) == 0){
						std::cout << "You win." << std::endl;
						break;
					}
					switch(type)
					{
						case 1:
						//MINIMAX
						minimax();
						break;
						case 2:
						//negamax
						negamax();
						break;
						case 3:
						//ALPHABETA
						alphabeta();
						break;
						case 4:
						//ALPHABETAKILLER
						alphabetakiller();
						break;
						case 5:
						//ALPHABETAKILLERHISTORY
						alphabetakillerhistory();
						break;
						default:
						//BASIC IA
						computer();
						break;
					}
				}
			}
		}
	}
	for(unsigned int i = 0; i < movesRegistered.size(); i++){
		delete movesRegistered[i];
	}
	movesRegistered.clear();
	return 0;
}
