#include<stdio.h>
#include<iostream>
#include<vector>
#include<graphics.h>

#define INTERVAL 50  //ǰ��ļ��
#define CHESS_GRID_SIZE 70  //���ӿ��
#define ROW  10
#define COL  9

using namespace std;

enum PIECES//ö������
{
	܇, �R, ��, ��, ��, ��, ��,//�췽
	��, ��, ��, ��, ��, �h, ��,//�ڷ�
	SPACE, BEGIN, END
};
//��ڷ�����
int redChess[] = { ܇, �R, ��, ��, ��, ��, �� };
int blackChess[] = { ��, ��, ��, ��, ��, �h, �� };
const TCHAR* chessName[] = { TEXT("܇"), TEXT("�R"), TEXT("��"), TEXT("��"), TEXT("��"), TEXT("��"), TEXT("��"), 
							TEXT("��"), TEXT("��"), TEXT("��"), TEXT("��"), TEXT("��"), TEXT("�h"), TEXT("��") };
//��������
struct Chess {
	int x;
	int y;
	int id;//�ĸ�����
	int type;//����һ�������� �죬�ڣ�
	bool river;//�ж�С���Ƿ���˺�
};
struct Chess map[ROW][COL];//�ṹ�����飬��������ÿһ�������Ϣ
//���̲���
//����Ϊ�ڷ���������Ϊ�췽����0Ϊ�յ�
//1xΪ�ڣ�2xΪ��
const int defaultchessboard[ROW][COL] = { {-1,-2,-3,-4,-5,-6,-7,-8,-9},
							{-20,-21,-22,-23,-24,-25,-26,-27,-28},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{20,0,21,0,22,0,23,0,24},
							{0,10,0,0,11,0,0,12,0},
							{0,0,0,0,0,0,0,0,0},
							{1,2,3,4,5,6,7,8,9} };
static int chessboard[ROW][COL] = {{-1,-2,-3,-4,-5,-6,-7,-8,-9},
							{-20,-21,-22,-23,-24,-25,-26,-27,-28},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{20,0,21,0,22,0,23,0,24},
							{0,10,0,0,11,0,0,12,0},
							{0,0,0,0,0,0,0,0,0},
							{1,2,3,4,5,6,7,8,9}};
int state = BEGIN;
bool pickup = false;//����������
POINT pointBegin = { -1, -1 }, pointEnd = { -1, -1 };//����ǰ�����ε���������±�
static vector<POINT> moveFrom;
static vector<POINT> moveTo;
static vector<int> moveEat;

void GameInit() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			chessboard[i][j] = defaultchessboard[i][j];
		}
	}
	moveFrom.clear();
	moveTo.clear();
	moveEat.clear();
	pointBegin = {-1, -1};
	pointEnd = {-1, -1};
	pickup = false;
	return;
	/*for (int i = 0; i < ROW; i++) {
		for (int k = 0; k < COL; k++) {
			int chessname = SPACE;
			int mcolor = BLACK;
			//����
			if (i <= 4) {
				mcolor = BLACK;
				//��ʼ����һ�е����ӣ�
				if (i == 0) {
					if (k <= 4)	{
						chessname = blackChess[k];
					}
					else {
						chessname = blackChess[8 - k];
					}
				}
				//������
				if (i == 2 && (k == 1 || k == 7)) {
					chessname = blackChess[5];
				}
				//����С��
				if (i == 3 && k % 2 == 0) {
					chessname = blackChess[6];
				}
			}
			//�t��
			else {
				mcolor = RED;
				//��ʼ����һ�е����ӣ�
				if (i == 9)	{
					if (k <= 4) {
						chessname = redChess[k];
					}
					else {
						chessname = redChess[8 - k];
					}
				}
				//������
				if (i == 7 && (k == 1 || k == 7)) {
					chessname = redChess[5];
				}
				//����С��
				if (i == 6 && k % 2 == 0) {
					chessname = redChess[6];
				}
			}
			map[i][k].id = chessname;
			map[i][k].river = false;
			map[i][k].type = mcolor;
			map[i][k].x = k * CHESS_GRID_SIZE + INTERVAL;
			map[i][k].y = i * CHESS_GRID_SIZE + INTERVAL;
		}
	}*/
}

