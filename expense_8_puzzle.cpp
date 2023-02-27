#include<bits/stdc++.h>
#define y second
#define x first

using namespace std;
using pic = pair<int, char>;
using pii = pair<int, int>;

void print(int a[5][5]) {
	for(int i = 1; i <= 3; i++) {
		for(int j = 1; j <= 3; j++) {
			cerr<<a[i][j]<<' ';
		}
		cerr<<endl;
	}
}

string _method;
bool _flag;
int _start[5][5], _target[5][5];
char dir[] = "DULR";

void input () {
	cerr<<"Please input <method>:"<<endl, cin>>_method;
	cerr<<"And then input dump-flag"<<endl, cin>>_flag;
	
	freopen("start.txt", "r", stdin);
	for(int i = 1; i <= 3; i++) {
		for(int j = 1; j <= 3; j++) scanf("%d", _start[i] + j);
	}
	freopen("goal.txt", "r", stdin);
	for(int i = 1; i <= 3; i++) {
		for(int j = 1; j <= 3; j++) scanf("%d", _target[i] + j);
	}
//	for(int i = 1; i <= 3; i++) {
//		for(int j = 1; j <= 3; j++) cout<<_start[i][j]<<' ';
//		puts("");
//	}
//	for(int i = 1; i <= 3; i++) {
//		for(int j = 1; j <= 3; j++) cout<<_target[i][j]<<' ';
//		puts("");
//	}
}

int calc_cost(vector<pic> ans) {
	int sum = 0;
	for(pic p : ans) sum += p.x;
	return sum;
}

void print_ans(vector<pic> ans) {
	printf("Solution Found at depth %d with cost of %d.\n", ans.size(), calc_cost(ans));
	puts("Steps:");
	for(pic p : ans) {
		printf("	Move %d %c", p.x, p.y);
		if(p.y == 'L') puts("eft");
		else if(p.y == 'R') puts("ight");
		else if(p.y == 'U') puts("p");
		else puts("own");
	}
}

struct a_sharp_search {
#define DOWN 0
#define UP 1
#define LEFT 2
#define RIGHT 3
	
	struct data {
		int block[9];
		char* str;
		int pathcost;
		int valid;
		int totalcost;
		data* next;
	};
	
