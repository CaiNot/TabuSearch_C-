#include "graph.h"


int *vexsColor = NULL;// ÿһ���ڵ���ɫ����
int **graph = NULL;// �ڽӱ� ����ÿһ���ڵ��Ӧ���ڽӽڵ㣨ָ�룩
int *edgeNum = NULL;// ÿһ���ڵ�ı���

int **adjColorGraph = NULL;// �ڽӱ� ����ÿһ���ڵ��Ӧ���ڽӽڵ���ɫ����

int **tabuList = NULL;// ���ɱ���ÿһ���ڵ㣬��ɫ������ɲ���


int *adjColors = NULL;// �ظ�ʹ�ã������ڽӵ���ɫ����
int *adjVexs = NULL;// �ظ�ʹ�ã�����ڵ���ڽӽڵ�
int *tabuVex = NULL;

int K = 0;// ��ɫ��
int N = 0;// �ڵ���
int V = 0;// ��ʷ���Ž��뵱ǰ��Ĳ�

int delt = 0, delt_tabu = 0, f = 0, f_best = 0;// �ǽ�������, �������� Ŀ�꺯��ֵ Ŀ�꺯����ʷ����ֵ
int vexToMove = 0, colorToMove = 0, oldColor = 0;

void setZero(int* array,int length) {
	for (int i = 0; i < length; i++) {
		array[i] = 0;
	}
}

void mallocGraph() {
	graph = new int*[N];
	edgeNum = new int[N];

	setZero(edgeNum, N);
	
	int i = 0;
	for (i = 0; i < N; i++) {
		adjVexs = new int[N];
		setZero(adjVexs, N);
		graph[i] = adjVexs;
	}

}


void getColorNum(int& K) {
	cout << "Please input color num:" << endl;
	cin >> K;
	if (K < 1) {
		cout << "Error!!\n color num is too low." << endl;
		exit(0);
	}
	vexsColor = new int[N];
	setZero(vexsColor, N);
	
	tabuList = new int*[N];

	for (int i = 0; i < N; i++) {
		tabuVex = new int[K];
		setZero(tabuVex, K);
		tabuList[i] = tabuVex;
	}
	//mallocGraph();
}

vector<string> split(const string &str, const string &pattern)
{
	//const char* convert to char*
	char * strc = new char[strlen(str.c_str()) + 1];
	strcpy(strc, str.c_str());
	vector<string> resultVec;
	char* tmpStr = strtok(strc, pattern.c_str());
	while (tmpStr != NULL)
	{
		resultVec.push_back(string(tmpStr));
		tmpStr = strtok(NULL, pattern.c_str());
	}

	delete[] strc;

	return resultVec;
}

int readFile(string PATH) {
	ifstream infile;
	infile.open(PATH.data());
	assert(infile.is_open());

	string line;
	string pattern(" ");
	//stringstream ss;
	vector<string> line_splited;
	int x = 0, y = 0;// ��ʱ����
	while (getline(infile, line)) {
		if (line.at(0) == 'p') {
			line_splited = split(line, pattern);
			//ss << line_splited.at(2);
			N = stoi(line_splited.at(2));
			//ss  >> N;
			//ss.clear();
			mallocGraph();
		}
		else if (line.at(0)=='e') {
			line_splited = split(line, pattern);
			x = stoi(line_splited.at(1));
			y = stoi(line_splited.at(2));
			//ss <<  << line_splited.at(2);
			//ss >> x >> y;
			//ss.clear();
			x--;
			y--;
			graph[x][edgeNum[x]] = y; // x�ڵ���һ��y
			graph[y][edgeNum[y]] = x;
			edgeNum[x]++;
			edgeNum[y]++;
		}
	}

	infile.close();
	return 0;
}

void allocVexColorRandom() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < N; i++) {
		vexsColor[i] = rand() % K;
	}
	setAdjColor();
}

void setAdjColor() {
	adjColorGraph = new int*[N];
	//int *colorsInVex = new int[K];
	int vex = 0, vexColorOne = 0;

	for (int i = 0; i < N; i++) {
		adjColors = new int[K];
		//colorsInVex = { 0 };
		//adjColors = { 0 };
		setZero(adjColors, K);
	
		for (int n = 0; n < edgeNum[i]; n++) {
			vex = graph[i][n];
			vexColorOne = vexsColor[i];
			if (vexColorOne == vexsColor[vex]) {
				f++;
			}
			adjColors[vexsColor[vex]]++;
		}
		adjColorGraph[i] = adjColors;
	}
	f = f / 2;
	f_best = f;
	V = f_best - f;
}