void DrawChessboard() {
	//���ñ�����ɫ red black 
	setbkcolor(RGB(252, 215, 162));
	cleardevice();
	//��������
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 2);
	for (int i = 0; i < 10; i++) {
		//������
		line(INTERVAL, i * CHESS_GRID_SIZE + INTERVAL, 8 * CHESS_GRID_SIZE + INTERVAL, i * CHESS_GRID_SIZE + INTERVAL);
		//������
		if (i < 9) {
			line(i * CHESS_GRID_SIZE + INTERVAL, INTERVAL, i * CHESS_GRID_SIZE + INTERVAL, 9 * CHESS_GRID_SIZE + INTERVAL);
		}
	}
	rectangle(INTERVAL - 5, INTERVAL - 5, 8 * CHESS_GRID_SIZE + INTERVAL + 5, 5 + 9 * CHESS_GRID_SIZE + INTERVAL);
	//���Ӻ�����ʾ
	setfillcolor(RGB(252, 215, 162));
	fillrectangle(INTERVAL, 4 * CHESS_GRID_SIZE + INTERVAL, 8 * CHESS_GRID_SIZE + INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL);
	//��ʾ����
	char river[20] = "����        ����";
	settextstyle(50, 0, _T("����"));
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(INTERVAL + 100, 4 * CHESS_GRID_SIZE + INTERVAL + 10, _T("����        ����"));
	//���Ź���
	//������
	line(3 * CHESS_GRID_SIZE + INTERVAL, INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL, 2 * CHESS_GRID_SIZE + INTERVAL);
	line(3 * CHESS_GRID_SIZE + INTERVAL, 2 * CHESS_GRID_SIZE + INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL, INTERVAL);
	//������
	line(3 * CHESS_GRID_SIZE + INTERVAL, 7 * CHESS_GRID_SIZE + INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL, 9 * CHESS_GRID_SIZE + INTERVAL);
	line(3 * CHESS_GRID_SIZE + INTERVAL, 9 * CHESS_GRID_SIZE + INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL, 7 * CHESS_GRID_SIZE + INTERVAL);
	//���������
	settextstyle(40, 0, _T("����"));
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(9 * CHESS_GRID_SIZE + INTERVAL, 9 * CHESS_GRID_SIZE + INTERVAL, _T("����"));
	outtextxy(9 * CHESS_GRID_SIZE + INTERVAL, 10 * CHESS_GRID_SIZE + INTERVAL, _T("����"));
}

