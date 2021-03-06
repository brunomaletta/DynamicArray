#pragma once

#include <vector>
#include <chrono>
#include <random>
#include <type_traits>
#include <utility>

#include "sms_mo.cpp"

std::mt19937 dyn_array_rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

// Dynamic Array
template<typename T, typename SIZE_T = int, typename SET = sms_mo<T, true, SIZE_T>>
struct dyn_array {
	static_assert(std::is_integral<T>::value, "Not an integral type");
	static_assert(std::is_integral<SIZE_T>::value, "Not an integral type");
	struct node {
		node *l, *r;
		int p;
		SIZE_T sz;
		bool rev, rev_lazy;
		SET val;
		T mi;
		node(SET& v) : l(NULL), r(NULL),
				p(dyn_array_rng()), sz(1), rev(0), rev_lazy(0) {
			swap(val, v);
			mi = val.min();
		}
		node(node* x) : l(NULL), r(NULL), p(x->p), sz(x->sz),
			rev(x->rev), rev_lazy(x->rev_lazy), val(x->val), mi(x->mi) {}
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
			mi = val.min();
			if (l) sz += l->sz, mi = std::min(mi, l->mi);
			if (r) sz += r->sz, mi = std::min(mi, r->mi);
		}
	};

	node* root;

	dyn_array() { root = NULL; }
	dyn_array(const dyn_array& v) : root(NULL) {
		if (!v.root) return;
		root = new node(v.root);
		std::vector<std::pair<node*, node*>> q = {{root, v.root}};
		while (q.size()) {
			auto [x, y] = q.back(); q.pop_back();
			if (y->l) {
				x->l = new node(y->l);
				q.emplace_back(x->l, y->l);
			}
			if (y->r) {
				x->r = new node(y->r);
				q.emplace_back(x->r, y->r);
			}
		}
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
	dyn_array& operator =(const dyn_array& v) {
		dyn_array tmp = v;
		swap(tmp, *this);
		return *this;
	}
	void clear() {
		dyn_array tmp;
		swap(tmp, *this);
	}
	SIZE_T size(node* x) const { return x ? x->sz : 0; }
	SIZE_T size() const { return size(root); }
	void join(node* l, node* r, node*& i) {
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
		if (size(i->l) + i->val.size() <= idx) { // take current and left subtree
			split(i->r, i->r, r, idx - size(i->l) - i->val.size());
			l = i;
		} else if (size(i->l) >= idx) { // go left
			split(i->l, l, i->l, idx);
			r = i;
		} else { // take left subtree and split current
			SET L;
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
	void split(SIZE_T k, dyn_array& v) { // take the 'k' smallests
		v.clear();
		split(root, v.root, root, std::min(k, size()));
	}
	void push_back(T val, SIZE_T qt = 1) {
		SET v;
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
	const T operator [](SIZE_T i) const { return get(root, i); }
	
	void update(SIZE_T idx, T val) {
		dyn_array L, M;
		split(idx+1, M);
		M.split(idx, L);
		L.push_back(val);
		L.concat(*this);
		swap(*this, L);
	}

	void merge(node*& i, SET& v) {
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
	void sort(SIZE_T l, SIZE_T r, bool reverse = false) {
		dyn_array L, M;
		split(r+1, M);
		M.split(l, L);
		M.sort(reverse);
		L.concat(M);
		L.concat(*this);
		swap(*this, L);
	}
	void reverse() { if (root) root->rev_lazy ^= 1; }
	void reverse(SIZE_T l, SIZE_T r) {
		dyn_array L, M;
		split(r+1, M);
		M.split(l, L);
		M.reverse();
		L.concat(M);
		L.concat(*this);
		swap(*this, L);
	}
	T min() { return root->mi; }
	T rmq(SIZE_T l, SIZE_T r) {
		dyn_array L, M;
		split(r+1, M);
		M.split(l, L);
		T ans = M.min();
		L.concat(M);
		L.concat(*this);
		swap(*this, L);
		return ans;
	}
};
