#pragma once

#include <vector>
#include <stdlib.h>
#include <algorithm>
#include "small.h"
#include "World.h"


class DecisionNode {
public:
	vector<Ride*> servedRides;
	vector<Decision> decisionsLeadingHere;
	std::vector<DecisionNode> children;
	DecisionNode(vector<Decision> decisionsLeadingHere);
	void generateChildren(World world, int depth, float branchingFactor, float decreaseInBranchingFactor);
	void evaluateLeafNodes(World world);
	void saveToFile(World world, string name);
	DecisionNode findDirectChildWithHighestScore();
	int score;
	DecisionNode* findHighestScore();

};


