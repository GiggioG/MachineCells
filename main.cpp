//g++ main.cpp -o main.exe -std=c++11 && start main.exe
#include "colors.cpp"
using namespace std;
//config consts

const unsigned char MOVER_CH_UP = '^';
const unsigned char MOVER_CH_RIGHT = '>';
const unsigned char MOVER_CH_DOWN = 'v';
const unsigned char MOVER_CH_LEFT = '<';
const unsigned char MOVER_CH[4] = {MOVER_CH_UP,MOVER_CH_RIGHT,MOVER_CH_DOWN,MOVER_CH_LEFT};

const COLORS MOVER_COL = BLUE;
const unsigned char STATIC_CH = 178;
const COLORS STATIC_COL = LIGHT_GREY;
const unsigned char ENEMY_CH = 234;
const COLORS ENEMY_COL = RED;

const unsigned char ROTATOR_CH_CLCW = 142;
const unsigned char ROTATOR_CH_CTCLCW = 162;
const unsigned char ROTATOR_CH[2] = {ROTATOR_CH_CLCW, ROTATOR_CH_CTCLCW};

const COLORS ROTATOR_COL = BROWN;

const unsigned char CLONER_CH_UP = 30;
const unsigned char CLONER_CH_RIGHT = 16;
const unsigned char CLONER_CH_DOWN = 31;
const unsigned char CLONER_CH_LEFT = 17;
const unsigned char CLONER_CH[4] = {CLONER_CH_UP, CLONER_CH_RIGHT, CLONER_CH_DOWN, CLONER_CH_LEFT};

const COLORS CLONER_COL = GREEN;
const unsigned char PUSHABLE_CH = '#';

const unsigned char UNIDIRECTIONAL_CH_HORZ = 205;
const unsigned char UNIDIRECTIONAL_CH_VERT = 186;
const unsigned char UNIDIRECTIONAL_CH[2] = {UNIDIRECTIONAL_CH_HORZ, UNIDIRECTIONAL_CH_VERT};

const COLORS PUSHABLE_COL = YELLOW;
const unsigned char EMPTY_CH = '.';

const COLORS CURSOR_FG_COL = PINK;
const COLORS CURSOR_BG_COL = WHITE;
const COLORS BLINK_FG_1 = RED;
const COLORS BLINK_BG_1 = WHITE;
const COLORS BLINK_FG_2 = BLUE;
const COLORS BLINK_BG_2 = BLACK;
const string VICTORY_MESSAGE = " > > > Victory < < < ";
const string GG_MESSAGE = "CONGRATULATIONS! YOU BEAT THE GAME!";
const int BLINK_PERIOD = 1000;
//

const unsigned char PUSHABLE_CH_LEGEND = '#';
const unsigned char MOVER_CH_LEGEND_0 = '^';
const unsigned char MOVER_CH_LEGEND_1 = '>';
const unsigned char MOVER_CH_LEGEND_2 = 'v';
const unsigned char MOVER_CH_LEGEND_3 = '<';
const unsigned char STATIC_CH_LEGEND = '%';
const unsigned char ENEMY_CH_LEGEND = 'Q';
const unsigned char ROTATOR_CH_LEGEND_0 = 'p';
const unsigned char ROTATOR_CH_LEGEND_1 = 'q';
const unsigned char CLONER_CH_LEGEND_0 = 'M';
const unsigned char CLONER_CH_LEGEND_1 = '}';
const unsigned char CLONER_CH_LEGEND_2 = 'W';
const unsigned char CLONER_CH_LEGEND_3 = '{';
const unsigned char UNIDIRECTIONAL_CH_LEGEND_0 = '-';
const unsigned char UNIDIRECTIONAL_CH_LEGEND_1 = '|';
const unsigned char EMPTY_CH_LEGEND = '.';

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
    int br_enemies = 0;
    Cell** field_copy;
    int br_enemies_copy = 0;
    int rOff, cOff;

    void drawCell(uchar ch, int y, int x, COLORS foreground_color, COLORS background_color);
    bool push(int r, int c, int d);
    bool readFromFile(string path);
    void initLevelFromData();
    void run();
    void printField();
    void stepThroughTime();
    void edit();
    void copyField();
    void deleteField(Cell**& f);
    void resetFieldFromCopy();
    bool blinkMessage(string msg);
};

