#ifndef CAT_CLASSES_H_

#include "cat_nodes.h"

/**
 * Convenience classes for forming classifier objects.
 */

namespace cats {

template <typename ElemT>
struct LeafClass {
    typedef TreeNode< LeafNode<ElemT> > CatGroup;
    typedef typename CatGroup::iterator iterator;

    template <typename NetworkT>
    double& get(NetworkT& N, int bin) {
        return rates.at(bin);
    }
    void resize(int n) {
        rates.resize(n);
    }
    int size() {
        return rates.size();
    }
    void append(double rate) {
        rates.push_back(rate);
    }

    void fill_rates(std::vector<double>& rates) {
        this->rates = rates;
    }
    std::vector<double> rates;
};

template <typename InnerT>
struct TreeClass {
    typedef TreeNode<typename InnerT::CatGroup> CatGroup;
    typedef typename CatGroup::iterator iterator;

    template <typename NetworkT>
    InnerT& get(NetworkT& N, int bin) {
        return classes.at(bin);
    }
    void resize(int n) {
        classes.resize(n);
    }
    void append(InnerT& inner) {
        classes.push_back(inner);
    }

    int size() {
        return classes.size();
    }

    template <typename T>
    void fill_rates(std::vector<T>& rates) {
        resize(rates.size());
        for (int i = 0; i < classes.size(); i++) {
            classes[i].fill_rates(rates[i]);
        }
    }
    std::vector<InnerT> classes;
};

}

#endif
