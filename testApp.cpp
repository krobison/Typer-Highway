//Include TestApp Header File
#include "testApp.h"

//Include necessary libraries
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

//Include custom class header files
#include "aCar.h"
#include "health.h"
#include "aButton.h"

//Namespace declaration for better IDE use
using namespace std;

//Declaration of all global constants
const int NUMBER_OF_LANES = 7;							//Number of Lanes in the road. Not intended for modification from 7
const int SCREEN_WIDTH = 800;							//Screen width and height
const int SCREEN_HEIGHT = 800;
const int ROADSTRIPES = 20;								//Variables for drawing the road on the screen
const int ROADSTRIPE_WIDTH = SCREEN_WIDTH/ROADSTRIPES;
const int LANE_SIZE = 96;
const int CROSSWALK_WIDTH = 64;
const int CROSSWALK_HEIGHT = 32;
const int mainColorR = 255;								//variables for the main drawing color in buttons, menus, etc.
const int mainColorG = 0;
const int mainColorB = 0;
const string WORDSFILE = "data\\words.txt";				//Directory of the file containing the wordbank
const string SCORESFILE = "data\\highscores.txt";		//Directory of the file containing the high scores.

//Declaration of all vectors
vector<aCar> cars;										//Custom datatype vectors soon to contain all car and button objects
vector<aButton> buttons;
vector<string> words;									//Vector for storing all of the words form the wordbank
vector<string> name;									//Vector for storing all letters of the entered name. Useful for entering name data. Is string instead of char to ease convertion problems when drawing the name

vector<int> highScores;									//Vectors to store all highscores and associated names from a file
vector<string> highScoresNames;

string room = "menu";									//A global variable to tell "where" the user is in the program (ie. menus, or game screen)

//global integer values for the actual game calculations. Names are self-explanatory
int difficulty = 30;									
int wordOn = 0;
int letterOn = 0;
int score = 0;
int target = difficulty*100;							//an integer for a 'target value' to increase the level upon.

bool typingWord = false;								//A boolean value to see if user is currently typing a certain word or not.

char firstLetters[NUMBER_OF_LANES];						//An array meant to store the first letters of all of the words currently on the screen. Two cars never have the same starting letter to prevent errors.

health userhealth(3,3,SCREEN_WIDTH-128,80);				//Declaration of a custom class health object called userhealth.

ofTrueTypeFont titleFont;								//Taking advantage of openFramworks to load fonts into the program for better graphical appearance.
ofTrueTypeFont aboutFont;\

ofSoundPlayer titleSound;
ofSoundPlayer deathSound;


//Quick Declaration of all functions to make sure order doesn't matter later and program can be more organized.
void initWords();bool isWordValid(int wordToCheck);void initCars();void initButtons();void initHighScores();void rewriteScores();string getName();void checkForHighScore(int number);
void checkIfTooFar();void drawVehicle(int size,int numberCar);void drawRoad(int startY);void drawBoarder();void drawCrossWalk();void drawHealth();void drawScore(int x,int y);
void drawStage(int x,int y);void drawLevel();void drawButtons();void drawMenu();void drawHighScores();void drawFinish();int getPoints();void newGame();void drawPause();void drawName();
void buttonClick(string t);void keyPressedInGame(int key);void keyPressedInName(int key);void initFonts();void initSounds();void playMusic();void drawOptions();void setMainColor();

//***************************************************************************************************************************************************************************************************************
//*																							PROGRAM BODY BEGINS HERE																							*
//***************************************************************************************************************************************************************************************************************