typedef const char CharMap[3][4];
CharMap MOVER_UP_CMP = {
    " ^ ",
    "/_\\",
    "^^^"
};
CharMap MOVER_RIGHT_CMP = {
    ">\\ ",
    ">|>",
    ">/ "
};
CharMap MOVER_DOWN_CMP = {
    "vvv",
    "\\-/",
    " v "
};
CharMap MOVER_LEFT_CMP = {
    " /<",
    "<|<",
    " \\<"
};
CharMap PUSHABLE_CMP = {
    "+-+",
    "| |",
    "+-+"
};
CharMap ENEMY_CMP = {
    ". .",
    " _ ",
    "/ \\"
};
CharMap ROTATOR_CLCW_CMP = {
    "/-\\",
    "| v",
    "\\-/"
};
CharMap ROTATOR_CTCLCW_CMP = {
    "/-\\",
    "v |",
    "\\-/"
};
CharMap CLONER_UP_CMP = {
    "/^\\",
    " | ",
    " \xc1 "
};
CharMap CLONER_RIGHT_CMP = {
    "  \\",
    "\xc3->",
    "  /"
};
CharMap CLONER_DOWN_CMP = {
    " \xc2 ",
    " | ",
    "\\v/"
};
CharMap CLONER_LEFT_CMP = {
    "/  ",
    "<-\xb4",
    "\\  "
};
CharMap UNIDIRECTIONAL_HORZ_CMP = {
    "---",
    "\xcd\xcd\xcd",
    "---"
};
CharMap UNIDIRECTIONAL_VERT_CMP = {
    "|\xba|",
    "|\xba|",
    "|\xba|"
};
CharMap STATIC_CMP = {
    "\xda\xc4\xbf",
    "\xb3 \xb3",
    "\xc0\xc4\xd9"
};

const int CELL_H_SIZE = 4;
const int CELL_V_SIZE = 4;

void GameLevel::drawCell(uchar ch, int row, int col, COLORS foreground_color, COLORS background_color) {   
    CharMap* charMap;

    switch (ch) {
        case MOVER_CH_UP: charMap = &MOVER_UP_CMP; break;
        case MOVER_CH_RIGHT: charMap = &MOVER_RIGHT_CMP; break;
        case MOVER_CH_DOWN: charMap = &MOVER_DOWN_CMP; break;
        case MOVER_CH_LEFT: charMap = &MOVER_LEFT_CMP; break;
        case PUSHABLE_CH: charMap = &PUSHABLE_CMP; break;
        case ENEMY_CH: charMap = &ENEMY_CMP; break;
        case ROTATOR_CH_CLCW: charMap = &ROTATOR_CLCW_CMP; break;
        case ROTATOR_CH_CTCLCW: charMap = &ROTATOR_CTCLCW_CMP; break;
        case CLONER_CH_UP: charMap = &CLONER_UP_CMP; break;
        case CLONER_CH_RIGHT: charMap = &CLONER_RIGHT_CMP; break;
        case CLONER_CH_DOWN: charMap = &CLONER_DOWN_CMP; break;
        case CLONER_CH_LEFT: charMap = &CLONER_LEFT_CMP; break;
        case UNIDIRECTIONAL_CH_HORZ: charMap = &UNIDIRECTIONAL_HORZ_CMP; break;
        case UNIDIRECTIONAL_CH_VERT: charMap = &UNIDIRECTIONAL_VERT_CMP; break;
        case STATIC_CH: charMap = &STATIC_CMP; break;
        default: charMap = nullptr;
    }

    const char H_LINE[] = { '\xda', '\xc4', '\xc4', '\xc4' };
    const char V = '\xb3';

    row = CELL_H_SIZE * row + rOff;
    col = CELL_V_SIZE * col + cOff;

    for(int i = 0; i < 4; i++){
        draw_char(H_LINE[i], row, col+i, WHITE, BLACK);
    }
    for(int i = 0; i < 3; i++){
        draw_char(V, row + 1 + i, col, WHITE, BLACK);
    }
    row++;
    col++;

    if(charMap == nullptr){
        for(int r = 0; r < CELL_V_SIZE - 1; r++){
            for(int c = 0; c < CELL_H_SIZE - 1; c++){
                draw_char(ch, row + r, col + c, foreground_color, background_color);
            }
        }
    } else {
        for(int r = 0; r < CELL_V_SIZE - 1; r++){
            for(int c = 0; c < CELL_H_SIZE - 1; c++){
                draw_char((*charMap)[r][c], row + r, col + c, foreground_color, background_color);
            }
        }
    }
}

