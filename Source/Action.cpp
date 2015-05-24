#include "Action.h"
#include "Resource.h"

namespace graph {

Action::Action(std::string name) {}

Action::~Action() {}

bool Action::controlled() {
	return true;
}

double Action::get_time() {
	return 1.0;
}

std::unordered_map<Resource *, double> Action::get_effects() {
	return resource;
}

void Action::add_effect(Resource *r, double amount, bool must_be_positive) {
	resource.emplace(std::make_pair(r, amount));
	require_positive.emplace(std::make_pair(r, must_be_positive));

	// link to the resource
	r->effect_of(this);
}



double Action::get_utility() {

	// calculate a value for each resource
	double sum = 0.0;
	for (auto &r : resource) {
		sum += r.second * r.first->get_value();
	}
	return sum;
}

bool Action::can_do_event(std::unordered_map<Resource *, double> amount) {
	
	// check all required resources are available
	for (auto &a : amount) {
		if (require_positive.count(a.first) > 0 &&
			require_positive[a.first] &&
			a.second < resource[a.first]) {
			return false;
		}
	}
	return true;
}

void Action::do_event() {
	if (act_func && act_func()) {

		// modify the resource amounts
		for (auto &r : resource) {
			r.first->adjust(r.second);
		}
	}
}

void Action::set_event(std::function<bool()> e) {
	act_func = e;
}

}


