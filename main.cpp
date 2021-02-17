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
const unsigned char UNIDIRECTIONAL_CH[2] = {205,186};
const COLORS PUSHABLE_COL = YELLOW;
const unsigned char EMPTY_CH = '.';
//
int br_enemies=0;

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
    if(field[newr][newc].ch == ENEMY_CH){
        setCell(field[newr][newc],EMPTY_CH);
        setCell(field[r][c],EMPTY_CH);
        br_enemies--;
        return true;
    }
    if(
        (cellchar == PUSHABLE_CH) ||
        ((cellchar == MOVER_CH[0]) && (field[newr][newc].ch != MOVER_CH[2])) ||
        ((cellchar == MOVER_CH[1]) && (field[newr][newc].ch != MOVER_CH[3])) ||
        ((cellchar == MOVER_CH[2]) && (field[newr][newc].ch != MOVER_CH[0])) ||
        ((cellchar == MOVER_CH[3]) && (field[newr][newc].ch != MOVER_CH[1])) ||
        (cellchar == CLONER_CH[0]) || (cellchar == CLONER_CH[1]) ||
        (cellchar == CLONER_CH[2]) || (cellchar == CLONER_CH[0]) ||
        (cellchar == ROTATOR_CH[0]) || (cellchar == ROTATOR_CH[1])
    ){
        if(!push(field,newr,newc,d)){return false;}
        setCell(field[newr][newc],cellchar);
        //field[newr][newc].touched = true;
        setCell(field[r][c],EMPTY_CH);
        // field[r][c].touched = false;
        //cout << "moved " << cellchar << " from " << r << ',' << c << " to " << newr << ',' << newc << '\n';
        return true;
    }
    if((cellchar == UNIDIRECTIONAL_CH[0]) || (cellchar == UNIDIRECTIONAL_CH[1])){
        if(((d == 0) || (d == 2)) && (cellchar == UNIDIRECTIONAL_CH[0])){return false;}
        if(((d == 1) || (d == 3)) && (cellchar == UNIDIRECTIONAL_CH[1])){return false;}
        if(!push(field,newr,newc,d)){return false;}
        setCell(field[newr][newc],cellchar);
        setCell(field[r][c],EMPTY_CH);
        return true;
    }
    return false;
}

void move_cursor(Cell** field){
    int rows=0, cols=0;
    int new_rows,new_cols;
    if(GetAsyncKeyState('A')){
        new_cols=cols-1;
    }
    if(GetAsyncKeyState('W')){
        new_rows=rows-1;
    }
    if(GetAsyncKeyState('D')){
        new_cols=cols+1;
    }
    if(GetAsyncKeyState('S')){
        new_rows=rows+1;
    }
    // if()
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
            }
            if((cellchar == CLONER_CH[0]) || (cellchar == CLONER_CH[1]) || (cellchar == CLONER_CH[2]) || (cellchar == CLONER_CH[3])){
                if(cellchar == CLONER_CH[0]){d = 0;}
                if(cellchar == CLONER_CH[1]){d = 1;}
                if(cellchar == CLONER_CH[2]){d = 2;}
                if(cellchar == CLONER_CH[3]){d = 3;}
                if(field[r-diffr[d]][c-diffc[d]].ch == EMPTY_CH){continue;}
                field[r][c].touched = push(field,r+diffr[d],c+diffc[d],d);
                if(field[r][c].touched){
                    setCell(field[r+diffr[d]][c+diffc[d]], field[r-diffr[d]][c-diffc[d]].ch);
                }
            }
            if((cellchar == ROTATOR_CH[0]) || (cellchar == ROTATOR_CH[1])){
                field[r][c].touched = true;
                for(int i = -1; i <= 1; i++){
                    for(int j = -1; j <= 1; j++){
                        if((i==0) != (j==0)){
                            if(cellchar == ROTATOR_CH[0]){
                                if(field[r+i][c+j].ch == MOVER_CH[0]){setCell(field[r+i][c+j], MOVER_CH[1]);}
                                else if(field[r+i][c+j].ch == MOVER_CH[1]){setCell(field[r+i][c+j], MOVER_CH[2]);}
                                else if(field[r+i][c+j].ch == MOVER_CH[2]){setCell(field[r+i][c+j], MOVER_CH[3]);}
                                else if(field[r+i][c+j].ch == MOVER_CH[3]){setCell(field[r+i][c+j], MOVER_CH[0]);}
                                else if(field[r+i][c+j].ch == CLONER_CH[0]){setCell(field[r+i][c+j], CLONER_CH[1]);}
                                else if(field[r+i][c+j].ch == CLONER_CH[1]){setCell(field[r+i][c+j], CLONER_CH[2]);}
                                else if(field[r+i][c+j].ch == CLONER_CH[2]){setCell(field[r+i][c+j], CLONER_CH[3]);}
                                else if(field[r+i][c+j].ch == CLONER_CH[3]){setCell(field[r+i][c+j], CLONER_CH[0]);}
                                else if(field[r+i][c+j].ch == UNIDIRECTIONAL_CH[0]){setCell(field[r+i][c+j], UNIDIRECTIONAL_CH[1]);}
                                else if(field[r+i][c+j].ch == UNIDIRECTIONAL_CH[1]){setCell(field[r+i][c+j], UNIDIRECTIONAL_CH[0]);}
                            }
                            if(cellchar == ROTATOR_CH[1]){
                                if(field[r+i][c+j].ch == MOVER_CH[0]){setCell(field[r+i][c+j], MOVER_CH[3]);}
                                else if(field[r+i][c+j].ch == MOVER_CH[1]){setCell(field[r+i][c+j], MOVER_CH[0]);}
                                else if(field[r+i][c+j].ch == MOVER_CH[2]){setCell(field[r+i][c+j], MOVER_CH[1]);}
                                else if(field[r+i][c+j].ch == MOVER_CH[3]){setCell(field[r+i][c+j], MOVER_CH[2]);}
                                else if(field[r+i][c+j].ch == CLONER_CH[0]){setCell(field[r+i][c+j], CLONER_CH[3]);}
                                else if(field[r+i][c+j].ch == CLONER_CH[1]){setCell(field[r+i][c+j], CLONER_CH[0]);}
                                else if(field[r+i][c+j].ch == CLONER_CH[2]){setCell(field[r+i][c+j], CLONER_CH[1]);}
                                else if(field[r+i][c+j].ch == CLONER_CH[3]){setCell(field[r+i][c+j], CLONER_CH[2]);}
                                else if(field[r+i][c+j].ch == UNIDIRECTIONAL_CH[0]){setCell(field[r+i][c+j], UNIDIRECTIONAL_CH[1]);}
                                else if(field[r+i][c+j].ch == UNIDIRECTIONAL_CH[1]){setCell(field[r+i][c+j], UNIDIRECTIONAL_CH[0]);}
                            }
                        }
                    }
                }
            }
        }
    }
}

