#pragma once

#include "Model.h"

class Drawable
{
private:
	Model _model;

public:
	Drawable();
	~Drawable();

	virtual void draw() = 0;
};