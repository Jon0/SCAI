#include "Action.h"
#include "Flow.h"
#include "Graph.h"
#include "Resource.h"

namespace graph {

Resource::Resource(Graph *g, std::string name)
	:
	graph(g),
	amount(0.0),
	updates(0) {
}

Resource::~Resource() {

}

void Resource::update() {

	// expected resource changes are given via adjust
	// this update must account for other incomes

	if (update_func) {
		double new_amount = update_func();

		double p = static_cast<double>(updates) / static_cast<double>(updates + 1);

		// average the income rate
		amount_d = p * amount_d + (1 - p) * (new_amount - amount);

		// set current amount
		amount = new_amount;

		updates++;

		// divide the income over incoming flows
		for (auto &in : flows_in) {
			in->set_rate(amount_d / flows_in.size());
		}
	}
}

void Resource::adjust(double a) {
	amount += a;
}

double Resource::get_amount() {
	return amount;
}

double Resource::get_value() {
	return value;
}

void Resource::set_value(double v) {
	value = v;
}

std::vector<Action *> Resource::get_effects() {
	return effect_action;
}

void Resource::set_update_function(std::function<double()> f) {
	update_func = f;
}

void Resource::produces(Resource *other) {
	flows_out.emplace_back(std::make_unique<Flow>(this, other));
	other->add_flow_in(flows_out.back().get());
	graph->register_flow(flows_out.back().get());
}

void Resource::effect_of(Action *a) {
	effect_action.push_back(a);
}

std::string Resource::debug() {
	return std::to_string(amount) + ", " + std::to_string(amount_d);
}

void Resource::add_flow_in(Flow *f) {
	flows_in.push_back(f);
}

}
