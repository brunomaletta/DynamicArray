#pragma once

#include <vector>
#include <chrono>
#include <random>

#include "sms.cpp"

std::mt19937 dyn_array_rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

// Dynamic Array
template<typename T, typename SIZE_T = int> struct dyn_array {
	struct node {
		node *l, *r;
		int p;
		SIZE_T sz;
		bool rev, rev_lazy;
		sms<T, true, SIZE_T> val;
		node(sms<T, true, SIZE_T>& v) : l(NULL), r(NULL),
				p(dyn_array_rng()), sz(1), rev(0), rev_lazy(0) {
			swap(val, v);
		}
		void prop() {
			if (rev_lazy) {
				rev ^= 1;
				std::swap(l, r);
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
	dyn_array(std::initializer_list<T> v) : dyn_array() { for (T i : v) push_back(i); }
	~dyn_array() {
		std::vector<node*> q = {root};
		while (q.size()) {
			node* x = q.back(); q.pop_back();
			if (!x) continue;
			q.push_back(x->l), q.push_back(x->r);
			delete x;
		}
	}

	friend void swap(dyn_array& a, dyn_array& b) {
		std::swap(a.root, b.root);
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
		split(root, v.root, root, std::min(k, size()));
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
