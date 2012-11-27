#include "health.h"


health::health(){

}

health::health(int t,int c,int initx,int inity){
	setTotalHealth(t);
	setCurrentHealth(c);
	setX(initx);
	setY(inity);
}

//getters:

int health::getTotalHealth() {
	return totalHealth;
}
int health::getCurrentHealth() {
	return currentHealth;
}
int health::getX() {
	return x;
}
int health::getY() {
	return y;
}

//setters:

void health::setTotalHealth(int t) {
	totalHealth = t;
}
void health::setCurrentHealth(int c) {
	currentHealth = c;
}
void health::setX(int initx) {
	x = initx;
}
void health::setY(int inity) {
	y = inity;
}

