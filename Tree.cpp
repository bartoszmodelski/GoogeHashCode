#include "stdafx.h"

#include "Tree.h"
#include "small.h"



DecisionNode::DecisionNode(vector<Decision> decisionsLeadingHere){
	this->decisionsLeadingHere = decisionsLeadingHere;
	this->servedRides = decisionsToRides(decisionsLeadingHere);
	score = -1;
}

void DecisionNode::generateChildren(World world, int depth, float branchingFactor, float decreaseInBranchingFactor) {
	
	if (depth == 0)
		return;

	auto decisions = world.generateDecisions(this->decisionsLeadingHere);

	if (decisions.size() == 0)
		return;

	if (branchingFactor == -1) {
		for (auto &decision : decisions) {
			vector<Decision> decisionsForChild = decisionsLeadingHere;
			decisionsForChild.push_back(decision);
			DecisionNode child(decisionsForChild);
			child.generateChildren(world, depth - 1, -1, 0);
			this->children.push_back(child);
		}
	}
	else 
	{
		for (int i = 0; i < round(branchingFactor); i++) {
			int index = rand() % decisions.size();
			vector<Decision> decisionsForChild = decisionsLeadingHere;
			decisionsForChild.push_back(decisions.at(index));
			DecisionNode child(decisionsForChild);
			child.generateChildren(world, depth - 1, max(1.0f, branchingFactor - decreaseInBranchingFactor), decreaseInBranchingFactor);
			this->children.push_back(child);
		}
	}


}


void DecisionNode::evaluateLeafNodes(World world) {
	
	if (this->children.size() != 0) {
		for (auto &child : this->children) {
			child.evaluateLeafNodes(world);
		}
		return;
	}

	auto score = world.evaluateDecisions(this->decisionsLeadingHere);
	//this->saveToFile(score, world);
	this->score = score;

	//cout << score << endl;
}

void DecisionNode::saveToFile(World world, string name) {

	string path = "C:/submissions/" + name;
	saveDecisions(this->decisionsLeadingHere, path, world.numOfCars);

}

DecisionNode* DecisionNode::findHighestScore() {
	if (this->children.size() != 0) {
		int topScore = children.at(0).score;
		DecisionNode* topChild = &children.at(0);
		for (auto &child : this->children) {
			auto buffer = child.findHighestScore();
			if (buffer->score > topScore) {
				topScore = buffer->score;
				topChild = buffer;
			}
		}
		return topChild;
	}
	else 
	{
		return this;
	}
}

DecisionNode DecisionNode::findDirectChildWithHighestScore() {
	if (children.size() == 0)
		return *this;

	int topScore = children.at(0).score;
	DecisionNode topChild = children.at(0);
	for (auto child : this->children) {
		auto buffer = child.findHighestScore();
		if (buffer->score > topScore) {
			topScore = buffer->score;
			topChild = child;
		}
	}
	return topChild;
}