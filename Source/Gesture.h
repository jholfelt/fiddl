/*
  ==============================================================================

    Gesture.h
    Created: 23 Oct 2017 1:11:42pm
    Author:  geri

    Description:  Receives mouse coordinates from the PlayComponent and calculates
    			  gesture parameters. 

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class Gesture 
{
	public:
        typedef struct Position { Point<float> pos; Point<float> prevPos; const MouseInputSource* mis; int sourceIndex; Path path; int totalPathLength; float pathAlpha;
                                    Position(const MouseInputSource& mouseInput, Point<float> point)
                                    {
                                        this->mis = &mouseInput;
                                        this->pos = point;
                                        this->prevPos = pos;
                                        this->sourceIndex = mouseInput.getIndex();
                                    }
                                } Position;
    
		static void setVelocity(float x, float y);
        static void setVelocity(float vel);
		static void setDirection(float p [][2]);
        static String getDirection();
        static void setTap (float p [2][2]);
        static bool tap();
        static float getVelocityX();
        static float getVelocityY();
        static float getVelocity();
        static void setVelocityMax(float vel);
        static float getVelocityMax();
        static void setCentroid(float C);
        static float getCentroid();
        static float spectralCentroid;
    
        static void setDistBetweenFingers(int i);
        static void resetDistBetweenFingers();
        static float getDistBetweenFingers();
    
        static void fillDirBuff(float x, float y);
        
        static void setAbsDistFromOrigin(float x, float y);
        static float getAbsDistFromOrigin();
    
        static float getDiscretePitch();

        // multi touch
        static void addFinger(const MouseEvent& e); // adds new input source to the array
        static void rmFinger(const MouseEvent& e); // removes input source from the array
        static void updateFingers(const MouseInputSource& mis, int index); // update finger coordinates
        static Point<float> getFingerPosition(int index);
        static Point<float> getFingerPositionScreen(int index);
        static int getSourceIndex(int index);
        static Path getPath(int index);
        static int getNumFingers(); // returns the number of fingers
        static void drawPath(Graphics& g, Path p, int i);

        static void setResetPos(bool reset);
        static bool getResetPos();

        static void setCompWidth(float w);
        static void setCompHeight(float h);
    
        static bool resetPos;
        static int directionBuffSize;
    
	private:
        static OwnedArray<Position> fingers; // stores the input sources, with their respective coordinates and indeces

        static Point<float> normalizeCoordinates(Point<float> p);

        static float compWidth;
        static float compHeight;

		static float xNew;
		static float xTemp;
        static float xDelta;
    
        static float yNew;
        static float yTemp;
        static float yDelta;
    
        static float dist;
        static float absDistFromOrigin;
        static float velMax;
    
        static float discretePitchVal;
    
        static float directionDeltaX;
        static float directionDeltaY;
        static float absDirectionDeltaX;
        static float absDirectionDeltaY;
        static String direction;
    
        static float distBetweenFingers;
        static Point<float> pinchP1;
        static Point<float> pinchP2;
    
        static float tapDist;
        static bool isTap;
};
