#include "window.h"

//--------------------------------------------------------------
void window::setup()
{
    offset = 1.0f;
    pitch = 0.0f;
    middle = (ofGetHeight() / 2) + 250;
    angle = 0.0f;

    discAngle = 0;
    headAngle = 0;
    center.x = 811;
    center.y = 599;
    radius = 0;
    oldRadius = 0;
    time = 0;

    mode = "NONE";

    loadDisc = false;
    scratching = false;
    forward = false;
    backward = false;
    moveHead = false;

    if(!player.loadImage("player.jpg")) printf("Player BG loading failed");
    if(!arm1.loadImage("right.png")) printf("Left Hand loading failed");
    if(!arm2.loadImage("right2.png")) printf("Left Hand 2 loading failed");
    if(!ring.loadImage("ring.png")) printf("Ring loading failed");
    //if(!red.loadImage("red.png")) printf("Red loading failed");
    if(!head.loadImage("head.png")) printf("Head loading failed");
    if(!readHead.loadImage("read-head.png")) printf("Read Head loading failed");
}


//--------------------------------------------------------------
void window::update()
{
    handOld.x = hand.x;
    handOld.y = hand.y;
    hand.x = kinect.x * (ofGetWidth() / 640);
    hand.y = kinect.y * (ofGetHeight() / 480);

    if( hand.x < 1100 )
    {
        if( scratching )
        {
            float overMiddle = 0;

            if( hand.x < 800 )
                overMiddle = middle - hand.y;
            else
                overMiddle = hand.y - middle;

            offset = (overMiddle / 300);
        }
        else
        {
            // Get the angle between the hand and the vinyl's center
            xCoord = 0;
            yCoord = 0;
            int fingerX = hand.x + 190;
            int fingerY = hand.y + 30;

            xCoord = fingerX - center.x;
            yCoord = fingerY - center.y;

            oldRadius = radius;
            radius = sqrt(xCoord*xCoord + yCoord*yCoord);

            oldAngle = angle;
            angle = ofRadToDeg(atan2(yCoord,xCoord));

            if( angle < 0 )
                angle = 360 + angle;

            // Recuperate the normal pitch after scratching or seeking
            if( !forward && !backward && offset != 1.0f )
            {
                recoverPitch();
            }

            // Find the hand's movement direction and increment
            if( oldAngle < angle && forward )
            {
                offset += (angle - oldAngle) / (360 * 2);
            }
            else if( oldAngle > angle && backward )
            {
                if( oldAngle - angle < 300 )
                    offset -= (oldAngle - angle) / (360 * 2);
            }
        }

        if( time > 24 )
        {
            recoverPitch();
        }
    }
    else
        recoverPitch();
}

//--------------------------------------------------------------
void window::draw()
{
    ofEnableAlphaBlending();

    player.draw(0,0);
    head.draw(1569, 791 + (pitch * 10));

    if( moveHead )
    {
        if( discAngle > 359 || discAngle < -359 )
            discAngle = 0;
        else
            discAngle += offset * (pitch + 10);

        ofPushMatrix();
            ofTranslate(271, 58, 0);
            ofPushMatrix();
                ofTranslate(ring.width/2, ring.height/2, 0);
                ofRotate(discAngle, 0, 0, 1);
                ofPushMatrix();
                    ring.draw(-ring.width/2,-ring.height/2);
                ofPopMatrix();
            ofPopMatrix();
        ofPopMatrix();

        if( disc.isAllocated() )
        {
            ofPushMatrix();
                ofTranslate(320, 107, 0);
                ofPushMatrix();
                    ofTranslate(disc.width/2, disc.height/2, 0);
                    ofRotate(discAngle, 0, 0, 1);
                    ofPushMatrix();
                        disc.draw(-disc.width/2,-disc.height/2);
                    ofPopMatrix();
                ofPopMatrix();
            ofPopMatrix();
        }

        ofPushMatrix();
            ofTranslate(1279, 92, 0);
            ofPushMatrix();
                ofTranslate(178, 225, 0);
                ofRotate(headAngle, 0, 0, 1);
                ofPushMatrix();
                    readHead.draw(-178,-225);
                ofPopMatrix();
            ofPopMatrix();
        ofPopMatrix();

        if( headAngle < 24 )
            headAngle += 1;
        else
            loadDisc = true;
    }
    else
        readHead.draw(1279, 92);

    ofSetColor(255);
    //ofCircle( lHand.x, lHand.y, 10);

    if( hand.x < 1100 )
    {
        arm1.draw( hand.x, hand.y );

        if( hand.y + 50 < handOld.y )
        {
            time = 0;
            //scratching = true;
            //ofBackground(ofRandom(255), ofRandom(255), ofRandom(255));
        }
        else
            time++;
    }
    else
    {
        arm2.draw( hand.x, hand.y );
    }

    ofDrawBitmapString("Absolute position: " + ofToString(hand.x) + ", " + ofToString(hand.y), 20,20);
    ofDrawBitmapString("Relative position: " + ofToString(xCoord) + ", " + ofToString(yCoord), 20,40);
    ofDrawBitmapString("Offset: " + ofToString(offset), 20,60);
    ofDrawBitmapString("Radius: " + ofToString(radius), 20,80);
    ofDrawBitmapString("Angle: " + ofToString(angle, 3), 20,100);
    ofDrawBitmapString("Recovery frame: " + ofToString(time), 20,120);
    ofDrawBitmapString("Elapsed time: " + ofToString(frame) + " / " + ofToString(total), 20,140);
    ofDrawBitmapString("Selected mode: " + mode, 20,160);
}

//--------------------------------------------------------------
void window::recoverPitch()
{
    if( offset > 1.0f )
        offset -= 0.03f;
    else if( offset < 1.0f )
        offset += 0.03f;

    if( offset > 0.95f && offset < 1.05f )
    {
        offset = 1.0f;

        time = 0;
        //scratching = false; // uncomment when enabled automatic scratching
    }
}

//--------------------------------------------------------------
void window::loadVinyl( string discName )
{
    if(!disc.loadImage(discName)) printf("Disc loading failed");
}