bool GameLevel::push(int r, int c, int d){
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
        if(!push(newr,newc,d)){return false;}
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
        if(!push(newr,newc,d)){return false;}
        setCell(field[newr][newc],cellchar);
        setCell(field[r][c],EMPTY_CH);
        return true;
    }
    return false;
}

bool GameLevel::readFromFile(string path){//definiciq
    string tmp;
    ifstream in_f(path);
    if (!in_f.good()) {
        return false;
    }
    in_f >> rows >> cols;
    in_f >> min_r >> min_c;
    in_f >> max_r >> max_c;
    rOff = (MAP_ROWS - rows) / 3;
    cOff = (MAP_COLS - cols) / 2;
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
                case STATIC_CH_LEGEND:setCell(field[r][c],STATIC_CH);break;
                case ENEMY_CH_LEGEND:setCell(field[r][c],ENEMY_CH);br_enemies++;break;
                case MOVER_CH_LEGEND_0:setCell(field[r][c],MOVER_CH[0]);break;
                case MOVER_CH_LEGEND_1:setCell(field[r][c],MOVER_CH[1]);break;
                case MOVER_CH_LEGEND_2:setCell(field[r][c],MOVER_CH[2]);break;
                case MOVER_CH_LEGEND_3:setCell(field[r][c],MOVER_CH[3]);break;
                case ROTATOR_CH_LEGEND_0:setCell(field[r][c],ROTATOR_CH[0]);break;
                case ROTATOR_CH_LEGEND_1:setCell(field[r][c],ROTATOR_CH[1]);break;
                case CLONER_CH_LEGEND_0:setCell(field[r][c],CLONER_CH[0]);break;
                case CLONER_CH_LEGEND_1:setCell(field[r][c],CLONER_CH[1]);break;
                case CLONER_CH_LEGEND_2:setCell(field[r][c],CLONER_CH[2]);break;
                case CLONER_CH_LEGEND_3:setCell(field[r][c],CLONER_CH[3]);break;
                case PUSHABLE_CH_LEGEND:setCell(field[r][c],PUSHABLE_CH);break;
                case UNIDIRECTIONAL_CH_LEGEND_0:setCell(field[r][c],UNIDIRECTIONAL_CH[0]);break;
                case UNIDIRECTIONAL_CH_LEGEND_1:setCell(field[r][c],UNIDIRECTIONAL_CH[1]);break;
                case EMPTY_CH_LEGEND:setCell(field[r][c],EMPTY_CH);break;
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
            drawCell(field[r][c].ch, r, c, fg, bg);
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
            drawCell(hand.ch, r_cur, c_cur, CURSOR_FG_COL, CURSOR_BG_COL);
        } else {
            drawCell(field[r_cur][c_cur].ch, r_cur, c_cur, field[r_cur][c_cur].col, CURSOR_BG_COL);
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

bool GameLevel::blinkMessage(string msg) {
    const int SLEEP_PERIOD = 100; // see https://www.nngroup.com/articles/response-times-3-important-limits/

    while (_kbhit()) {
        getch();
    }

    /**
        ___________________
       |   (rOff, cOff)
       |   @____
       |   |    | 
       |  VVVVVVVV
       |   |____|
       |___________________ 
    */

    msg = ' ' + msg + ' ';

    int row = rOff + (CELL_H_SIZE * rows)/2;
    int col = cOff + (CELL_V_SIZE * cols)/2 - msg.length()/2;

    bool variant = false;
    while (true) {
        for (int i = 0; i < msg.length(); i++) {
            if (variant) {
                draw_char(' ', row - 1, col + i, BLINK_FG_1, BLINK_BG_1);
                draw_char(msg[i], row, col + i, BLINK_FG_1, BLINK_BG_1);
                draw_char(' ', row + 1, col + i, BLINK_FG_1, BLINK_BG_1);
            } else {
                draw_char(' ', row - 1, col + i, BLINK_FG_2, BLINK_BG_2);
                draw_char(msg[i], row, col + i, BLINK_FG_2, BLINK_BG_2);
                draw_char(' ', row + 1, col + i, BLINK_FG_2, BLINK_BG_2);
            }
        }
        variant = !variant;
        for (int totalSleep = 0; totalSleep < BLINK_PERIOD; totalSleep += SLEEP_PERIOD) {
            Sleep(SLEEP_PERIOD);
            if (GetAsyncKeyState('Q')) {
                return false;
            }
            if (_kbhit()) {
                return true;
            }
        }
    }
}

void GameLevel::copyField(){
    field_copy = new Cell*[rows];
    for (int r = 0; r < rows; r++) {
        field_copy[r] = new Cell[cols];
        for (int c = 0; c < cols; c++){
            field_copy[r][c] = field[r][c];
        }
    }
    br_enemies_copy = br_enemies;
}

void GameLevel::deleteField(Cell**& f) {
    for (int r = 0; r < rows; r++) {
        delete[] f[r];
    }
    delete[] f;
    f = nullptr;
}

void GameLevel::resetFieldFromCopy(){
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++){
            field[r][c] = field_copy[r][c];
        }
    }
    br_enemies = br_enemies_copy;
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
    while (br_enemies>0) {
        edit(); // <-----
        copyField();
        
        printField();
        while(br_enemies>0){
            if(GetAsyncKeyState(VK_SPACE)){
                printField();
                stepThroughTime();
                Sleep(100);
            }
            if(GetAsyncKeyState('R')){
                //reset level
                resetFieldFromCopy();
                break;
            }
        }
    }
    printField();
    deleteField(field);
    deleteField(field_copy);
    if(!blinkMessage(VICTORY_MESSAGE)){
        exit(0);
    }
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
                field[r+diffr[d]][c+diffc[d]].touched = push(r,c,d);
            }
            if((cellchar == CLONER_CH[0]) || (cellchar == CLONER_CH[1]) || (cellchar == CLONER_CH[2]) || (cellchar == CLONER_CH[3])){
                if(cellchar == CLONER_CH[0]){d = 0;}
                if(cellchar == CLONER_CH[1]){d = 1;}
                if(cellchar == CLONER_CH[2]){d = 2;}
                if(cellchar == CLONER_CH[3]){d = 3;}
                if(field[r-diffr[d]][c-diffc[d]].ch == EMPTY_CH){continue;}
                field[r][c].touched = push(r+diffr[d],c+diffc[d],d);
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
    int colorsInitError = colorsInit();
    if(colorsInitError != 0){return colorsInitError;}
    srand(time(0));
    for (int i = 1; true; i++) {
        GameLevel gameLevel;
        if (!gameLevel.readFromFile(string("level-") + (char)(i+'0') + ".txt")) {
            gameLevel.blinkMessage(GG_MESSAGE);
            break;
        }
        Sleep(100);
        gameLevel.run();
        Sleep(100);
    }
    Sleep(100);
    return 0;
}