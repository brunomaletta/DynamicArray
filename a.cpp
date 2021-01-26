#include <bits/stdc++.h>

#define _ ios_base::sync_with_stdio(0);cin.tie(0);
#define endl '\n'

using namespace std;

template<typename T, bool MULTI=false, typename SIZE_T=int> struct sms {
	struct node {
		node *l, *r;
		SIZE_T cnt;
		node() : l(NULL), r(NULL), cnt(0) {}
		void update() {
			cnt = 0;
			if (l) cnt += l->cnt;
			if (r) cnt += r->cnt;
		}
	};

	node* root;
	T N;

	sms() : root(NULL), N(0) {}
	sms(T v) : sms() { while (v >= N) N = 2*N+1; }
	sms(const sms& t) : root(NULL), N(t.N) {
		for (SIZE_T i = 0; i < t.size(); i++) {
			T at = t[i];
			SIZE_T qt = t.count(at);
			insert(at, qt);
			i += qt-1;
		}
	}
	sms(initializer_list<T> v) : sms() { for (T i : v) insert(i); }
	~sms() {
		vector<node*> q = {root};
		while (q.size()) {
			node* x = q.back(); q.pop_back();
			if (!x) continue;
			q.push_back(x->l), q.push_back(x->r);
			delete x;
		}
	}

	friend void swap(sms& a, sms& b) {
		swap(a.root, b.root), swap(a.N, b.N);
	}
	SIZE_T size() const { return root ? root->cnt : 0; }
	SIZE_T count(node* x) const { return x ? x->cnt : 0; }
	void clear() {
		sms tmp;
		swap(*this, tmp);
	}
	void expand(T v) {
		for (; N < v; N = 2*N+1) if (root) {
			node* nroot = new node();
			nroot->l = root;
			root = nroot;
			root->update();
		}
	}

	node* insert(node* at, T idx, SIZE_T qt, T l, T r) {
		if (!at) at = new node();
		if (l == r) {
			at->cnt += qt;
			if (!MULTI) at->cnt = 1;
			return at;
		}
		T m = l + (r-l)/2;
		if (idx <= m) at->l = insert(at->l, idx, qt, l, m);
		else at->r = insert(at->r, idx, qt, m+1, r);
		return at->update(), at;
	}
	void insert(T v, SIZE_T qt=1) { // insere 'qt' ocorrencias de 'v'
		if (qt <= 0) return erase(v, -qt);
		assert(v >= 0);
		expand(v);
		root = insert(root, v, qt, 0, N);
	}

	node* erase(node* at, T idx, SIZE_T qt, T l, T r) {
		if (!at) return at;
		if (l == r) at->cnt = at->cnt < qt ? 0 : at->cnt - qt;
		else {
			T m = l + (r-l)/2;
			if (idx <= m) at->l = erase(at->l, idx, qt, l, m);
			else at->r = erase(at->r, idx, qt, m+1, r);
			at->update();
		}
		if (!at->cnt) delete at, at = NULL;
		return at;
	}
	void erase(T v, SIZE_T qt=1) { // remove 'qt' ocorrencias de 'v'
		if (v < 0 or v > N or !qt) return;
		if (qt < 0) insert(v, -qt);
		root = erase(root, v, qt, 0, N);
	}
	void erase_all(T v) { // remove todos os 'v'
		if (v < 0 or v > N) return;
		root = erase(root, v, numeric_limits<SIZE_T>::max(), 0, N);
	}

	SIZE_T count(node* at, T a, T b, T l, T r) const {
		if (!at or b < l or r < a) return 0;
		if (a <= l and r <= b) return at->cnt;
		T m = l + (r-l)/2;
		return count(at->l, a, b, l, m) + count(at->r, a, b, m+1, r);
	}
	SIZE_T count(T v) const { return count(root, v, v, 0, N); }
	SIZE_T order_of_key(T v) { return count(root, 0, v-1, 0, N); }
	SIZE_T lower_bound(T v) { return order_of_key(v); }

	const T operator [](SIZE_T i) const { // i-esimo menor elemento
		assert(i >= 0 and i < size());
		node* at = root;
		T l = 0, r = N;
		while (l < r) {
			T m = l + (r-l)/2;
			if (count(at->l) > i) at = at->l, r = m;
			else {
				i -= count(at->l);
				at = at->r; l = m+1;
			}
		}
		return l;
	}

	node* merge(node* l, node* r) {
		if (!l or !r) return l ? l : r;
		if (!l->l and !l->r) { // folha
			if (MULTI) l->cnt += r->cnt;
			delete r;
			return l;
		}
		l->l = merge(l->l, r->l), l->r = merge(l->r, r->r);
		l->update(), delete r;
		return l;
	}
	void merge(sms& s) { // mergeia dois sets
		if (N > s.N) swap(*this, s);
		expand(s.N);
		root = merge(root, s.root);
		s.root = NULL;
	}

	node* split(node*& x, SIZE_T k) {
		if (k <= 0 or !x) return NULL;
		node* ret = new node();
		if (!x->l and !x->r) x->cnt -= k, ret->cnt += k;
		else {
			if (k <= count(x->l)) ret->l = split(x->l, k);
			else {
				ret->r = split(x->r, k - count(x->l));
				swap(x->l, ret->l);
			}
			ret->update(), x->update();
		}
		if (!x->cnt) delete x, x = NULL;
		return ret;
	}
	void split(SIZE_T k, sms& s) { // pega os 'k' menores
		s.clear();
		s.root = split(root, min(k, size()));
		s.N = N;
	}
	// pega os menores que 'k'
	void split_val(T k, sms& s) { split(order_of_key(k), s); }
};

