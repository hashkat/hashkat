#ifndef RATEVECTORTREE_H_
#define RATEVECTORTREE_H_

template <typename T, int N_ELEM, int N_CHILDREN = 10>
struct RateVectorTree {
    typedef int ref_t;

    struct RateVec {
        double tuple[N_ELEM];
        double tuple_sum;
        RateVec() {
            for (int i = 0; i < N_ELEM; i++) {
                tuple[i] = 0;
            }
            tuple_sum = 0;
        }
        void print() const {
            printf("(Total = %.2f)[", tuple_sum);
            for (int i = 0; i < N_ELEM; i++) {
                printf("%.2f ", tuple[i]);
            }
            printf("]");
        }
        void add(RateVec& o) {
            for (int i = 0; i < N_ELEM; i++) {
                tuple[i] += o.tuple[i];
            }
            tuple_sum += o.tuple_sum;
        }
        void sub(RateVec& o) {
            for (int i = 0; i < N_ELEM; i++) {
                tuple[i] -= o.tuple[i];
            }
            tuple_sum -= o.tuple_sum;
        }
        RateVec delta(const RateVec& o) {
            RateVec ret = o;
            ret.sub(*this);
            return ret;
        }
    };

    struct Node {
        ref_t parent; // -1 if root
        short depth;
        bool is_leaf; // Default true
        bool is_allocated; // Default false
        T data;
        RateVec rates;

        ref_t children[N_CHILDREN]; // -1 if not allocated

        bool has_vacancy() {
            for (int i = 0; i < N_CHILDREN; i++) {
                if (children[i] == -1) {
                    return true; // At least one free slot still
                }
            }
            return false;
        }
        void link_to_parent(ref_t parent, int depth) {
            this-> parent = parent;
            this-> depth = depth;
        }

        ref_t alloc_child(RateVectorTree& tree, ref_t self, int child) {
             ref_t ref = tree.alloc_node();
             children[child] = ref;
             tree.get(ref).link_to_parent(self, depth + 1);
             return ref;
        }

        ref_t* seek_ref(ref_t ref) {
            for (int i = 0; i < N_CHILDREN; i++) {
                if (children[i] == ref) {
                    return &children[i];
                }
            }
            return NULL;
        }

        // Shift so that the tree can grow
        void alloc_parent(RateVectorTree& tree, ref_t self) {
             ref_t ref = tree.alloc_node();
             tree.get(ref) = *this; // Make a copy
             tree.get(ref).is_leaf = false;
             depth++;
             // Update pointer from parent to self
             ref_t* self_loc = tree.get(parent).seek_ref(self);
             *self_loc = ref;
             parent = ref; // Take new node as parent
             // Become first child of new parent
             tree.get(parent).children[0] = self;
        }

        // If a child node was allocated, place it in child_output, otherwise place self.
        // Does NOT have any effect on the total rate balance.
        // Returns false if fails to do so.
        bool alloc_with_next_vacancy(RateVectorTree& tree, ref_t self, ref_t& child_output) {
            // Leaf case:
            if (is_leaf) {
                alloc_parent(tree, self);
                tree.post_vacancy(self); // Vacant to expand again, if eventually necessary
                // Call into a parent case:
                tree.get(parent).alloc_with_next_vacancy(tree, parent, child_output);
                return true;
            }
            // Parent case:
            int i = 0;
            for (; i < N_CHILDREN; i++) {
                if (children[i] == -1) {
                    child_output = alloc_child(tree, self, i);
                    tree.post_vacancy(child_output); // Post child to vacancy list
                    break;
                }
            }
            if (i >= N_CHILDREN) {
                return false; // No vacancy
            }

            for (; i < N_CHILDREN; i++) {
                if (children[i] == -1) {
                    // At least one free slot still, re-post to vacancy list
                    tree.post_vacancy(self);
                    return true;
                }
            }
            // All slots have been used, stay removed from vacancy list
            return true;
        }
        Node() {
            parent = -1;
            is_leaf = true;
            is_allocated = false;
            depth = 0;
            for (int i =0; i < N_CHILDREN; i++) {
                children[i] = -1;
            }
        }

        void rate_add(RateVectorTree& tree, RateVec& vec) {
            rates.add(vec);
            if (parent != -1) {
                tree.get(parent).rate_add(tree, vec);
            }
        }
        /* Principal KMC method, choose with respect to bin rates. */
        int random_weighted_bin(RateVectorTree& tree, MTwist& rng) {
            const double ZEROTOL = 1e-16;

            double num = rng.rand_real_not1() * rates.tuple_sum;
            for (int i = 0; i < N_CHILDREN; i++) {
                int c = children[i];
                if (c != -1) {
                    num -= tree.get(c).rates.tuple_sum;
                    if (num <= ZEROTOL) {
                        return i;
                    }
                }
            }
            ASSERT(false, "Logic error! No child to choose from.");
            return -1;
        }

