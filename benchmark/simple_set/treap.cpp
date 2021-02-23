#include <iostream>
#include <chrono>
#include <random>
#include <vector>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());

template<typename T> struct treap {
	struct node {
		node *l, *r;
		int p, sz;
		T val;
		node(T v) : l(NULL), r(NULL), p(rng()), sz(1), val(v) {}
		void update() {
			sz = 1;
			if (l) sz += l->sz;
			if (r) sz += r->sz;
		}
	};

	node* root;

	treap() { root = NULL; }
	~treap() {
		std::vector<node*> q = {root};
		while (q.size()) {
			node* x = q.back(); q.pop_back();
			if (!x) continue;
			q.push_back(x->l), q.push_back(x->r);
			delete x;
		}
	}

	int size(node* x) { return x ? x->sz : 0; }
	int size() { return size(root); }
	void join(node* l, node* r, node*& i) { // assume que l < r
		if (!l or !r) return void(i = l ? l : r);
		if (l->p > r->p) join(l->r, r, l->r), i = l;
		else join(l, r->l, r->l), i = r;
		i->update();
	}
	void split(node* i, node*& l, node*& r, T v) {
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
};

std::chrono::time_point<std::chrono::high_resolution_clock> beg;

void reset_time() {
	beg = std::chrono::high_resolution_clock::now();
}

double get_time() {
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - beg;
	return elapsed.count();
}

int main(int argc, char** argv) {
	srand(atoi(argv[1]));

	long long MAX;
	std::cin >> MAX;
	int n;
	std::cin >> n;

	int sum = 0;

	treap<long long> s;

	reset_time();

	for (int i = 0; i < n; i++) {
		int op = rand()%3;
		long long a = (rand()*(long long)rand())%MAX;
		if (op == 0) s.insert(a);
		else if (op == 1) s.erase(a);
		else sum += s.count(a);
	}

	double time = get_time();

	std::cout << "treap" << std::endl;
	std::cout << sum << std::endl;
	std::cout << int(1000*time) << std::endl;

	return 0;
}
