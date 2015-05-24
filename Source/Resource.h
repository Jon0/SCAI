#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace graph {

class Action;
class Flow;
class Graph;

class Resource {
public:
	Resource(Graph *g, std::string name);
	~Resource();

	/**
	 * use the update function
	 */
	void update();

	/**
	 * adds an amount to the resource
	 */
	void adjust(double a);

	/**
	 * return the stockpiled amount
	 */ 
	double get_amount();

	/**
	 * calculates the value of this resource
	 */
	double get_value();

	void set_value(double v);

	/**
	 * return actions which affect this resource
	 */
	std::vector<Action *> get_effects();

	/**
	 * pass a function returning the current 
	 * amount of this resource
	 */
	void set_update_function(std::function<double()> f);

	void produces(Resource *other);

	void effect_of(Action *a);

	std::string debug();

private:
	Graph *graph;
	double amount, amount_d, value;
	int updates;

	void add_flow_in(Flow *);

	std::function<double()> update_func;

	std::vector<std::unique_ptr<Flow>> flows_out;
	std::vector<Flow *> flows_in;
	std::vector<Action *> effect_action;

};

}