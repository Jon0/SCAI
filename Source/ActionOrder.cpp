#include <algorithm>

#include "Action.h"
#include "ActionOrder.h"

namespace graph {

bool compare(std::pair<double, Action *> a, std::pair<double, Action *> b) {
	return a.first < b.first;
}

ActionOrder::ActionOrder() {}

ActionOrder::~ActionOrder() {}

double ActionOrder::length() {
	// time of the last action
	return actions.back().first;
}

void ActionOrder::add_action(double time, Action *a) {
	actions.push_back(std::make_pair(time, a));

	// maintain ordering
	std::sort(std::begin(actions), std::end(actions), compare);
}

std::pair<double, Action *> ActionOrder::first_action() {
	if (actions.empty()) {
		return std::make_pair(0, nullptr);
	}
	return actions[0];
}

std::unordered_map<Resource *, double> ActionOrder::get_effect(std::unordered_map<Resource *, double> a) {
	return get_effect(a, length());
}

std::unordered_map<Resource *, double> ActionOrder::get_effect(std::unordered_map<Resource *, double> a, double time) {
	
	// resource, amount, time
	std::vector<std::tuple<Resource *, double, double>> future_effects;

	for (double t = 0.0; t < time; t += 1.0) {
		
		// apply edges

		// add effects


	}

	// TODO find the outcome
	return a;
}

}
