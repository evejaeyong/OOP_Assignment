#include <iostream>
#include <cstdlib>
#include <time.h>
#include <Windows.h>
using namespace std;

char map[63][63];			//미로 여기에 넣음
char short_map[63][63];		//이 안에 경로 집어넣음
int maze[4][2]{ {0, 2}, {0, -2}, {-2, 0}, {2, 0} };	//상,하,좌,우 순서
int solve_maze[4][2]{ {0, 1}, {0, -1}, {-1, 0}, {1, 0} };	//상,하,좌,우 순서
int queue[63 * 63 * 2];		// bfs에서 사용
int front = 0, rear = 0;
bool check = 0;

void gotoxy(int x, int y) {	// 콘솔창 좌표 지정
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void Print(int row, int col) {
	gotoxy(0, 1);
	for (int i = 0; i < col * 2 + 1; i++) {
		for (int j = 0; j < row * 2 + 1; j++) {
			if (map[i][j] == '1') cout << "O";			//벽
			else if (map[i][j] == '0') cout << " ";		//탐색 안한 경로
			else if (map[i][j] == '2') cout << "@";		//탐색 경로
			else if (map[i][j] == '3') cout << "X";		//목표 지점
			else if (map[i][j] == '7') cout << "$";		//최단경로
		}
		cout << "\n";
	}
	cout << "\n";
}


void maze_generate(int x, int y, int row, int col) {	//미로 생성
	int new_x, new_y;
	int random = rand() % 4;
	for (int i = 0; i < 4; i++) {
		new_x = x + maze[(i + random) % 4][0];
		new_y = y + maze[(i + random) % 4][1];

		if (((x - 1 < 0) || (x + 1 > row * 2 + 1) || (y - 1 < 0) || (y + 1 > col * 2 + 1)) == 0
			&& map[new_y][new_x] == '1') {
			Print(row, col);
			Sleep(100);

			if (new_x == x) {
				map[new_y][x] = '0';
				if (new_y - y > 0) map[new_y - 1][x] = '0';
				else map[new_y + 1][x] = '0';
			}
			else {
				map[y][new_x] = '0';
				if (new_x - x > 0) map[y][new_x - 1] = '0';
				else map[y][new_x + 1] = '0';
			}

			maze_generate(new_x, new_y, row, col);
		}
	}
}

void bfs(int sx, int sy, int ex, int ey, int row, int col) {
	queue[rear++] = sx;
	queue[rear++] = sy;

	while (front != rear) {
		int x1 = queue[front++];
		int y1 = queue[front++];

		for (int i = 0; i < 4; i++) {

			int new_x = x1 + solve_maze[i][0];	//x, y 좌표를 변경해준 새로운 좌표값
			int new_y = y1 + solve_maze[i][1];

			if (new_x == ex && new_y == ey) {
				map[new_y][new_x] = '3';
				if (new_x == x1) {
					if (new_y > y1) {
						short_map[new_y][new_x] = 1;
					}
					else {
						short_map[new_y][new_x] = 0;
					}
				}
				else {
					if (new_x > x1) {
						short_map[new_y][new_x] = 2;
					}
					else {
						short_map[new_y][new_x] = 3;
					}
				}
				return;
			}

			if (((sx - 1 < 0) || (sx + 1 > row * 2 + 1) || (sy - 1 < 0) || (sy + 1 > col * 2 + 1)) == 0
				&& map[new_y][new_x] == '0') {
				if (new_x == x1) {
					if (new_y > y1) {
						short_map[new_y][new_x] = 1;
					}
					else {
						short_map[new_y][new_x] = 0;
					}
				}
				else {
					if (new_x > x1) {
						short_map[new_y][new_x] = 2;
					}
					else {
						short_map[new_y][new_x] = 3;
					}
				}

				map[new_y][new_x] = '2';
				queue[rear++] = new_x;
				queue[rear++] = new_y;
			}

			Print(row, col);
			Sleep(20);

		}
	}
}

void dfs(int sx, int sy, int ex, int ey, int row, int col) {
	int new_x, new_y;
	for (int i = 0; i < 4; i++) {
		
		new_x = sx + solve_maze[i][0];
		new_y = sy + solve_maze[i][1];

		if (check) return;

		if (new_y == ey && new_x == ex) {
			map[new_y][new_x] = '3';
			check = 1;

			if (new_x == sx) {
				if (new_y > sy) {
					short_map[new_y][new_x] = 1;
				}
				else {
					short_map[new_y][new_x] = 0;
				}
			}
			else {
				if (new_x > sx) {
					short_map[new_y][new_x] = 2;
				}
				else {
					short_map[new_y][new_x] = 3;
				}
			}
			return;
		}

		if (((sx - 1 < 0) || (sx + 1 > row * 2 + 1) || (sy - 1 < 0) || (sy + 1 > col * 2 + 1)) == 0
			&& map[new_y][new_x] == '0') {
			Print(row, col);
			Sleep(50);

			if (new_x == sx) {
				map[new_y][sx] = '2';
				if (new_y > sy) {
					short_map[new_y][new_x] = 1;
				}
				else {
					short_map[new_y][new_x] = 0;
				}
			}
			else {
				map[sy][new_x] = '2';
				if (new_x > sx) {
					short_map[new_y][new_x] = 2;
				}
				else {
					short_map[new_y][new_x] = 3;
				}
			}
			dfs(new_x, new_y, ex, ey, row, col);
		}
	}
}

void short_road(int sx, int sy, int ex, int ey) {		//최단 경로
	int new_x = ex + solve_maze[short_map[ey][ex]][0];
	int new_y = ey + solve_maze[short_map[ey][ex]][1];

	map[new_y][new_x] = '7';
	if ((ex != sx) || (ey != sy)) short_road(sx, sy, new_x, new_y);
}

int main() {
	srand((unsigned int)time(NULL));
	int row, col;
	int sx, sy, ex, ey;
	cin >> row >> col;

	for (int i = 0; i < col * 2 + 1; i++) {
		for (int j = 0; j < row * 2 + 1; j++) {
			map[i][j] = '1';
		}
	}

	int random_x = (rand() % row) * 2 + 1;
	int random_y = (rand() % col) * 2 + 1;
	map[random_y][random_x] = '0';
	maze_generate(random_x, random_y, row, col);

	Print(row, col);

	cin >> sx >> sy >> ex >> ey;
	map[sy * 2 - 1][sx * 2 - 1] = '2';
	check = 0;
	
	//dfs(sx * 2 - 1, sy * 2 - 1, ex * 2 - 1, ey * 2 - 1, row, col);
	bfs(sx * 2 - 1, sy * 2 - 1, ex * 2 - 1, ey * 2 - 1, row, col);
	Print(row, col);

	short_road(sx * 2 - 1, sy * 2 - 1, ex * 2 - 1, ey * 2 - 1);
	Print(row, col);

	return 0;
}