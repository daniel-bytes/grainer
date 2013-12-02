/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ApplicationController.h"

//==============================================================================
GrainerAudioProcessorEditor::GrainerAudioProcessorEditor(GrainerAudioProcessor* ownerFilter)
    : AudioProcessorEditor (ownerFilter), 
	  controller(nullptr),
	  gainLabel("", "Gain"),
	  speedLabel("", "Speed"),
	  grainsLabel("", "Grains"),
	  pitchLabel("", "Pitch"),
	  filePathLabel("", "Sample: ")
{
	Font labelFont(14.f, Font::FontStyleFlags::bold);
	Font valueFont(14.f);

	addAndMakeVisible(&selectSampleButton);
	selectSampleButton.setButtonText("...");
	selectSampleButton.setTooltip("Load a new sample");
	selectSampleButton.setBounds(20, 20, 30, 20);
	selectSampleButton.addListener(this);

	addAndMakeVisible(&filePathLabel);
	filePathLabel.setFont(labelFont);
	filePathLabel.setBounds(50, 20, 50, 20);

	addAndMakeVisible(&filePathValueLabel);
	filePathValueLabel.setFont(valueFont);
	filePathValueLabel.setBounds(100, 20, 600, 20);

	addAndMakeVisible(&gainSlider);
	gainSlider.setDoubleClickReturnValue(true, .9f);
	gainSlider.setBounds(20, 80, 100, 100);
	gainSlider.setRange(0, 1.0);
	gainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	gainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
	gainSlider.addListener(this);
    gainLabel.attachToComponent (&gainSlider, false);
    gainLabel.setFont(labelFont);
	
	addAndMakeVisible(&speedSlider);
	speedSlider.setDoubleClickReturnValue(true, 1.f);
	speedSlider.setBounds(170, 80, 100, 100);
	speedSlider.setRange(0, 2.0);
	speedSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	speedSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
	speedSlider.addListener(this);
    speedLabel.attachToComponent (&speedSlider, false);
    speedLabel.setFont(labelFont);
	
	addAndMakeVisible(&grainsSlider);
	grainsSlider.setDoubleClickReturnValue(true, .5f);
	grainsSlider.setBounds(320, 80, 100, 100);
	grainsSlider.setRange(0, 1.0);
	grainsSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	grainsSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
	grainsSlider.addListener(this);
    grainsLabel.attachToComponent (&grainsSlider, false);
    grainsLabel.setFont(labelFont);
	
	addAndMakeVisible(&pitchSlider);
	pitchSlider.setDoubleClickReturnValue(true, 0.f);
	pitchSlider.setBounds(470, 80, 100, 100);
	pitchSlider.setRange(-2.f, 2.0f);
	pitchSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
	pitchSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, false, 100, 20);
	pitchSlider.addListener(this);
    pitchLabel.attachToComponent (&pitchSlider, false);
    pitchLabel.setFont(labelFont);
	
	addAndMakeVisible(&directionButton);
	directionButton.setBounds(600, 160, 100, 20);
	directionButton.setButtonText("Reverse");
	directionButton.addListener(this);

    setSize (800, 600);
}

GrainerAudioProcessorEditor::~GrainerAudioProcessorEditor()
{

}

void GrainerAudioProcessorEditor::initialize(ApplicationController *controller)
{
	this->controller = controller;
}


//==============================================================================
void GrainerAudioProcessorEditor::paint (Graphics &g)
{
    g.fillAll(Colours::white);
}

void GrainerAudioProcessorEditor::buttonClicked (Button *button)
{
	jassert(controller != nullptr);

	if (button == &selectSampleButton) {
		WildcardFileFilter wildcardFilter ("*.wav", String::empty, "WAV files");

		FileBrowserComponent browser(FileBrowserComponent::canSelectFiles | FileBrowserComponent::openMode,
										File::nonexistent,
										&wildcardFilter,
										nullptr);
		FileChooserDialogBox dialogBox ("Select a sample",
										"Select a new sample to load...",
										browser,
										false,
										Colours::lightgrey);
		if (dialogBox.show())
		{
			File file = browser.getSelectedFile(0);
			var path = file.getFullPathName();
			this->setGlobalParameterValue(GlobalParameter::GrainSampler_FilePath, path);
			controller->updateParameterModel(GlobalParameter::GrainSampler_FilePath, path);
		}
	}
}

void GrainerAudioProcessorEditor::buttonStateChanged (Button* button)
{
	jassert(controller != nullptr);

	if (button == &directionButton) {
		controller->updateParameterModel(GlobalParameter::GrainSampler_Direction, button->getToggleState() ? 0 : 1.f);
	}
}

void GrainerAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
	jassert(controller != nullptr);

	if (slider == &gainSlider) {
		controller->updateParameterModel(GlobalParameter::GrainSampler_Gain, (float)slider->getValue());
	}
	else if (slider == &speedSlider) {
		controller->updateParameterModel(GlobalParameter::GrainSampler_Speed, (float)slider->getValue() * .5f);
	}
	else if (slider == &grainsSlider) {
		controller->updateParameterModel(GlobalParameter::GrainSampler_GrainSize, (float)slider->getValue());
	}
}

void GrainerAudioProcessorEditor::setGlobalParameterValue(GlobalParameter parameter, var value)
{
	switch(parameter)
	{
	case GlobalParameter::GrainSampler_Gain:
		gainSlider.setValue((float)value, NotificationType::dontSendNotification);
		break;
	case GlobalParameter::GrainSampler_Speed:
		speedSlider.setValue((float)value * 2.f, NotificationType::dontSendNotification);
		break;
	case GlobalParameter::GrainSampler_GrainSize:
		grainsSlider.setValue((float)value, NotificationType::dontSendNotification);
		break;
	case GlobalParameter::GrainSampler_Direction:
		directionButton.setToggleState((float)value < .5f, NotificationType::dontSendNotification);
		break;
	case GlobalParameter::GrainSampler_Pitch:
		pitchSlider.setValue(((float)value * 4.f) - 2.f, NotificationType::dontSendNotification);
		break;
	case GlobalParameter::GrainSampler_FilePath:
		filePathValueLabel.setText((String)value, NotificationType::dontSendNotification);
		break;
	}
}