int isTabu(int iter,int i,int aimColor) {// ��������, �����ڵ�, Ŀ����ɫ
	if (tabuList[i][aimColor] >iter) {// �ڵ��Ŀ����ɫ�ͽ�����ɫһ��, Ŀǰ��δ���ͷ�
		return 1;
	}
	return 0;
}

void findMove(int iter) {
	int delt_temp = 1000;

	int count_tabu = 0, count = 0;

	int vex_tabu = 0, color_tabu = 0;
	int vex_normal = 0, color_normal = 0;
	delt_tabu = 1000;
	delt = 1000;

	for (int i = 0; i < N; i++) {
		if (adjColorGraph[i][vexsColor[i]] != 0) {// �ýڵ��г�ͻ

			for (int k = 0; k < K; k++) {
				if (k != vexsColor[i]) {// �ڵ����ɫҪ�ܹ������ı�

					delt_temp = adjColorGraph[i][k] - adjColorGraph[i][vexsColor[i]];

					if (isTabu(iter,i,k)==1) {
						if (delt_temp <= delt_tabu) {
							if (delt_temp < delt_tabu) {
								count_tabu = 0;
								delt_tabu = delt_temp;
								vex_tabu = i;
								color_tabu = k;
							}
							else {
								count_tabu++;
								if (0 == rand() % count_tabu) {
									vex_tabu = i;
									color_tabu = k;
								}
							}
						}
					}
					else {
						if (delt_temp <= delt) {
							if (delt_temp < delt) {
								count = 0;
								delt = delt_temp;
								vex_normal = i;
								color_normal = k;
							}
							else {
								count++;
								//int ran = rand();
								////cout <<" " <<ran << "\t";
								//ran =;
								if (0 == rand() % count) {
									vex_normal = i;
									color_normal = k;
								}
							}

						}
					}
				}

			}

		}
	}
	//cout << endl;

	

	if (delt_tabu < delt && delt_tabu < V) {
		vexToMove = vex_tabu;
		colorToMove = color_tabu;
		oldColor = vexsColor[vex_tabu];
		delt = delt_tabu;
	}
	else {
		vexToMove = vex_normal;
		colorToMove = color_normal;
		oldColor = vexsColor[vex_normal];
	}
}

void makeMove(int iter) {
	f = f + delt;
	//if (f < 5)
		//cout << "  " << delt << "\t";
	if (f < f_best) {
		f_best = f;
		cout << "iter: " << iter<<"\tf_best: "<<f_best << "\t" << endl;
	}
	V = f_best - f;
	vexsColor[vexToMove] = colorToMove;
	tabuList[vexToMove][oldColor] = iter + f + rand() % 10;

	int adjVex = 0;
	//adjColorGraph[vexToMove][oldColor]--;
	for (int i = 0; i < edgeNum[vexToMove]; i++) {
		adjVex = graph[vexToMove][i];
		adjColorGraph[adjVex][oldColor]--;
		adjColorGraph[adjVex][colorToMove]++;
	}
}

void tabuSearch(int limitMins) {
	//K = 10;
	//while (K > 3) {
		getColorNum(K);
		allocVexColorRandom();
		int iter = 0;
		while (f > 0) {
			findMove(iter);
			makeMove(iter);
			iter++;
		}
		cout << "Color num is: " << K << endl;
		//K--;
	//}
}

void freeDoubleList(int**&list, int length) {
	for (int i = 0; i < length; i++) {
		delete list[i];
	}
	delete list;
}

void freeGraph() {
	delete vexsColor;
	delete edgeNum;
	freeDoubleList(graph, N);
	freeDoubleList(adjColorGraph, N);
	freeDoubleList(tabuList, N);
}


int main() {
	while (true) {
		readFile("D:/instances/instances/DSJC500.5.col");
		tabuSearch(2);
		freeGraph();
	}
	system("pause");
	return 0;
}