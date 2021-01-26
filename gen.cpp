#include <bits/stdc++.h>

using namespace std;

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'
#define f first
#define s second
#define pb push_back

typedef long long ll;
typedef pair<int, int> ii;

const int INF = 0x3f3f3f3f;
const ll LINF = 0x3f3f3f3f3f3f3f3fll;

const int LIM = 1e3;

int main(int argc, char** argv) { _
	srand(atoi(argv[1]));
	int n = rand()%LIM+1, q = rand()%LIM+1;
	cout << n << " " << q << endl;
	for (int i = 0; i < n; i++) cout << rand()%LIM << " ";
	cout << endl;
	while (q--) {
		int t = rand()%4;
		if (t == 0) {
			cout << "? " << rand()%n << endl;
			continue;
		} 
		if (t == 1) cout << "< ";
		if (t == 2) cout << "> ";
		if (t == 3) cout << "~ ";
		int l = rand()%n, r = rand()%n;
		if (l > r) swap(l, r);
		cout << l << " " << r << endl;
	}
	exit(0);
}

