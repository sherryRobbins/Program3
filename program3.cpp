#include <string>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include<cstdlib>/*Used for the random number generator*/

using namespace std;

const float GAMMA = .9;

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
	populateGrid(gridArray);
	int i = 0;
	while (i < 10000) {
		trial(robot.location, gridArray, 0, -1);
		i++;
	}
	displayGrid(gridArray);
	system("Pause");
	return 0;
}
/*
* Purpose:
*/
int randomNumberGen(int maxNum) {
	int num;
	num = rand() % maxNum;
	return num;
}
/*
* Purpose:
*/
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

	for (int i = 0; i < 4; i++) {
		gridArray[2][2].qValue[i] = 100;
	}

}
/*
* Purpose:
*/
void displayGrid(GridBox gridArray[6][5]) {
	int value, index, maxCount = 0, maxQValues[4];
	cout << "Table of N(s, a)" << endl;
	for (int i = 0; i < 6; i++) { //q-value table
		for (int k = 0; k < 3; k++) {
			for (int j = 0; j < 5; j++) {
				switch (k) {
				case 0: //printing northern values
					switch (j) {
					case 0:
						if (gridArray[i][j].isObstacle == 1)
							cout << right << setw(7);
						else
							cout << right << setw(7) << gridArray[i][j].accessFrequency[1];
						break;
					case 1:
						if (gridArray[i][j].isObstacle == 1)
							cout << right << setw(18) << "" << setw(18);
						else
							cout << right << setw(18) << gridArray[i][j].accessFrequency[1];
						break;
					case 2:
						if (gridArray[i][j].isObstacle == 1)
							cout << right << setw(18) << "" << setw(18) << " ";
						else
							cout << right << setw(18) << gridArray[i][j].accessFrequency[1] << setw(18) << " ";
						break;
					case 3:
						if (gridArray[i][j].isObstacle == 1)
							cout << left << setw(18) << " ";
						else
							cout << left << setw(18) << gridArray[i][j].accessFrequency[1];
						break;
					case 4:
						if (gridArray[i][j].isObstacle == 1)
							cout << left << setw(7) << " " << endl;
						else
							cout << left << setw(7) << gridArray[i][j].accessFrequency[1] << endl;
						break;
					}
					break;
				case 1: //printing west and east values
					if (gridArray[i][j].isObstacle == 1)
						cout << left << setw(6) << "    ####    " << setw(6) << right << "      ";
					else if (j == 4)
						cout << left << setw(7) << gridArray[i][j].accessFrequency[0] << setw(8) << right << gridArray[i][j].accessFrequency[2] << "            " << endl;
					else
						cout << left << setw(6) << gridArray[i][j].accessFrequency[0] << setw(6) << right << gridArray[i][j].accessFrequency[2] << "       ";
					break;
				case 2: //printing south values
					switch (j) {
					case 0:
						if (gridArray[i][j].isObstacle == 1)
							cout << right << setw(7);
						else
							cout << right << setw(7) << gridArray[i][j].accessFrequency[3];
						break;
					case 1:
						if (gridArray[i][j].isObstacle == 1)
							cout << right << setw(18) << " " << setw(18);
						else
							cout << right << setw(18) << gridArray[i][j].accessFrequency[3];
						break;
					case 2:
						if (gridArray[i][j].isObstacle == 1)
							cout << right << setw(18) << "" << setw(18) << "";
						else
							cout << right << setw(18) << gridArray[i][j].accessFrequency[3] << setw(18) << " ";
						break;
					case 3:
						if (gridArray[i][j].isObstacle == 1)
							cout << left << setw(18) << " ";
						else
							cout << left << setw(18) << gridArray[i][j].accessFrequency[3];
						break;
					case 4:
						if (gridArray[i][j].isObstacle == 1)
							cout << left << setw(7) << " " << endl;
						else
							cout << left << setw(7) << gridArray[i][j].accessFrequency[3] << endl;
						break;
					}
					break;
				}
			}

			cout << endl;

		}
	}
	cout << "Table of Q(s, a):" << endl;
	for (int i = 0; i < 6; i++) { //q-value table
		for (int k = 0; k < 3; k++) {
			for (int j = 0; j < 5; j++) {
				switch (k) {
				case 0: //printing northern values
					switch (j) {
					case 0:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << right << setw(10);
						else
							cout << right << setw(10) << fixed << setprecision(2) << gridArray[i][j].qValue[1];
						break;
					case 1:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << right << setw(20) << "" << setw(18);
						else
							cout << right << setw(20) << fixed << setprecision(2) << gridArray[i][j].qValue[1];
						break;
					case 2:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << right << setw(20) << "" << setw(18) << " ";
						else
							cout << right << setw(20) << fixed << setprecision(2) << gridArray[i][j].qValue[1] << setw(18) << " ";
						break;
					case 3:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << left << setw(20) << " ";
						else
							cout << left << setw(20) << fixed << setprecision(2) << gridArray[i][j].qValue[1];
						break;
					case 4:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << left << setw(10) << " " << endl;
						else
							cout << left << setw(10) << fixed << setprecision(2) << gridArray[i][j].qValue[1] << endl;
						break;
					}
					break;
				case 1: //printing west and east values
					if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13) {
						if (gridArray[i][j].position == 13)
							cout << left << setw(8) << "    +100    " << setw(8) << right << "      ";
						else
							cout << left << setw(8) << "    ####    " << setw(8) << right << "      ";
					}
					else if (j == 4)
						cout << left << setw(6) << fixed << setprecision(2) << gridArray[i][j].qValue[0] << setw(9) << fixed << setprecision(2) << right << gridArray[i][j].qValue[2] << "           " << endl;
					else
						cout << left << setw(6) << fixed << setprecision(2) << gridArray[i][j].qValue[0] << setw(9) << fixed << setprecision(2) << right << gridArray[i][j].qValue[2] << "      ";
					break;
				case 2: //printing south values
					switch (j) {
					case 0:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << right << setw(10);
						else
							cout << right << setw(10) << fixed << setprecision(2) << gridArray[i][j].qValue[3];
						break;
					case 1:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << right << setw(20) << " " << setw(18);
						else
							cout << right << setw(20) << fixed << setprecision(2) << gridArray[i][j].qValue[3];
						break;
					case 2:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << right << setw(20) << "" << setw(18) << "";
						else
							cout << right << setw(20) << setprecision(2) << gridArray[i][j].qValue[3] << setw(18) << " ";
						break;
					case 3:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << left << setw(20) << " ";
						else
							cout << left << setw(20) << fixed << setprecision(2) << gridArray[i][j].qValue[3];
						break;
					case 4:
						if (gridArray[i][j].isObstacle == 1 || gridArray[i][j].position == 13)
							cout << left << setw(10) << " " << endl;
						else
							cout << left << setw(10) << fixed << setprecision(2) << gridArray[i][j].qValue[3] << endl;
						break;
					}
					break;
				}
			}

			cout << endl;

		}
	}
	cout << "Table of the optimal policy :" << endl;
	for (int i = 0; i < 6; i++) { //optimal policy table
		for (int j = 0; j < 5; j++) {
			if (gridArray[i][j].isObstacle == 1) {
				cout << "####" << "  ";
			}
			else {
				float maxQValue = max(gridArray[i][j].qValue);
				for (int k = 0; k < 4; k++) {
					if (compareTo(maxQValue, gridArray[i][j].qValue[k])) {
						index = k;
					}
				}
				if (gridArray[i][j].position == 13) {
					cout << "+100  ";
				}
				else {
					switch (index) {
					case 0:
						cout << "<<<<  ";
						break;
					case 1:
						cout << "^^^^  ";
						break;
					case 2:
						cout << ">>>>  ";
						break;
					case 3:
						cout << "VVVV  ";
						break;
					default:
						cout << "The direction did not match any cases.\n";
						break;
					}
				}
			}
			if ((gridArray[i][j].position % 5) == 0)
				cout << endl;
		}
	}
}


