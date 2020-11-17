#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class GridBox {
public: //attributes
	double probability;
	double condProbability;
	double transProbability;
	int position;
	bool isObstacle;

	GridBox() { //constructor
		this->probability = 4.17;
		this->condProbability = 0;
		this->transProbability = 0;
		this->position = -1;
		this->isObstacle = 0;
	}
};

void populateGrid(GridBox gridArray[6][5]);
void displayGrid(GridBox gridArray[6][5]);
void sensing(GridBox gridArray[6][5], bool evidence[4]);
void motion(GridBox gridArray[6][5], char direction);

int main() {
	GridBox gridArray[6][5];
	populateGrid(gridArray);
	cout << "Initial Location Probabilities" << endl;
	displayGrid(gridArray);
	bool evidence[4];
	evidence[0] = 0; evidence[1] = 0; evidence[2] = 0; evidence[3] = 0;
	sensing(gridArray, evidence);
	cout << endl << "Filtering After Evidence [0, 0, 0, 0]" << endl;
	displayGrid(gridArray);
	char direction = 'W';
	motion(gridArray, direction);
	cout << endl << "Prediction After Action W" << endl;
	displayGrid(gridArray);
	evidence[0] = 1; evidence[1] = 1; evidence[2] = 0; evidence[3] = 1;
	sensing(gridArray, evidence);
	cout << endl << "Filtering After Evidence [1, 1, 0, 1]" << endl;
	displayGrid(gridArray);
	direction = 'N';
	motion(gridArray, direction);
	cout << endl << "Prediction After Action N" << endl;
	displayGrid(gridArray);
	evidence[0] = 1; evidence[1] = 1; evidence[2] = 0; evidence[3] = 1;
	sensing(gridArray, evidence);
	cout << endl << "Filtering After Evidence [1, 1, 0, 1]" << endl;
	displayGrid(gridArray);
	system("Pause");
	return 0;
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

/*Purpose: Handles the robot's sensing as well as filtering after the sensor
	gridArray: the array where the values of squares are kept
	evidence: the obstacles that the robot sensed
*/
void sensing(GridBox gridArray[6][5], bool evidence[4]) {
	//Following code block is evidence conditional probability
	double probability[4];
	double total = 0;
	for (int i = 0; i < 6; i++) { //we iterate through the grid
		for (int j = 0; j < 5; j++) {
			if (gridArray[i][j].isObstacle != 1) {//if not an obstacle then sense for obstacles
				//check west
				if (j == 0) {
					if (evidence[0] == 1)
						probability[0] = .75;
					else
						probability[0] = .25;
				}
				else if (gridArray[i][j - 1].isObstacle == 1) {
					if (evidence[0] == 1) {
						probability[0] = .75;
					}
					else
						probability[0] = .25;
				}
				else if (gridArray[i][j - 1].isObstacle == 0) {
					if (evidence[0] == 1)
						probability[0] = .2;
					else
						probability[0] = .8;
				}
				//check north
				if (i == 0) {
					if (evidence[1] == 1)
						probability[1] = .75;
					else
						probability[1] = .25;
				}
				else if (gridArray[i - 1][j].isObstacle == 1) {
					if (evidence[1] == 1)
						probability[1] = .75;
					else
						probability[1] = .25;
				}
				else if (gridArray[i - 1][j].isObstacle == 0) {
					if (evidence[1] == 1)
						probability[1] = .2;
					else
						probability[1] = .8;
				}
				//check east
				if (j == 4) {
					if (evidence[2] == 1)
						probability[2] = .75;
					else
						probability[2] = .25;
				}
				else if (gridArray[i][j + 1].isObstacle == 1) {
					if (evidence[2] == 1)
						probability[2] = .75;
					else
						probability[2] = .25;
				}
				else if (gridArray[i][j + 1].isObstacle == 0) {
					if (evidence[2] == 1)
						probability[2] = .2;
					else
						probability[2] = .8;
				}
				//check south
				if (i == 5) {
					if (evidence[3] == 1)
						probability[3] = .75;
					else
						probability[3] = .25;
				}
				else if (gridArray[i + 1][j].isObstacle == 1) {
					if (evidence[3] == 1)
						probability[3] = .75;
					else
						probability[3] = .25;
				}
				else if (gridArray[i + 1][j].isObstacle == 0) {
					if (evidence[3] == 1)
						probability[3] = .2;
					else
						probability[3] = .8;
				}
				gridArray[i][j].condProbability = probability[0] * probability[1] * probability[2] * probability[3] * gridArray[i][j].probability;
				total += gridArray[i][j].condProbability;
			}
		}
	}
	for (int i = 0; i < 6; i++) { //now we conduct the filtering here
		for (int j = 0; j < 5; j++) {
			if (gridArray[i][j].isObstacle != 1) {
				gridArray[i][j].probability = (gridArray[i][j].condProbability / total) * 100;
			}
		}
	}
}
/* Purpose: handles the robot's motion as well as prediction after the motion
	gridArray: the array that stores values of squares
	direction: the direction the robot is moving in
	*/
void motion(GridBox gridArray[6][5], char direction) {
	//Following code block is transitional probability
	double probability[4];
	GridBox newMap[6][5];
	switch (direction) {
	case 'W':
		for (int i = 0; i < 6; i++) { //we iterate through the grid
			for (int j = 0; j < 5; j++) {
				if (gridArray[i][j].isObstacle != 1) {//we only check motion for open squares
					//check west probability. this only matters if it is an obstacle as this can affect prediction
					if (j == 0)
						probability[0] = .7 * gridArray[i][j].probability;
					else if (gridArray[i][j - 1].isObstacle == 1)
						probability[0] = .7 * gridArray[i][j].probability;
					else
						probability[0] = 0;
					//check north probability
					if (i == 0) //we only have a chance of moving into this square from the north if it drifted or bounced
						probability[1] = .15 * gridArray[i][j].probability;
					else if (gridArray[i - 1][j].isObstacle == 1)
						probability[1] = .15 * gridArray[i][j].probability;
					else
						probability[1] = .15 * gridArray[i - 1][j].probability;
					//check east probability. this is because we see the chance the robot moved from the east into this square
					if (j == 4)
						probability[2] = 0;
					else if (gridArray[i][j + 1].isObstacle == 1)
						probability[2] = 0;
					else
						probability[2] = .7 * gridArray[i][j + 1].probability;
					//check south probability
					if (i == 5) //we only have a chance of moving into this square from the south if it drifted or bounced
						probability[3] = .15 * gridArray[i][j].probability;
					else if (gridArray[i + 1][j].isObstacle == 1)
						probability[3] = .15 * gridArray[i][j].probability;
					else
						probability[3] = .15 * gridArray[i + 1][j].probability;
				}
				//this section covers prediction
				gridArray[i][j].transProbability = ((probability[0] + probability[1] + probability[2] + probability[3]));
			}
		}
		break;
	case 'N':
		for (int i = 0; i < 6; i++) { //we iterate through the grid
			for (int j = 0; j < 5; j++) {
				if (gridArray[i][j].isObstacle != 1) {//we only check motion for open squares
					//check west probability. we only have a chance of moving from the west if the robot drifted or bounced
					if (j == 0)
						probability[0] = .15 * gridArray[i][j].probability;
					else if (gridArray[i][j - 1].isObstacle == 1)
						probability[0] = .15 * gridArray[i][j].probability;
					else
						probability[0] = .15 * gridArray[i][j-1].probability;
					//check north probability
					if (i == 0) //this only matters if it is an obstacle as this can affect prediction
						probability[1] = .7 * gridArray[i][j].probability;
					else if (gridArray[i - 1][j].isObstacle == 1)
						probability[1] = .7 * gridArray[i][j].probability;
					else
						probability[1] = 0;
					//check east probability. we only have a chance of moving from the east if the robot drifted or bounced
					if (j == 4)
						probability[2] = .15 * gridArray[i][j].probability;
					else if (gridArray[i][j + 1].isObstacle == 1)
						probability[2] = .15 * gridArray[i][j].probability;
					else
						probability[2] = .15 * gridArray[i][j + 1].probability;
					//check south probability
					if (i == 5) //this is because we see the chance the robot moved from the south into this square
						probability[3] = 0;
					else if (gridArray[i + 1][j].isObstacle == 1)
						probability[3] = 0;
					else
						probability[3] = .7 * gridArray[i + 1][j].probability;
				}
				//this section covers prediction
				gridArray[i][j].transProbability = ((probability[0] + probability[1] + probability[2] + probability[3]));
			}
		}
		break;
	}
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			gridArray[i][j].probability = gridArray[i][j].transProbability;
		}
	}
}
