#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioRecorder.h"

// used for initialising the deviceManager
static ScopedPointer<AudioDeviceManager> sharedAudioDeviceManager; 

class MainContentComponent   : public AudioAppComponent,
                               public Button::Listener
{
public:
    //==============================================================================
    MainContentComponent ()
       : deviceManager (getSharedAudioDeviceManager ())
    {
        addAndMakeVisible (recordButton);
        recordButton.setButtonText("Record");
        recordButton.addListener (this);

        addAndMakeVisible (playButton);
        playButton.setButtonText ("Play");
        playButton.addListener (this);

        // screen size in pixels
        setSize (400, 400);
        
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
        
        // sampleRate is hard coded for now
        // this is because the recorder cannot be initialised in prepareToPlay()
        // where the real sampleRate can be used (assumed to be 44.1K)
        int sampleRate = 44100;
        
        // create a new recorder for the microphone which stores samples
        // in a 2-channel buffer with a size of 10*sampleRate samples
        recorder = new AudioRecorder (sampleRate, 2, 3.f);
        // initialising sample rate in the recorder
        recorder->setSampleRate (sampleRate);
         //deviceManager needs to be setup for the recorder
        deviceManager.addAudioCallback (recorder);
    }

    ~MainContentComponent ()
    {
        deviceManager.removeAudioCallback (recorder);
        delete recorder;
        shutdownAudio ();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        
        isPlaying = false;

    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion (); // clearing the buffer frame BEFORE writing to it
        if (readIndex < recorder->getBufferLengthInSamples () && isPlaying)
        {

            const int numInputChannels = recorder->getNumChannels ();
            const int numOutputChannels = bufferToFill.buffer->getNumChannels ();

            int outputSamples = bufferToFill.buffer->getNumSamples (); // number of samples need to be output next frame
            writeIndex = bufferToFill.startSample; // write index, which is passed to the copyFrom() function

            while(outputSamples > 0 && readIndex != recorder->getSampBuff().getNumSamples()) // run this until the frame buffer is filled and the readindex does not exceeded the input
            {
                int samplesToProcess = jmin(outputSamples, recorder->getSampBuff().getNumSamples() - readIndex);
            
                for (int ch = 0; ch < numOutputChannels; ch++) // iterate through output channels
                {

                    bufferToFill.buffer->copyFrom(
                        ch, // destination channel
                        writeIndex, // destination sample
                        recorder->getSampBuff(), // source buffer
                        ch % numInputChannels, // source channel
                        readIndex, // source sample
                        samplesToProcess); // number of samples to copy
                }
                
                outputSamples -= samplesToProcess; // decrement the number of output samples rquired to be written into the framebuffer
                readIndex += samplesToProcess;
                writeIndex += samplesToProcess;
            }
        }
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.
    }   

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
    }

    void resized() override
    {
        recordButton.setBounds (0, 0, getWidth()/2, getHeight());
        playButton.setBounds (getWidth()/2, 0, getWidth()/2, getHeight());
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }

private:
    void startRecording ()
    {
        recorder->startRecording ();

        recordButton.setButtonText ("Stop");
    }

    void stopRecording()
    {
        recorder->stop();
        recordButton.setButtonText ("Record");
    }
    
    /* This function sets up the I/O to stream audio to/from a device
     * RuntimePermissions::recordAudio requests the microphone be used as audio input
     */
    AudioDeviceManager& getSharedAudioDeviceManager()  
    {  
        if (sharedAudioDeviceManager == nullptr)
        {
            sharedAudioDeviceManager = new AudioDeviceManager();
            RuntimePermissions::request (RuntimePermissions::recordAudio, runtimePermissionsCallback);
        }
        return *sharedAudioDeviceManager;
    }
    
    // checks to see if the request for microphone access was granted
    static void runtimePermissionsCallback (bool wasGranted)
    {
        int numInputChannels = wasGranted ? 2 : 0;
        sharedAudioDeviceManager->initialise (numInputChannels, 2, nullptr, true, String(), nullptr);
    }

    void startPlaying ()
    {
        playButton.setButtonText ("Stop");
        isPlaying = true;
    }

    void stopPlaying()
    {
        
        playButton.setButtonText ("Play");       
        isPlaying = false;
        readIndex = 0;
    }

    void buttonClicked (Button* button) override
    {
        if (button == &recordButton)
        {
            if (recorder->isRecording())
                stopRecording();
            else
                startRecording();
        }
        
        if(button == &playButton)
        {
            if(isPlaying)
                stopPlaying();
            else
                startPlaying();
        }
    }

    //void isDown (Button* button)
    //{
    //    if (button == &recordButton)
    //    {
    //        if (!recorder->isRecording())
    //            startRecording();
    //    }
    //}
     
    //==============================================================================
    TextButton recordButton;
    TextButton playButton;
    Boolean isPlaying;
    int readIndex;
    int writeIndex;
    AudioDeviceManager& deviceManager; // manages audio I/O devices 
    AudioRecorder *recorder; // recording from a device to a file
    int sampleRate;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
    
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