void testApp::setup() {
	initFonts();		//runs all initializations functions
	initSounds();
	initWords();
	initCars();
	initButtons();
	initHighScores();
	ofBackground (0,0,0);	//sets background to black
	ofSetFrameRate(60);		//VERY IMPORTANT. Ensures that, because the speed of the words is based on how frequently draw and update are run, the game speed in consistant across different machines.
	playMusic();			//begins playing music
}
void testApp::update() {
	if (room == "game") {													//checks if the player is in the "game" room, and if so, increments the cars' xpositions based on the difficulty.
		for (int i = 0; i < NUMBER_OF_LANES;i++){
			double newx = cars[i].getXpos() + (cars[i].getSpeed())/100;
			cars[i].setXpos(newx);
		}
		checkIfTooFar();													//constantly checks if the cars have reached the end of the screen.											
	}
}
void testApp::draw() {
	if (room == "game") {			//a series of if statements to ensure that only the current room is being drawn
		drawLevel();
	}
	else if (room == "menu") {
		drawMenu();
	}
	else if (room == "finish") {
		drawFinish();
	}
	else if (room == "pause") {
		drawPause();
	}
	else if (room == "name") {
		drawName();
	}
	else if (room == "highscores") {
		drawHighScores();
	}
	else if (room == "options") {
		drawOptions();
	}
	drawButtons();					//draws buttons to the screen
}

void initWords() {		//simply streams words from the wordbank file and stores them into the word vector

	ifstream wordstream(WORDSFILE);		
	if (!wordstream) {
		cout << "The file could not be opened"<<endl;
		exit(1);
	}
	string word;
	while (wordstream >> word) {
		words.push_back(word);
	}

}
void initSounds() {
	titleSound.loadSound("HappyRag.mp3");		//all sounds are in public domain. This is loading them from the data folder and ensuring that they loop.
	titleSound.setLoop(true);
	deathSound.loadSound("Sad.mp3");
	deathSound.setLoop(true);
}
void initCars() {
												//initializes all car objects with random colors, and appropriate lengths
	score = 0;

	int initSpeed[NUMBER_OF_LANES];
	int initReds[NUMBER_OF_LANES];
	int initGreens[NUMBER_OF_LANES];
	int initBlues[NUMBER_OF_LANES];
	for (int i = 0;i<NUMBER_OF_LANES;i++) {
		initSpeed[i] = ( (difficulty) + rand()%( (difficulty/3) *2) );

		initReds[i] = rand()%256;
		initGreens[i] = rand()%256;
		initBlues[i] = rand()%256;
		
		aCar c(0,i,initReds[i],initGreens[i],initBlues[i],initSpeed[i],0,i);
		cars.push_back(c);
	}
	for (int i = 0;i<NUMBER_OF_LANES;i++) {
		int newlength = words[cars[i].getWordNumber()].length()*9+12;
		cars[i].setBodyLength(newlength);
	}
}
void initButtons() {		//Because each button must be instantiated with very non pattern-like arguments, loops were unreasonable in this function, which instantiates all buttons and stores them into a vector

	int xCenter = (SCREEN_WIDTH-aButton::WIDTH)/2;
	aButton a(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*7-16,"menu","New Game"); //START BUTTON (in menu)
	aButton b(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)-16,"menu","Exit"); //EXIT BUTTON (in menu)
	aButton c(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*5-16,"finish","New Game"); //New Game BUTTON (in finish)
	aButton d(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)-16,"finish","Exit"); //EXIT BUTTON (in finish)
	aButton e(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*3-16,"finish","Main Menu"); //MENU BUTTON (in finish)
	aButton f(xCenter,128-(aButton::HEIGHT)-16,"game","Pause"); //pause button (in game)
	aButton g(16,32+(aButton::HEIGHT),"game","Main Menu"); //menu button (in game)
	aButton h(16,16,"game","Exit"); //exit button (in game)
	aButton i(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*7-16,"name","Submit"); //Submit button (in game)
	aButton j(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*3-16,"menu","High Scores"); //score button (in menu)
	aButton k(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*3-16,"highscores","Main Menu"); //menu button (in scores)
	aButton l(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*5-16,"menu","Options"); //OPTIONS BUTTON (in menu)
	aButton m(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*5-16,"options","Main Menu"); //MENU BUTTON (in options)
	aButton n(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*16-24,"options","+"); //MENU BUTTON (in options)
	aButton o(xCenter,SCREEN_HEIGHT-(aButton::HEIGHT)*14-8,"options","-"); //MENU BUTTON (in options)

	buttons.push_back(a);
	buttons.push_back(b);
	buttons.push_back(c);
	buttons.push_back(d);
	buttons.push_back(e);
	buttons.push_back(f);
	buttons.push_back(g);
	buttons.push_back(h);
	buttons.push_back(i);
	buttons.push_back(j);
	buttons.push_back(k);
	buttons.push_back(l);
	buttons.push_back(m);
	buttons.push_back(n);
	buttons.push_back(o);
}
void initHighScores() {								//loads high score data
	ifstream scoreStream("data//highscores.txt");
	if (!scoreStream) {
		cout << "Cannot open highscore file." << endl;
		exit(1);
	}
	int number;
	string name;
	int i = 0;
	while(scoreStream >> number >> name) {
		highScores.push_back(number);
		highScoresNames.push_back(name);
		i++;
	}
}
void initFonts(){							//initializes fonts
	titleFont.loadFont("swissko.ttf", 48);
	aboutFont.loadFont("swsimp.ttf",12);
} 