        ref_t pick_random_weighted(RateVectorTree& tree, MTwist& rng, ref_t self) {
            if (is_leaf) {
                return self; // We are done
            }
            ref_t child = children[random_weighted_bin(tree, rng)];
            return tree.get(child).pick_random_weighted(tree, rng, child);
        }

        void rate_sub(RateVectorTree& tree, RateVec& vec) {
            rates.sub(vec);
            if (parent != -1) {
                tree.get(parent).rate_sub(tree, vec);
            }
        }

        void print(RateVectorTree& tree, ref_t self, int tab) {
            for (int i = 0; i < tab; i++) { printf("\t"); }
            printf("%s Node %d p=%d data=%d depth=%d\n", (!is_leaf ? "Parent" : "Leaf"), self, parent, data, depth);
            for (int i = 0; i < tab; i++) { printf("\t"); }
            rates.print();
            printf("\n");
            for (int i =0; i < N_CHILDREN; i++) {
                if (children[i] != -1) {
                    tree.get(children[i]).print(tree, children[i], tab + 1);
                }
            }
        }
    };

    RateVectorTree() {
        n_elems = 0;
        node_pool.resize(1); // Allocate root
        ensure_vacancy_depth(0).push_back(0); // Starts vacant itself, and with vacant children slots
        get(0).is_leaf = false; // Root of the tree is not a leaf
    }
    Node& get(ref_t handle) {
        return node_pool[handle];
    }

    // NOTE: Should only be exposing ref's to leaf nodes,
    // don't call with parent nodes.
    void remove(ref_t handle) {
        DEBUG_CHECK(get(handle).is_leaf, "Cannot remove parent nodes in this tree implementation.");
        Node& n = get(handle);
        get(n.parent).rate_sub(*this, n.rates);
        n_elems--;
        free_list.push_back(handle);
        n = Node();// 'Wipe' the node
    }

    ref_t pick_random_weighted(MTwist& rng) {
        ASSERT(size() > 0, "No element to pick!");
        return get(0).pick_random_weighted(*this, rng, 0);
    }
    ref_t add(const T& data, const RateVec& tuple) {
        node_pool.reserve(node_pool.size() + 3); // Resolve cases where nodes can become deallocated during use
        ref_t node = find_vacancy();
        Node& n = get(node);
        n.data = data;
        n.rates = tuple;
        get(n.parent).rate_add(*this, n.rates);
        n_elems++;
        return node;
    }
    void replace_rate(ref_t ref, const RateVec& tuple) {
        Node& n = get(ref);
        RateVec delta = n.rates.delta(tuple);
        n.rates = tuple;
        get(n.parent).rate_add(*this, delta);
    }

    RateVec rate_summary() {
        return get(0).rates;
    }

    void print() {
        get(0).print(*this, 0, 0);
        printf("Leaf nodes = %d, total number of nodes = %d\n", size(), node_pool.size());
    }
    size_t size() const {
        return n_elems;
    }
private:
    typedef std::vector<ref_t> ref_list;
    ref_t alloc_node() {
        ref_t n;
        if (!free_list.empty()) {
            // Reuse deallocated nodes before anything else:
            n = free_list.back();
            free_list.pop_back();
        } else {
            int size = node_pool.size();
            node_pool.resize(size + 1);
            n = size;
        }
        get(n).is_allocated = true;
        return n;
    }

    void post_vacancy(ref_t ref) {
        ensure_vacancy_depth(get(ref).depth).push_back(ref);
    }

    bool check_validity(int depth, ref_t ref) {
        return (get(ref).depth == depth);
    }

    ref_t find_vacancy(int depth, ref_list& sub_list) {
        while (!sub_list.empty()) {
            ref_t vacancy = sub_list.back();
            sub_list.pop_back(); // We assume we need to pop the element, we let the below routine re-add as necessary
            if (!check_validity(depth, vacancy) ) {
                continue;
            }
            ref_t new_node;
            bool valid = get(vacancy).alloc_with_next_vacancy(*this, vacancy, new_node);
            if (!valid) {
                continue;
            }
            return new_node;
        }
        return -1;
    }
    // Find a vacant parent to link our node to
    // Does NOT propagate changes up the tree (add does)
    ref_t find_vacancy() {
        // A node with some sort of vacancy *must* exist
        for (int depth = 0; depth < vacancy_list.size(); depth++) {
            ref_list& sub_list = vacancy_list[depth];
            ref_t vacancy = find_vacancy(depth, sub_list);
            if (vacancy != -1) {
                return vacancy;
            }
        }
        ASSERT(false, "Should never happen!");
        return -1;
    }
    ref_list& ensure_vacancy_depth(int depth) {
        if (vacancy_list.size() < depth + 1) {
            vacancy_list.resize(depth + 1);
        }
        return vacancy_list[depth];
    }

    size_t n_elems;
    std::vector<ref_t> free_list; //Freed nodes
    std::vector<Node> node_pool; // 0 is the root node
    std::vector<ref_list> vacancy_list;
};

#endif /* RATEVECTORTREE_H_ */