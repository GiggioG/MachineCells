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
struct Cell{
    uchar ch = EMPTY_CH;
    COLORS col = BKG_COL;
    bool touched = false;
};
int diffr[4] = {-1,0,1, 0};
int diffc[4] = { 0,1,0,-1};
COLORS getColor(uchar ch){
    if(ch == MOVER_CH[0]){return MOVER_COL;}
    if(ch == MOVER_CH[1]){return MOVER_COL;}
    if(ch == MOVER_CH[2]){return MOVER_COL;}
    if(ch == MOVER_CH[3]){return MOVER_COL;}
    if(ch == STATIC_CH){return STATIC_COL;}
    if(ch == ENEMY_CH){return ENEMY_COL;}
    if(ch == ROTATOR_CH[0]){return ROTATOR_COL;}
    if(ch == ROTATOR_CH[1]){return ROTATOR_COL;}
    if(ch == PUSHABLE_CH){return PUSHABLE_COL;}
    if(ch == UNIDIRECTIONAL_CH[0]){return PUSHABLE_COL;}
    if(ch == UNIDIRECTIONAL_CH[1]){return PUSHABLE_COL;}
    if(ch == CLONER_CH[0]){return CLONER_COL;}
    if(ch == CLONER_CH[1]){return CLONER_COL;}
    if(ch == CLONER_CH[2]){return CLONER_COL;}
    if(ch == CLONER_CH[3]){return CLONER_COL;}
    return BKG_COL;
}
void setCell(Cell& cell, uchar ch){
    cell.ch = ch;
    cell.col = getColor(ch);
}
void init(Cell** field,int rows, int cols){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if((r == 0 || r == rows-1) || (c == 0 || c == cols-1)){
                setCell(field[r][c],STATIC_CH);
            }
        }
    }
}
void printField(Cell** field,int rows, int cols){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            draw_char(field[r][c].ch,r,c,BKG_COL,field[r][c].col);
        }
    }
}
bool push(Cell** field, int r, int c, int d){
    int newr = r+diffr[d], newc = c+diffc[d];
    uchar cellchar = field[r][c].ch;
    if(cellchar == STATIC_CH){return false;}
    if(cellchar == EMPTY_CH){return true;}
    if(
        (cellchar == PUSHABLE_CH) ||
        ((cellchar == MOVER_CH[0]) && (field[newr][newc].ch != MOVER_CH[2])) ||
        ((cellchar == MOVER_CH[1]) && (field[newr][newc].ch != MOVER_CH[3])) ||
        ((cellchar == MOVER_CH[2]) && (field[newr][newc].ch != MOVER_CH[0])) ||
        ((cellchar == MOVER_CH[3]) && (field[newr][newc].ch != MOVER_CH[1])) ||
        (cellchar == CLONER_CH[0]) || (cellchar == CLONER_CH[1]) ||
        (cellchar == CLONER_CH[2]) || (cellchar == CLONER_CH[0])
    ){
        if(!push(field,newr,newc,d)){return false;}
        setCell(field[newr][newc],cellchar);
        //field[newr][newc].touched = true;
        setCell(field[r][c],EMPTY_CH);
        // field[r][c].touched = false;
        //cout << "moved " << cellchar << " from " << r << ',' << c << " to " << newr << ',' << newc << '\n';
        return true;
    }
    return false;
}
void stepThroughTime(Cell** field,int rows, int cols){
    //if(!GetAsyncKeyState(VK_SPACE)){return;}
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            field[r][c].touched = false;
        }
    }
    // 
    int d;
    uchar cellchar;
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if(field[r][c].touched){continue;}
            cellchar = field[r][c].ch;
            if((cellchar == MOVER_CH[0]) || (cellchar == MOVER_CH[1]) || (cellchar == MOVER_CH[2]) || (cellchar == MOVER_CH[3])){
                if(cellchar == MOVER_CH[0]){d = 0;}
                if(cellchar == MOVER_CH[1]){d = 1;}
                if(cellchar == MOVER_CH[2]){d = 2;}
                if(cellchar == MOVER_CH[3]){d = 3;}
                field[r+diffr[d]][c+diffc[d]].touched = push(field,r,c,d);
                printField(field,rows,cols);
            }
        }
    }
}
int main(){
    for(int i = 0; i < 42; i++){
        cout << '\n';
    }
    int rows = 40;
    int cols = 50;
    Cell** field = new Cell*[rows];
    for (int r = 0; r < rows; r++) {
        field[r] = new Cell[cols];
    }
    //
    init(field,rows,cols);
    printField(field,rows,cols);
    while(true){
        if(GetAsyncKeyState(VK_SPACE)){
            printField(field,rows,cols);
            stepThroughTime(field, rows, cols);
        }
        //Sleep(1);
    }
    //
    for (int r = 0; r < rows; r++) {
        delete field[r];
    }
    delete[] field;
    field = nullptr; 
    cin >> rows;
}