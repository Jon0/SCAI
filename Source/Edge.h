#pragma once

#include <unordered_map>

namespace graph {
	
class Resource;

class Edge {
public:
	virtual ~Edge();

	virtual bool controlled() = 0;

	virtual double get_time() = 0;

	virtual std::unordered_map<Resource *, double> get_effects() = 0;

};

}