int getPoints() {	//returns the amount of points a word was worth. This is a formula based on the length of the word as well as the current difficulty level, for balanced gameplay.
	return difficulty*(words[cars[wordOn].getWordNumber()].length());
}
string getName() {	//if you recall, the player name is stored as a vector of small strings (letters), and this returns the combination of all of the letters.
	stringstream out;
	for (int i = 0;i<name.size();i++){
		out << name[i];
	}
	return out.str();
}

bool isWordValid(int wordToCheck){
	int x = 0;									//This is a very important function. It checks, when adding new words, that they dont start with the same first letter as any other words. This prevents massive bugs.
	for (int i = 0;i<NUMBER_OF_LANES;i++){
			firstLetters[i] = words[cars[i].getWordNumber()].at(0);
	}
	char letterToCheck = firstLetters[wordToCheck];
	for (int i = 0;i<NUMBER_OF_LANES;i++){
		if ( (i != wordToCheck)&&(letterToCheck == firstLetters[i]) ){
			x+=1;
		}
	}
	if (x>0) {
		return false;							//The word is invalid because it starts with the same letter as another already on screen
	}
	else{
		return true;							//The word is valid
	}
}

void rewriteScores(){						//saves high score data
	ofstream output("data\\highscores.txt");
	if(!output){
		cout << "could not open highscores file to rewrite."<<endl;
		exit(1);
	}
	for (int i = 0;i<10;i++){
		output << highScores[i] << " "<< highScoresNames[i]<<endl;
	}
}
void checkForHighScore(int number){		//This takes your final score and checks if it is higher than a previous score, from top down. If so, it inserts it into the vector and pops off the end
	for(int i = 0;i<10;i++){
		if (number > highScores[i]) {
			vector<int>::iterator loc = highScores.begin();					//creates iterator datatypes in order to call the insert member function
			vector<string>::iterator locS = highScoresNames.begin();

			highScores.insert(loc+i,number);

			string playerName = getName();
			highScoresNames.insert(locS+i,playerName);

			highScores.pop_back();
			highScoresNames.pop_back();

			rewriteScores();			//rewrite the new score table to the highscore txt file.
			break;
		}
	}
}
void checkIfTooFar() {		// checks if the cars have reached the end of the screen.			
	int targetX,carX;
	for (int i = 0; i < NUMBER_OF_LANES;i++){
		targetX = SCREEN_WIDTH-CROSSWALK_WIDTH-4;
		carX = cars[i].getXpos() + cars[i].getBodyLength() - 10;
		if (carX > targetX){	//if so, reset the car
			bool x = false;
			while (x == false) {
				cars[i].reset(rand()%256,rand()%256,rand()%256,(rand()%(words.size()-1))+1,( (difficulty) + rand()%( (difficulty/3) *2) ));
				x = isWordValid(i);
			}
			cars[i].setBodyLength(words[cars[i].getWordNumber()].length()*9+12);	//reset car length
			userhealth.setCurrentHealth(userhealth.getCurrentHealth()-1);			//subtract health
			if (wordOn == i) {														//if the car was the one that the user was typing, free the user to type any car, and reset typing variables
				letterOn = 0;
				typingWord = false;
			}
			if (userhealth.getCurrentHealth() == 0) {								//if you are now out of lives, display the endscreen
				checkForHighScore(score);
				room = "finish";
				playMusic();
			}
		}
	}
}

