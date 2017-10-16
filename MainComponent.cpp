/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioRecorderComponent.h"
#include "LogWindowComponent.h"
#include "AudioPlayBackComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
    {

		addAndMakeVisible(recorder);
		addAndMakeVisible(playBack1);
		addAndMakeVisible(playBack2);
		addAndMakeVisible(playBack3);
		addAndMakeVisible(logWindow);
		addAndMakeVisible(logWindow2);
		addAndMakeVisible(logWindow3);

		//dummy buttons
		addAndMakeVisible(featureExtractionButton);
		addAndMakeVisible(trainNeuralNetworkButton);
		addAndMakeVisible(activateFreePlayButton);

        setSize (1200, 1200);
        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
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
		Rectangle<int> area(getLocalBounds());
		Rectangle<int> sector1(area.removeFromLeft(getLocalBounds().getWidth()/3));
		Rectangle<int> sector2(area.removeFromLeft(getLocalBounds().getWidth()/3));
		Rectangle<int> sector3(area.removeFromLeft(getLocalBounds().getWidth()/3));

		int buttonSize = area.getHeight() / 10;
		int regItemSize = area.getHeight() / 5;
		int logWindowSize = area.getHeight() / 3;

		//sector 1
		recorder.setBounds(sector1.removeFromTop(regItemSize));
		logWindow.setBounds(sector1.removeFromTop(logWindowSize));
		
		//sector 2
		featureExtractionButton.setBounds(sector2.removeFromTop(buttonSize));
		logWindow2.setBounds(sector2.removeFromTop(logWindowSize));
		trainNeuralNetworkButton.setBounds(sector2.removeFromTop(buttonSize));
		logWindow3.setBounds(sector2.removeFromTop(logWindowSize));
		
		//sector 3
		activateFreePlayButton.setBounds(sector3.removeFromTop(buttonSize));
		playBack1.setBounds(sector3.removeFromTop(regItemSize));
		playBack2.setBounds(sector3.removeFromTop(regItemSize));
		playBack3.setBounds(sector3.removeFromTop(regItemSize));

	}


private:
    //==============================================================================

    // Your private member variables go here...
	AudioRecorderComponent recorder;
	LogWindowComponent logWindow;
	LogWindowComponent logWindow2;
	LogWindowComponent logWindow3;
	AudioPlayBackComponent playBack1;
	AudioPlayBackComponent playBack2;
	AudioPlayBackComponent playBack3;
	
	//Dummy Buttons for Now - need to create them as components
	TextButton featureExtractionButton;
	TextButton trainNeuralNetworkButton;
	TextButton activateFreePlayButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
