#ifndef __APPLICATIONMODEL_H__
#define __APPLICATIONMODEL_H__

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterID.h"

class Parameter;

/*
 * ApplicationModel defines an interface for defining storing any parameter values and other data.
 * This class will usually be the primary application class which implements the core DSP algorithms.
 *
 * The class implementing this interface should hold a reference to the ApplicationController so it could
 * notify the controller of any internal parameter changes (via things like timer callbacks, sequencers, etc).
 * The controller can then notify any attached ApplicationViews.
 */

#include "StepSequencerData.h"

class ApplicationModel
{
public:
	// Plugin parameter handlers
	virtual int getNumPluginParameters(void) const = 0;
	virtual float getPluginParameterValue(int index) const = 0;
	virtual void setPluginParameterValue(int index, float value) = 0;
	virtual String getPluginParameterName(int index) const = 0;
	virtual String getPluginParameterDisplay(int index) const = 0;

	// Global parameter handlers
	virtual var getParameterValue(ParameterID parameter) const = 0;
	virtual void setParameterValue(ParameterID parameter, var value) = 0;
	virtual const Array<Parameter*> getAllParameters(void) const = 0;

	virtual AudioFormatManager* getAudioFormatManager(void) = 0;
	virtual const StepSequencerData* getSequencerData(void) const = 0;
	virtual void resetSequencerSlices(void) = 0;
};

#endif //__APPLICATIONMODEL_H__