#pragma once

#include <cstdlib>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

class Car {
public:
	int x, y, id;
	Car(int x, int y, int id);
	Car(Car* car);
	int getDistanceToLocation(int targetX, int targetY);
};


class Ride {
public:
	int earliestPickup, latestPickup, latestFinish;
	int xStart, yStart;
	int xFinish, yFinish;
	int distance, id;
	Ride(int earliestPickup, int latestFinish, int xStart, int yStart, int xFinish, int yFinish, int id);
};

class Decision {
public:
	Car* car;
	Ride* ride;
	bool skip;
	Decision(Car* car, Ride* ride, bool skip);
};

class CalculationsBackup {
	vector<Car*> cars;
	int points;
	unordered_map<Car*, int> unavailableUntil;
	CalculationsBackup(vector<Car*> cars, int points, unordered_map<Car*, int> unavailableUntil);
	~CalculationsBackup();
};

vector<Ride*> decisionsToRides(vector<Decision> decisions);
void saveDecisions(vector<Decision> decisions, string path, int numOfCars);