void setMainColor() {
	ofSetColor(mainColorR,mainColorG,mainColorB);
}

//***************************************************************************************************************************************************************************************************************
//*																							DRAW FUNCTIONS																										*
//***************************************************************************************************************************************************************************************************************
void drawBoarder() {	//This draws the red, and two yellow lines on the game screen
	ofSetColor(256,16,16);
	ofRect(SCREEN_WIDTH-CROSSWALK_WIDTH-4,128,4,SCREEN_HEIGHT-128);
	ofSetColor(255,215,0);
	ofRect(0,128,SCREEN_WIDTH,8);
	ofRect(0,SCREEN_HEIGHT-8,SCREEN_WIDTH,8);
}
void drawButtons() {	//A Functions that draws every button on the screen, if the player is the the button's containing room.
	for (int i = 0; i < buttons.size();i++) {		//checks through every button in the vector.
		if (room == buttons[i].getContainingRoom()) {
			for (int j = 0;j<2;j++){				//This for loop is mearly for stylistic appearance of the buttons (they have a double line border)
				setMainColor();
				ofRect((2*j)+buttons[i].getX(),(2*j)+buttons[i].getY(),aButton::WIDTH-(4*j),aButton::HEIGHT-(4*j));
				ofSetColor(0,0,0);
				ofRect(1+(2*j)+buttons[i].getX(),1+(2*j)+buttons[i].getY(),aButton::WIDTH-(4*j)-2,aButton::HEIGHT-(4*j)-2);
			}
			setMainColor();
			int offset = ((aButton::WIDTH)-(((buttons[i].getText()).length())*8))/2;
			ofDrawBitmapString(buttons[i].getText(),buttons[i].getX()+offset,buttons[i].getY()+18);
		}
	}
}
void drawCrossWalk(){	//Draws the crosswalk on the game screen
	ofSetColor(256,256,256);
	for (int i = 164;i<SCREEN_HEIGHT;i+=CROSSWALK_HEIGHT*2){
		ofRect(SCREEN_WIDTH-CROSSWALK_WIDTH,i,CROSSWALK_WIDTH,CROSSWALK_HEIGHT);
	}
}
void drawFinish() { //draws the death screen

	int level = (difficulty/10 - 2);		//Two string streams to convert integers to strings
	stringstream out;
	out << level;
	string strLevel = out.str();

	stringstream out2;
	out2 << score;
	string number = out2.str();

	string plural;
	if (level == 1) {
		plural = "level";
	}
	else{
		plural = "levels";
	}
	string display = "You lasted "+strLevel+" "+plural+", with "+number+" points.";
	int offset = (SCREEN_WIDTH-(display.length()*8))/2;

	setMainColor();
	ofDrawBitmapString(display,offset,(SCREEN_HEIGHT/2)+156);
	drawHighScores();
}
void drawHealth() {	//Draws the user's lives in the playing screen
	int lives = userhealth.getTotalHealth();
	int currentLives = userhealth.getCurrentHealth();
	for (int i = 0;i<lives;i++){
		if(i<currentLives) {
			setMainColor();
		}
		else{
			ofSetColor(96,96,96);
		}
		ofCircle(userhealth.getX()+i*16,userhealth.getY(),8);
	}
	setMainColor();
	ofDrawBitmapString("Lives:",userhealth.getX()-64,userhealth.getY()+4);
}
void drawHighScores() {		//draws the high score table

	string display = "High Scores";
	int offset = (SCREEN_WIDTH-(display.length()*40))/2;
	titleFont.drawString(display,offset,82);
	setMainColor();
	ofLine(128,128,SCREEN_WIDTH-128,128);
	ofLine(128,128,128,160);
	ofLine(SCREEN_WIDTH-128,128,SCREEN_WIDTH-128,160);
	ofLine(128,128,128,448);//number bar down
	ofLine(152,128,152,448);//number bar down right side
	ofLine(128,448,SCREEN_WIDTH-128,448);//bottom bar

	ofLine(412,128,412,448);

	ofLine(SCREEN_WIDTH-128,128,SCREEN_WIDTH-128,448);


	for (int i = 0;i<10;i++){
		setMainColor();
		stringstream out;
		out << i+1;
		string strNumber = out.str();
		ofDrawBitmapString(strNumber,132,148+i*32);
		ofLine(128,160+i*32,SCREEN_WIDTH-128,160+i*32);//bar below number

		stringstream outII;			//string streams to convert integers to strings
		outII << highScores[i];
		string strPoints = outII.str();

		stringstream outIII;
		outIII << score;
		string strCurrentPoints = outIII.str();

		if ( (getName() == highScoresNames[i])&&(strCurrentPoints == strPoints)&&(room =="finish") ){
			ofSetColor(rand()%256,rand()%256,rand()%256);
		}
		else {
			setMainColor();
		}
		ofDrawBitmapString(strPoints,424,148+i*32);
		ofDrawBitmapString(highScoresNames[i],164,148+i*32);
	}


}
void drawLevel(){		//The main draw command for the "game" room. This calls all other draw commands, as well as draws the letters of the cars
	drawRoad(128);
	drawBoarder();
	drawCrossWalk();
	drawHealth();
	drawScore(SCREEN_WIDTH - 192, 16);
	drawStage(SCREEN_WIDTH - 192, 48);

	for (int i = 0;i<NUMBER_OF_LANES;i++){
		drawVehicle(cars[i].getBodyLength(),i);	
	}

	for (int i = 0;i<NUMBER_OF_LANES;i++) {
		for (int j = 0;j<words[cars[i].getWordNumber()].length();j++) {
			if ( (i == wordOn)&&(j<letterOn) ) {
				ofSetColor(250,42,82);
			}
			else {
				ofSetColor(0,0,0);
			}
			ofDrawBitmapString(string(1,words[cars[i].getWordNumber()].at(j)),6+9*j+cars[i].getXpos(),(164+(cars[i].getLaneNumber())*LANE_SIZE+12));   
		}
	}

}
void drawMenu() {		//Draws the main menu
	setMainColor();
	string about[5];
	about[0] = "Welcome to Typer Highway! The town of trafficville needs your help";
	about[1] = "to solve its crisis! Last night, while the good townsfolk were asleep,";
	about[2] = "a terrible villain snuck in and cut the brakes of all the trucks in";
	about[3] = "town! Now you, with your mighty typing skills, need to type away the";
	about[4] = "words on the trucks before they reach the crosswalk!";
	for (int i = 0;i<5;i++){
		aboutFont.drawString(about[i],92,218+18*i);
	}
	string display = "Typer Highway";
	int offset = (SCREEN_WIDTH-(display.length()*40))/2;
	titleFont.drawString(display,offset,82);

	ofLine(0,128,SCREEN_WIDTH,128);	//These draw the outlines road at the top of the menu
	ofLine(0,129,SCREEN_WIDTH,129);
	ofLine(0,192,SCREEN_WIDTH,192);
	ofLine(0,193,SCREEN_WIDTH,193);
	for (int x = 0;x < SCREEN_WIDTH;x+=64){	//the road lines in the road at the top
		ofLine(x,152,x,168);
		ofLine(x+32,152,x+32,168);
		ofLine(x,152,x+32,152);
		ofLine(x,168,x+32,168);
	}
}
void drawOptions() {//Draws the options
	setMainColor();
	string display = "Options";
	int offset = (SCREEN_WIDTH-(display.length()*40))/2;
	titleFont.drawString(display,offset,82);

	stringstream out;
	out << int(titleSound.volume*100);

	aboutFont.drawString("Music Volume:",128,320);
	aboutFont.drawString(out.str(),382,320);
}
void drawRoad(int startY) {	//Draws the road

	ofSetColor(128,128,128);
	ofRect(0,startY,SCREEN_WIDTH,LANE_SIZE);
	for (int i = 0;i<ROADSTRIPES/2-1;i++){
		ofSetColor(256,256,256);
		ofRect(ROADSTRIPE_WIDTH+2*i*ROADSTRIPE_WIDTH,startY,ROADSTRIPE_WIDTH,8);
	}
	startY+=LANE_SIZE;
	if (startY < SCREEN_HEIGHT) {
		drawRoad(startY);
	}
}
void drawScore(int x, int y) {	//uses a string stream to draw the score
	setMainColor();
	stringstream out;
	out << score;
	string number = out.str();
	ofDrawBitmapString("Score: "+number,x,y);
}
void drawStage(int x, int y) {	//uses a string stream to draw the current level
	setMainColor();
	int level = (difficulty/10 - 2);
	stringstream out;
	out << level;
	string number = out.str();
	ofDrawBitmapString("Level: "+number,x,y);
}
void drawVehicle(int size,int numberCar) {	//Draws the cars

	int x = cars[numberCar].getXpos();
	int y = 164+cars[numberCar].getLaneNumber()*LANE_SIZE;


	ofSetColor(255,255,255);

	ofRect(x,y,size,32);
	ofSetColor(0,0,0);
	ofEllipse(x+10,y+32,8,6);
	ofEllipse(x+size-10,y+32,8,6);
	ofSetColor(cars[numberCar].getColorR(),cars[numberCar].getColorG(),cars[numberCar].getColorB());
	ofRect(x+size,y+8,18,22);
	ofSetColor(0,0,0);
	ofRect(x+size+8,y+12,6,4);
}
//***************************************************************************************************************************************************************************************************************
//*																							END DRAW FUNCTIONS																									*
//***************************************************************************************************************************************************************************************************************

