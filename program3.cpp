#include <string>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include<cstdlib>/*Used for the random number generator*/

using namespace std;

class GridBox {
public: //attributes
	double probability;
	double transProbability;
	int position;
	bool isObstacle;
	float qValue[4];
	int accessFrequency[4];

	GridBox() { //constructor
		this->probability = 4.17;
		this->transProbability = 0;
		this->position = -1;
		this->isObstacle = 0;
		for (int i = 0; i < 4; i++) {
			this->qValue[i] = { 0 };
			this->accessFrequency[i] = { 0 };
		}
	}
};

class Robot {
public:
	int location;
	int reward;

	Robot() {
		this->location = -1;
		this->reward = 0;
	}
};

void populateGrid(GridBox gridArray[6][5]);
void displayGrid(GridBox gridArray[6][5]);
int *motion(GridBox gridArray[6][5], char direction, int robotLocation);
int randomNumberGen(int maxNum);//Returns the random number
float max(float array[]);
int* findLocation(int robotLocation);
int* findLocation(int *robotLocation);
void trial(int robotLocation, GridBox gridArray[6][5], int moves);
char findAction(GridBox gridArray[6][5], int robotLocation);
void updateQValues(GridBox gridArray[6][5], int tuple[5]);

int main() {
	Robot robot = Robot();
	GridBox gridArray[6][5];
	populateGrid(gridArray);
	int i = 0;
	while (i < 10000) {
		trial(robot.location, gridArray, 0);
		i++;
	}

	system("Pause");
	return 0;
}

int randomNumberGen(int maxNum)
{
	int num;
	srand(time(0));
	num = rand() % (maxNum + 1);
	return num;
}

void populateGrid(GridBox gridArray[6][5]) {
	int currentPosition = 1;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			gridArray[i][j].position = currentPosition;
			currentPosition++;
		}
	}
	gridArray[1][1].isObstacle = 1; gridArray[1][2].isObstacle = 1;
	gridArray[2][1].isObstacle = 1;
	gridArray[3][1].isObstacle = 1; gridArray[3][2].isObstacle = 1;
	gridArray[4][1].isObstacle = 1;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			if (gridArray[i][j].isObstacle == 1)
				gridArray[i][j].probability = -1;
		}
	}

}

void displayGrid(GridBox gridArray[6][5]) {
	for (int i = 0; i < 6; i++) { //access frequency table
		for (int j = 0; j < 5; j++) {
			if (gridArray[i][j].isObstacle == 1) {
				cout << "####" << "  ";
			}
			else {
				cout << "   " << gridArray[i][j].accessFrequency[1] << setw(20);
			}
			if ((gridArray[i][j].position % 5) == 0)
				cout << std::endl;
		}
	}
	for (int i = 0; i < 6; i++) { //q-value table
		for (int j = 0; j < 5; j++) {
			if (gridArray[i][j].isObstacle == 1) {
				cout << "####" << "  ";
			}
			else
				cout << fixed << setprecision(2) << gridArray[i][j].probability << "  ";

			if ((gridArray[i][j].position % 5) == 0)
				cout << std::endl;
		}
	}
	for (int i = 0; i < 6; i++) { //optimal policy table
		for (int j = 0; j < 5; j++) {
			if (gridArray[i][j].isObstacle == 1) {
				cout << "####" << "  ";
			}
			else
				cout << fixed << setprecision(2) << gridArray[i][j].probability << "  ";

			if ((gridArray[i][j].position % 5) == 0)
				cout << std::endl;
		}
	}
}


/* Purpose: handles the robot's motion as well as prediction after the motion
	gridArray: the array that stores values of squares
	direction: the direction the robot is moving in
	*/
