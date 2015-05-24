#include "Flow.h"

namespace graph {

Flow::Flow(Resource *t, Resource *f)
	:
	to(t),
	from(f) {
	effect.emplace(std::make_pair(get_from(), get_in_flow()));
	effect.emplace(std::make_pair(get_to(), get_out_flow()));
}

Flow::~Flow() {}

bool Flow::controlled() {
	return false;
}

double Flow::get_time() {
	return 1.0;
}

std::unordered_map<Resource *, double> Flow::get_effects() {
	return effect;
}

Resource *Flow::get_to() {
	return to;
}

Resource *Flow::get_from() {
	return from;
}

double Flow::get_in_flow() {
	return 0.0;
}

double Flow::get_out_flow() {
	return rate;
}

void Flow::set_rate(double d) {
	rate = d;
}

}