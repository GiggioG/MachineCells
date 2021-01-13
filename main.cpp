//g++ main.cpp -o main.exe -std=c++11 && start main.exe
#include <iostream>
using namespace std;
//config consts
const unsigned char MOVER_CH[4] = {'^','>','v','<'};
const unsigned char STATIC_CH = '@';
const unsigned char ENEMY_CH = '&';
const unsigned char ROTATOR_CH[2] = {'/','\\'};
const char CLONER_CH[4] = {30,16,31,17};
const unsigned char PUSHABLE_CH = '#';
const unsigned char UNIDIRECTIONAL_CH[2] = {186,205};
const unsigned char EMPTY_CH = '.';
void init(char** field,int rows, int cols){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if((r == 0 || r == rows-1) || (c == 0 || c == cols-1)){
                field[r][c] = STATIC_CH;
            }
            else{
                field[r][c] = EMPTY_CH;
            }
        }
    }
    field[1][1] = MOVER_CH[0];
    field[1][2] = MOVER_CH[1];
    field[1][3] = MOVER_CH[2];
    field[1][4] = MOVER_CH[3];
    field[1][5] = STATIC_CH;
    field[1][6] = ENEMY_CH;
    field[1][7] = ROTATOR_CH[0];
    field[1][8] = ROTATOR_CH[1];
    field[1][9] = PUSHABLE_CH;
    field[1][10] = UNIDIRECTIONAL_CH[0];
    field[1][11] = UNIDIRECTIONAL_CH[1];
    field[1][12] = CLONER_CH[0];
    field[1][13] = CLONER_CH[1];
    field[1][14] = CLONER_CH[2];
    field[1][15] = CLONER_CH[3];
}
int main(){
    int rows = 40;
    int cols = 50;
    char** field = new char*[rows];
    for (int r = 0; r < rows; r++) {
        field[r] = new char[cols];
    }
    init(field,rows,cols);
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            cout << field[r][c] << ' ';
        }
        cout << '\n';
    }
    for (int r = 0; r < rows; r++) {
        delete field[r];
    }
    delete[] field;
    field = nullptr; 
    cin >> rows;
}







