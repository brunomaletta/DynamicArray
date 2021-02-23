#pragma once

#include <vector>

#include <iostream>

template<typename T> struct splaytree {
	struct node {
		node *ch[2], *p;
		int sz;
		T val;
		T mi;
		node(T v) {
			ch[0] = ch[1] = p = NULL;
			sz = 1;
			val = mi = v;
		}
		void update() {
			sz = 1;
			mi = val;
			for (int i = 0; i < 2; i++) if (ch[i]) {
				mi = std::min(mi, ch[i]->mi);
				sz += ch[i]->sz;
			}
		}
	};

	node* root;

	splaytree() { root = NULL; }
	~splaytree() {
		std::vector<node*> q = {root};
		while (q.size()) {
			node* x = q.back(); q.pop_back();
			if (!x) continue;
			q.push_back(x->ch[0]), q.push_back(x->ch[1]);
			delete x;
		}
	}

	friend void swap(splaytree& a, splaytree& b) {
		std::swap(a.root, b.root);
	}
	void rotate(node* x) {
		node *p = x->p, *pp = p->p;
		if (pp) pp->ch[pp->ch[1] == p] = x;
		bool d = p->ch[0] == x;
		p->ch[!d] = x->ch[d], x->ch[d] = p;
		if (p->ch[!d]) p->ch[!d]->p = p;
		x->p = pp, p->p = x;
		p->update(), x->update();
	}
	node* splay(node* x, bool muda_r = true) {
		if (!x) return x;
		if (muda_r) root = x;
		while (x->p) {
			node *p = x->p, *pp = p->p;
			if (!pp) return rotate(x), x; // zig
			if ((pp->ch[0] == p)^(p->ch[0] == x))
				rotate(x), rotate(x); // zigzag
			else rotate(p), rotate(x); // zigzig
		}
		return x;
	}
	node* insert(T v, bool lb=0) {
		if (!root) return lb ? NULL : root = new node(v);
		node *x = root, *last = NULL;
		while (1) {
			bool d = x->val < v;
			if (!d) last = x;
			if (x->val == v) break;
			if (x->ch[d]) x = x->ch[d];
			else {
				if (lb) break;
				x->ch[d] = new node(v);
				x->ch[d]->p = x;
				x = x->ch[d];
				break;
			}
		}
		splay(x);
		return lb ? splay(last) : x;
	}
	int size() { return root ? root->sz : 0; }
	int count(T v) { return insert(v, 1) and root->val == v; }
	node* lower_bound(T v) { return insert(v, 1); }
	void erase(T v) {
		if (!count(v)) return;
		node *x = root, *l = x->ch[0];
		if (!l) {
			root = x->ch[1];
			if (root) root->p = NULL;
			return delete x;
		}
		root = l, l->p = NULL;
		while (l->ch[1]) l = l->ch[1];
		splay(l);
		l->ch[1] = x->ch[1];
		if (l->ch[1]) l->ch[1]->p = l;
		delete x;
		l->update();
	}
	void join(node* l, node* r, node*& i) {
		if (!l or !r) return void(i = l ? l : r);
		while (l->ch[1]) l = l->ch[1];
		splay(l, false);
		l->ch[1] = r, r->p = l;
		l->update();
		i = l;
	}
	void split(node* i, node*& l, node*& r, T v) {
		if (!i) return void(l = r = NULL);
		node* last = NULL;
		while (1) {
			bool d = i->val < v;
			if (!d) last = i;
			if (i->val == v) break;
			if (i->ch[d]) i = i->ch[d];
			else break;
		}
		splay(i, false);
		r = splay(last, false);
		if (!r) {
			l = i;
			return;
		}
		l = r->ch[0];
		r->ch[0] = NULL;
		if (l) l->p = NULL;
		r->update();
	}
	void split(T v, splaytree& t) {
		node *L, *R;
		split(root, L, R, v);
		t.root = L;
		root = R;
	}
	void merge(splaytree& t) { // segmented merge
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
