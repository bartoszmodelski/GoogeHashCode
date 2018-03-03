// cashhode.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <time.h>

#include "Tree.h"

using namespace std;

int main()
{
	srand(time(NULL));

	World world("C:/datasets/c.in");
	world.sortRidesByLatestPickup();
	world.print(false);
	//world.print();


	/*
	auto decisions = world.generateDecisions(vector<Decision>());

	cout << "Generated decisions: " << endl;
	for (Decision decision : decisions) {
		cout << "Car id: " << decision.car->id << " handles ride id: " << decision.ride->id << endl;

		auto taken = vector<Decision>();
		taken.push_back(decision);
		cout << "Score: " << world.evaluateDecisions(taken) << endl;
	}*/

	auto path = vector<Decision>();
	int counter = 0;
	// iterate until not all rides are allocated
	while (true) {
		counter++;

		DecisionNode root(world.decisionsTaken);
		root.generateChildren(world, 3, 3, 0);
		root.evaluateLeafNodes(world);


		auto bestProspect = root.findDirectChildWithHighestScore();



		world.decisionsTaken.push_back(bestProspect.decisionsLeadingHere.back());

		cout << "Iteration: " << counter << endl;
		//cout << "Current best: " << world.evaluateDecisions(bestProspect.decisionsLeadingHere) << endl;

		//if (world.getAvailableRides(root.decisionsLeadingHere).size() == 0) {
		if (counter == 5000) {
			cout << "Saving. Unallocated rides: " << world.getAvailableRides(root.decisionsLeadingHere).size() << endl;
			DecisionNode buffer(world.decisionsTaken);
			root.saveToFile(world, "final" + to_string(world.evaluateDecisions(world.decisionsTaken)));
			break;
		}

	}
	getchar();

    return 0;
}

