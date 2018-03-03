#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include <iterator>

#include "small.h"


using namespace std;

class World {
public:
	vector<Ride*> rides;
	vector<Car*> cars;
	int width, height, bonus, numOfCars, numOfRides, timespan;
	World(string path);
	void print(bool verbose);

	void sortRidesByLatestPickup();

	int evaluateDecisions(vector<Decision> decisions);
	vector<Decision> generateDecisions(vector<Decision> previousDecisions);
	vector<Ride*> getAvailableRides(vector<Decision> previousDecisions);

	//prepare for steps
	vector<Decision> decisionsTaken;
};