void newGame() {		//resets all variables needed for a game, and restarts a game

	difficulty = 30;
	wordOn = 0;
	letterOn = 0;
	score = 0;
	target = difficulty*100;
	room = "game";
	typingWord = false;

	for (int i = 0;i<NUMBER_OF_LANES;i++){
		bool x = false;
		while (x == false) {
			cars[i].reset(rand()%256,rand()%256,rand()%256,(rand()%(words.size()-1))+1,( (difficulty) + rand()%( (difficulty/3) *2) ));
			x = isWordValid(i);
		}
	}
	for (int i = 0;i<NUMBER_OF_LANES;i++){
		cars[i].setBodyLength(words[cars[i].getWordNumber()].length()*9+12);
	}
	userhealth.setCurrentHealth(3);
}
void drawPause() {		//draws the pause screen
	string display = "Paused";
	int offset = (SCREEN_WIDTH-(display.length()*8))/2;
	setMainColor();
	ofDrawBitmapString(display,offset,(SCREEN_HEIGHT/2)-4);
	display = "Press any key to resume";
	offset = (SCREEN_WIDTH-(display.length()*8))/2;
	ofDrawBitmapString(display,offset,(SCREEN_HEIGHT/2)-4+16);
}
void drawName() {		//Draws the enter name screen
	string display = "Enter Name";
	int offset = (SCREEN_WIDTH-(display.length()*8))/2;
	setMainColor();
	ofDrawBitmapString(display,offset,(SCREEN_HEIGHT/2)-4);
	ofRect(offset-8,(SCREEN_HEIGHT/2)+16,96,32);
	ofSetColor(0,0,0);
	ofRect(offset-6,(SCREEN_HEIGHT/2)+18,92,28);
	display = "";
	for (int i = 0; i < name.size();i++) {
		display += name[i];
	}
	setMainColor();
	ofDrawBitmapString(display,offset+4,(SCREEN_HEIGHT/2)+36);
}

