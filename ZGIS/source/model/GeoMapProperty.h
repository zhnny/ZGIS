#pragma once
#include <QtCore/qstring.h>
#include "GeoBase.hpp"

class GeoMapProperty
{
public:
	QString name = "untitled";
	GeoExtent extent;

public:
	GeoMapProperty() {};
	~GeoMapProperty() {};

private:

};