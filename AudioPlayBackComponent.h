/*
  ==============================================================================

    AudioPlayBackComponent.h
    Created: 13 Oct 2017 4:41:40pm
    Author:  Owner

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DemoThumbnailComp.h"

//==============================================================================
/*
*/
class AudioPlayBackComponent    : public AudioAppComponent,

	//adjustment
	private AudioIODeviceCallback,

	private FileBrowserListener,
	private Button::Listener,
	private Slider::Listener,
	private ChangeListener
{
public:
    AudioPlayBackComponent() : //deviceManager(MainAppWindow::getSharedAudioDeviceManager()),
							//adjustement
		audioSetupComp(audioDeviceManager, 0, 0, 0, 256,
			true, // showMidiInputOptions must be true
			true, true, false),
		//end of adjustment
		thread("audio file preview"),
		directoryList(nullptr, thread),
		fileTreeComp(directoryList)
    {
		//adjustment
		audioDeviceManager.initialise(0, 2, nullptr, true, String(), nullptr);
		audioDeviceManager.addAudioCallback(this);
		//deviceManager.addAudioCallback(&recorder);
		//end of adjustment

		addAndMakeVisible(zoomLabel);
		zoomLabel.setText("zoom:", dontSendNotification);
		zoomLabel.setFont(Font(15.00f, Font::plain));
		zoomLabel.setJustificationType(Justification::centredRight);
		zoomLabel.setEditable(false, false, false);
		zoomLabel.setColour(TextEditor::textColourId, Colours::black);
		zoomLabel.setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		addAndMakeVisible(followTransportButton);
		followTransportButton.setButtonText("Follow Transport");
		followTransportButton.addListener(this);

		addAndMakeVisible(explanation);
		explanation.setText("Select an audio file in the treeview above, and this page will display its waveform, and let you play it..", dontSendNotification);
		explanation.setFont(Font(14.00f, Font::plain));
		explanation.setJustificationType(Justification::bottomRight);
		explanation.setEditable(false, false, false);
		explanation.setColour(TextEditor::textColourId, Colours::black);
		explanation.setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		addAndMakeVisible(zoomSlider);
		zoomSlider.setRange(0, 1, 0);
		zoomSlider.setSliderStyle(Slider::LinearHorizontal);
		zoomSlider.setTextBoxStyle(Slider::NoTextBox, false, 80, 20);
		zoomSlider.addListener(this);
		zoomSlider.setSkewFactor(2);

		addAndMakeVisible(thumbnail = new DemoThumbnailComp(formatManager, transportSource, zoomSlider));
		thumbnail->addChangeListener(this);

		addAndMakeVisible(startStopButton);
		startStopButton.setButtonText("Play/Stop");
		startStopButton.addListener(this);
		startStopButton.setColour(TextButton::buttonColourId, Colour(0xff79ed7f));
		startStopButton.setColour(TextButton::textColourOffId, Colours::black);

		addAndMakeVisible(fileTreeComp);

		// audio setup
		formatManager.registerBasicFormats();

		directoryList.setDirectory(File::getSpecialLocation(File::userHomeDirectory), true, true);
		thread.startThread(3);

		fileTreeComp.setColour(FileTreeComponent::backgroundColourId, Colours::lightgrey.withAlpha(0.6f));
		fileTreeComp.addListener(this);

		deviceManager.addAudioCallback(&audioSourcePlayer);
		audioSourcePlayer.setSource(&transportSource);

		setOpaque(true);

		//originally here
		setSize(800, 600);
		setAudioChannels(2, 2);

    }

    ~AudioPlayBackComponent()
    {
		transportSource.setSource(nullptr);
		audioSourcePlayer.setSource(nullptr);

		deviceManager.removeAudioCallback(&audioSourcePlayer);
		fileTreeComp.removeListener(this);
		thumbnail->removeChangeListener(this);
		followTransportButton.removeListener(this);
		zoomSlider.removeListener(this);
		//originally here
		//		deviceManager.removeAudioCallback(&recorder);
		shutdownAudio();
    }

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		// This function will be called when the audio device is started, or when
		// its settings (i.e. sample rate, block size, etc) are changed.

		// You can use this function to initialise any resources you might need,
		// but be careful - it will be called on the audio thread, not the GUI thread.

		// For more details, see the help for AudioProcessor::prepareToPlay()
	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
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
	void paint(Graphics& g) override
	{
		// (Our component is opaque, so we must completely fill the background with a solid colour)
		g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

		// You can add your drawing code here!
	}

	void resized() override
	{
		Rectangle<int> r(getLocalBounds().reduced(4));

		Rectangle<int> controls(r.removeFromBottom(90));

		explanation.setBounds(controls.removeFromRight(controls.getWidth() / 3));
		Rectangle<int> zoom(controls.removeFromTop(25));
		zoomLabel.setBounds(zoom.removeFromLeft(50));
		zoomSlider.setBounds(zoom);
		followTransportButton.setBounds(controls.removeFromTop(25));
		startStopButton.setBounds(controls);

		r.removeFromBottom(6);
		thumbnail->setBounds(r.removeFromBottom(140));
		r.removeFromBottom(6);
		fileTreeComp.setBounds(r);
	}

	//adjustment
	void audioDeviceIOCallback(const float** /*inputChannelData*/, int /*numInputChannels*/,
		float** outputChannelData, int numOutputChannels,
		int numSamples) override
	{
		//so the compiler doesn't cry - grab from the recording button sample code
	}

	void audioDeviceAboutToStart(AudioIODevice* device) override
	{
		//so the compiler doesn't cry - grab from the recording button sample code.
	}

	void audioDeviceStopped() override
	{
		//so the compiler doesn't cry - originally empty - grab from the recording button sample code.
	}
	// Your private member variables go here...

	//end of adjustment

private:
	//ADJUSTMENT
	//AudioDeviceManager& deviceManager;
	AudioDeviceManager audioDeviceManager;         // [3]
	AudioDeviceSelectorComponent audioSetupComp;   // [4]
												   //END OF ADJUSTMENT

	AudioFormatManager formatManager;
	TimeSliceThread thread;
	DirectoryContentsList directoryList;

	AudioSourcePlayer audioSourcePlayer;
	AudioTransportSource transportSource;
	ScopedPointer<AudioFormatReaderSource> currentAudioFileSource;

	ScopedPointer<DemoThumbnailComp> thumbnail;
	Label zoomLabel, explanation;
	Slider zoomSlider;
	ToggleButton followTransportButton;
	TextButton startStopButton;
	FileTreeComponent fileTreeComp;

	//==============================================================================

	void showFile(const File& file)
	{
		loadFileIntoTransport(file);

		zoomSlider.setValue(0, dontSendNotification);
		thumbnail->setFile(file);
	}

	void loadFileIntoTransport(const File& audioFile)
	{
		// unload the previous file source and delete it..
		transportSource.stop();
		transportSource.setSource(nullptr);
		currentAudioFileSource = nullptr;

		AudioFormatReader* reader = formatManager.createReaderFor(audioFile);

		if (reader != nullptr)
		{
			currentAudioFileSource = new AudioFormatReaderSource(reader, true);

			// ..and plug it into our transport source
			transportSource.setSource(currentAudioFileSource,
				32768,                   // tells it to buffer this many samples ahead
				&thread,                 // this is the background thread to use for reading-ahead
				reader->sampleRate);     // allows for sample rate correction
		}
	}

	void selectionChanged() override
	{
		showFile(fileTreeComp.getSelectedFile());
	}

	void fileClicked(const File&, const MouseEvent&) override {}
	void fileDoubleClicked(const File&) override {}
	void browserRootChanged(const File&) override {}

	void sliderValueChanged(Slider* sliderThatWasMoved) override
	{
		if (sliderThatWasMoved == &zoomSlider)
			thumbnail->setZoomFactor(zoomSlider.getValue());
	}

	void buttonClicked(Button* buttonThatWasClicked) override
	{
		if (buttonThatWasClicked == &startStopButton)
		{
			if (transportSource.isPlaying())
			{
				transportSource.stop();
			}
			else
			{
				transportSource.setPosition(0);
				transportSource.start();
			}
		}
		else if (buttonThatWasClicked == &followTransportButton)
		{
			thumbnail->setFollowsTransport(followTransportButton.getToggleState());
		}
	}

	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		if (source == thumbnail)
			showFile(thumbnail->getLastDroppedFile());
	}
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayBackComponent)
};