void playMusic(){			//plays the correct music. If the end room, play end music, otherwise, play the main music
	if (room == "finish") {
		ofSoundStopAll();
		deathSound.play();
	}
	else {
		if (titleSound.getIsPlaying() == false) {
			ofSoundStopAll();
			titleSound.play();
		}
	}
}

void buttonClick(string t) {	//Determines what to do when a button is clicked. Because we don't have two buttons with the same text that do different things, we can read the text on the button to see what to do.
	if (t == "New Game") {
		room = "name";
		playMusic();
	}
	else if (t == "Exit") {
		exit(0);
	}
	else if (t == "Pause") {
		room = "pause";
	}
	else if (t == "Submit") {
		newGame();
	}
	else if (t == "High Scores") {
		room = "highscores";
	}
	else if (t == "Main Menu") {
		room = "menu";
		playMusic();
	}
	else if (t == "Options") {
		room = "options";
	}
	else if (t == "+") {
		if (titleSound.volume < 1) {
			titleSound.setVolume(titleSound.volume+0.1);
			deathSound.setVolume(deathSound.volume+0.1);
		}
	}
	else if (t == "-") {
		if (titleSound.volume > 0) {
			titleSound.setVolume(titleSound.volume-0.1);
			deathSound.setVolume(deathSound.volume-0.1);
		}
	}
}
void keyPressedInGame(int key) {		//semi-complex functions that checks for if the user is typing a word and where in the word and if the word is finished.
	char letter = key;
	if (!typingWord) {
		for (int i = 0;i<NUMBER_OF_LANES;i++){
			if (letter == words[cars[i].getWordNumber()].at(0)){
				wordOn = i;
				letterOn = 1;
				typingWord = true;
			}
		}
	}
	else {
		if ( (letter == words[cars[wordOn].getWordNumber()].at(letterOn))&&(letterOn < words[cars[wordOn].getWordNumber()].length()-1) ){
			letterOn+=1;
		}
		else if (letter == words[cars[wordOn].getWordNumber()].at(letterOn)) {
			score += getPoints();
			target -= getPoints();
			if (target < 0) {
				difficulty += 10;
				target = difficulty * 100;
			}

			letterOn = 0;

			bool x = false;
			while (x == false) {
				cars[wordOn].reset(rand()%256,rand()%256,rand()%256,(rand()%(words.size()-1))+1,( (difficulty) + rand()%( (difficulty/3) *2) ));
				x = isWordValid(wordOn);
			}

			cars[wordOn].setBodyLength(words[cars[wordOn].getWordNumber()].length()*9+12);
			typingWord = false;

		}
	}
}
void keyPressedInName(int key) {	//add characters to the name vector, or deletes them if the user presses backspace
	if (key == 13) {
		newGame();
	}
	else if  (key!= 8) {
		if (name.size()<9) {
			char letter = key;
			name.push_back(string(1,key));
		}
	}
	else {
		if (name.size() > 0) {
			name.pop_back();
		}
	}
}