void DrawChess() {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (chessboard[i][j] != 0) {
				Chess thisChess;
				thisChess.x = 0;
				thisChess.y = 0;
				thisChess.id = 0;
				thisChess.type = 0;
				thisChess.river = false;
				bool exist = false;
				switch (chessboard[i][j]) {
				case 1: thisChess.id = redChess[0]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break;
				case 2: thisChess.id = redChess[1]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 3: thisChess.id = redChess[2]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 4: thisChess.id = redChess[3]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 5: thisChess.id = redChess[4]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 6: thisChess.id = redChess[3]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 7: thisChess.id = redChess[2]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 8: thisChess.id = redChess[1]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 9: thisChess.id = redChess[0]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 10: thisChess.id = redChess[5]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 11: thisChess.id = redChess[5]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 12: thisChess.id = redChess[5]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 13: thisChess.id = redChess[5]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 20: thisChess.id = redChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 21: thisChess.id = redChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 22: thisChess.id = redChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 23: thisChess.id = redChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case 24: thisChess.id = redChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = RED; exist = true; break; 
				case -1: thisChess.id = blackChess[0]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -2: thisChess.id = blackChess[1]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -3: thisChess.id = blackChess[2]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -4: thisChess.id = blackChess[3]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -5: thisChess.id = blackChess[4]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -6: thisChess.id = blackChess[3]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -7: thisChess.id = blackChess[2]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -8: thisChess.id = blackChess[1]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -9: thisChess.id = blackChess[0]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -20: thisChess.id = blackChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -21: thisChess.id = blackChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -22: thisChess.id = blackChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -23: thisChess.id = blackChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -24: thisChess.id = blackChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -25: thisChess.id = blackChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -26: thisChess.id = blackChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -27: thisChess.id = blackChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				case -28: thisChess.id = blackChess[6]; thisChess.x = j * CHESS_GRID_SIZE + INTERVAL; thisChess.y = i * CHESS_GRID_SIZE + INTERVAL; thisChess.river = false; thisChess.type = BLACK; exist = true; break; 
				}
				if (exist) {
					setlinecolor(thisChess.type);
					fillcircle(thisChess.x, thisChess.y, 30);
					fillcircle(thisChess.x, thisChess.y, 25);
					settextcolor(thisChess.type);
					outtextxy(thisChess.x - 20, thisChess.y - 20, chessName[thisChess.id]);
				}
				/* {
					map[i][k].id = chessname;
					map[i][k].river = false;
					map[i][k].type = mcolor;
					map[i][k].x = k * CHESS_GRID_SIZE + INTERVAL;
					map[i][k].y = i * CHESS_GRID_SIZE + INTERVAL;
					setlinecolor(map[i][k].type);
					fillcircle(map[i][k].x, map[i][k].y, 30);
					fillcircle(map[i][k].x, map[i][k].y, 25);
					settextcolor(map[i][k].type);
					outtextxy(map[i][k].x - 15, map[i][k].y - 15, chessName[map[i][k].id]);
				}*/
			}
		}
	}
}

void GameDraw() {
	//���ñ�����ɫ red black 
	setbkcolor(RGB(252, 215, 162));
	cleardevice();
	//��������
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 2);
	for (int i = 0; i < 10; i++) {
		//������
		line(INTERVAL, i * CHESS_GRID_SIZE + INTERVAL, 8 * CHESS_GRID_SIZE + INTERVAL, i * CHESS_GRID_SIZE + INTERVAL);
		//������
		if (i < 9) {
			line(i * CHESS_GRID_SIZE + INTERVAL, INTERVAL, i * CHESS_GRID_SIZE + INTERVAL, 9 * CHESS_GRID_SIZE + INTERVAL);
		}
	}
	rectangle(INTERVAL - 5, INTERVAL - 5, 8 * CHESS_GRID_SIZE + INTERVAL + 5, 5 + 9 * CHESS_GRID_SIZE + INTERVAL);
	//���Ӻ�����ʾ
	setfillcolor(RGB(252, 215, 162));
	fillrectangle(INTERVAL, 4 * CHESS_GRID_SIZE + INTERVAL, 8 * CHESS_GRID_SIZE + INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL);
	//��ʾ����
	char river[20] = "����        ����";
	settextstyle(50, 0, _T("����"));
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	outtextxy(INTERVAL + 100, 4 * CHESS_GRID_SIZE + INTERVAL + 10, _T("����        ����"));
	//���Ź���
	//������
	line(3 * CHESS_GRID_SIZE + INTERVAL, INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL, 2 * CHESS_GRID_SIZE + INTERVAL);
	line(3 * CHESS_GRID_SIZE + INTERVAL, 2 * CHESS_GRID_SIZE + INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL, INTERVAL);
	//������
	line(3 * CHESS_GRID_SIZE + INTERVAL, 7 * CHESS_GRID_SIZE + INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL, 9 * CHESS_GRID_SIZE + INTERVAL);
	line(3 * CHESS_GRID_SIZE + INTERVAL, 9 * CHESS_GRID_SIZE + INTERVAL, 5 * CHESS_GRID_SIZE + INTERVAL, 7 * CHESS_GRID_SIZE + INTERVAL);
	//������
	settextstyle(30, 0, _T("����"));
	for (int i = 0; i < ROW; i++)
	{
		for (int k = 0; k < COL; k++)
		{
			if (map[i][k].id != SPACE)
			{
				setlinecolor(map[i][k].type);
				fillcircle(map[i][k].x, map[i][k].y, 30);
				fillcircle(map[i][k].x, map[i][k].y, 25);
				settextcolor(map[i][k].type);
				outtextxy(map[i][k].x - 15, map[i][k].y - 15, chessName[map[i][k].id]);
			}

		}
	}
}