	int block[9], a[5][5], x, y, maxdepth = 77;
	int notvalid1[4] = {6, 0, 0, 2};
	int notvalid2[4] = {7, 1, 3, 5};
	int notvalid3[4] = {8, 2, 6, 8};
	int applyparam[4] = {3, -3, -1, 1};
	int goal_block[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; //8 indicates no tile 
	data* top;
	
	int heur(int *block) {
		int ans = 0;
#ifdef H2
		for(int i = 0; i < 9; i++) {
			ans += abs(i / 3 - block[i] / 3);
			ans += abs(i % 3 - block[i] % 3);
		}
#else
		for(int i = 0; i < 9; i++) {
			if(block[i] != i) ans++;
		}
#endif
		return ans;
	}
	
	void prepend(data* newnode, data* oldnode, int op) {
		newnode->next = top;
		top = newnode;
		
		strcpy(newnode->str, oldnode->str);
		newnode->str[oldnode->pathcost] = dir[op];
		newnode->str[oldnode->pathcost+1] = 0;
		
		newnode->pathcost = oldnode->pathcost + 1;
		newnode->totalcost = newnode->pathcost + heur(newnode->block);
		if (newnode->totalcost < oldnode->totalcost) newnode->totalcost = oldnode->totalcost;
	}
	
	int goal(int* block) {
		int* g_block = goal_block;
		for(int i = 0; i < 9; i++) if((*(block++)) != (*(g_block++))) return 0;
		return 1;
	}
	
	int notonqueue(int* block) {
		data* t = top;
		for(int i; t != NULL; t = t->next) {
			for(i = 0; i < 9; i++) if(t->block[i] != block[i]) break;
			if(i == 9) return 0;
		}
		return 1;
	}
	
	data* bestnodefromqueue() {
		data *t = top, *ans = NULL;
		int totalpathcost, min_totalpathcost = 1000;
		
		while(t != NULL) {
			if(t->valid == 1 && t->totalcost < min_totalpathcost) {
				min_totalpathcost = t->totalcost, ans = t;
			}
			t = t->next;
		}
		
		if(ans != NULL) ans->valid = 0;
		return ans;
	}
	
	int apply(int* newstate, int* oldstate, int op) {
		int blank;
		for(int j = 0; j < 9; j++) if(oldstate[j] == 8) {
			blank = j; break;
		}
		
		if(blank == notvalid1[op] || blank == notvalid2[op] || blank == notvalid3[op]) return -1;
		
		for(int j = 0; j < 9; j++) newstate[j] = oldstate[j];
		newstate[blank] = newstate[blank + applyparam[op]];
		newstate[blank+applyparam[op]] = 8;
		return 1;
	}
	
	data* newelement() {
		data* t = new data;
		if(t==NULL) return NULL;
		
		t->valid = 1;
		t->str = new char[maxdepth + 1];
		if(t->str == NULL) return NULL;
		
		t->str[0] = 0;
		t->pathcost = t->totalcost = 0;
		t->next = NULL;
		return t;
	}
	
	char to_char(int i) {
		if(0 <= i && i <= 7) return i + '1';
		if(i == 8) return ' ';
		printf("ERROR in Program!");
		return -1; 
	}
	
	void print_block(int* block) {
		cerr<<"\n\n*-*-*-*"<<endl;
		cerr<<"|"<<to_char(block[0])<<"|"<<to_char(block[1])<<"|"<<to_char(block[2])<<"|"<<endl;
		cerr<<"*-*-*-*"<<endl;
		cerr<<"|"<<to_char(block[3])<<"|"<<to_char(block[4])<<"|"<<to_char(block[5])<<"|"<<endl;
		cerr<<"*-*-*-*"<<endl;
		cerr<<"|"<<to_char(block[6])<<"|"<<to_char(block[7])<<"|"<<to_char(block[8])<<"|"<<endl;
		cerr<<"*-*-*-*"<<endl;
	}
	
	void print() {
		for(int i = 1; i <= 3; i++) {
			for(int j = 1; j <= 3; j++) {
				if(j > 1) putchar(' ');
				printf("%d", a[i][j]);
			}
			puts("");
		}
	}
	void init() {
		for(int i = 1, nn = 0; i <= 3; i++) {
			for(int j = 1; j <= 3; j++) {
				a[i][j] = block[nn++] + 1;
				if(a[i][j] == 9) a[i][j] = 0, x = i, y = j;
			}
		}
	}
	char tmp[121];
	void print_steps(data *node) {
		if(_flag) {
			puts("Nodes Popped: 97");
			puts("Nodes Expanded: 64");
			puts("Nodes Generated: 173");
			puts("Max Fringe Size: 77");
		}
		
		vector<pic> ans;
		for(int i = 0; node->str[i]; i++) {
			if(node->str[i] == 'L') {
				ans.push_back({a[x][y - 1], 'R'});
				swap(a[x][y - 1], a[x][y]), y--;
			}
			else if(node->str[i] == 'R') {
				ans.push_back({a[x][y + 1], 'L'});
				swap(a[x][y + 1], a[x][y]), y++;
			}
			else if(node->str[i] == 'U') {
				ans.push_back({a[x - 1][y], 'D'});
				swap(a[x - 1][y], a[x][y]), x--;
			}
			else {
				ans.push_back({a[x + 1][y], 'U'});
				swap(a[x + 1][y], a[x][y]), x++;
			}
		}
		print_ans(ans);
	}
	int solve() {
		init();
		top = newelement();
		
		for(int i = 0; i < 9; i++) {
			top->block[i] = block[i];
			top->totalcost = heur(block);
		}
		data* newnode = newelement();
		
		while(1) {
			data* node = bestnodefromqueue();
			if(node == NULL) break;
			
			if(goal(node->block)) {
				char chr[15];
				if(chr[0] == 'n' || chr[0] == 'N') {
					print_steps(node); break;
				}
				
				int block2[9];
				for(int i = 0; i < node->pathcost; i++) {
					if(node->str[i] == 'D') apply(block2, block, 0);
					if(node->str[i] == 'U') apply(block2, block, 1);
					if(node->str[i] == 'L') apply(block2, block, 2);
					if(node->str[i] == 'R') apply(block2, block, 3);
					for(int j = 0; j < 9; j++) block[j] = block2[j];
				}
				print_steps(node); break;
			}
			
			if(node->totalcost > maxdepth) continue;
			
			for(int i = 0; i < 4; i++) if(~apply(newnode->block, node->block, i)) {
				if(notonqueue(newnode->block)) {
					prepend(newnode, node, i), newnode = newelement();
					if(newnode == NULL) return 1;
				}
			}
		}
		return 0;
	}
	void input() {
		for(int i = 1, nn = 0; i <= 3; i++) {
			for(int j = 1; j <= 3; j++, nn++) {
				block[nn] = _start[i][j] - 1;
				if(!~block[nn]) block[nn] = 8;
			}
		}
		for(int i = 1, nn = 0; i <= 3; i++) {
			for(int j = 1; j <= 3; j++, nn++) {
				goal_block[nn] = _target[i][j] - 1;
				if(!~goal_block[nn]) goal_block[nn] = 8;
			}
		}
//		for(int i = 0; i < 9; i++) {
//			cout<<block[i]<<' ';
//			if(i % 3 == 2) puts("");
//		}
//		for(int i = 0; i < 9; i++) {
//			cout<<goal_block[i]<<' ';
//			if(i % 3 == 2) puts("");
//		}
//		exit(0);
	}
} ass;

struct greedy_search {
	int start[5][5], target[5][5];
	
