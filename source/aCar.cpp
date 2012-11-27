
#include "aCar.h"


aCar::aCar(){

}

aCar::aCar(int length,int word,int red,int green,int blue,int s,double x,int lane){
	setBodyLength(length);
	setWordNumber(word);
	setColorR(red);
	setColorB(blue);
	setColorG(green);
	setSpeed(s);
	setXpos(x);
	setLaneNumber(lane);
}

//getters:

int aCar::getBodyLength() {
	return bodyLength;
}
int aCar::getWordNumber() {
	return wordNumber;
}
int aCar::getColorR() {
	return colorR;
}
int aCar::getColorG() {
	return colorG;
}
int aCar::getColorB() {
	return colorB;
}
double aCar::getSpeed() {
	return speed;
}
double aCar::getXpos() {
	return xpos;
}
int aCar::getLaneNumber() {
	return laneNumber;
}

//setters:

void aCar::setBodyLength(int l) {
	 bodyLength = l;
}
void aCar::setWordNumber(int n) {
	 wordNumber = n;
}
void aCar::setColorR(int r) {
	 colorR = r;
}
void aCar::setColorG(int g) {
	 colorG = g;
}
void aCar::setColorB(int b) {
	 colorB = b;
}
void aCar::setSpeed(double s) {
	 speed = s;
}
void aCar::setXpos(double x) {
	 xpos = x;
}
void aCar::setLaneNumber(int l) {
	 laneNumber = l;
}

//Other member functions:

void aCar::reset(int r, int g, int b, int n, double s) {
	setXpos(0);
	setColorR(r);
	setColorG(g);
	setColorB(b);
	setWordNumber(n);
	setSpeed(s);
}