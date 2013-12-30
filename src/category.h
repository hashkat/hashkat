#ifndef CATEGORY_H_
#define CATEGORY_H_

#include "util.h"
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
};

struct CategoryEntityList {
	// Upper bound that fits into this category
	double threshold;
	std::vector<int> entities;
	CategoryEntityList(double thresh) {
		threshold = thresh;
	}
};

struct CategoryGrouper {
	std::vector<Cat> categorizations;
	std::vector<CategoryEntityList> categories;
	// Incrementally update a categorization
	void categorize(int entity, double parameter) {
		if (categorizations.size() <= entity) {
			// Make sure 'entity' is a valid index
			categorizations.resize(entity + 1);
		}
		Cat& c = categorizations.at(entity);
		for (int i = 0; i < categories.size(); i++) {
			CategoryEntityList& C = categories[i];
			if (parameter <= C.threshold) {
				if (i != c.category) {
					// We have to move ourselves into the new list
					remove(c);
					c = add(entity, i);
				}
				return; // Categorized, done.
			}
		}
		DEBUG_CHECK(false, "Logic error");
	}

	void remove(Cat& c) {
		if (c.category != -1) {
			CategoryEntityList& C = categories.at(c.category);
			DEBUG_CHECK(within_range(c.index, 0, (int)C.entities.size()), "Logic error");
			// Move the element at the top to our index:
			Cat& c_top = categorizations[C.entities.back()];
			c_top.index = c.index;
			C.entities[c.index] = C.entities.back();
			C.entities.pop_back();
			// Reset:
			c = Cat();
		}
	}

	Cat add(int entity, int new_cat) {
		CategoryEntityList& C = categories.at(new_cat);
		C.entities.push_back(entity);
		return Cat(new_cat, C.entities.size() - 1);
	}
};

#endif