mt19937 rng((int) chrono::steady_clock::now().time_since_epoch().count());

template<typename T, typename SIZE_T = int> struct dyn_array {
	struct node {
		node *l, *r;
		int p;
		SIZE_T sz;
		bool rev, rev_lazy;
		sms<T, true, SIZE_T> val;
		node(sms<T, true, SIZE_T>& v) :
				l(NULL), r(NULL), p(rng()), sz(1), rev(0), rev_lazy(0) {
			swap(val, v);
		}
		void prop() {
			if (rev_lazy) {
				rev ^= 1;
				swap(l, r);
				if (l) l->rev_lazy ^= 1;
				if (r) r->rev_lazy ^= 1;
			}
			rev_lazy = false;
		}
		void update() {
			sz = val.size();
			if (l) sz += l->sz;
			if (r) sz += r->sz;
		}
	};

	node* root;

	dyn_array() { root = NULL; }
	dyn_array(const dyn_array& v) : root(NULL) {
		for (int i = 0; i < v.size(); i++)
			push_back(v[i]);
	}
	~dyn_array() {
		vector<node*> q = {root};
		while (q.size()) {
			node* x = q.back(); q.pop_back();
			if (!x) continue;
			q.push_back(x->l), q.push_back(x->r);
			delete x;
		}
	}

	friend void swap(dyn_array& a, dyn_array& b) {
		swap(a.root, b.root);
	}
	void clear() {
		dyn_array<T> tmp;
		swap(tmp, *this);
	}
	SIZE_T size(node* x) const { return x ? x->sz : 0; }
	SIZE_T size() const { return size(root); }
	void join(node* l, node* r, node*& i) { // assume que l < r
		if (!l or !r) return void(i = l ? l : r);
		l->prop(), r->prop();
		if (l->p > r->p) join(l->r, r, l->r), i = l;
		else join(l, r->l, r->l), i = r;
		i->update();
	}
	void concat(dyn_array& v) {
		join(root, v.root, root);
		v.root = NULL;
	}
	void split(node* i, node*& l, node*& r, SIZE_T idx) {
		if (!i) return void(r = l = NULL);
		i->prop();
		if (size(i->l) + i->val.size() <= idx) { // pega o 'i' e toda a sub-arvore da esquerda
			split(i->r, i->r, r, idx - size(i->l) - i->val.size());
			l = i;
		} else if (size(i->l) >= idx) { // vai pra esquerda
			split(i->l, l, i->l, idx);
			r = i;
		} else { // tem que quebrar esse no e pega a sub-arvore da esquerda toda
			sms<T, true> L;
			if (!i->rev) i->val.split(idx - size(i->l), L);
			else {
				i->val.split(i->val.size() - (idx - size(i->l)), L);
				swap(L, i->val);
			}
			node* novo = new node(L);
			novo->rev = i->rev;
			novo->l = i->l;
			i->l = NULL;
			novo->update();
			l = novo, r = i;
		}
		i->update();
	}
	void split(SIZE_T k, dyn_array& v) { // pega os 'k' menores
		v.clear();
		split(root, v.root, root, min(k, size()));
	}
	void push_back(T val, SIZE_T qt = 1) {
		sms<T, true> v;
		v.insert(val, qt);
		node* i = new node(v);
		join(root, i, root);
	}

	T get(node* i, SIZE_T idx) const {
		i->prop();
		if (size(i->l) + i->val.size() <= idx)
			return get(i->r, idx - size(i->l) - i->val.size());
		if (size(i->l) > idx) return get(i->l, idx);

		if (!i->rev) return i->val[idx - size(i->l)];
		return i->val[i->val.size() - 1 - (idx - size(i->l))];
	}
	const T operator[](SIZE_T i) const { return get(root, i); }

	void merge(node*& i, sms<T, true, SIZE_T>& v) {
		if (!i) return;
		i->prop();
		merge(i->l, v), merge(i->r, v);
		v.merge(i->val);
		delete i;
		i = NULL;
	}
	void sort(bool reverse = false) {
		if (!root) return;
		root->prop();
		merge(root->l, root->val);
		merge(root->r, root->val);
		root->rev = reverse;
		root->update();
	}
	void sort(int l, int r, bool reverse = false) {
		dyn_array L, M;
		split(r+1, M);
		M.split(l, L);
		M.sort(reverse);
		L.concat(M);
		L.concat(*this);
		swap(*this, L);
	}
	void reverse() { if (root) root->rev_lazy ^= 1; }
	void reverse(int l, int r) {
		dyn_array L, M;
		split(r+1, M);
		M.split(l, L);
		M.reverse();
		L.concat(M);
		L.concat(*this);
		swap(*this, L);
	}
};

int main() { _
	int n, q; cin >> n >> q;
	dyn_array<int> v;
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
			v.sort(l, r);
		} else if (c == '>') {
			int l, r; cin >> l >> r;
			v.sort(l, r, true);
		} else if (c == '~') {
			int l, r; cin >> l >> r;
			v.reverse(l, r);
		}
	}
	exit(0);
}
