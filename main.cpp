//g++ main.cpp -o main.exe -std=c++11 && start main.exe
#include <iostream>
using namespace std;
int main(){
    int rows = 4;
    int cols = 5;
    char** field = new char*[rows];
    for (int r = 0; r < rows; r++) {
        field[r] = new char[cols];
    }
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







