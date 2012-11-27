#pragma once


class aCar {

public:
	aCar();
	aCar(int length,int word,int red,int green,int blue,int s,double x,int lane);
	//getters:
	int getBodyLength();
	int getWordNumber();
	int getColorR();
	int getColorG();
	int getColorB();
	double getSpeed();
	double getXpos();
	int getLaneNumber();
	//setters:
	void setBodyLength(int l);
	void setWordNumber(int n);
	void setColorR(int r);
	void setColorG(int g);
	void setColorB(int b);
	void setSpeed(double s);
	void setXpos(double x);
	void setLaneNumber(int l);
	//Other Member Funcitons:
	void reset(int r, int g, int b, int n, double s);

private:
	int bodyLength;
	int wordNumber;
	int colorR;
	int colorG;
	int colorB;
	double speed;
	double xpos;
	int laneNumber;

};