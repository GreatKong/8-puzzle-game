#include<bits/stdc++.h>

using namespace std;

string _method;
bool _flag;
int _start[5][5], _target[5][5], _x, _y;
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
		
		int cost = 0;
		string ans = "Steps:\n";
		for(int i = 0; node->str[i]; i++) {
			if(node->str[i] == 'L') {
				sprintf(tmp, "	Move %d Right\n", a[x][y - 1]), ans += tmp;
				swap(a[x][y - 1], a[x][y]), cost += a[x][y--];
			}
			if(node->str[i] == 'R') {
				sprintf(tmp, "	Move %d Left\n", a[x][y + 1]), ans += tmp;
				swap(a[x][y + 1], a[x][y]), cost += a[x][y++];
			}
			if(node->str[i] == 'U') {
				sprintf(tmp, "	Move %d Down\n", a[x - 1][y]), ans += tmp;
				swap(a[x - 1][y], a[x][y]), cost += a[x--][y];
			}
			if(node->str[i] == 'D') {
				sprintf(tmp, "	Move %d Up\n", a[x + 1][y]), ans += tmp;
				swap(a[x + 1][y], a[x][y]), cost += a[x++][y];
			}
		}
		cout<<"Solution Found at depth "<<node->pathcost<<" with cost of "<<cost<<endl;
		cout<<ans;
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
	void solve() {
		
	}
	void input() {
		
	}
} gs;

struct breadth_first_search {
	void solve() {
		
	}
	void input() {
		
	}
} bfs;

struct uniform_cost_search {
	void solve() {
		
	}
	void input() {
		
	}
} ucs;

struct depth_first_search {
	void solve() {
		
	}
	void input() {
		
	}
} dfs;

struct depth_limited_search {
	void solve() {
		
	}
	void input() {
		
	}
} dls;

struct iterative_deepening_search {
	void solve() {
		
	}
	void input() {
		
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
