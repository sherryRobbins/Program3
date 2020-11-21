#include <string>
#include <iostream>
#include <iomanip>
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
		this->qValue[4] = {0};
		this->accessFrequency[4] = {0};
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
void sensing(GridBox gridArray[6][5], bool evidence[4]);
void motion(GridBox gridArray[6][5], char direction, int robotLocation);
int randomNumberGen();//Returns the random number
int main() {
	Robot robot = new Robot();
	GridBox gridArray[6][5];
	populateGrid(gridArray);
	
	system("Pause");
	return 0;
}
int randomNumberGen()
{
	int maxNum=100;/*Maximum number to go to*/
	int num;
	srand(time(0));
	num=rand() % max;
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
	for (int i = 0; i < 6; i++) {
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
int motion(GridBox gridArray[6][5], char direction, int robotLocation) {
	int val;	
	//This switch statement finds if we end up drifting or not
	switch (direction) {
		case 'W':
			char actualDirection[3]; //0 is north, 1 is south, 2 is west
			actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
			val = randomNumberGen();
			if(val <= 15)
				direction = 'N';
			else if (val > 15 && val <= 30)
				direction = 'S';
			else
				direction = 'W';
			break;
		case 'N':
			char actualDirection[3]; //0 is west, 1 is east, 2 is north
			actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
			val = randomNumberGen();
			if(val <= 15)
				direction = 'W';
			else if (val > 15 && val <= 30)
				direction = 'E';
			else
				direction = 'N';
			break;
		case 'E':
			char actualDirection[3]; //0 is north, 1 is south, 2 is east
			actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
			val = randomNumberGen();
			if(val <= 15)
				direction = 'N';
			else if (val > 15 && val <= 30)
				direction = 'S';
			else
				direction = 'E';
			break;
		case 'S':
			char actualDirection[3]; //0 is west, 1 is east, 2 is south
			actualDirection[0] = 15; actualDirection[1] = 30; actualDirection[2] = 100;
			val = randomNumberGen();
			if(val <= 15)
				direction = 'W';
			else if (val > 15 && val <= 30)
				direction = 'E';
			else
				direction = 'S';
			break;
	}
	int reward, directionInt;
	double probability[4];
	int robotPos[2];
	//This switch statement performs the actual movement
	switch (direction) {
		//Translate robot location to row and column of grid
		robotPos[0] = (robotLocation / 6); //row
		robotPos[1] = (robotLocation % 5) - 1; //column; we subtract 1 so that it translates into an array value nicely
		
		case 'W':
			gridArray[robotPos[0]][robotPos[1]].accessFrequency[0] += 1;
			reward = -2;
			directionInt = 1;
			//TODO: Implement movement, and possible bouncing.
			break;
		case 'N':
			gridArray[robotPos[0]][robotPos[1]].accessFrequency[1] += 1;
			reward = -3;
			directionInt = 2;
			break;
		case 'E':
			gridArray[robotPos[0]][robotPos[1]].accessFrequency[2] += 1;
			reward = -2;
			directionInt = 3;
			break;		
		case 'S':
			gridArray[robotPos[0]][robotPos[1]].accessFrequency[3] += 1;
			reward = -1;
			directionInt = 4;
			break;
	}
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			gridArray[i][j].probability = gridArray[i][j].transProbability;
		}
	}
	int tuple[2];
	tuple[0] = directionInt;
	tuple[1] = reward;
	return tuple;
}
