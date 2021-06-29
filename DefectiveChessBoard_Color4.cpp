//Defective Chessboard 
//Added Color to the Trminoes

#include<iostream>
#include<vector>
#include<utility>
#include<windows.h>
#include<time.h>
using namespace std;

typedef vector<vector<int>> vvi;
typedef vector<int> vi;
typedef pair<int,int> index;

vvi grid;
int tromino = 2;

bool inline is2power(int n){
	return (!(n&(n-1)));
}

bool isWithinLimit(index v_limit, index h_limit, index defect){
	return ((v_limit.first <= defect.first)&&(defect.first <= v_limit.second)&&
	        (h_limit.first <= defect.second)&&(defect.second <= h_limit.second));
}

void inline assignTromino(int row, int col){
	grid[row][col] = tromino;
}

void solveDCB(index v_limit, index h_limit, index defect){
	//Base Cases
	int size = (h_limit.second - h_limit.first) + 1;
	if(size == 1)
		return;
	if(size == 2){
		for(int i=v_limit.first;i<=v_limit.second;++i){
			for(int j=h_limit.first;j<=h_limit.second;++j){
				if((i!=defect.first)||(j!=defect.second))
					grid[i][j] = tromino;
			}
		}
		++tromino;
		return;
	}
	
	//Recursive Case
	int v_mid = ((v_limit.second - v_limit.first)>>1) + v_limit.first;
	int h_mid = ((h_limit.second - h_limit.first)>>1) + h_limit.first;
	
	//Finding the Original Defect's position
	bool *quadrant = new bool[4]();
	
	//Quadrant1
	index v_limit_q1(v_limit.first,v_mid);
	index h_limit_q1(h_limit.first,h_mid);
	quadrant[0] = isWithinLimit(v_limit_q1,h_limit_q1,defect);
	
	//Quadrant2
	index v_limit_q2(v_limit.first,v_mid);
	index h_limit_q2(h_mid+1,h_limit.second);
	quadrant[1] = isWithinLimit(v_limit_q2,h_limit_q2,defect);
	
	//Quadrant3
	index v_limit_q3(v_mid+1,v_limit.second);
	index h_limit_q3(h_limit.first,h_mid);
	quadrant[2] = isWithinLimit(v_limit_q3,h_limit_q3,defect);
	
	//Quadrant4
	index v_limit_q4(v_mid+1,v_limit.second);
	index h_limit_q4(h_mid+1,h_limit.second);
	quadrant[3] = isWithinLimit(v_limit_q4,h_limit_q4,defect);
	
	//Introducing the New Defect
	index q1d,q2d,q3d,q4d;		//New Set of defects
	q1d = q2d = q3d = q4d = defect;
	if(!quadrant[0]){
		q1d = {v_mid,h_mid};
		assignTromino(v_mid,h_mid);
	}
	if(!quadrant[1]){
		q2d = {v_mid,h_mid+1};
		assignTromino(v_mid,h_mid+1);
	}
	if(!quadrant[2]){
		q3d = {v_mid+1,h_mid};
		assignTromino(v_mid+1,h_mid);
	}
	if(!quadrant[3]){
		q4d = {v_mid+1,h_mid+1};
		assignTromino(v_mid+1,h_mid+1);
	}
	++tromino;
	
	delete[] quadrant;
	
	solveDCB(v_limit_q1,h_limit_q1,q1d);
	solveDCB(v_limit_q2,h_limit_q2,q2d);
	solveDCB(v_limit_q3,h_limit_q3,q3d);
	solveDCB(v_limit_q4,h_limit_q4,q4d);
}

void printGrid(){
	cout<<endl;
	for(vi row:grid){
		for(int element:row)
			cout<<element<<" ";
		cout<<endl;
	}
	cout<<endl;
}

void color(int ForgC = 15, int BackC = 0){
	WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
 	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
 	SetConsoleTextAttribute(hStdOut, wColor);
}

void printGridWithColor(){
	vi colorCodes = {13,1,2,3,10,15};
	int colorCodes_size = colorCodes.size();
	cout<<endl;
	for(vi row:grid){
		for(int element:row){
			if(element == 0)
				cout<<'#'<<" ";
			else{
				color(15,colorCodes[element % colorCodes_size]);
				cout<<"  ";
				color();
			}
		}
		cout<<endl;
	}
	cout<<endl;
}

int main(){
	ShowWindow(GetConsoleWindow() , SW_SHOWMAXIMIZED);
	int n; 
	cout<<"Enter grid size (Power of 2): ";
	cin>>n;
	
	if(n == 0){
		cout<<"Grid size can't be zero"<<endl;
		return 0;
	}else if(!is2power(n)){
		cout<<"Grid size should power of 2"<<endl;
		return 0;
	}
	
	for(int i=0;i<n;++i){
		vi row(n,1);
		grid.push_back(row);
	}
	
//	printGrid();
//	printGridWithColor();
	index defect;
	cout<<"Enter the coordinates of defective piece (Array Indexing): ";
	cin>>defect.first>>defect.second;
	
	if((defect.first >= n)||(defect.second >= n)){
		cout<<"Defect should be within the bounds"<<endl;
		return 0;
	}
	
	grid[defect.first][defect.second] = 0;
	
//	printGrid();
	
	index h_limit(0,n-1);
	index v_limit(0,n-1);
	
	solveDCB(v_limit,h_limit,defect);
	
//	printGrid();
	printGridWithColor();
	
	return 0;
}

