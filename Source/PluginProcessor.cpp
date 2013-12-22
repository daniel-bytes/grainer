/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "AudioEngine.h"
#include "ApplicationController.h"

//==============================================================================
GrainerAudioProcessor::GrainerAudioProcessor()
{
	model = new AudioEngine();

	controller = new ApplicationController(
							std::bind(&GrainerAudioProcessor::getModel, this),
							std::bind(&GrainerAudioProcessor::getView, this));
}

GrainerAudioProcessor::~GrainerAudioProcessor()
{
	model = nullptr;
	controller = nullptr;
}

//==============================================================================
const String GrainerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int GrainerAudioProcessor::getNumParameters()
{
	return model->getNumPluginParameters();
}

float GrainerAudioProcessor::getParameter (int index)
{
	return model->getPluginParameterValue(index);
}

void GrainerAudioProcessor::setParameter (int index, float newValue)
{
	model->setPluginParameterValue(index, newValue);
}

const String GrainerAudioProcessor::getParameterName (int index)
{
	return model->getPluginParameterName(index);
}

const String GrainerAudioProcessor::getParameterText (int index)
{
	return model->getPluginParameterDisplay(index);
}

const String GrainerAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String GrainerAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool GrainerAudioProcessor::isInputChannelStereoPair (int index) const
{
	(void)index;
    return true;
}

bool GrainerAudioProcessor::isOutputChannelStereoPair (int index) const
{
	(void)index;
    return true;
}

bool GrainerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GrainerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GrainerAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double GrainerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GrainerAudioProcessor::getNumPrograms()
{
    return 0;
}

int GrainerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GrainerAudioProcessor::setCurrentProgram (int index)
{
	(void)index;
}

const String GrainerAudioProcessor::getProgramName (int index)
{
	(void)index;
    return String::empty;
}

void GrainerAudioProcessor::changeProgramName (int index, const String& newName)
{
	(void)index;
	(void)newName;
}

//==============================================================================
void GrainerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	(void)samplesPerBlock;
	// Initialize model and wire up to controller
	model->initialize(this->controller, sampleRate);
}

void GrainerAudioProcessor::releaseResources()
{
	model->stop();
}

void GrainerAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
	// Handle clock & midi
    AudioPlayHead::CurrentPositionInfo posInfo;
	auto playhead = this->getPlayHead();

	if (playhead != nullptr) {
		playhead->getCurrentPosition(posInfo);
		model->processClockMessage(posInfo);
	}

	model->processMidi(midiMessages);

	// Handle current audio frame
	model->processBlock(buffer, getNumInputChannels(), getNumOutputChannels());

    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i) {
        buffer.clear (i, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool GrainerAudioProcessor::hasEditor() const
{
	return true;
}

AudioProcessorEditor* GrainerAudioProcessor::createEditor()
{
    auto view = new GrainerAudioProcessorEditor(this);
	view->initialize(this->controller);

	return view;
}

ApplicationModel* GrainerAudioProcessor::getModel(void) 
{
	return this->model;
}

ApplicationView* GrainerAudioProcessor::getView(void)
{
	auto editor = this->getActiveEditor();

	if (editor == nullptr) {
		return nullptr;
	}

	return dynamic_cast<ApplicationView*>(editor);
}

//==============================================================================
void GrainerAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // Create an outer XML element..
    XmlElement xml("GrainrSettings");

	controller->serializeParameters(&xml);

    copyXmlToBinary(xml, destData);
}

void GrainerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	ScopedPointer<XmlElement> xml(getXmlFromBinary (data, sizeInBytes));

    if (xml != nullptr)
	{
		controller->deserializeParameters(xml);
	}
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GrainerAudioProcessor();
}
