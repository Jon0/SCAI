#pragma once

#include <vector>

namespace graph {

class Action;

bool compare(std::pair<double, Action *> a, std::pair<double, Action *> b);

/**
 * An order actions
 */
class ActionOrder
{
public:
	ActionOrder();
	~ActionOrder();

	/**
	 * time of last action
	 */
	double length();

	void add_action(double time, Action *a);

	std::pair<double, Action *> first_action();

	std::unordered_map<Resource *, double> get_effect(std::unordered_map<Resource *, double> a);

	std::unordered_map<Resource *, double> get_effect(std::unordered_map<Resource *, double> a, double time);


private:

	// time and action pairs, ordered by times
	std::vector<std::pair<double, Action *>> actions;

};

}

