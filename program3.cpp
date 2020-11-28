#include <string>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include<cstdlib>/*Used for the random number generator*/
#include <random>
#include <functional>

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
int* motion(GridBox gridArray[6][5], char direction, int robotLocation);
int randomNumberGen(int maxNum);//Returns the random number
float max(float array[]);
int* findLocation(int robotLocation);
int* findLocation(int* robotLocation);
void trial(int robotLocation, GridBox gridArray[6][5], int moves, int action);
char findAction(GridBox gridArray[6][5], int robotLocation, int iteration);
void updateQValues(GridBox gridArray[6][5], int tuple[5]);
bool compareTo(float A, float B, float epsilon = 0.005f);

int main() {
	srand(time(0)); 
	Robot robot = Robot();
	GridBox gridArray[6][5];
	mt19937::result_type seed = time(0);
	populateGrid(gridArray);
	int i = 0;
	while (i < 10000) {
		cout << "**********Trial " << i+1 << "**********" << endl;
		trial(robot.location, gridArray, 0, -1);
		i++;
	}

	system("Pause");
	return 0;
}

int randomNumberGen(int maxNum)
{
	
	int num;
	
	num = rand() % maxNum;
	
	//mt19937::result_type seed = time(0);
	//auto num = bind(uniform_int_distribution<int>(1, maxNum), mt19937(seed));
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
int* motion(GridBox gridArray[6][5], char action, int state) {
	cout << "Action is: " << action << endl;
	static int tuple[5];
	tuple[0] = state; //'state' value in the tuple
	int directionInt, nextActionInt;
	char direction, nextAction;
	char actualDirection[3];
	int* robotPos;
	robotPos = findLocation(state);
	int i = robotPos[0];
	int j = robotPos[1];
	//This switch statement finds if we end up drifting or not
	switch (action) {
	case 'W':
		actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
		int val;
		val = randomNumberGen(100);
		gridArray[i][j].accessFrequency[0] += 1;
		directionInt = 0; //for tuple
		if (val <= 15) { //0 is north, 1 is south, 2 is west
			direction = 'N';
			cout << "We drifted north\n";
		}
		else if (val > 15 && val <= 30) {
			direction = 'S';
			cout << "We drifted south\n";
		}
		else {
			cout << "We did not drift.\n";
			direction = 'W';
		}
		break;
	case 'N':
		actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
		val = randomNumberGen(100);
		gridArray[i][j].accessFrequency[1] += 1;
		directionInt = 1;
		if (val <= 15) { //0 is west, 1 is east, 2 is north
			direction = 'W';
			cout << "We drifted west\n";
		}
		else if (val > 15 && val <= 30) {
			direction = 'E';
			cout << "We drifted east\n";
		}
		else {
			cout << "We did not drift.\n";
			direction = 'N';
		}
		break;
	case 'E':
		actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
		val = randomNumberGen(100);
		gridArray[i][j].accessFrequency[2] += 1;
		directionInt = 2;
		if (val <= 15) { //0 is north, 1 is south, 2 is east
			direction = 'N';
			cout << "We drifted north\n";
		}

		else if (val > 15 && val <= 30) {
			direction = 'S';
			cout << "We drifted south\n";
		}
		else {
			cout << "We did not drift.\n";
			direction = 'E';
		}
		break;
	case 'S':
		actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
		val = randomNumberGen(100);
		gridArray[i][j].accessFrequency[3] += 1;
		directionInt = 3;
		if (val <= 15) {//0 is west, 1 is east, 2 is south
			direction = 'W';
			cout << "We drifted west\n";
		}
		else if (val > 15 && val <= 30) {
			direction = 'E';
			cout << "We drifted east\n";
		}
		else {
			cout << "We did not drift.\n";
			direction = 'S';
		}
		break;
	}

	int reward = 0;
	double probability[4];
	//This switch statement performs the actual movement
	switch (direction) {
	case 'W':
		reward = -2; //for tuple
		if (j != 0) {//if this is not the edge of the maze
			if (gridArray[i][j - 1].isObstacle == 0) {
				state -= 1; //moving the robot
			}
		}
		break;
	case 'N':
		reward = -3;
		if (i != 0) {//if this is not the edge of the maze
			if (gridArray[i - 1][j].isObstacle == 0) {
				state -= 5; //moving the robot
			}
		}
		break;
	case 'E':
		reward = -2;
		if (j != 4) {//if this is not the edge of the maze
			if (gridArray[i][j + 1].isObstacle == 0) {
				state += 1; //moving the robot
			}
		}
		break;
	case 'S':
		reward = -1;
		if (i != 5) {//if this is not the edge of the maze
			if (gridArray[i + 1][j].isObstacle == 0) {
				state += 5; //moving the robot
			}
		}
		break;
	}

	nextAction = findAction(gridArray, state, 1);

	switch (nextAction) {
	case 'W':
		nextActionInt = 0;
		break;
	case 'N':
		nextActionInt = 1;
		break;
	case 'E':
		nextActionInt = 2;
		break;
	case 'S':
		nextActionInt = 3;
		break;
	}
	tuple[1] = directionInt; //current action in tuple
	tuple[2] = reward; //reward in tuple
	tuple[3] = state; //next state in tuple
	tuple[4] = nextActionInt; //next action in tuple
	//so by this time we have a tuple with current state, current action, reward, next state, and next action. This gets us ready for Q-learning
	return tuple;
}

void trial(int robotLocation, GridBox gridArray[6][5], int moves = 0, int action = -1) {
	bool obstacleFound = 1;
	int startingSquare;
	char direction;
	int* tuple;
	if (moves == 0) { //we have to spawn the robot
		while (obstacleFound) {
			startingSquare = randomNumberGen(30) + 1;
			int* squareInGrid; //this is for translating the integer value into the array

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
	if (action == -1)
		direction = findAction(gridArray, robotLocation, 0);
	else {
		switch (action) {
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
		}
	}
	//TODO: set a tuple variable to the result of motion so we can update the q-values
	cout << "//////////Motion " << moves << "//////////" << endl << endl;
	tuple = motion(gridArray, direction, robotLocation);
	robotLocation = tuple[3];
	//TODO: function that updates q-values
	updateQValues(gridArray, tuple);
	if (moves != 100) {//base case
		trial(robotLocation, gridArray, ++moves, tuple[4]);
	}
}

/*
	findAction
	purpose: to determine whether the robot will follow the optimal policy or choose a random action
		gridArray: the grid that determines the qValues for the optimal policy
		robotLocation: the robot's location in integer value
*/
char findAction(GridBox gridArray[6][5], int robotLocation, int iteration) {
	int value = randomNumberGen(100);
	char direction;
	int* robotPos;
	if (value <= 5) { //this is our implementation of epsilon-greedy reinforcement learning. 5% chance the robot chooses a random action rather than the optimal policy
		value = randomNumberGen(100);
		cout << "We chose a random direction!\n";
		if (value <= 25)
			direction = 'W';
		else if (value > 25 && value <= 50)
			direction = 'N';
		else if (value > 50 && value <= 75)
			direction = 'E';
		else
			direction = 'S';
	}
	else {

		robotPos = findLocation(robotLocation);
		int i = robotPos[0];
		int j = robotPos[1];
		cout << "gridArray[" << i << "][" << j << "] qvalues: " << gridArray[i][j].qValue[0] << " " << gridArray[i][j].qValue[1] << " " << gridArray[i][j].qValue[2] << " " << gridArray[i][j].qValue[3] << endl;
		int maxQValues[4];
		int index = -1;
		int maxCount = 0;
		float maxQValue = max(gridArray[i][j].qValue);
		for (int k = 0; k < 4; k++) {
			if (compareTo(maxQValue, gridArray[i][j].qValue[k])) {
				maxQValues[maxCount] = k;
				maxCount++;
			}
		}
		if (maxCount > 1) {
			value = randomNumberGen(maxCount) + 1;
			index = maxQValues[value - 1];
			cout << "Index is: " << index << endl;
		}
		else
			index = maxQValues[0];
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
		default:
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

int* findLocation(int* location) {
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
	int* currentState = findLocation(tuple[0]); //finds robot's current location
	cout << "currentState: " << currentState[0] << ' ' << currentState[1] << ": " << tuple[0] << endl;
	int i = currentState[0]; int j = currentState[1];
	int currentAction = tuple[1];
	int reward = tuple[2];
	int* nextState = findLocation(tuple[3]);
	cout << "nextState: " << nextState[0] << ' ' << nextState[1] << ": " << tuple[3] << endl;
	int nextAction = tuple[4];
	int k = nextState[0]; int l = nextState[1];
	cout << "Current action is " << tuple[1] << " and next action is " << tuple[4] << endl;
	cout << "2.The value of the access Frequency before : " << gridArray[i][j].accessFrequency[0] << " " << gridArray[i][j].accessFrequency[1] << " " << gridArray[i][j].accessFrequency[2] << " " << gridArray[i][j].accessFrequency[3] << endl;
	gridArray[i][j].qValue[currentAction] = (1 / (gridArray[i][j].accessFrequency[currentAction]*1.0)) * (reward + .9 * max(gridArray[k][l].qValue) - gridArray[i][j].qValue[currentAction]);
	cout << "gridArray[" << i << "][" << j << "] qvalues: " << gridArray[i][j].qValue[0] << " " << gridArray[i][j].qValue[1] << " " << gridArray[i][j].qValue[2] << " " << gridArray[i][j].qValue[3] << endl;
}

float max(float array[]) {
	float max = array[0];
	for (int i = 1; i < sizeof(array); i++) {
		if (array[i] > max)
			max = array[i];
	}
	return max;
}

bool compareTo(float A, float B, float epsilon)
{
	return (fabs(A - B) < epsilon);
}
