#pragma once
#include "ofMain.h"
#include "ofxMaxim.h"

class window
{
    public:

        void setup();
        void update();
        void draw();
        void recoverPitch();
        void loadVinyl( string discName );

        ofPoint kinect, hand, handOld, center;

        bool loadDisc;
		bool scratching;
		bool forward;
		bool backward;
		bool moveHead;

        float offset;
        float middle;
		float pitch;
		float increment;
		double angle, oldAngle;

        int discAngle;
		int headAngle;
		int radius, oldRadius;
		int xCoord, yCoord;
        int time;
        int frame;
        int total;

        ofImage player;
        ofImage disc;
		ofImage arm1;
		ofImage arm2;
		ofImage ring;
		ofImage red;
		ofImage head;
		ofImage readHead;

		string mode;
};
