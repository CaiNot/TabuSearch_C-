#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <sstream>
#include <ctime>
#include <cstdlib>


using namespace std;

void getColorNum(int& K);

void mallocGraph();
void freeGraph();

int readFile(string PATH);
int writeFile(string PATH);

int isTabu(int iter);
void setAdjColor();
void allocVexColorRandom();
void findMove(int iter);
void makeMove(int iter);


