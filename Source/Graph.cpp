#include "Action.h"
#include "ActionOrder.h"
#include "Flow.h"
#include "Graph.h"
#include "Resource.h"

namespace graph {

Graph *Graph::instance = nullptr;

void Graph::initialise() {
	if (Graph::instance) {
		return;
	}
	Graph::instance = new Graph();
}

Graph *Graph::get() {
	return Graph::instance;
}

Graph::Graph() {}

Graph::~Graph() {}

void Graph::update() {

	// check for resource changes
	for (auto &r : resource) {
		r.second->update();
	}
	update_resource_values();

	// find the best actions
	double best_score = 0;
	Action *best = nullptr;
	for (auto &a : action) {
		double score = a.second->get_utility();
		if (score > best_score) {
			best_score = score;
			best = a.second.get();
		}
	}

	if (best_score > 0) {
		best->do_event();
	}
}

void Graph::add_action(std::string name) {
	action.emplace(std::make_pair(name, std::make_unique<Action>(name)));
	edge.push_back(action[name].get());
}

Action *Graph::get_action(std::string name) {
	return action[name].get();
}

void Graph::add_resource(std::string name) {
	resource.emplace(std::make_pair(name, std::make_unique<Resource>(this, name)));
}

Resource *Graph::get_resource(std::string name) {
	return resource[name].get();
}

std::unordered_map<Resource *, double> Graph::get_amounts() {
	std::unordered_map<Resource *, double> result;
	for (auto &r : resource) {
		result.emplace(std::make_pair(r.second.get(), r.second->get_amount()));
	}
	return result;
}

void Graph::set_win_func(std::function<double(ResourceSet)> f) {
	win_func = f;
}

void Graph::register_flow(Flow *f) {
	edge.push_back(f);
	flow.push_back(f);
}

void Graph::update_actions() {
	
	// current amounts
	std::unordered_map<Resource *, double> current_amounts = get_amounts();

	std::vector<ActionOrder> orders;

	// add blank order
	orders.push_back(ActionOrder());

	//
	int max_iterations = 100;
	for (int time = 0; time < max_iterations; ++time) {
		for (auto &order : orders) {
			std::unordered_map<Resource *, double> order_amounts = order.get_effect(current_amounts, time);

			// get available actions
			// at time within the orders context
			std::vector<Action *> available;
			for (auto &a : action) {
				if (a.second->can_do_event(order_amounts)) {
					available.push_back(a.second.get());
				}
			}

			for (auto &a : available) {
				// add to the order
				// and add a new branch
				ActionOrder new_order = order;
				new_order.add_action(time, a);
				orders.push_back(new_order);
			}

		}
	}

	// find the most valuable
	ActionOrder *best = nullptr;
	double best_score = 0;
	for (auto &order : orders) {
		double score = win_func(order.get_effect(current_amounts)) / order.length();
		if (score > best_score) {
			best = &order;
			best_score = score;
		}
	}

	if (best) {

		// execute first action
		std::pair<double, Action *> a = best->first_action();
		if (a.second) {
			a.second->do_event();
		}
	}

}

void Graph::update_resource_values() {
	std::unordered_map<Resource *, double> value;

	// initialise each resource to value
	for (auto &r : resource) {
		double initial = 1.0;
		if (r.second->get_amount() > 0) {
			initial /= r.second->get_amount();
		}
		value.emplace(std::make_pair(r.second.get(), initial));
	}

	//
	int iterations = 8;
	for (int i = 0; i < iterations; ++i) {
		for (auto &a : action) {
			std::unordered_map<Resource *, double> effect = a.second->get_effects();

			// normalize the effect vector
			double m = 0.0;
			for (auto &e : effect) {
				m += e.second;
			}
			for (auto &e : effect) {
				e.second /= m;
			}

			// apply normalized effects to values
			for (auto &e : effect) {
				value[e.first] *= e.second;
			}
		}

		// 
		for (auto &f : flow) {
			value[f->get_from()] += value[f->get_to()] * f->get_out_flow();
		}
	}

	// save values
	for (auto &v : value) {
		v.first->set_value(v.second);
	}
}

std::vector<std::string> Graph::debug() {
	std::vector<std::string> result;
	for (auto &a : resource) {
		double amount = a.second->get_value();
		result.push_back(a.first + " = " + std::to_string(amount));
	}
	for (auto &a : action) {
		//double score = a.second->get_utility();
		//result.push_back(a.first + " = " + std::to_string(score));
	}
	return result;
}

}
