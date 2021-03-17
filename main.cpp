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

//times(4,"lopio") -> "lopiolopiolopiolopio"
string times(int n, string s){
  string ret = "";
  for(int i = 0; i < n; i++){
    ret += s;
  }
  return ret;
}

struct GameLevel {
    int rows,cols;
    int min_r,min_c;
    int max_r,max_c;
    vector<string> levelData;
    Cell** field;

    bool readFromFile(string path);
    void initLevelFromData();
    void run();
    void printField();
    void stepThroughTime();
    void edit();
};

bool GameLevel::readFromFile(string path){//definiciq
    string tmp;
    ifstream in_f(path);
    if (!in_f.good()) {
        return false;
    }
    in_f >> rows >> cols;
    in_f >> min_r >> min_c;
    in_f >> max_r >> max_c;
    getline(in_f,tmp);
    cout << tmp;
    int nRow = 0;
    levelData.push_back(times(cols+2, "%"));
    while (getline (in_f, tmp)) {
        nRow++;
        if (tmp.length() != cols) {
            cerr << "Error in " << path << ", row #" << nRow << " of field has wrong size.";
            exit(-1);
        }
        levelData.push_back('%' + tmp + '%');
    }
    levelData.push_back(times(cols+2, "%"));
    if(levelData.size() != rows+2){
        cerr << "Error in " << path << ", field has wrong number of rows.";
        exit(-1);
    }
    
    in_f.close();
    //walls offset
    rows += 2;
    cols += 2;
    min_r ++;
    min_c ++;
    max_r ++;
    max_c ++;
    // for(int i = 0; i < levelData.size(); i++){
    //     cout << levelData[i] << '\n';
    // }
    return true;
}

void GameLevel::initLevelFromData(){
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            switch(levelData[r][c]){
                case '^':setCell(field[r][c],MOVER_CH[0]);break;
                case '>':setCell(field[r][c],MOVER_CH[1]);break;
                case 'v':setCell(field[r][c],MOVER_CH[2]);break;
                case '<':setCell(field[r][c],MOVER_CH[3]);break;
                case '%':setCell(field[r][c],STATIC_CH);break;
                case 'Q':setCell(field[r][c],ENEMY_CH);br_enemies++;break;
                case 'p':setCell(field[r][c],ROTATOR_CH[0]);break;
                case 'q':setCell(field[r][c],ROTATOR_CH[1]);break;
                case 'M':setCell(field[r][c],CLONER_CH[0]);break;
                case '}':setCell(field[r][c],CLONER_CH[1]);break;
                case 'W':setCell(field[r][c],CLONER_CH[2]);break;
                case '{':setCell(field[r][c],CLONER_CH[3]);break;
                case '#':setCell(field[r][c],PUSHABLE_CH);break;
                case '-':setCell(field[r][c],UNIDIRECTIONAL_CH[0]);break;
                case '|':setCell(field[r][c],UNIDIRECTIONAL_CH[1]);break;
                case '.':setCell(field[r][c],EMPTY_CH);break;
            }
        }
    }
}

void GameLevel::printField(){
    COLORS fg;
    COLORS bg;
    for(int r = 0; r < rows; r++){
        for(int c = 0; c < cols; c++){
            if (r >= min_r && r <= max_r && c >= min_c && c <= max_c && field[r][c].ch == EMPTY_CH) {
                fg = GREY;
                bg = GREY;
            } else {
                fg = BKG_COL;
                bg = field[r][c].col;
            }
            draw_char(field[r][c].ch,r,c,fg,bg);
        }
    }
}

void GameLevel::edit(){
    int r_cur = min_r, c_cur = min_c;
    Cell hand;
    bool handFull = false;
    while (true) {
        printField();
        if (handFull) {
            draw_char(hand.ch, r_cur, c_cur, PINK, WHITE);
        } else {
            draw_char(field[r_cur][c_cur].ch, r_cur, c_cur, field[r_cur][c_cur].col, WHITE);
        }
        if (GetAsyncKeyState('W') && (r_cur > min_r)) {
            r_cur--;
        }
        if (GetAsyncKeyState('A') && (c_cur > min_c)) {
            c_cur--;
        }
        if (GetAsyncKeyState('S') && (r_cur < max_r)) {
            r_cur++;
        }
        if (GetAsyncKeyState('D') && (c_cur < max_c)) {
            c_cur++;
        }
        if (GetAsyncKeyState('E')) {
            if (!handFull) {
                if (field[r_cur][c_cur].ch != EMPTY_CH) {
                    hand = field[r_cur][c_cur];
                    handFull = true;
                    setCell(field[r_cur][c_cur], EMPTY_CH);
                }
            } else {
                if (field[r_cur][c_cur].ch == EMPTY_CH) {
                    field[r_cur][c_cur] = hand;
                    handFull = false;                    
                }
            }
        }
        if (GetAsyncKeyState('Q') && !handFull) {
            return;
        }
        Sleep(50);
    }
}

void GameLevel::run(){//definiciq
    system("cls");
    for(int i = 0; i < rows; i++){
        cout << '\n';
    }
    field = new Cell*[rows];
    for (int r = 0; r < rows; r++) {
        field[r] = new Cell[cols];
    }
    initLevelFromData();
    edit();
    printField();
    while(br_enemies>0){
        if(GetAsyncKeyState(VK_SPACE)){
            printField();
            stepThroughTime();
            Sleep(100);
        }
    }
    printField();
    for (int r = 0; r < rows; r++) {
        delete field[r];
    }
    delete[] field;
    field = nullptr;
    cout<<"Victory!"<<endl;
    system("pause");
    //Sleep(-1);
}

void GameLevel::stepThroughTime(){
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

int main(){
    srand(time(0));

    for (int i = 1; true; i++) {
        GameLevel gameLevel;
        if (!gameLevel.readFromFile(string("level-") + (char)(i+'0') + ".txt")) {
            cout << "CONGRATULATIONS! YOU BEAT THE GAME!\n";
            system("pause");
            return 0;
        }
        gameLevel.run();
    }
}
