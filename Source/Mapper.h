/*
  ==============================================================================

    Mapper.h
    Created: 23 Oct 2017 11:56:31pm
    Author:  geri

    Description:  This class holds the AudioParameter objects, routes the gesture
    			  parameters to audio parameters and updates them. 

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Gain.h"

class Mapper
{
	public:
		static void routeParameters(); // wrapper mapping functions
		static void updateParameters(); // update mappings when gesture values are updated
		static void mapToGain(float val);
		// this method takes a string pair to map a "gestureParameter" to an "audioParameter"
		static void mapFromTo(std::string gestureParameter, std::string audioParameter);
		
		private:		
		static AudioParameterFloat *gainLevel;
		static Gain *gain;
		static std::vector< std::pair <std::string,std::string> > mapping;
};