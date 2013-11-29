#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#include "../JuceLibraryCode/JuceHeader.h"

class Parameter
{
public:
	Parameter(int globalID, int localID, const String &name, const String &displayName, float initialValue)
	{
		this->globalID = globalID;
		this->localID = localID;
		this->name = name;
		this->displayName = displayName;
		this->value = initialValue;
	}

	int getGlobalID(void) const {
		return this->globalID;
	}

	int getLocalID(void) const {
		return this->localID;
	}

	String getName(void) const {
		return this->name;
	}

	String getDisplayName(void) const {
		return this->displayName;
	}

	float getValue(void) const {
		return this->value;
	}

	void setValue(float value) {
		this->value = value;
	}

private:
	int globalID;
	int localID;
	String name;
	String displayName;
	float value;
};

#endif //__PARAMETER_H__