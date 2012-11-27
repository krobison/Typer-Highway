#include "aButton.h"

aButton::aButton(){

}

aButton::aButton(int initx, int inity, string r, string t){
	setX(initx);
	setY(inity);
	setContainingRoom(r);
	setText(t);
}

//getters:

int aButton::getX() {
	return x;
}
int aButton::getY() {
	return y;
}
string aButton::getContainingRoom() {
	return containingRoom;
}
string aButton::getText() {
	return text;
}

//setters:

void aButton::setX(int newX){
	x = newX;
}
void aButton::setY(int newY){
	y = newY;
}
void aButton::setContainingRoom(string r){
	containingRoom = r;
}
void aButton::setText(string t){
	text = t;
}
