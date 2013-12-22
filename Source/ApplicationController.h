#ifndef __APPLICATIONCONTROLLER_H__
#define __APPLICATIONCONTROLLER_H__

#include <functional>
#include "ParameterID.h"
#include "../JuceLibraryCode/JuceHeader.h"

class ApplicationModel;
class ApplicationView;
class Parameter;

/*
 * This is a traditional MVC controller class.
 * It's purpose is to facilitate data passing between the application (model) and
 * interface (view) layers.
 * This data is primarily Parameters, which represent both normal host-automated VST
 * parameters as well as any internal parameters (such as note sequencer values passed
 * from a sequencer to a synth).
 *
 * The core application logic will usually live in the class implementing ApplicationModel.
 *
 * In terms of the JUCE plugin wrapper system, the PluginProcessor class will hold an instance
 * of the ApplicationController and a class implementing ApplicationModel, and forward most calls
 * directly to the model class.  
 * The PluginEditor will usually implement ApplicationView.
 */
class ApplicationController
	: private Timer
{
public:
	ApplicationController(std::function<ApplicationModel*()> modelFetch, std::function<ApplicationView*()> viewFetch);
	~ApplicationController();

	void beginUITimer(void) { this->startTimer(20); }
	void endUITimer(void) { this->stopTimer(); }

public:
	virtual const Array<Parameter*> getAllParameters(void) const;
	var getParameterValue(ParameterID parameter) const;
	void serializeParameters(XmlElement *xml);
	void deserializeParameters(XmlElement *xml);
	void updateParameterUI(ParameterID parameter, var value);
	void updateParameterModel(ParameterID parameter, var value);
	void updateParameterModelAndUI(ParameterID parameter, var value);

	AudioFormatManager* getAudioFormatManager(void);
	ChangeListener* getWaveformChangeListener(void);

	bool sequencerIsPlaying(void) const { return isPlaying; }
	double getSequencerPosition(void) const { return playbackPosition; }

	void setSequencerParameters(bool isPlaying, double position) {
		this->isPlaying = isPlaying;
		this->playbackPosition = position;
	}

private:
	virtual void timerCallback();

private:
	std::function<ApplicationModel*()> getModel;
	std::function<ApplicationView*()> getView;
	bool isPlaying;
	double playbackPosition;
};

#endif //__APPLICATIONCONTROLLER_H__