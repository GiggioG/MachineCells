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
//
int diffr[4] = {-1,0,1, 0};
int diffc[4] = { 0,1,0,-1};
void init(uchar** field,int rows, int cols){
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
    field[1][1] = MOVER_CH[2];
    field[2][1] = MOVER_CH[2];
    // field[1][2] = MOVER_CH[1];
    // field[1][3] = MOVER_CH[2];
    // field[1][4] = MOVER_CH[3];
    // field[1][5] = STATIC_CH;
    // field[1][6] = ENEMY_CH;
    // field[1][7] = ROTATOR_CH[0];
    // field[1][8] = ROTATOR_CH[1];
    // field[1][9] = PUSHABLE_CH;
    // field[1][10] = UNIDIRECTIONAL_CH[0];
    // field[1][11] = UNIDIRECTIONAL_CH[1];
    // field[1][12] = CLONER_CH[0];
    // field[1][13] = CLONER_CH[1];
    // field[1][14] = CLONER_CH[2];
    // field[1][15] = CLONER_CH[3];
    // field[2][3]  = PUSHABLE_CH;
    // field[2][14] = MOVER_CH[1];
    // field[2][15] = MOVER_CH[0];
}
COLORS getColorAt(uchar** field,int r, int c){
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
    if(field[r][c] == UNIDIRECTIONAL_CH[1]){return PUSHABLE_COL;}
    if(field[r][c] == CLONER_CH[0]){return CLONER_COL;}
    if(field[r][c] == CLONER_CH[1]){return CLONER_COL;}
    if(field[r][c] == CLONER_CH[2]){return CLONER_COL;}
    if(field[r][c] == CLONER_CH[3]){return CLONER_COL;}
    return GREEN;
}
void printField(uchar** field,int rows, int cols){
    COLORS col;
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            col = getColorAt(field,r,c);
            draw_char(field[r][c],r,c,BKG_COL,col);
        }
    }
}
bool push(uchar** field, uchar** newfield, int r, int c, int d){
    int newr = r+diffr[d], newc = c+diffc[d];
    if(field[r][c] == STATIC_CH){return false;}
    if(field[r][c] == EMPTY_CH){return true;}
    if(
        (field[r][c] == PUSHABLE_CH) ||
        ((field[r][c] == MOVER_CH[0]) && (field[newr][newc] != MOVER_CH[2])) ||
        ((field[r][c] == MOVER_CH[1]) && (field[newr][newc] != MOVER_CH[3])) ||
        ((field[r][c] == MOVER_CH[2]) && (field[newr][newc] != MOVER_CH[0])) ||
        ((field[r][c] == MOVER_CH[3]) && (field[newr][newc] != MOVER_CH[1])) ||
        (field[r][c] == CLONER_CH[0]) || (field[r][c] == CLONER_CH[1]) ||
        (field[r][c] == CLONER_CH[2]) || (field[r][c] == CLONER_CH[0])
    ){
        if(!push(field,newfield,newr,newc,d)){return false;}
        newfield[r][c] = EMPTY_CH;
        newfield[newr][newc] = field[r][c];
        //cout << "moved " << field[r][c] << " from " << r << ',' << c << " to " << newr << ',' << newc << '\n';
        return true;
    }
    return false;
}
void stepThroughTime(uchar** field,int rows, int cols){
    if(!GetAsyncKeyState(VK_SPACE)){return;}
    uchar** newfield = new uchar*[rows];
    for (int r = 0; r < rows; r++) {
        newfield[r] = new uchar[cols];
    }
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            newfield[r][c] = 0;
        }
    }
    // 
    int d;
    uchar cellchar;
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            cellchar = field[r][c];
            if((cellchar == MOVER_CH[0]) || (cellchar == MOVER_CH[1]) || (cellchar == MOVER_CH[2]) || (cellchar == MOVER_CH[3])){
                if(cellchar == MOVER_CH[0]){d = 0;}
                if(cellchar == MOVER_CH[1]){d = 1;}
                if(cellchar == MOVER_CH[2]){d = 2;}
                if(cellchar == MOVER_CH[3]){d = 3;}
                push(field,newfield,r,c,d);
                printField(newfield,rows,cols);
                Sleep(500);
            }
        }
    }
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if(newfield[r][c] != 0){
                field[r][c] = newfield[r][c];
            }
        }
    }
    cout << "flushed\n";
    // 
    for (int r = 0; r < rows; r++) {
        delete newfield[r];
    }
    delete[] newfield;
    newfield = nullptr; 
}
int main(){
    for(int i = 0; i < 42; i++){
        cout << '\n';
    }
    int rows = 40;
    int cols = 50;
    uchar** field = new uchar*[rows];
    for (int r = 0; r < rows; r++) {
        field[r] = new uchar[cols];
    }
    //
    init(field,rows,cols);
    while(true){
        printField(field,rows,cols);
        stepThroughTime(field, rows, cols);
        Sleep(500);
    }
    //
    for (int r = 0; r < rows; r++) {
        delete field[r];
    }
    delete[] field;
    field = nullptr; 
    cin >> rows;
}