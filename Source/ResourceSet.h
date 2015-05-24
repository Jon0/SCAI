#pragma once

#include <unordered_map>

namespace graph {

class Resource;

/**
 * pairs resources with an amount
 */
class ResourceSet {
public:
	ResourceSet(std::unordered_map<Resource *, double> a);
	~ResourceSet();

	double get_amount(Resource *) const;

	std::unordered_map<Resource *, double> get_amounts() const;

	ResourceSet operator+(const ResourceSet &other);

private:
	std::unordered_map<Resource *, double> amounts;

};

}


