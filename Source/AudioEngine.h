#ifndef __AUDIOENGINE_H__
#define __AUDIOENGINE_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ApplicationModel.h"
#include "GlobalParameters.h"

class Parameter;
class ApplicationController;
class GrainSampler;

class AudioEngine
	: public ApplicationModel
{
public:
	AudioEngine(void);
	~AudioEngine(void);

public:
	void initialize(ApplicationController *controller, double sampleRate);
	void stop();

public:
	// Plugin parameter handling
	virtual int getNumPluginParameters(void) const;
	virtual float getPluginParameterValue(int index) const;
	virtual void setPluginParameterValue(int index, float value);
	virtual String getPluginParameterName(int index) const;
	virtual String getPluginParameterText(int index) const;

public:
	virtual float getGlobalParameterValue(GlobalParameter parameter) const;
	virtual void setGlobalParameterValue(GlobalParameter parameter, float value);

public:
	void processClockMessage(AudioPlayHead::CurrentPositionInfo &posInfo);
	void processMidi(MidiBuffer& midiMessages);
	float processSample(int channel, float value);

private:
	void configureParameters();
	Parameter* configureParameter(GlobalParameter globalID, int localID, String name, String displayName, float initialValue, bool isPluginParameter);

private:
	// Parameter collections
	OwnedArray<Parameter> allParameters;
	HashMap<GlobalParameter, Parameter*, GlobalParameterHash> parameterMap;
	Array<GlobalParameter> pluginParameterLookups;

	// Application controller
	ApplicationController *controller;

	// Dsp elements
	float masterGain;
	ScopedPointer<GrainSampler> grainSampler1;
};

#endif //__AUDIOENGINE_H__