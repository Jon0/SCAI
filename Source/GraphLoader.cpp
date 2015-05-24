#include "Action.h"
#include "EventHandler.h"
#include "Graph.h"
#include "GraphLoader.h"
#include "Resource.h"

using namespace BWAPI;

namespace graph {

bool create_unit(UnitType type) {
	if (type.isBuilding()) {
		for (auto &u : Broodwar->self()->getUnits()) {

			// Retrieve a unit that is capable of constructing the building
			if (u->getType() == type.whatBuilds().first &&
				(u->isIdle() || u->isGatheringMinerals())) {

				TilePosition targetBuildLocation = Broodwar->getBuildLocation(type, u->getTilePosition());
				if (targetBuildLocation) {

					// Register an event that draws the target build location
					Broodwar->registerEvent([targetBuildLocation, type](Game*) {
						Broodwar->drawBoxMap(Position(targetBuildLocation),
							Position(targetBuildLocation + type.tileSize()),
							Colors::Blue);
						},
						nullptr,  // condition
						type.buildTime() + 100);  // frames to run

					// Order the builder to construct the supply structure
					Broodwar->sendText("Creating %s", type.c_str());
					u->build(type, targetBuildLocation);
					return true;
				}
			}
		}
	}
	else {

		// Train the unit if the provider is not a structure
		for (auto &u : Broodwar->self()->getUnits()) {
			if (u->isIdle() && u->train(type)) {
				Broodwar->sendText("Creating %s", type.c_str());
				return true;
			}
		}
	}
	return false;
}

double unit_count(UnitType type) {
	double count = 0;
	for (auto &u : Broodwar->self()->getUnits()) {
		if (u->getType() == type) {
			count++;
		}
	}
	return count;
}

void init_starcraft() {

	// setup the graph with
	// specific starcaft info
	Graph::initialise();
	Graph *g = Graph::get();

	g->add_resource("Minerals");
	Resource *minerals = g->get_resource("Minerals");
	minerals->set_update_function([] {
		return Broodwar->self()->minerals();
	});

	g->add_resource("Gas");
	Resource *gas = g->get_resource("Gas");
	gas->set_update_function([] {
		return Broodwar->self()->gas();
	});

	g->add_resource("Supply");
	Resource *supply = g->get_resource("Supply");
	supply->set_update_function([] {
		return Broodwar->self()->supplyTotal() - Broodwar->self()->supplyUsed();
	});

	g->set_win_func([g](ResourceSet r) {
		return r.get_amount(g->get_resource("Minerals"));
	});

	// players race
	Race race = Broodwar->self()->getRace();
	std::vector<UnitType> types;
	types.push_back(race.getWorker());
	types.push_back(race.getCenter());
	types.push_back(race.getSupplyProvider());
	types.push_back(race.getRefinery());

	// create resources to count each unit type
	std::unordered_map<UnitType *, Resource *> type_resource;
	for (auto &t : types) {
		std::string type_name = std::string(t.c_str());

		// create and insert in map
		g->add_resource(type_name);
		Resource *type_r = g->get_resource(type_name);
		type_resource.emplace(std::make_pair(&t, type_r));

		// tracking count of the unit
		type_r->set_update_function([t] {
			return unit_count(t);
		});

		// action to create
		std::string create_action_name = "Create " + type_name;
		g->add_action(create_action_name);
		Action *a = g->get_action(create_action_name);
		a->add_effect(type_r, 1);
		a->add_effect(minerals, -t.mineralPrice(), true);
		a->add_effect(gas, -t.gasPrice(), true);
		a->add_effect(supply, t.supplyProvided() - t.supplyRequired(), true);
		a->set_event([t]() {
			return create_unit(t);
		});
	}

	// flow rules created after
	for (auto &t : type_resource) {

		if (t.first->canProduce()) {
			t.second->produces(minerals);
			t.second->produces(gas);
		}
	}

	// workers->produces(minerals);
	// workers->produces(gas);
	// supplyUnit->produces(supply);

}

}
