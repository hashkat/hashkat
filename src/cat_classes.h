/*
 * This file is part of the #KAT Social Network Simulator.
 *
 * The #KAT Social Network Simulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The #KAT Social Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the #KAT Social Network Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Addendum:
 *
 * Under this license, derivations of the #KAT Social Network Simulator typically must be provided in source
 * form. The #KAT Social Network Simulator and derivations thereof may be relicensed by decision of 
 * the original authors (Isaac Tamblyn, Kevin Ryczko & Adam Domurad), as well, in the case of a derivation,
 * subsequent authors. 
 */

#ifndef CAT_CLASSES_H_
#define CAT_CLASSES_H_

#include "cat_nodes.h"

/**
 * Convenience classes for forming classifier objects.
 */

namespace cats {

template <typename ElemT, typename CatData = std::vector<LeafNode<ElemT> >, int N_ELEMS = 1>
struct BaseLeafClass {
    typedef TreeNode< LeafNode<ElemT, N_ELEMS>, CatData > CatGroup;
    typedef typename CatGroup::iterator iterator;
};

template <typename ElemT, int MAX_CATS, int N_ELEMS = 1>
struct StaticBaseLeafClass : BaseLeafClass<ElemT, StaticVector<LeafNode<ElemT, N_ELEMS>, MAX_CATS>, N_ELEMS > {
};

template <typename ElemT, typename CatData = std::vector<LeafNode<ElemT> >, int N_ELEMS = 1>
struct LeafClass : BaseLeafClass<ElemT, CatData, N_ELEMS> {
    template <typename NetworkT>
    double& get(NetworkT& N, int bin) {
        return rates.at(bin);
    }

    template <typename NetworkT>
    void print(NetworkT& N, int bin, int layer) {
        for (int i = 0; i < layer; i++) {
            printf("  ");
        }
        printf("Layer %d (Bin %d) \n", layer, bin);
        std::vector<double>::iterator it = rates.begin();
        for (int i = 0; i < layer; i++) {
            printf("  ");
        }

        for (; it != rates.end(); ++it) {
            printf("%.2f ", *it);
        }
        printf("\n");
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

    template <typename NetworkT>
    std::string cat_name(NetworkT& N, int bin) {
        return "";
    }

    void fill_rates(std::vector<double>& rates) {
        this->rates = rates;
    }
    std::vector<double> rates;
};

template <typename ElemT, int MAX_CATS, int N_ELEMS = 1>
struct StaticLeafClass : LeafClass<ElemT, StaticVector<LeafNode<ElemT, N_ELEMS>, MAX_CATS>, N_ELEMS> {
};

template <typename InnerT, typename CatData = std::vector<typename InnerT::CatGroup>, int N_ELEMS = 1 >
struct TreeClass {
    typedef TreeNode<typename InnerT::CatGroup, CatData, N_ELEMS> CatGroup;
    typedef typename CatGroup::iterator iterator;

    template <typename NetworkT>
    InnerT& get(NetworkT& N, int bin) {
        if (size() < bin + 1) {
            resize(bin + 1);
        }
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

    template <typename NetworkT>
    std::string cat_name(NetworkT& N, int bin) {
        return "";
    }

    template <typename NetworkT>
    void print(NetworkT& N, int bin = 0, int layer = 0) {
        for (int i = 0; i < layer; i++) {
            printf("  ");
        }

        printf("Layer %d (Bin %d) \n", layer, bin);

        /* Print children: */
        int child_bin = 0;
        typename std::vector<InnerT>::iterator it = classes.begin();
        for (; it != classes.end(); ++it) {
            it->print(N, child_bin, layer + 1);
            child_bin++;
        }
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

template <typename InnerT, int MAX_CATS, int N_ELEMS = 1>
struct StaticTreeClass : TreeClass<InnerT, StaticVector<typename InnerT::CatGroup, MAX_CATS>, N_ELEMS> {
};

}

#endif
