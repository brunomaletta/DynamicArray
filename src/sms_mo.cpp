#pragma once

#include <vector>
#include <type_traits>

// DS to store a set/multiset of non-negative integer keys
// Supports split and merge at cost O(log(N)) amortized,
// where N = largest element in the set
// Uses memory linear on the number of distinct elements
template<typename T, bool MULTI=false, typename SIZE_T=int> struct sms_mo {
	static_assert(std::is_integral<T>::value, "Not an integral type");
	static_assert(std::is_integral<SIZE_T>::value, "Not an integral type");
	struct node {
		node* ch[2];
		SIZE_T cnt;
		char d;
		T v, mi;
		node(int d_, T v_, SIZE_T cnt_) : cnt(cnt_), d(d_), v(v_), mi(v) {
			ch[0] = ch[1] = NULL;
		}
		node(node* x, bool b=true) :
				cnt(x->cnt), d(x->d), v(x->v), mi(x->mi) {
			if (b) ch[0] = x->ch[0], ch[1] = x->ch[1];
			else ch[0] = ch[1] = NULL;
		}
		void update() {
			if (!ch[0] ^ !ch[1]) { // remove current if degree is 1
				for (int i = 0; i < 2; i++) if (ch[i]) {
					node* tmp = ch[i];
					ch[0] = tmp->ch[0], ch[1] = tmp->ch[1];
					d = tmp->d, v = tmp->v;
					cnt = tmp->cnt, mi = tmp->mi;
					delete tmp;
					return;
				}
			}
			cnt = 0;
			mi = std::numeric_limits<T>::max();
			for (int i = 0; i < 2; i++) if (ch[i]) {
				cnt += ch[i]->cnt;
				mi = std::min(mi, ch[i]->mi);
			}
		}
	};

	node* root;
	T N;
	char n;

	sms_mo() : root(NULL), N(0), n(0) {}
	sms_mo(T v) : sms_mo() { while (v>>n) n++, N = 2*N+1; }
	sms_mo(const sms_mo& t) : root(NULL), N(t.N), n(t.n) {
		if (!t.root) return;
		root = new node(t.root, false);
		std::vector<std::pair<node*, node*>> q = {{root, t.root}};
		while (q.size()) {
			auto [x, y] = q.back(); q.pop_back();
			for (int i = 0; i < 2; i++) if (y->ch[i]) {
				x->ch[i] = new node(y->ch[i], false);
				q.emplace_back(x->ch[i], y->ch[i]);
			}
		}
	}
	sms_mo(std::initializer_list<T> v) : sms_mo() { for (T i : v) insert(i); }
	~sms_mo() {
		std::vector<node*> q = {root};
		while (q.size()) {
			node* x = q.back(); q.pop_back();
			if (!x) continue;
			q.push_back(x->ch[0]), q.push_back(x->ch[1]);
			delete x;
		}
	}

	friend void swap(sms_mo& a, sms_mo& b) {
		std::swap(a.root, b.root), std::swap(a.N, b.N), std::swap(a.n, b.n);
	}
	sms_mo& operator =(const sms_mo& v) {
		sms_mo tmp = v;
		swap(tmp, *this);
		return *this;
	}
	SIZE_T size() const { return root ? root->cnt : 0; }
	SIZE_T count(node* x) const { return x ? x->cnt : 0; }
	void clear() {
		sms_mo tmp;
		swap(*this, tmp);
	}
	void expand(T v) { while (v>>n) n++, N = 2*N+1; }
	std::vector<T> to_vector() {
		std::vector<T> ret;
		std::vector<node*> q = {root};
		while (q.size()) {
			node* x = q.back(); q.pop_back();
			if (!x) continue;
			if (x->d == -1) for (int i = 0; i < x->cnt; i++) ret.push_back(x->v);
			else q.push_back(x->ch[1]), q.push_back(x->ch[0]);
		}
		return ret;
	}

	char msb(T v, char l, char r) { // msb in range (l, r]
		for (char i = r; i > l; i--) if (v>>i&1) return i;
		return -1;
	}
	void cut(node* at, T v, char i, node* prev = NULL) {
		char d = msb(v ^ at->v, prev ? std::max(at->d, prev->d) : at->d, i);
		if (d == -1) { // no need to split
			if (!prev or prev->d <= at->d) return;
			d = prev->d, v = ~at->v;
		}
		node* nxt = new node(at);
		at->ch[v>>d&1] = NULL;
		at->ch[!(v>>d&1)] = nxt;
		at->d = d;
	}

	node* insert(node* at, T v, SIZE_T qt, char i) {
		if (!at) return new node(-1, v, qt);
		cut(at, v, i);
		if (at->d == -1) { // leaf
			at->cnt += qt;
			at->mi = v;
			if (!MULTI) at->cnt = 1;
			return at;
		}
		bool dir = v>>at->d&1;
		at->ch[dir] = insert(at->ch[dir], v, qt, at->d-1);
		at->update();
		return at;
	}
	void insert(T v, SIZE_T qt=1) { // inserts 'qt' occurrences of 'v'
		if (qt <= 0) return erase(v, -qt);
		expand(v);
		root = insert(root, v, qt, n-1);
	}

	node* erase(node* at, T v, SIZE_T qt, char i) {
		if (!at) return at;
		if (at->d == -1) at->cnt = at->cnt < qt ? 0 : at->cnt - qt;
		else {
			bool dir = v>>at->d&1;
			at->ch[dir] = erase(at->ch[dir], v, qt, i-1);
			at->update();
		}
		if (!at->cnt) delete at, at = NULL;
		return at;
	}
	void erase(T v, SIZE_T qt=1) { // removes 'qt' occurrences of 'v'
		if (v < 0 or v > N or !qt) return;
		if (qt < 0) insert(v, -qt);
		if (!count(v)) return;
		root = erase(root, v, qt, n-1);
	}
	void erase_all(T v) { // remove all occurrences of 'v'
		if (v < 0 or v > N or !count(v)) return;
		root = erase(root, v, std::numeric_limits<SIZE_T>::max(), n-1);
	}

	SIZE_T count(node* at, T a, T b, T l, T r, char i) const {
		if (!at or b < l or r < a) return 0;
		if (a <= l and r <= b) return at->cnt;
		T m = l + (r-l)/2;
		if (at->d < i) {
			if ((at->v>>i&1) == 0) return count(at, a, b, l, m, i-1);
			else return count(at, a, b, m+1, r, i-1);
		}
		return count(at->ch[0], a, b, l, m, i-1) + count(at->ch[1], a, b, m+1, r, i-1);
	}
	SIZE_T count(T v) const { return count(root, v, v, 0, N, n-1); }
	SIZE_T order_of_key(T v) { return count(root, 0, v-1, 0, N, n-1); }
	SIZE_T lower_bound(T v) { return order_of_key(v); }

	const T operator [](SIZE_T k) const { // k-th smallest value
		node* at = root;
		T l = 0, r = N;
		char i = n-1;
		while (l < r) {
			T m = l + (r-l)/2;
			if (at->d < i) {
				if ((at->v>>i&1) == 0) r = m;
				else l = m+1;
				i--;
				continue;
			}
			if (count(at->ch[0]) > k) at = at->ch[0], r = m;
			else {
				k -= count(at->ch[0]);
				at = at->ch[1]; l = m+1;
			}
			i--;
		}
		return l;
	}

	node* merge(node* l, node* r, char i) {
		if (!l or !r) return l ? l : r;
		cut(l, r->v, i, r), cut(r, l->v, i, l);
		if (l->d == -1) { // leaf
			if (MULTI) l->cnt += r->cnt;
			delete r;
			return l;
		}
		for (int j = 0; j < 2; j++)
			l->ch[j] = merge(l->ch[j], r->ch[j], l->d-1);
		l->update(), delete r;
		return l;
	}
	void merge(sms_mo& s) { // merge two sets
		if (N > s.N) swap(*this, s);
		expand(s.N);
		root = merge(root, s.root, n-1);
		s.root = NULL;
	}


	node* split(node*& x, SIZE_T k) {
		if (k <= 0 or !x) return NULL;
		node* ret = new node(x);
		ret->ch[0] = ret->ch[1] = NULL;
		ret->cnt = 0;

		if (x->d == -1) x->cnt -= k, ret->cnt += k, ret->mi = x->mi;
		else {
			if (k <= count(x->ch[0])) ret->ch[0] = split(x->ch[0], k);
			else {
				ret->ch[1] = split(x->ch[1], k - count(x->ch[0]));
				std::swap(x->ch[0], ret->ch[0]);
			}
			ret->update(), x->update();
		}
		if (!x->cnt) delete x, x = NULL;
		return ret;
	}
	void split(SIZE_T k, sms_mo& s) { // take the 'k' smallests elements
		s.clear();
		s.root = split(root, std::min(k, size()));
		s.N = N, s.n = n;
	}
	// take the elements smaller than 'k'
	void split_val(T k, sms_mo& s) { split(order_of_key(k), s); }
	// get minimum
	T min() { return root->mi; };
};
