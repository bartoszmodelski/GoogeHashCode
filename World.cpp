#include "stdafx.h"

#include "World.h"


World::World(string path) {

	ifstream file(path);
	vector<Ride> rides;
	string line;
	

	if (getline(file, line)) 
	{
		istringstream iss(line);
		if (!(iss >> this->height >> this->width >> this->numOfCars >> this->numOfRides >> this->bonus >> this->timespan)) 
		{ 
			cout << "Error reading file. Couldn't parse first line. " << endl;
		}
		else 
		{
			cout << "Parsed header" << endl;
		}
	}
	for (int i = 0; i < this->numOfCars; i++)
		this->cars.push_back(new Car(0, 0, i));


	int counter = 0;
	while (getline(file, line))
	{
		istringstream iss(line);
		int yStart, xStart, yFinish, xFinish, earliestStart, latestFinish;
		if (!(iss >> yStart >> xStart >> yFinish >> xFinish >> earliestStart >> latestFinish))
		{
			break;
		}

		Ride* ride = new Ride(earliestStart, latestFinish, xStart, yStart, xFinish, yFinish, counter);
		this->rides.push_back(ride);
		counter++;
	}
	this->sortRidesByLatestPickup();
	cout << "Parsed " << counter << " rides" << endl;
}

void World::sortRidesByLatestPickup() {
	sort(rides.begin(), rides.end(),
		[](const Ride* a, const Ride* b) -> bool
	{
		return a->earliestPickup < b->earliestPickup;
	});
}


void World::print(bool verbose) {
	cout << "Available cars: " << this->numOfCars << endl;
	cout << "Rides count: " << this->numOfRides << endl;
	cout << "Bonus: " << this->bonus << endl;
	cout << "Timespan: " << this->bonus << endl;
	if (verbose)
	{
		for (Ride* ride : rides) {
			cout << "Ride: " << ride->id << endl;
			cout << " - earliest pickup: " << ride->earliestPickup << endl;
			cout << " - latest finish: " << ride->latestFinish << endl;
			cout << " - distance - " << ride->distance << endl;
			cout << " - latest pickup - " << ride->latestPickup << endl;
			cout << "Coords: (" << ride->xStart << ", " << ride->yStart << ") -> (" << ride->xFinish << ", " << ride->yFinish << ")" << endl;
		}
	}
}

int World::evaluateDecisions(vector<Decision> decisions) {
	unordered_map<Car*, int> unavailableUntil;
	vector<Car*> cars_deepcopy;
	// init map with car availability
	for (auto car : this->cars) {
		auto temp = new Car(car);
		cars_deepcopy.push_back(temp);
		unavailableUntil[temp] = 0;
	}

	// sort decisions chronologically (according to rides)
	sort(decisions.begin(), decisions.end(),
		[](const Decision a, const Decision b) -> bool
	{
		return a.ride->earliestPickup < b.ride->earliestPickup;
	});
	
	int points = 0;
	for (auto decision : decisions) {
		if (decision.skip)
			continue;

		Car* car = cars_deepcopy[decision.car->id];
		Ride* ride = decision.ride;

		if (decision.car->id != car->id) {
			throw "Id do not mach. Internal error";
		}

		int distanceToPickUpPlace = car->getDistanceToLocation(ride->xStart, ride->yStart);
		int totalTimeNeeded = distanceToPickUpPlace + ride->distance;

		// wil the car make it in time?
		int availableFrom = unavailableUntil[car];

		if (availableFrom + totalTimeNeeded < ride->latestFinish) {
			//yeah it will make it

			//updates coordinates
			car->x = ride->xFinish;
			car->y = ride->yFinish;

			//will it make it for the bonus?
			if (availableFrom + distanceToPickUpPlace < ride->earliestPickup) {
				points += this->bonus;
			}
			points += ride->distance;
			unavailableUntil[car] = max(ride->earliestPickup, distanceToPickUpPlace) + ride->distance;
		}

	}

	for (auto car : cars_deepcopy) {
		delete car;
	}


	return points;
}



vector<Ride*> World::getAvailableRides(vector<Decision> previousDecisions) {

	auto allocatedRides = decisionsToRides(previousDecisions);
	sort(allocatedRides.begin(), allocatedRides.end());

	auto availableRides = this->rides;
	for (int i = 0; i < availableRides.size(); i++) {
		if (binary_search(allocatedRides.begin(), allocatedRides.end(), availableRides.at(i))) {
			availableRides.erase(availableRides.begin() + i);
			i--;
		}
	}

	return availableRides;
}

vector<Decision> World::generateDecisions(vector<Decision> previousDecisions) {
	vector<Decision> decisions;

	std::vector<Ride*> availableRides = this->getAvailableRides(previousDecisions);


	sort(availableRides.begin(), availableRides.end(),
		[](const Ride* a, const Ride* b) -> bool
	{
		return a->earliestPickup < b->earliestPickup;
	});


	int counter = 0;
	for (Ride* ride : availableRides) {
		if (counter == 5)
			break;
		for (Car* car : this->cars) {
			bool ignore = rand() % 10 == 9;
			decisions.push_back(Decision(car, ride, ignore));
		}
		counter++;
		
	}

	return decisions;
}