void initLevelFromData(Cell** field, int rows, int cols, vector<string>& levelData){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if((r == 0 || r == rows-1) || (c == 0 || c == cols-1)){
                setCell(field[r][c],STATIC_CH);
            }
        }
    }
    for(int r = 0; r < rows-2; r++){
        for(int c = 0; c < cols-2; c++){
            switch(levelData[r+2][c]){
                case '^':setCell(field[r+1][c+1],MOVER_CH[0]);break;
                case '>':setCell(field[r+1][c+1],MOVER_CH[1]);break;
                case 'v':setCell(field[r+1][c+1],MOVER_CH[2]);break;
                case '<':setCell(field[r+1][c+1],MOVER_CH[3]);break;
                case '%':setCell(field[r+1][c+1],STATIC_CH);break;
                case 'Q':setCell(field[r+1][c+1],ENEMY_CH);br_enemies++;break;
                case 'p':setCell(field[r+1][c+1],ROTATOR_CH[0]);break;
                case 'q':setCell(field[r+1][c+1],ROTATOR_CH[1]);break;
                case 'M':setCell(field[r+1][c+1],CLONER_CH[0]);break;
                case '}':setCell(field[r+1][c+1],CLONER_CH[1]);break;
                case 'W':setCell(field[r+1][c+1],CLONER_CH[2]);break;
                case '{':setCell(field[r+1][c+1],CLONER_CH[3]);break;
                case '#':setCell(field[r+1][c+1],PUSHABLE_CH);break;
                case '-':setCell(field[r+1][c+1],UNIDIRECTIONAL_CH[0]);break;
                case '|':setCell(field[r+1][c+1],UNIDIRECTIONAL_CH[1]);break;
                case '.':setCell(field[r+1][c+1],EMPTY_CH);break;
            }
        }
    }
}

struct GameLevel {
    int rows,cols;
    int min_r,min_c;
    int max_r,max_c;
    vector<string> levelData;

    void readFromFile(string path);//deklaraciq
    void run();//deklaraciq
};

void GameLevel::readFromFile(string path){//definiciq
    string tmp;
    ifstream in_f(path);
    in_f >> rows >> cols;
    in_f >> min_r >> min_c;
    in_f >> max_r >> max_c;
    getline(in_f,tmp);
    cout<<tmp;
    while (getline (in_f, tmp)) {
        levelData.push_back(tmp);
    }
    in_f.close();
}

void GameLevel::run(){//definiciq
    readFromFile("level.txt");
    cout << "test1\n";
    srand(time(0));
    // int rows = stoi(gameLevel.levelData[0])+2;
    // int cols = stoi(gameLevel.levelData[1])+2;
    cout << "test2\n";
    for(int i = 0; i < rows; i++){
        cout << '\n';
    }
    Cell** field = new Cell*[rows];
    for (int r = 0; r < rows; r++) {
        field[r] = new Cell[cols];
    }
    cout << "test3\n";
    initLevelFromData(field, rows, cols, levelData);
    printField(field,rows,cols);
    cout << "test4\n";
    while(br_enemies>0){
        if(GetAsyncKeyState(VK_SPACE)){
            printField(field,rows,cols);
            stepThroughTime(field, rows, cols);
            Sleep(100);
        }
    }
    cout << "test5\n";
    printField(field,rows,cols);
    for (int r = 0; r < rows; r++) {
        delete field[r];
    }
    delete[] field;
    field = nullptr;
    cout<<"Victory!"<<endl;
    Sleep(-1);
}

int main(){
    GameLevel gameLevel;
    gameLevel.run();
}
