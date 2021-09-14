//
// Created by pe200012 on 2021/09/14.
//

#ifndef JSONLIB_MAP_H
#define JSONLIB_MAP_H


class value_base {
public:
    std::string key;
    int num;

    value_base(std::string a, int b) : key(std::move(a)), num(b) {};
};

struct tree_node {
    std::shared_ptr <value_base> element;
    std::shared_ptr <tree_node> left, right;
    int height;

    tree_node() : element(nullptr), left(nullptr), right(nullptr), height(0) {};

    explicit tree_node(std::shared_ptr <value_base> e,
                       std::shared_ptr <tree_node> l = nullptr,
                       std::shared_ptr <tree_node> r = nullptr) :
            element(std::move(e)), left(std::move(l)), right(std::move(r)), height(1) {}
};

//声明
int get_height(const std::shared_ptr <tree_node> &p);

class result {
public:
    void insert_value(std::shared_ptr <value_base> &a) {
        root = insert(root, a);
    }

    std::shared_ptr <value_base> f(std::string k);

    void pre_order_for_each(void (*print)(std::shared_ptr <value_base>)) {
        pre_order(root, print);
    };
private:
    void pre_order(std::shared_ptr <tree_node> r, void (*print)(std::shared_ptr <value_base>)) {
        if (!r)return;
        print(r->element);
        pre_order(r->left, print);
        pre_order(r->right, print);
    }

    std::shared_ptr <tree_node> root = nullptr;

    std::shared_ptr <tree_node> insert(std::shared_ptr <tree_node>, const std::shared_ptr <value_base> &);

    static std::shared_ptr <tree_node> RL(const std::shared_ptr <tree_node> &a);

    static std::shared_ptr <tree_node> LR(const std::shared_ptr <tree_node> &a);

    static std::shared_ptr <tree_node> LL(const std::shared_ptr <tree_node> &a);

    static std::shared_ptr <tree_node> RR(const std::shared_ptr <tree_node> &a);
};

//ok
std::shared_ptr <tree_node> result::LL(const std::shared_ptr <tree_node> &a) {
    std::shared_ptr <tree_node> b = a->left;
    std::shared_ptr <tree_node> c = b->right;
    a->left = c;
    b->right = a;
    a->height = max(get_height(a->left), get_height(a->right)) + 1;
    return b;
}

//ok
std::shared_ptr <tree_node> result::RR(const std::shared_ptr <tree_node> &a) {
    std::shared_ptr <tree_node> b = a->right;
    std::shared_ptr <tree_node> c = b->left;
    a->right = c;
    b->left = a;
    a->height = max(get_height(a->left), get_height(a->right)) + 1;
    return b;
}

std::shared_ptr <tree_node> result::LR(const std::shared_ptr <tree_node> &a) {
    std::shared_ptr <tree_node> b = a->left;
    std::shared_ptr <tree_node> c = b->right;
    std::shared_ptr <tree_node> d = c->left;
    std::shared_ptr <tree_node> e = c->right;
    b->right = d;
    a->left = e;
    c->left = b;
    c->right = a;
    a->height = max(get_height(a->left), get_height(a->right)) + 1;
    b->height = max(get_height(b->left), get_height(b->right)) + 1;
    return c;
}

std::shared_ptr <tree_node> result::RL(const std::shared_ptr <tree_node> &a) {
    std::shared_ptr <tree_node> b = a->right;
    std::shared_ptr <tree_node> c = b->left;
    std::shared_ptr <tree_node> d = c->right;
    std::shared_ptr <tree_node> e = c->left;
    b->left = d;
    a->right = e;
    c->right = b;
    c->left = a;
    a->height = max(get_height(a->left), get_height(a->right)) + 1;
    b->height = max(get_height(b->left), get_height(b->right)) + 1;
    return c;
}

int get_height(const std::shared_ptr <tree_node> &p) {
    if (!p)
        return 0;
    else
        return p->height;
}

std::shared_ptr <tree_node> result::insert(std::shared_ptr <tree_node> t, const std::shared_ptr <value_base> &temp) {
    if (!t) {
        t = std::make_shared<tree_node>(temp);
        return t;
    }
    if (temp->key < root->element->key)
        t->left = insert(t->left, temp);
    if ((temp->key) > (t->element->key))
        t->right = insert(t->right, temp);
    if (temp->key == t->element->key)
        return t;

    //判断旋转

    if ((get_height(t->left) - get_height(t->right)) == -2) {
        //右过高
        if (temp->key > t->right->element->key)//右右
        {
            t = RR(t);
        } else {//右左
            t = RL(t);
        }
    }
    if ((get_height(t->left) - get_height(t->right)) == 2) {
        //左过高
        if (temp->key < t->left->element->key) {//左左
            t = LL(t);
        } else {//左右
            t = LR(t);
        }
    }

    t->height = max(get_height(t->left), get_height(t->right)) + 1;
    return t;
}

std::shared_ptr <value_base> result::f(std::string k) {
    std::shared_ptr <tree_node> ptn = root;
    while (ptn) {
        if (ptn->element->key < k)
            ptn = ptn->right;
        else if (ptn->element->key > k)
            ptn = ptn->left;
        else
            return ptn->element;
    }
    return nullptr;
}

#endif //JSONLIB_MAP_H
