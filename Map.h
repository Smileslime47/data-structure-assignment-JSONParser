//
// Created by chenshao on 2021/9/15.
//

#ifndef JSONLIB_MAP_H
#define JSONLIB_MAP_H

#include <memory>

template<typename KEY, typename VALUE>
struct tree_node {
    using pt_node = std::shared_ptr<tree_node<KEY, VALUE>>;

    explicit tree_node(std::pair<KEY, VALUE> &);
    explicit tree_node(KEY k):key(k),left(nullptr),right(nullptr),height(1){};
    int get_height();

    KEY key;
    VALUE value;
    int height;
    pt_node left, right;
};


template<typename KEY, typename VALUE>
class my_map {
public:
    using twin = std::pair<KEY, VALUE>;
    using pt_node = std::shared_ptr<tree_node<KEY, VALUE>>;

    void insert_value(twin a);

    pt_node find(KEY k);

    void pre_order_for_each_value(void (*print)(twin));

    VALUE &operator[](KEY);

private:
    void pre_order(const pt_node &r, void (*print)(twin));

    void insert_value_only_key(KEY k);


    pt_node insert_only_key(pt_node, KEY);

    pt_node root = nullptr;

    pt_node insert(pt_node, twin);

    static pt_node RL(const pt_node &a);

    static pt_node LR(const pt_node &a);

    static pt_node LL(const pt_node &a);

    static pt_node RR(const pt_node &a);
};


template<typename KEY, typename VALUE>
tree_node<KEY, VALUE>::tree_node(std::pair<KEY, VALUE> &s)  : key(s.first), value(s.second), left(nullptr),
                                                              right(nullptr), height(1) {}

template<typename KEY, typename VALUE>
int tree_node<KEY, VALUE>::get_height() {
    if (!this)
        return 0;
    else
        return this->height;
}

template<typename KEY, typename VALUE>
VALUE &my_map<KEY, VALUE>::operator[](KEY key) {
    pt_node p = find(key);
    if (p)return p->value;
    else {
        insert_value_only_key(key);
        return find(key)->value;

    }
}

//ok
template<typename KEY, typename VALUE>
std::shared_ptr<tree_node<KEY, VALUE>> my_map<KEY, VALUE>::LL(const pt_node &a) {
    pt_node b = a->left;
    pt_node c = b->right;
    a->left = c;
    b->right = a;
    a->height = max(a->left->get_height(), (a->right)->get_height()) + 1;
    return b;
}

//ok
template<typename KEY, typename VALUE>
std::shared_ptr<tree_node<KEY, VALUE>> my_map<KEY, VALUE>::RR(const pt_node &a) {
    pt_node b = a->right;
    pt_node c = b->left;
    a->right = c;
    b->left = a;
    a->height = max((a->left)->get_height(), (a->right)->get_height()) + 1;
    return b;
}

template<typename KEY, typename VALUE>
std::shared_ptr<tree_node<KEY, VALUE>> my_map<KEY, VALUE>::LR(const pt_node &a) {
    pt_node b = a->left;
    pt_node c = b->right;
    pt_node d = c->left;
    pt_node e = c->right;
    b->right = d;
    a->left = e;
    c->left = b;
    c->right = a;
    a->height = max((a->left)->get_height(), (a->right)->get_height()) + 1;
    b->height = max((b->left)->get_height(), (b->right)->get_height()) + 1;
    return c;
}

template<typename KEY, typename VALUE>
std::shared_ptr<tree_node<KEY, VALUE>> my_map<KEY, VALUE>::RL(const pt_node &a) {
    pt_node b = a->right;
    pt_node c = b->left;
    pt_node d = c->right;
    pt_node e = c->left;
    b->left = d;
    a->right = e;
    c->right = b;
    c->left = a;
    a->height = max((a->left)->get_height(), (a->right)->get_height()) + 1;
    b->height = max((b->left)->get_height(), (b->right)->get_height()) + 1;
    return c;
}


template<typename KEY, typename VALUE>
std::shared_ptr<tree_node<KEY, VALUE>> my_map<KEY, VALUE>::
insert(pt_node t, twin temp) {
    if (!t) {
        t = std::make_shared<tree_node<KEY, VALUE>>(temp);
        return t;
    }
    if (temp.first < t->key)
        t->left = insert(t->left, temp);
    if ((temp.first) > (t->key))
        t->right = insert(t->right, temp);
    if (temp.first == t->key)
        return t;

    //判断旋转

    //
    if (((t->left)->get_height() - (t->right)->get_height()) == -2) {
        //右过高
        if (temp.first > t->right->key)//右右
        {
            t = RR(t);
        } else {//右左
            t = RL(t);
        }
    }
    if (((t->left)->get_height() - (t->right)->get_height()) == 2) {
        //左过高
        if (temp.first < t->left->key)//左左
        {
            t = LL(t);
        } else {//左右
            t = LR(t);
        }
    }

    t->height = max((t->left)->get_height(), (t->right)->get_height()) + 1;
    return t;
}


template<typename KEY, typename VALUE>
std::shared_ptr<tree_node<KEY, VALUE>> my_map<KEY, VALUE>::find(KEY k) {
    pt_node ptn = root;
    while (ptn) {
        if (ptn->key < k)
            ptn = ptn->right;
        else if (ptn->key > k)
            ptn = ptn->left;
        else
            return ptn;
    }
    return nullptr;
}

template<typename KEY, typename VALUE>
void my_map<KEY, VALUE>::insert_value(my_map::twin a) {
    root = insert(root, a);
}

template<typename KEY, typename VALUE>
void my_map<KEY, VALUE>::pre_order_for_each_value(void (*print)(twin)) {
    pre_order(root, print);
}

template<typename KEY, typename VALUE>
void my_map<KEY, VALUE>::pre_order(const my_map::pt_node &r, void (*print)(twin)) {
    if (!r)return;
    print({r->key, r->value});
    pre_order(r->left, print);
    pre_order(r->right, print);
}

template<typename KEY, typename VALUE>
void my_map<KEY, VALUE>::insert_value_only_key(KEY k) {
    root=insert_only_key(root, k);
}


template<typename KEY, typename VALUE>
std::shared_ptr<tree_node<KEY, VALUE>> my_map<KEY, VALUE>::insert_only_key(pt_node t, KEY k) {
    if (!t) {
        t = std::make_shared<tree_node<KEY, VALUE>>(k);
        return t;
    }
    if (k < t->key)
        t->left = insert_only_key(t->left, k);
    if ((k) > (t->key))
        t->right = insert_only_key(t->right, k);
    if (k == t->key)
        return t;

    //判断旋转

    //
    if (((t->left)->get_height() - (t->right)->get_height()) == -2) {
        //右过高
        if (k > t->right->key)//右右
        {
            t = RR(t);
        } else {//右左
            t = RL(t);
        }
    }
    if (((t->left)->get_height() - (t->right)->get_height()) == 2) {
        //左过高
        if (k < t->left->key)//左左
        {
            t = LL(t);
        } else {//左右
            t = LR(t);
        }
    }

    t->height = max((t->left)->get_height(), (t->right)->get_height()) + 1;
    return t;
}

#endif //JSONLIB_MAP_H
