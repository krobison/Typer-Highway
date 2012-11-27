/* CSCI261 Final Project: finalProject
 *
 * Authors: Kolten Robison and Meghan Way
 *
 * This program... DESCRIPTION HERE
*/


#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"



int main() {

    ofAppGlutWindow window;
	  ofSetupOpenGL(&window, 800,800, OF_WINDOW);

	  ofRunApp(new testApp());

}
