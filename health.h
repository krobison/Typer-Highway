#pragma once


class health {

public:

	health();
	health(int t,int c,int initx,int inity);

	//getters:
	int getTotalHealth();
	int getCurrentHealth();
	int getX();
	int getY();
	//setters:
	void setTotalHealth(int t);
	void setCurrentHealth(int c);
	void setX(int initx);
	void setY(int inity);

private:
	int totalHealth;
	int currentHealth;
	int x;
	int y;
};