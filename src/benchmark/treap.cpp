#pragma once

#include <random>
#include <chrono>
#include <vector>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

template<typename T> struct treap {
	struct node {
		node *l, *r;
		int p, sz;
		T val, mi;
		node(T v) : l(NULL), r(NULL), p(rng()), sz(1), val(v), mi(v) {}
		void update() {
			sz = 1;
			mi = val;
			if (l) sz += l->sz, mi = std::min(mi, l->mi);
			if (r) sz += r->sz, mi = std::min(mi, r->mi);
		}
	};

	node* root;
	int calls;

	treap() { root = NULL, calls = 0; }
	~treap() {
		std::vector<node*> q = {root};
		while (q.size()) {
			node* x = q.back(); q.pop_back();
			if (!x) continue;
			q.push_back(x->l), q.push_back(x->r);
			delete x;
		}
	}

	friend void swap(treap& a, treap& b) {
		std::swap(a.root, b.root);
	}
	int size(node* x) { return x ? x->sz : 0; }
	int size() { return size(root); }
	void join(node* l, node* r, node*& i) { // assume que l < r
		calls++;
		if (!l or !r) return void(i = l ? l : r);
		if (l->p > r->p) join(l->r, r, l->r), i = l;
		else join(l, r->l, r->l), i = r;
		i->update();
	}
	void split(node* i, node*& l, node*& r, T v) {
		calls++;
		if (!i) return void(r = l = NULL);
		if (i->val < v) split(i->r, i->r, r, v), l = i;
		else split(i->l, l, i->l, v), r = i;
		i->update();
	}
	int count(node* i, T v) {
		if (!i) return 0;
		if (i->val == v) return 1;
		if (v < i->val) return count(i->l, v);
		return count(i->r, v);
	}
	void index_split(node* i, node*& l, node*& r, int v, int key = 0) {
		if (!i) return void(r = l = NULL);
		if (key + size(i->l) < v) index_split(i->r, i->r, r, v, key+size(i->l)+1), l = i;
		else index_split(i->l, l, i->l, v, key), r = i;
		i->update();
	}
	int count(T v) {
		return count(root, v);
	}
	void insert(T v) {
		if (count(v)) return;
		node *L, *R;
		split(root, L, R, v);
		node* at = new node(v);
		join(L, at, L);
		join(L, R, root);
	}
	void erase(T v) {
		node *L, *M, *R;
		split(root, M, R, v+1), split(M, L, M, v);
		if (M) delete M;
		M = NULL;
		join(L, R, root);
	}
	void split(T v, treap& t) {
		node *L, *R;
		split(root, L, R, v);
		t.root = L;
		root = R;
	}
	void merge(treap& t) { // segmented merge
		node *L = root, *R = t.root;
		root = NULL;
		while (L or R) {
			if (!L or (L and R and L->mi > R->mi)) std::swap(L, R);
			if (!R) join(root, L, root), L = NULL;
			else if (L->mi == R->mi) {
				node* LL;
				split(L, LL, L, R->mi+1);
				delete LL;
			} else {
				node* LL;
				split(L, LL, L, R->mi);
				join(root, LL, root);
			}
		}
		t.root = NULL;
	}
};
