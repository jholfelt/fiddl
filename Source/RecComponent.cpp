/*
  ==============================================================================

    RecComponent.cpp
    Created: 11 Oct 2017 1:06:39pm
    Author:  Jonas Holfelt

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "RecComponent.h"

//==============================================================================
RecComponent::RecComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible (recordButton);
    recordButton.setButtonText ("Hold to record");
    recordButton.addListener(this);
    isRecording = false;

}

RecComponent::~RecComponent()
{
}

void RecComponent::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

}

void RecComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    recordButton.setBounds(getWidth()/2-(getWidth()/3/2),getHeight()/2-(getHeight()/3/2),getWidth()/3,getHeight()/3);

    
}

void RecComponent::buttonClicked (Button* button)
{
    
}

void RecComponent::buttonStateChanged(Button* button)
{
    if(recordButton.isDown())
        startRecording();
    else
        stopRecording();
        
}

void RecComponent::startRecording()
{
    recordButton.setButtonText("recording");
}

void RecComponent::stopRecording()
{
    recordButton.setButtonText("Hold to Record");
}
