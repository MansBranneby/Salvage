#pragma once
#include "DynamicObject.h"

class Robot : public DynamicObject
{
private:

public:
	Robot();
	~Robot();
	virtual void abstract() = 0;
};