#include "stdafx.h"

#include "small.h"


int getDistance(int x1, int y1, int x2, int y2) {
	return abs(x1 - x2) + abs(y1 - y2);
}

Car::Car(int x, int y, int id) {
	this->x = x;
	this->y = y;
	this->id = id;
}

Car::Car(Car* car) {
	this->x = car->x;
	this->y = car->y;
	this->id = car->id;
}

int Car::getDistanceToLocation(int targetX, int targetY) {
	return getDistance(x, y, targetX, targetY) + 1;
}

Ride::Ride(int earliestPickup, int latestFinish, int xStart, int yStart, int xFinish, int yFinish, int id) {
	this->earliestPickup = earliestPickup;
	this->latestFinish = latestFinish;
	this->xFinish = xFinish;
	this->yFinish = yFinish;
	this->xStart = xStart;
	this->yStart = yStart;
	this->distance = getDistance(xStart, yStart, xFinish, yFinish);
	this->latestPickup = latestFinish - this->distance;
	this->id = id;
};


Decision::Decision(Car* car, Ride* ride, bool skip) {
	this->car = car;
	this->ride = ride;
	this->skip = skip;
}

vector<Ride*> decisionsToRides(vector<Decision> decisions) {
	vector<Ride*> rides;
	for (auto decision : decisions)
		rides.push_back(decision.ride);
	return rides;
}

void saveDecisions(vector<Decision> decisions, string path, int numOfCars) {
	
	map<int, vector<int>> data;

	for (auto decision : decisions) {
		if (!decision.skip)
			data[decision.car->id].push_back(decision.ride->id);
	}
	ofstream submission;
	submission.open(path);
	bool omitEndl = true;
	for (int i = 0; i < numOfCars; i++) {
		if (omitEndl) 
			omitEndl = false;
		else 
			submission << endl;
		

		submission << data[i].size() << " ";


		for (auto rideIndex : data[i]) {
			submission << rideIndex << " ";
		}
	}
	submission.close();
}


CalculationsBackup::CalculationsBackup(vector<Car*> cars, int points, unordered_map<Car*, int> unavailableUntil) {
	this->cars = cars;
	this->points = points;
	this->unavailableUntil = unavailableUntil;
}

CalculationsBackup::~CalculationsBackup() {
	for (auto car : this->cars) {
		delete car;
	}
}