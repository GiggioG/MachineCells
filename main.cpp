//g++ main.cpp -o main.exe -std=c++11 && start main.exe
#include "colors.cpp"
using namespace std;
//config consts
const unsigned char MOVER_CH[4] = {'^','>','v','<'};
const COLORS MOVER_COL = BLUE;
const unsigned char STATIC_CH = 178;
const COLORS STATIC_COL = LIGHT_GREY;
const unsigned char ENEMY_CH = 234;
const COLORS ENEMY_COL = RED;
const unsigned char ROTATOR_CH[2] = {149,162};
const COLORS ROTATOR_COL = BROWN;
const unsigned char CLONER_CH[4] = {30,16,31,17};
const COLORS CLONER_COL = GREEN;
const unsigned char PUSHABLE_CH = '#';
const unsigned char UNIDIRECTIONAL_CH[2] = {186,205};
const COLORS PUSHABLE_COL = YELLOW;
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
COLORS getColorAt(char** field,int r, int c){
    if(field[r][c] == MOVER_CH[0]){return MOVER_COL;}
    if(field[r][c] == MOVER_CH[1]){return MOVER_COL;}
    if(field[r][c] == MOVER_CH[2]){return MOVER_COL;}
    if(field[r][c] == MOVER_CH[3]){return MOVER_COL;}
    if(field[r][c] == STATIC_CH){return STATIC_COL;}
    if(field[r][c] == ENEMY_CH){return ENEMY_COL;}
    if(field[r][c] == ROTATOR_CH[0]){return ROTATOR_COL;}
    if(field[r][c] == ROTATOR_CH[1]){return ROTATOR_COL;}
    if(field[r][c] == PUSHABLE_CH){return PUSHABLE_COL;}
    if(field[r][c] == UNIDIRECTIONAL_CH[0]){return PUSHABLE_COL;}
    if(field[r][c] == UNIDIRECTIONAL_CH[0]){return PUSHABLE_COL;}
    if(field[r][c] == CLONER_CH[0]){return CLONER_COL;}
    if(field[r][c] == CLONER_CH[1]){return CLONER_COL;}
    if(field[r][c] == CLONER_CH[2]){return CLONER_COL;}
    if(field[r][c] == CLONER_CH[3]){return CLONER_COL;}
    return BKG_COL;
}
void printField(char** field,int rows, int cols){
    COLORS col;
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            col = getColorAt(field,r,c);
            draw_char(field[r][c],r,c,BKG_COL,col);
        }
    }
}
int main(){
    int rows = 40;
    int cols = 50;
    char** field = new char*[rows];
    for (int r = 0; r < rows; r++) {
        field[r] = new char[cols];
    }
    //
    init(field,rows,cols);
    printField(field,rows,cols);
    //
    for (int r = 0; r < rows; r++) {
        delete field[r];
    }
    delete[] field;
    field = nullptr; 
    cin >> rows;
}