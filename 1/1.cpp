#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <set>
#include <map>
using namespace std;

struct info {
	int r, c, live;

	info(int aa, int bb, int cc) {
		r = aa;
		c = bb;
		live = cc;
	}
};

int n, m, t, min_dist;
int dr[] = { -1, 0, 0, 1 };
int dc[] = { 0, -1, 1, 0 };
int camp[15][15], des[15][15];
int check_b[15][15], check_d[15][15], find_route[15][15];
vector<info> person;
vector<pair<int, int> > camp_list, store_list;

void check_clear() {
	min_dist = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			check_b[i][j] = 0;
			check_d[i][j] = 0;
			find_route[i][j] = 0;
		}
	}
}

void backtrack(int r, int c, int cnt, int idx) {
	if (cnt > min_dist) {
		return;
	}

	if (r == store_list[idx].first && c == store_list[idx].second) {
		for (int a = 0; a < n; a++) {
			for (int b = 0; b < n; b++) {
				if (check_d[a][b] == 1) {
					find_route[a][b] = check_d[a][b];
				}
			}
		}

		return;
	}

	else {
		for (int i = 0; i < 4; i++) {
			int nr = r + dr[i];
			int nc = c + dc[i];

			if (nr < 0 || nr >= n || nc < 0 || nc >= n || camp[nr][nc] == -1 || des[nr][nc] == -1 || check_d[nr][nc] != 0) {
				continue;
			}

			else {
				if (check_d[nr][nc] == 0 || camp[nr][nc] != -1 || des[nr][nc] != -1) {
					check_d[nr][nc] = 1;
					backtrack(nr, nc, cnt + 1, idx);
					check_d[nr][nc] = 0;
				}
			}
		}
	}

	
}

pair<int, int> search_route(int r, int c, int idx) {
	check_clear();
	pair<int, int> res;
	queue<pair<int, int> > q;

	check_b[r][c] = 1;
	q.push(make_pair(r, c));

	while (!q.empty()) {
		pair<int, int> temp;
		temp = q.front();
		q.pop();

		for (int i = 0; i < 4; i++) {
			int nr = temp.first + dr[i];
			int nc = temp.second + dc[i];

			if (nr < 0 || nr >= n || nc < 0 || nc >= n || camp[nr][nc] == -1 || des[nr][nc] == -1 || check_b[nr][nc] != 0) {
				continue;
			}

			else {
				if (check_b[nr][nc] == 0 || camp[nr][nc] != -1 || des[nr][nc] != -1) {
					check_b[nr][nc] = check_b[temp.first][temp.second] + 1;
					q.push(make_pair(nr, nc));
				}
			}
		}
	}

	min_dist = check_b[store_list[idx].first][store_list[idx].second] - 1;

	check_d[r][c] = 1;
	backtrack(r, c, 0, idx);

	for (int i = 0; i < 4; i++) {
		int nr = r + dr[i];
		int nc = c + dc[i];

		if (nr < 0 || nr >= n || nc < 0 || nc >= n) {
			continue;
		}

		else {
			if (find_route[nr][nc] == 1) {
				res.first = nr;
				res.second = nc;
				break;
			}
		}
	}

	return res;
}

pair<int, int> search_camp(int r, int c) {
	int check[15][15] = { 0, };
	pair<int, int> res;
	queue<pair<int, int> > q;

	check[r][c] = 1;
	q.push(make_pair(r, c));
	
	while (!q.empty()) {
		pair<int, int> temp;
		temp = q.front();
		q.pop();

		for (int i = 0; i < 4; i++) {
			int nr = temp.first + dr[i];
			int nc = temp.second + dc[i];
			
			if (nr < 0 || nr >= n || nc < 0 || nc >= n || camp[nr][nc] == -1 || des[nr][nc] == -1 || check[nr][nc] != 0) {
				
				continue;
			}

			else {
				if (check[nr][nc] == 0 || camp[nr][nc] != -1 || des[nr][nc] != -1) {
				
					check[nr][nc] = check[temp.first][temp.second] + 1;
					q.push(make_pair(nr, nc));
				}
			}
		}
	}

	min_dist = 2147000000;
	
	for (int i = 0; i < camp_list.size(); i++) {
		if (camp[camp_list[i].first][camp_list[i].second] != -1 && check[camp_list[i].first][camp_list[i].second] > 0) {
			min_dist = min(check[camp_list[i].first][camp_list[i].second], min_dist);
		}
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (check[i][j] == min_dist && camp[i][j] == 1) {
				
				res = make_pair(i, j);
				return res;
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	freopen("input.txt", "rt", stdin);

	cin >> n >> m;
	

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> camp[i][j];

			if (camp[i][j] == 1) {
				camp_list.push_back(make_pair(i, j));
			}
		}
	}

	for (int i = 0; i < m; i++) {
		int r, c;
		cin >> r >> c;
		des[r - 1][c-1] = i + 1;
		store_list.push_back(make_pair(r - 1, c - 1));
	}

	while (1) {
		t++;

		for (int i = 0; i < person.size(); i++) {
			if (person[i].live == 1) {
				pair<int, int> move_idx;
				
				move_idx = search_route(person[i].r, person[i].c, i);
				person[i].r = move_idx.first;
				person[i].c = move_idx.second;
			}
		}
		
		for (int i = 0; i < person.size(); i++) {
			int temp_r = person[i].r;
			int temp_c = person[i].c;

			if (temp_r == store_list[i].first && temp_c == store_list[i].second) {
				des[temp_r][temp_c] = -1;
				person[i].live = 0;
			}
		}

		if (t <= m) {
			pair<int, int> camp_idx;
			camp_idx = search_camp(store_list[t - 1].first, store_list[t - 1].second);

			camp[camp_idx.first][camp_idx.second] = -1;
			person.push_back(info(camp_idx.first, camp_idx.second, 1));
		}


		int sum = 0;

		for (int i = 0; i < person.size(); i++) {
			sum += person[i].live;
		}

		if (sum == 0) {
			break;
		}
	}
	
	cout << t << "\n";
	return 0;
}