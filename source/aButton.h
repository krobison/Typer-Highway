#pragma once

#include <string>

using namespace std;

class aButton {

public:

	aButton();
	aButton(int initx, int inity, string r, string t);

	static const int WIDTH = 112;
	static const int HEIGHT = 32;

	//getters:
	int getX();
	int getY();
	string getContainingRoom();
	string getText();
	//setters:
	void setX(int newX);
	void setY(int newY);
	void setContainingRoom(string r);
	void setText(string t);

private:
	int x;
	int y;
	string containingRoom;
	string text;


};