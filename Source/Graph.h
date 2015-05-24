#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ResourceSet.h"

namespace graph {

class Action;
class Flow;
class Resource;

class Graph {
public:
	~Graph();

	static void initialise();
	
	/**
	 * returns the static instance
	 */
	static Graph *get();

	void update();

	// actions
	void add_action(std::string name);
	Action *get_action(std::string name);

	// resources
	void add_resource(std::string name);
	Resource *get_resource(std::string name);

	std::unordered_map<Resource *, double> get_amounts();

	void set_win_func(std::function<double(ResourceSet)> f);

	std::vector<std::string> debug();

private:
	Graph();

	void update_actions();
	void update_resource_values();
	void register_flow(Flow *f);
	
	static Graph *instance;

	std::vector<Edge *> edge;
	std::vector<Flow *> flow;
	std::unordered_map<std::string, std::unique_ptr<Action>> action;
	std::unordered_map<std::string, std::unique_ptr<Resource>> resource;

	std::function<double(ResourceSet)> win_func;

	friend class Resource;

};

}