bool ValidateToMove(int chessType, POINT pointBegin, POINT pointEnd) {
	bool flag = true;
	//��������Ϊ�գ���׼�ƶ�
	if (chessType == 0) flag = false;
	//����յ�������ͬ����׼�ƶ�
	if (pointBegin.x == pointEnd.x && pointBegin.y == pointEnd.y) flag = false;
	//�յ����곬�����̣���׼�ƶ�
	if (pointEnd.x < 0 || pointEnd.x >= ROW || pointEnd.y < 0 || pointEnd.y >= COL) flag = false;
	return flag;
}

void UndoMove() {
	if (moveTo.empty()) {
		pickup = false;
	}
	else {
		POINT undoFrom = moveTo.back();
		POINT undoTo = moveFrom.back();
		chessboard[undoTo.x][undoTo.y] = chessboard[undoFrom.x][undoFrom.y];
		chessboard[undoFrom.x][undoFrom.y] = moveEat.back();
		moveFrom.pop_back();
		moveTo.pop_back();
		moveEat.pop_back();
		pickup = false;
	}
	return;
}

void MouseControl() {
	if (MouseHit())	{
		MOUSEMSG msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN)	{
			//��ȡ�������������±�
			int row = (msg.y - INTERVAL/2) / CHESS_GRID_SIZE;
			int col = (msg.x - INTERVAL/2) / CHESS_GRID_SIZE;
			printf("%d,%d  %d\n", row, col, state);
			if ((row == 9 && col == 9) || (row == 9 && col == 10)) {
				UndoMove();
				return;
			}
			if ((row == 10 && col == 9) || (row == 10 && col == 10)) {
				GameInit();
				return;
			}
			
			if (!pickup) {
				pointBegin.x = row;
				pointBegin.y = col;
				pickup = true;
			}
			else {
				pointEnd.x = row;
				pointEnd.y = col;
				//����ܷ��ƶ�
				bool canMove = ValidateToMove(chessboard[pointBegin.x][pointBegin.y], pointBegin, pointEnd);
				if (canMove) {
					moveEat.push_back(chessboard[pointEnd.x][pointEnd.y]);
					moveFrom.push_back(pointBegin);
					moveTo.push_back(pointEnd);
					chessboard[pointEnd.x][pointEnd.y] = chessboard[pointBegin.x][pointBegin.y];
					chessboard[pointBegin.x][pointBegin.y] = 0;
				}
				pickup = false;
			}

			/* if (state == BEGIN) {
				state = END;
				pointBegin.x = row;
				pointBegin.y = col;
			}
			else if (state == END) {
				state = BEGIN;
				pointBegin.x = row;
				pointBegin.y = col;
			}*/
		}
	}
}

void LogicControl() {

}

void chessMove() {
	if (pointBegin.x != -1 && pointEnd.x != -1 && !(pointBegin.x == pointEnd.x && pointBegin.y == pointEnd.y)) {
		map[pointEnd.x][pointEnd.y].id = map[pointBegin.x][pointBegin.y].id;
		map[pointEnd.x][pointEnd.y].type = map[pointBegin.x][pointBegin.y].type;
		map[pointEnd.x][pointEnd.y].river = map[pointBegin.x][pointBegin.y].river;
		map[pointBegin.x][pointBegin.y].id = SPACE;
	}
}

int main() {
	//����һ��ͼ�δ���
	initgraph(800, 800, SHOWCONSOLE);
	GameInit();
	BeginBatchDraw();
	while (1) {
		//GameDraw();
		FlushBatchDraw();
		DrawChessboard();
		DrawChess();
		MouseControl();
		//chessMove();
	}
	return 0;
}