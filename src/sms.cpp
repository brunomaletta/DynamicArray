#pragma once

#include <vector>
#include <type_traits>

// DS to store a set/multiset of non-negative integer keys
// Supports split and merge at cost O(log(N)) amortized,
// where N = largest element in the set
template<typename T, bool MULTI=false, typename SIZE_T=int> struct sms {
	static_assert(std::is_integral<T>::value, "Not an integral type");
	static_assert(std::is_integral<SIZE_T>::value, "Not an integral type");
	struct node {
		node *l, *r;
		SIZE_T cnt;
		T mi;
		node() : l(NULL), r(NULL), cnt(0),
				mi(std::numeric_limits<T>::max()) {}
		void update() {
			cnt = 0;
			mi = std::numeric_limits<T>::max();
			if (l) cnt += l->cnt, mi = std::min(mi, l->mi);
			if (r) cnt += r->cnt, mi = std::min(mi, r->mi);
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
	sms(std::initializer_list<T> v) : sms() { for (T i : v) insert(i); }
	~sms() {
		std::vector<node*> q = {root};
		while (q.size()) {
			node* x = q.back(); q.pop_back();
			if (!x) continue;
			q.push_back(x->l), q.push_back(x->r);
			delete x;
		}
	}

	friend void swap(sms& a, sms& b) {
		std::swap(a.root, b.root), std::swap(a.N, b.N);
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
			at->mi = l;
			if (!MULTI) at->cnt = 1;
			return at;
		}
		T m = l + (r-l)/2;
		if (idx <= m) at->l = insert(at->l, idx, qt, l, m);
		else at->r = insert(at->r, idx, qt, m+1, r);
		return at->update(), at;
	}
	void insert(T v, SIZE_T qt=1) { // inserts 'qt' occurrences of 'v'
		if (qt <= 0) return erase(v, -qt);
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
	void erase(T v, SIZE_T qt=1) { // removes 'qt' occurrences of 'v'
		if (v < 0 or v > N or !qt) return;
		if (qt < 0) insert(v, -qt);
		root = erase(root, v, qt, 0, N);
	}
	void erase_all(T v) { // remove all occurrences of 'v'
		if (v < 0 or v > N) return;
		root = erase(root, v, std::numeric_limits<SIZE_T>::max(), 0, N);
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

	const T operator [](SIZE_T i) const { // i-th smallest value
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
		if (!l->l and !l->r) { // leaf
			if (MULTI) l->cnt += r->cnt;
			delete r;
			return l;
		}
		l->l = merge(l->l, r->l), l->r = merge(l->r, r->r);
		l->update(), delete r;
		return l;
	}
	void merge(sms& s) { // merge two sets
		if (N > s.N) swap(*this, s);
		expand(s.N);
		root = merge(root, s.root);
		s.root = NULL;
	}

	node* split(node*& x, SIZE_T k) {
		if (k <= 0 or !x) return NULL;
		node* ret = new node();
		if (!x->l and !x->r) x->cnt -= k, ret->cnt += k, ret->mi = x->mi;
		else {
			if (k <= count(x->l)) ret->l = split(x->l, k);
			else {
				ret->r = split(x->r, k - count(x->l));
				std::swap(x->l, ret->l);
			}
			ret->update(), x->update();
		}
		if (!x->cnt) delete x, x = NULL;
		return ret;
	}
	void split(SIZE_T k, sms& s) { // take the 'k' smallests elements 
		s.clear();
		s.root = split(root, std::min(k, size()));
		s.N = N;
	}
	// take the elements smaller than 'k'
	void split_val(T k, sms& s) { split(order_of_key(k), s); }
	// get minimum
	T min() { return root->mi; };
};