int *motion(GridBox gridArray[6][5], char action, int state) {
	cout << "Action is: " << action;
	static int tuple[5];
	tuple[0] = state; //'state' value in the tuple
	int val, directionInt, nextActionInt;
	char direction, nextAction;
	char actualDirection[3];
	//This switch statement finds if we end up drifting or not
	switch (action) {
	case 'W':
		actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
		val = randomNumberGen(100);
		directionInt = 1; //for tuple
		if (val <= 15) //0 is north, 1 is south, 2 is west
			direction = 'N';
		else if (val > 15 && val <= 30)
			direction = 'S';
		else {
			cout << "We did not drift.\n";
			direction = 'W';
		}
		break;
	case 'N':
		actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
		val = randomNumberGen(100);
		directionInt = 2;
		if (val <= 15) //0 is west, 1 is east, 2 is north
			direction = 'W';
		else if (val > 15 && val <= 30)
			direction = 'E';
		else {
			cout << "We did not drift.\n";
			direction = 'N';
		}
		break;
	case 'E':
		actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
		val = randomNumberGen(100);
		directionInt = 3;
		if (val <= 15)  //0 is north, 1 is south, 2 is east
			direction = 'N';
		else if (val > 15 && val <= 30)
			direction = 'S';
		else {
			cout << "We did not drift.\n";
			direction = 'E';
		}
		break;
	case 'S':
		actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
		val = randomNumberGen(100);
		directionInt = 4;
		if (val <= 15) //0 is west, 1 is east, 2 is south
			direction = 'W';
		else if (val > 15 && val <= 30)
			direction = 'E';
		else{
			cout << "We did not drift.\n";
			direction = 'S';
		}
		break;
	}
	int reward = 0;
	double probability[4];
	int* robotPos;
	robotPos = findLocation(state);
	int i = robotPos[0];
	int j = robotPos[1];
	cout << robotPos[0] << "   " << robotPos[1];
	//This switch statement performs the actual movement
	switch (direction) {
		//Translate robot location to row and column of grid

	case 'W':
		gridArray[i][j].accessFrequency[0] += 1;
		reward = -2; //for tuple
		if (gridArray[i][j - 1].isObstacle == 0) {
			state -= 1; //moving the robot
		}
		nextAction = findAction(gridArray, state);
		break;
	case 'N':
		gridArray[i][j].accessFrequency[1] += 1;
		reward = -3;
		if (gridArray[i - 1][j].isObstacle == 0) {
			state -= 5; //moving the robot
		}
		nextAction = findAction(gridArray, state);
		break;
	case 'E':
		gridArray[i][j].accessFrequency[2] += 1;
		reward = -2;
		if (gridArray[i][j + 1].isObstacle == 0) {
			state += 1; //moving the robot
		}
		nextAction = findAction(gridArray, state);
		break;
	case 'S':
		gridArray[i][j].accessFrequency[3] += 1;
		reward = -1;
		if (gridArray[i + 1][j].isObstacle == 0) {
			state += 5; //moving the robot
		}
		nextAction = findAction(gridArray, state);
		break;
	}
	switch (nextAction) {
	case 'W':
		nextActionInt = 1;
		break;
	case 'N':
		nextActionInt = 2;
		break;
	case 'E':
		nextActionInt = 3;
		break;
	case 'S':
		nextActionInt = 4;
		break;
	}
		tuple[1] = directionInt; //current action in tuple
		tuple[2] = reward; //reward in tuple
		tuple[3] = state; //next state in tuple
		tuple[4] = nextActionInt; //next action in tuple
		//so by this time we have a tuple with current state, current action, reward, next state, and next action. This gets us ready for Q-learning
	return tuple;
}

void trial(int robotLocation, GridBox gridArray[6][5], int moves = 0) {
	bool obstacleFound = 1;
	int startingSquare;
	char direction;
	int *tuple;
	if (moves == 0) { //we have to spawn the robot
		while (obstacleFound) {
			startingSquare = randomNumberGen(30);
			int *squareInGrid; //this is for translating the integer value into the array

			squareInGrid = findLocation(startingSquare);

			int i = squareInGrid[0];
			int j = squareInGrid[1];
			if (gridArray[i][j].isObstacle == 0) {
				obstacleFound = 0;
			}
		}
		moves = 1;
		robotLocation = startingSquare;
	}
	direction = findAction(gridArray, robotLocation);
	//TODO: set a tuple variable to the result of motion so we can update the q-values
	tuple = motion(gridArray, direction, robotLocation);
	//TODO: function that updates q-values
	updateQValues(gridArray, tuple);
	if (moves != 101) {//base case
		trial(robotLocation, gridArray, ++moves);
	}
}

