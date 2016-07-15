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
 * the original authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case of a derivation,
 * subsequent authors. 
 */

#ifndef CATEGORYGROUPER_H_
#define CATEGORYGROUPER_H_

#include "util.h"
#include "serialization.h"

#include <vector>

// Each category is defined with respect to a series of bounds
// and a categorization variable. This categorization variable can
// come from any source, and is fed into CategoryGroup to
// incrementally update a Categorization.

// 'Cat' represents a categorization for a single user.
struct Cat {
	int category, index;
	Cat(int g = -1, int i = -1) {
		category = g, index = i;
	}

    template <typename Archive>
    void serialize(Archive& ar) {
        ar(category, index);
    }
};

struct CategoryAgentList {
	// Upper bound that fits into this category
	double threshold, /*Optional*/ prob;
	std::vector<int> agents;
	CategoryAgentList(double t = 0, double p = 0) {
		threshold = t, prob = p;
	}
	size_t size() const {
	    return agents.size();
	}
	int operator[](size_t idx) const {
	    return agents[idx];
	}

	template <typename Archive>
        void serialize(Archive& ar) {
	    ar(threshold, prob, agents);
	}
};

struct CategoryGrouper {
	std::vector<Cat> categorizations;
	std::vector<CategoryAgentList> categories;

	// Incrementally update a categorization
	void categorize(int agent, double parameter) {
		if (categorizations.size() <= agent) {
			// Make sure 'agent' is a valid index
			categorizations.resize(agent + 1);
		}
		Cat& c = categorizations.at(agent);
		for (int i = 0; i < categories.size(); i++) {
			CategoryAgentList& C = categories[i];
			if (parameter <= C.threshold) {
				if (i != c.category) {
					// We have to move ourselves into the new list
					remove(c);
					c = add(agent, i);
				}
				return; // Categorized, done.
			}
		}
		DEBUG_CHECK(false, "Logic error");
	}

	/* Remove a categorized agent. */
	void remove(Cat& c) {
		if (c.category != -1) {
			CategoryAgentList& C = categories.at(c.category);
			DEBUG_CHECK(within_range(c.index, 0, (int)C.agents.size()), "Logic error");
			// Move the element at the top to our index:
			Cat& c_top = categorizations[C.agents.back()];
			c_top.index = c.index;
			C.agents[c.index] = C.agents.back();
			C.agents.pop_back();
			// Reset:
			c = Cat();
		}
	}

	/* Synchronize rates from a loaded configuration.
	 * This is done because, although we can load a new configuration,
	 * some rates remain duplicated in our state object. */
	void sync_rates(CategoryGrouper& C) {
        for (int i = 0; i < categories.size(); i++) {
            categories[i].prob = C.categories[i].prob;
            categories[i].threshold = C.categories[i].threshold;
        }
	}

	Cat add(int agent, int new_cat) {
		CategoryAgentList& C = categories.at(new_cat);
		C.agents.push_back(agent);
		return Cat(new_cat, C.agents.size() - 1);
	}

	template <typename Archive>
    void serialize(Archive& ar) {
	    ar(categorizations, categories);
	}
};

#endif
