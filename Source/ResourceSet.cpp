#include "ResourceSet.h"

namespace graph {

ResourceSet::ResourceSet(std::unordered_map<Resource *, double> a)
	:
	amounts(a) {
}

ResourceSet::~ResourceSet() {}

double ResourceSet::get_amount(Resource *r) const {
	if (amounts.count(r) > 0) {
		return amounts.at(r);
	}
	return 0.0;
}

std::unordered_map<Resource *, double> ResourceSet::get_amounts() const {
	return amounts;
}

ResourceSet ResourceSet::operator+(const ResourceSet &other) {
	std::unordered_map<Resource *, double> result = other.get_amounts();
	for (auto &a : amounts) {
		if (result.count(a.first) == 0) {
			result.emplace(a);
		}
		else {
			result[a.first] += a.second;
		}
	}
	return ResourceSet(result);
}

}
