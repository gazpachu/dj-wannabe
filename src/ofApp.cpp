#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	//ofSetFrameRate(60);
	//ofSetVerticalSync(true);
	//font.loadFont("bebasneue-webfont.ttf", 25);

    // Init Kinect
	nearThreshold = 400;
    farThreshold = 700;

	niContext.setup();
	//niContext.toggleRegisterViewport();
	niContext.toggleMirror();

    niDepthGenerator.setup( &niContext );
    //niImageGenerator.setup( &niContext );

    //niUserGenerator.setup( &niContext );
    //niUserGenerator.setSmoothing(0.1f);
	//niUserGenerator.setMaxNumberOfUsers(1);

    niHandGenerator = new ofxHandGenerator();
	niHandGenerator->setup(&niContext,1);
	niHandGenerator->setMaxNumHands(1);
	niHandGenerator->setSmoothing(0.1f);
	//niHandGenerator->setFilterFactors(0.0f);
    //niHandGenerator->startTrackHands();
    
    turnTable1.setup();
    //turnTable2.setup();

	// Maxim stuff
	sampleRate = 44100;
	initialBufferSize = 512;
	ofSoundStreamSetup(2,0,this, sampleRate, initialBufferSize, 4);
}

//--------------------------------------------------------------
void ofApp::update()
{
    // Update Kinect
    niDepthGenerator.setDepthThreshold(0, nearThreshold, farThreshold);
    niContext.update();
    niDepthGenerator.update();
    //niImageGenerator.update();
    //niUserGenerator.update();
    tracked = niHandGenerator->getHand(0);
    //tracked2 = niHandGenerator->getHand(1);

    // Start moving things if we are tracking hands
    if( tracked->projectPos.y )
        started1 = true;

//    if( tracked2->projectPos.y )
//        started2 = true;

    // Set the new hand positions in the turntables
    if( started1 )
    {
        turnTable1.kinect.x = tracked->projectPos.x;
        turnTable1.kinect.y = tracked->projectPos.y;
    }

//    if( started2 )
//    {
//        turnTable2.kinect.x = tracked2->projectPos.x;
//        turnTable2.kinect.y = tracked2->projectPos.y;
//    }

    ofSoundUpdate();
    
    turnTable1.update();
    //turnTable2.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    //ofEnableAlphaBlending();
    //font.drawString("", ofGetWidth()/2 - 350, ofGetHeight()/2);
    //niHandGenerator->drawHands();
    
    turnTable1.draw();
    //turnTable2.draw();
}

//--------------------------------------------------------------
void ofApp::audioRequested(float * output, int bufferSize, int nChannels)
{
    for (int i = 0; i < bufferSize; i++)
    {
        if( turnTable1.loadDisc && turnTable1.headAngle >= 24 )
        {
            sample = track1.play( turnTable1.offset + ( turnTable1.pitch / 10 ) );
            turnTable1.frame = track1.position * track1.myChannels;
        }

//        if( turnTable2.loadDisc && turnTable2.headAngle >= 24 )
//        {
//            sample2 = track2.play( turnTable2.offset + ( turnTable2.pitch / 10 ) );
//            turnTable2.frame = track2.position * track2.myChannels;
//        }

        if( turnTable1.frame > turnTable1.total )
        {
            turnTable1.moveHead = false;
            turnTable1.loadDisc = false;
        }

//        if( turnTable2.frame > turnTable2.total )
//        {
//            turnTable2.moveHead = false;
//            turnTable2.loadDisc = false;
//        }

        mymix.stereo( sample + sample2, outputs, 0.5 );

        output[i*nChannels    ] = outputs[0];
        output[i*nChannels + 1] = outputs[1];
    }
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if( key == '1')
    {
        track1.load(ofToDataPath("sounds/track1.wav"));
        track1.getLength();
        turnTable1.total = track1.length;
        turnTable1.loadVinyl( "disc1.png" );
        turnTable1.moveHead = true;
    }

    if( key == '2')
    {
//        track2.load(ofToDataPath("sounds/track2.wav"));
//        track2.getLength();
//        turnTable2.total = track2.length;
//        turnTable2.loadVinyl( "disc2.png" );
//        turnTable2.moveHead = true;
    }

    if( key == '+' && turnTable1.pitch < 16 )
        turnTable1.pitch += 1;

    if( key == '-' && turnTable1.pitch > -16 )
        turnTable1.pitch -= 1;

	if( key == 32 ) //spacebar
	{
	    started1 = false;
//        started2 = false;

        niHandGenerator->dropHands();

        delete tracked;
//        delete tracked2;
        delete niHandGenerator;

        niHandGenerator = new ofxHandGenerator();
        niHandGenerator->setup(&niContext,1);
        niHandGenerator->setMaxNumHands(1);
	}

	if( key == 'x' )
	{
	    if( turnTable1.forward )
	    {
	        turnTable1.forward = false;
	        turnTable1.mode = "NONE";
	    }
        else
        {
            disableModes(1);
            turnTable1.forward = true;
            turnTable1.mode = "FORWARD";
        }
	}

    if( key == 'z' )
    {
        if( turnTable1.backward )
        {
            turnTable1.backward = false;
            turnTable1.mode = "NONE";
        }
        else
        {
            disableModes(1);
            turnTable1.backward = true;
            turnTable1.mode = "BACKWARD";
        }
    }

    if( key == 'c' )
    {
        if( turnTable1.scratching )
        {
            turnTable1.scratching = false;
            turnTable1.mode = "NONE";
        }
        else
        {
            disableModes(1);
            turnTable1.scratching = true;
            turnTable1.mode = "SCRATCHING";
        }
    }
}

void ofApp::disableModes( int player )
{
    if( player == 1 )
    {
        turnTable1.forward = false;
        turnTable1.backward = false;
        turnTable1.scratching = false;
        turnTable1.mode = "NONE";
    }

    if( player == 2 )
    {
//        turnTable2.forward = false;
//        turnTable2.backward = false;
//        turnTable2.scratching = false;
//        turnTable2.mode = "NONE";
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