/* Purpose: handles the robot's motion as well as prediction after the motion
	gridArray: the array that stores values of squares
	direction: the direction the robot is moving in
	*/
int* motion(GridBox gridArray[6][5], char action, int state) {

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
			//cout << "We drifted north" << endl;
		}
		else if (val > 15 && val <= 30) {
			direction = 'S';
			//cout << "We drifted south" << endl;
		}
		else {
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
			//cout << "We drifted west" << endl;
		}
		else if (val > 15 && val <= 30) {
			direction = 'E';
			//cout << "We drifted east" << endl;
		}
		else {
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
			//cout << "We drifted north" << endl;
		}

		else if (val > 15 && val <= 30) {
			direction = 'S';
			//cout << "We drifted south" << endl;
		}
		else {
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
			//cout << "We drifted west" << endl;
		}
		else if (val > 15 && val <= 30) {
			direction = 'E';
			//cout << "We drifted east" << endl;
		}
		else {
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

	if (state == 13) {
		reward = 100;
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
			if (gridArray[i][j].isObstacle == 0 && startingSquare != 13) {
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

	tuple = motion(gridArray, direction, robotLocation);
	robotLocation = tuple[3];
	updateQValues(gridArray, tuple);
	if (tuple[2] == 100) {
		moves = 100;
	}
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
		//cout << "We took random path" << endl;
		value = randomNumberGen(100);
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
		//cout << "We took optimal path" << endl;
		robotPos = findLocation(robotLocation);
		int i = robotPos[0];
		int j = robotPos[1];
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
			//cout << "Index is: " << index << endl;
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
			//cout << "The direction did not match any cases.\n";
			break;
		}
	}
	return direction;
}
/*
* Purpose:
*/
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
/*
* Purpose:
*/
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

	int i = currentState[0]; int j = currentState[1];
	int currentAction = tuple[1];
	int reward = tuple[2];
	int* nextState = findLocation(tuple[3]);
	int nextAction = tuple[4];
	int k = nextState[0]; int l = nextState[1];
	float accessFrequency = (1 / (gridArray[i][j].accessFrequency[currentAction] * 1.0));
	gridArray[i][j].qValue[currentAction] = gridArray[i][j].qValue[currentAction] + accessFrequency * (reward + GAMMA * max(gridArray[k][l].qValue) - gridArray[i][j].qValue[currentAction]);

}
/*
* Purpose:
*/
float max(float array[]) {
	float max = array[0];
	for (int i = 1; i < sizeof(array); i++) {
		if (array[i] > max)
			max = array[i];
	}
	return max;
}
/*
* Purpose:
*/
bool compareTo(float A, float B, float epsilon)
{
	return (fabs(A - B) < epsilon);
}
