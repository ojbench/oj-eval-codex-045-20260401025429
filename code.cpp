#include <cstdio>

struct Node {
    int key;
    unsigned int pr;
    Node* l;
    Node* r;
};

static unsigned int rng_state = 2463534242u;
static inline unsigned int rng() {
    // Xorshift32
    unsigned int x = rng_state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    rng_state = x;
    return x;
}

static inline Node* new_node(int key) {
    Node* n = new Node;
    n->key = key;
    n->pr = rng();
    n->l = n->r = nullptr;
    return n;
}

static inline void rotate_left(Node*& t) {
    Node* r = t->r;
    t->r = r->l;
    r->l = t;
    t = r;
}

static inline void rotate_right(Node*& t) {
    Node* l = t->l;
    t->l = l->r;
    l->r = t;
    t = l;
}

static void insert(Node*& t, int key) {
    if (!t) {
        t = new_node(key);
        return;
    }
    if (key < t->key) {
        insert(t->l, key);
        if (t->l && t->l->pr > t->pr) rotate_right(t);
    } else if (key > t->key) {
        insert(t->r, key);
        if (t->r && t->r->pr > t->pr) rotate_left(t);
    } else {
        // duplicate should not occur per problem statement
    }
}

static void erase(Node*& t, int key) {
    if (!t) return;
    if (key < t->key) {
        erase(t->l, key);
    } else if (key > t->key) {
        erase(t->r, key);
    } else {
        if (!t->l) {
            Node* tmp = t->r;
            delete t;
            t = tmp;
        } else if (!t->r) {
            Node* tmp = t->l;
            delete t;
            t = tmp;
        } else {
            if (t->l->pr > t->r->pr) {
                rotate_right(t);
                erase(t->r, key);
            } else {
                rotate_left(t);
                erase(t->l, key);
            }
        }
    }
}

static int successor(Node* t, int x) {
    int ans = -1; // indicates not found
    while (t) {
        if (t->key > x) {
            ans = t->key;
            t = t->l;
        } else {
            t = t->r;
        }
    }
    return ans;
}

static int predecessor(Node* t, int x) {
    int ans = -1; // indicates not found
    while (t) {
        if (t->key < x) {
            ans = t->key;
            t = t->r;
        } else {
            t = t->l;
        }
    }
    return ans;
}

static int exists(Node* t, int x) {
    while (t) {
        if (x < t->key) t = t->l;
        else if (x > t->key) t = t->r;
        else return 1;
    }
    return 0;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    Node* root = nullptr;
    int cur = 0;
    int dir_up = 1; // 1: up, 0: down

    char op[16];
    for (int i = 0; i < n; ++i) {
        if (scanf("%15s", op) != 1) return 0;
        if (op[0] == 'a') { // action or add
            if (op[1] == 'c') {
                // action
                if (!root) {
                    // no pending requests; do nothing
                } else if (dir_up) {
                    int s = successor(root, cur);
                    if (s != -1) {
                        cur = s;
                        erase(root, s);
                    } else {
                        dir_up = 0;
                        int p = predecessor(root, cur);
                        if (p != -1) {
                            cur = p;
                            erase(root, p);
                        } else {
                            // no requests in either direction (shouldn't happen since root exists), but guard anyway
                        }
                    }
                } else {
                    int p = predecessor(root, cur);
                    if (p != -1) {
                        cur = p;
                        erase(root, p);
                    } else {
                        dir_up = 1;
                        int s = successor(root, cur);
                        if (s != -1) {
                            cur = s;
                            erase(root, s);
                        } else {
                            // no requests
                        }
                    }
                }
            } else {
                // add x
                int x;
                scanf("%d", &x);
                // x != cur and no duplicate guaranteed by problem, but guard lightly
                if (x != cur && !exists(root, x)) insert(root, x);
            }
        } else if (op[0] == 'c') {
            // cancel x
            int x;
            scanf("%d", &x);
            erase(root, x);
        } else if (op[0] == 'l') {
            // locate
            printf("%d\n", cur);
        } else {
            // unknown op; ignore
        }
    }

    return 0;
}

