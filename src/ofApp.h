#pragma once
#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofTrueTypeFont.h"
#include "ofxMaxim.h"
#include "window.h"

class ofApp : public ofBaseApp
{
	public:

		void setup();
		void update();
		void draw();
		void disableModes( int player );

		void audioRequested(float * input, int bufferSize, int nChannels);

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

        // Kinect
		ofxOpenNIContext niContext;
        ofxDepthGenerator niDepthGenerator;
        ofxImageGenerator niImageGenerator;
        ofxUserGenerator niUserGenerator;
        ofxHandGenerator* niHandGenerator;
        ofxTrackedHand* tracked;
        //ofxTrackedHand* tracked2;

        int nearThreshold;
        int farThreshold;

        // Multi windows
        window turnTable1;
        //window turnTable2;

        // Maximilian
        int	initialBufferSize;
		int	sampleRate;
        double sample, sample2, outputs[2];
        ofxMaxiMix mymix;
		ofxMaxiSample track1;
		ofxMaxiSample track2;

        // App vars
        ofTrueTypeFont font;
        bool started1, started2;
};