void testApp::keyPressed(int key) {	//what to do if the user presses a key.
	if (room == "game") {
		keyPressedInGame(key);
	}
	else if (room == "name") {
		keyPressedInName(key);
	}
	else if (room == "pause") {
		room = "game";
	}
}
void testApp::mouseReleased(int x, int y, int button) {	//checks if the user clicks within the boundaries of any currently-drawn buttons, and if so, calls their click function
	for (int i = 0; i < buttons.size();i++) {	
		if ( room == buttons[i].getContainingRoom()) {
			if ( ( (x>=buttons[i].getX())&&(x<=buttons[i].getX()+aButton::WIDTH) )&&( (y>=buttons[i].getY())&&(y<=buttons[i].getY()+aButton::HEIGHT) ) ) {
				buttonClick(buttons[i].getText());
				break;
			}
		}
	}
}


//***************************************************************************************************************************************************************************************************************
//*																							PROGRAM BODY ENDS HERE						     																	*
//***************************************************************************************************************************************************************************************************************

void testApp::keyReleased(int key) {

}
void testApp::mouseMoved(int x, int y) {

}
void testApp::mouseDragged(int x, int y, int button) {

}
void testApp::mousePressed(int x, int y, int button) {

}
void testApp::windowResized(int w, int h) {

}