/*
	findAction
	purpose: to determine whether the robot will follow the optimal policy or choose a random action
		gridArray: the grid that determines the qValues for the optimal policy
		robotLocation: the robot's location in integer value
*/
char findAction(GridBox gridArray[6][5], int robotLocation) {
	int value = randomNumberGen(100);
	char direction;
	int *robotPos;
	if (value <= 5) { //this is our implementation of epsilon-greedy reinforcement learning. 5% chance the robot chooses a random action rather than the optimal policy
		value = randomNumberGen(4);
		cout << "We chose a random direction!\n";
		switch (value) {
			case 1: //choose west
				direction = 'W';
				break;
			case 2: //choose north
				direction = 'N';
				break;
			case 3: //choose east
				direction = 'E'; 
				break;
			case 4: //choose south
				direction = 'S';
				break;
		}
	}
	else {
		robotPos = findLocation(robotLocation);
		int i = robotPos[0];
		int j = robotPos[1];
		int maxQValues[4];
		int index = -1;
		int maxCount = 0;
		float maxQValue = max(gridArray[i][j].qValue);
		//TODO: this sometimes gives an incredibly negative Index and we need to figure out why
		for (int k = 0; k < 4; k++) {
			if (maxQValue == gridArray[i][j].qValue[k]) {
				maxQValues[maxCount] = k;
				maxCount++;
			}
		}
		cout << "Why the hell is maxCount " << maxCount << endl;
		if (maxCount > 1) {
			value = randomNumberGen(maxCount);
			index = maxQValues[value - 1];
			cout << "Index is: " << index << endl;
		}
		else
			index = maxQValues[0];
		cout << "NOW Index is: " << index << endl;
		switch (index) {
			case 0:
				direction = 'W';
				break;
			case 1:
				direction = 'N';
				break;
			case 2:
				direction = 'E';
				break;
			case 3:
				direction = 'S';
				break;
			case -1:
				cout << "The direction did not match any cases.\n";
				break;
		}
	}
	return direction;
}

int* findLocation(int location) {
	static int position[2];
	if (location % 5 != 0) {
		position[0] = (location / 5); //row
		position[1] = (location % 5) - 1; //column
	}
	else {
		position[0] = (location / 5) - 1;
		position[1] = 4;
	}
	
	return position;
}

int* findLocation(int *location) {
	int foundLocation = *location;
	static int position[2];
	//in case things get wonky it is possible there is an issue here
	if (foundLocation % 5 != 0) {
		position[0] = (foundLocation / 5); //row
		position[1] = (foundLocation % 5) - 1; //column
	}
	else {
		position[0] = (foundLocation / 5) - 1;
		position[1] = 4;
	}

	return position;
}

/*
	updateQValues
	purpose: updates the q values of the box in the grid after the robot takes its action
		gridArray: grid that contains boxes the robot traverses
		tuple: contains information for updating q-values; 0-current state, 1-current action, 2-reward, 3-next state, 4-next action
*/
void updateQValues(GridBox gridArray[6][5], int tuple[5]) {
	int *currentState = findLocation(tuple[0]); //finds robot's current location
	int currentAction = tuple[1];
	int reward = tuple[2];
	int *nextState = findLocation(tuple[3]);
	int *nextStateLocation = findLocation(nextState);
	int nextAction = tuple[4];
	int i = currentState[0]; int j = currentState[1]; int k = nextStateLocation[0]; int l = nextStateLocation[1];
	gridArray[i][j].qValue[currentAction - 1] = (1 / gridArray[i][j].accessFrequency[currentAction - 1]) * (reward + .9 * max(gridArray[k][l].qValue) - gridArray[i][j].qValue[currentAction - 1]);

}

float max(float array[]) {
	float max = array[0];
	for (int i = 1; i < sizeof(array); i++) {
		if (array[i] > max)
			max = array[i];
	}
	return max;
}