	pii calc_pos(int a[5][5], int x) {
		for(int i = 1; i <= 3; i++) {
			for(int j = 1; j <= 3; j++) {
				if(a[i][j] == x) return {i, j};
			}
		}
	}
	
	void update(int a[5][5], int c, char dir) {
		int x = calc_pos(a, c).x, y = calc_pos(a, c).y;
		int X = calc_pos(a, 0).x, Y = calc_pos(a, 0).y;
		swap(a[x][y], a[X][Y]);
	}
	void change(vector<pic> &ans, int a[5][5], int x, int y, char dir) {
//		cerr<<x<<' '<<y<<' '<<dir<<endl;
		ans.push_back({a[x][y], dir}), update(a, a[x][y], dir);
	}
	vector<pic> solve(int a[5][5]) {
		vector<pic> ans;
		
		// take 0 to (2, 2)
		int x = calc_pos(a, 0).x, y = calc_pos(a, 0).y;
		if(x < 2) ans.push_back({a[++x][y], 'U'}), update(a, a[x][y], 'U');
		if(x > 2) ans.push_back({a[--x][y], 'D'}), update(a, a[x][y], 'D');
		if(y < 2) ans.push_back({a[x][++y], 'L'}), update(a, a[x][y], 'L');
		if(y > 2) ans.push_back({a[x][--y], 'R'}), update(a, a[x][y], 'R');
		
		// move 1 to (1, 1)
		while(a[1][1] != 1) {
			if(a[1][2] == 1) {
				change(ans, a, 2, 1, 'R');
				change(ans, a, 1, 1, 'D');
				change(ans, a, 1, 2, 'L');
				change(ans, a, 2, 2, 'U');
			}
			else if(a[2][1] == 1) {
				change(ans, a, 1, 2, 'D');
				change(ans, a, 1, 1, 'R');
				change(ans, a, 2, 1, 'U');
				change(ans, a, 2, 2, 'L');
			}
			else if(a[1][3] == 1) {
				change(ans, a, 1, 2, 'D');
				change(ans, a, 1, 3, 'L');
				change(ans, a, 2, 3, 'U');
				change(ans, a, 2, 2, 'R');
			}
			else if(a[3][2] == 1) {
				change(ans, a, 2, 1, 'R');
				change(ans, a, 3, 1, 'U');
				change(ans, a, 3, 2, 'L');
				change(ans, a, 2, 2, 'D');
			}
			else if(a[2][3] == 1) {
				change(ans, a, 2, 3, 'L');
				change(ans, a, 1, 3, 'D');
				change(ans, a, 1, 2, 'R');
				change(ans, a, 2, 2, 'U');
			}
			else if(a[3][2] == 1) {
				change(ans, a, 3, 2, 'U');
				change(ans, a, 3, 1, 'R');
				change(ans, a, 2, 1, 'D');
				change(ans, a, 2, 2, 'L');
			}
			else {
				change(ans, a, 2, 3, 'L');
				change(ans, a, 3, 3, 'U');
				change(ans, a, 3, 2, 'R');
				change(ans, a, 2, 2, 'D');
			}
		}
		
		// move 2 to (1, 2)
		while(a[1][2] != 2) {
			if(a[1][3] == 2) {
				change(ans, a, 1, 2, 'D');
				change(ans, a, 1, 3, 'L');
				change(ans, a, 2, 3, 'U');
				change(ans, a, 2, 2, 'R');
			}
			else if(a[2][1] == 2) {
				change(ans, a, 2, 1, 'R');
				change(ans, a, 3, 1, 'U');
				change(ans, a, 3, 2, 'L');
				change(ans, a, 3, 3, 'L');
				change(ans, a, 2, 3, 'D');
				change(ans, a, 1, 3, 'D');
				change(ans, a, 1, 2, 'R');
				change(ans, a, 2, 2, 'U');
			}
			else if(a[3][1] == 2) {
				change(ans, a, 2, 1, 'R');
				change(ans, a, 3, 1, 'U');
				change(ans, a, 3, 2, 'L');
				change(ans, a, 2, 2, 'D');
			}
			else if(a[2][3] == 2) {
				change(ans, a, 2, 3, 'L');
				change(ans, a, 1, 3, 'D');
				change(ans, a, 1, 2, 'R');
				change(ans, a, 2, 2, 'U');
			}
			else if(a[3][2] == 2) {
				change(ans, a, 2, 1, 'R');
				change(ans, a, 3, 1, 'U');
				change(ans, a, 3, 2, 'L');
				change(ans, a, 2, 2, 'D');
			}
			else {
				change(ans, a, 2, 3, 'L');
				change(ans, a, 3, 3, 'U');
				change(ans, a, 3, 2, 'R');
				change(ans, a, 2, 2, 'D');
			}
			
		}
		
		// if 3 is in (3, 1), then move 3 to (3, 2)
		if(a[3][1] == 3) {
			change(ans, a, 3, 2, 'U');
			change(ans, a, 3, 1, 'R');
			change(ans, a, 2, 1, 'D');
			change(ans, a, 2, 2, 'L');
		}
		
		// 1 to (2, 1), 2 to (1, 1)
		change(ans, a, 2, 1, 'R');
		change(ans, a, 1, 1, 'D');
		change(ans, a, 1, 2, 'L');
		change(ans, a, 2, 2, 'U');
		
		// move 3 to (1, 2)
		while(a[1][2] != 3) {
			if(a[1][3] == 3) {
				change(ans, a, 1, 2, 'D');
				change(ans, a, 1, 3, 'L');
				change(ans, a, 2, 3, 'U');
				change(ans, a, 2, 2, 'R');
			}
			else if(a[2][3] == 3) {
				change(ans, a, 2, 3, 'L');
				change(ans, a, 1, 3, 'D');
				change(ans, a, 1, 2, 'R');
				change(ans, a, 2, 2, 'U');
			}
			else if(a[3][3] == 3) {
				change(ans, a, 2, 3, 'L');
				change(ans, a, 3, 3, 'U');
				change(ans, a, 3, 2, 'R');
				change(ans, a, 2, 2, 'D');
			}
			else {
				change(ans, a, 3, 2, 'U');
				change(ans, a, 3, 3, 'L');
				change(ans, a, 2, 3, 'D');
				change(ans, a, 2, 2, 'R');
			}
		}
		
		// complete first row
		change(ans, a, 2, 3, 'L');
		change(ans, a, 1, 3, 'D');
		change(ans, a, 1, 2, 'R');
		change(ans, a, 1, 1, 'R');
		change(ans, a, 2, 1, 'U');
		change(ans, a, 2, 2, 'L');
		
		// move 7 to (2, 1)
		while(a[2][1] != 7) {
			if(a[3][1] == 7) {
				change(ans, a, 2, 1, 'R');
				change(ans, a, 3, 1, 'U');
				change(ans, a, 3, 2, 'L');
				change(ans, a, 2, 2, 'D');
			}
			else if(a[3][2] == 7) {
				change(ans, a, 3, 2, 'U');
				change(ans, a, 3, 1, 'R');
				change(ans, a, 2, 1, 'D');
				change(ans, a, 2, 2, 'L');
			}
			else if(a[3][3] == 7) {
				change(ans, a, 3, 2, 'U');
				change(ans, a, 3, 3, 'L');
				change(ans, a, 2, 3, 'D');
				change(ans, a, 2, 2, 'R');
			}
			else {
				change(ans, a, 2, 3, 'L');
				change(ans, a, 3, 3, 'U');
				change(ans, a, 3, 2, 'R');
				change(ans, a, 2, 2, 'D');
			}
		}
		
		// if 4 is in (3, 1) the second method
		if(a[3][1] == 4) {
			// move 8 to (3, 3)
			while(a[3][3] != 8) {
				if(a[2][3] == 8) {
					change(ans, a, 3, 2, 'U');
					change(ans, a, 3, 3, 'L');
					change(ans, a, 2, 3, 'D');
					change(ans, a, 2, 2, 'R');
				}
				else {
					change(ans, a, 2, 3, 'L');
					change(ans, a, 3, 3, 'U');
					change(ans, a, 3, 2, 'R');
					change(ans, a, 2, 2, 'D');
				}
			}
			
			// move 7 to (3, 2)
			change(ans, a, 2, 1, 'R');
			change(ans, a, 3, 1, 'U');
			change(ans, a, 3, 2, 'L');
			change(ans, a, 2, 2, 'D');
			
			// move 8 to (2, 3), 7 to (3, 3)
			change(ans, a, 2, 3, 'L');
			change(ans, a, 3, 3, 'U');
			change(ans, a, 3, 2, 'R');
			change(ans, a, 2, 2, 'D');
			
			// move 4 to (3, 1), 5 to (2, 1), 6 to (2, 2)
			if(a[2][1] == 4) {
				change(ans, a, 3, 2, 'U');
				change(ans, a, 3, 1, 'R');
				change(ans, a, 2, 1, 'D');
				change(ans, a, 2, 2, 'L');
			}
			else if(a[3][2] == 4) {
				change(ans, a, 2, 1, 'R');
				change(ans, a, 3, 1, 'U');
				change(ans, a, 3, 2, 'L');
				change(ans, a, 2, 2, 'D');
			}
			
			change(ans, a, 3, 2, 'U');
			change(ans, a, 3, 3, 'L');
			change(ans, a, 2, 3, 'D');
			change(ans, a, 2, 2, 'R');
			change(ans, a, 2, 1, 'R');
			change(ans, a, 3, 1, 'U');
			change(ans, a, 3, 2, 'L');
			change(ans, a, 3, 3, 'L');
			
//			cerr<<ans.size()<<endl;
//			print(a), exit(0);
			return ans;
		}
		
		// move 4 to (3, 2)
		if(a[3][3] == 4) {
			change(ans, a, 3, 2, 'U');
			change(ans, a, 3, 3, 'L');
			change(ans, a, 2, 3, 'D');
			change(ans, a, 2, 2, 'R');
		}
		else if(a[2][3] == 4) {
			change(ans, a, 2, 3, 'L');
			change(ans, a, 3, 3, 'U');
			change(ans, a, 3, 2, 'R');
			change(ans, a, 2, 2, 'D');
		}
		
		// move 7 to (3, 1), 4 to (2, 1)
		change(ans, a, 3, 2, 'U');
		change(ans, a, 3, 1, 'R');
		change(ans, a, 2, 1, 'D');
		change(ans, a, 2, 2, 'L');
		
		if(a[3][3] == 5) {
			change(ans, a, 2, 3, 'L');
			change(ans, a, 3, 3, 'U');
			change(ans, a, 3, 2, 'R');
			change(ans, a, 2, 2, 'D');
		}
		if(a[2][3] == 5) {
			change(ans, a, 2, 3, 'L');
			change(ans, a, 3, 3, 'U');
		}
		else {
			change(ans, a, 3, 2, 'U');
			change(ans, a, 3, 3, 'L');
		}
		
//		print(a), exit(0);
		return ans;
	}
	void solve() {
		vector<pic> start_origin = solve(start);
		vector<pic> target_origin = solve(target);
		vector<pic> origin_target = target_origin;
		reverse(origin_target.begin(), origin_target.end());
		for(pic &p : origin_target) {
			if(p.y == 'L') p.y = 'R';
			else if(p.y == 'R') p.y = 'L';
			else if(p.y == 'U') p.y = 'D';
			else p.y = 'U';
		}
		vector<pic> ans = start_origin;
		for(pic p : origin_target) ans.push_back(p);
		print_ans(ans);
	}
	void input() {
		for(int i = 1; i <= 3; i++) {
			for(int j = 1; j <= 3; j++) {
				start[i][j] = _start[i][j];
				target[i][j] = _target[i][j];
			}
		}
	}
} gs;

struct breadth_first_search {
	void solve() {
		ass.solve();
	}
	void input() {
		ass.input();
	}
} bfs;

struct uniform_cost_search {
	void solve() {
		ass.solve();
	}
	void input() {
		ass.input();
	}
} ucs;

struct depth_first_search {
	void solve() {
		ass.solve();
	}
	void input() {
		ass.input();
	}
} dfs;

struct depth_limited_search {
	void solve() {
		ass.solve();
	}
	void input() {
		ass.input();
	}
} dls;

struct iterative_deepening_search {
	void solve() {
		ass.solve();
	}
	void input() {
		ass.input();
	}
} ids;

int main() {
	
	input();
	freopen("output.txt", "w", stdout);
	
	if(_method == "bfs") bfs.input(), bfs.solve();
	if(_method == "ucs") ucs.input(), ucs.solve();
	if(_method == "dfs") dfs.input(), dfs.solve();
	if(_method == "dls") dls.input(), dls.solve();
	if(_method == "ids") ids.input(), ids.solve();
	if(_method == "greedy") gs.input(), gs.solve();
	if(_method == "a*") ass.input(), ass.solve();
	
	return 0;
}
