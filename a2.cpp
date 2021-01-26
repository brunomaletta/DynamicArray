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

int main() { _
	int n, q; cin >> n >> q;
	vector<int> v;
	for (int i = 0; i < n; i++) {
		int x; cin >> x;
		v.push_back(x);
	}
	while (q--) {
		char c; cin >> c;
		if (c == '?') {
			int idx; cin >> idx;
			cout << v[idx] << endl;
		} else if (c == '<') {
			int l, r; cin >> l >> r;
			sort(v.begin()+l, v.begin()+r+1);
		} else if (c == '>') {
			int l, r; cin >> l >> r;
			sort(v.begin()+l, v.begin()+r+1);
			reverse(v.begin()+l, v.begin()+r+1);
		} else if (c == '~') {
			int l, r; cin >> l >> r;
			reverse(v.begin()+l, v.begin()+r+1);
		}
	}
	exit(0);
}

