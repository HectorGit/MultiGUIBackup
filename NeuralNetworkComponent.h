/*
  ==============================================================================

    NeuralNetworkComponent.h
    Created: 16 Oct 2017 10:29:13pm
    Author:  Owner

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class NeuralNetworkComponent : public Component,
	private Button::Listener,
	private Timer
{
public:
    NeuralNetworkComponent()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
		trainNetworkButton.setButtonText("Train the Neural Network");
		addAndMakeVisible(trainNetworkButton);
		trainNetworkButton.addListener(this);
		message = "Initializing the Neural Network";
		setSize(200,200);
    }

    ~NeuralNetworkComponent()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        /*g.drawText ("NeuralNetworkComponent", getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text*/
    }

    void resized() override
    {
		Rectangle<int> brownies(getLocalBounds());
		int buttonHeight = 80;
		trainNetworkButton.setBounds(brownies.removeFromTop(buttonHeight).reduced(5));
    }

	String message;

	//dummy function
	void trainNeuralNetwork() {
		startTimer(500);
		if (getTimerInterval() >= 1000) {
			message = "Receiving Feature Vectors";
			stopTimer();
		}
		startTimer(500);
		if (getTimerInterval() >= 1000) {
			message = "Feedforward";
			stopTimer();
		}
		startTimer(500);
		if (getTimerInterval() > 1000) {
			message = "BackPropagation";
			stopTimer();
		}
		startTimer(500);
		if (getTimerInterval() > 1000) {
			message = "Done";
			stopTimer();
		}
	}

	TextButton* getButton() {
		return &trainNetworkButton;
	}

private:

	TextButton trainNetworkButton;
	void timerCallback() override 
	{
		trainNetworkButton.setButtonText("Neural Network Training Starting");
	}

	void buttonClicked(Button* button) override
	{	if (button == &trainNetworkButton)
		{
			startTimer(500);
			
		}
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NeuralNetworkComponent)
};
