#pragma once
#include <QtCore/qstring.h>

enum GeoFieldType
{
	FieldInt = 0,
	FieldDouble,
	FieldText,
	FeildUnknown
};

class GeoFieldDefine
{
public:
	GeoFieldDefine();
	~GeoFieldDefine();
	GeoFieldDefine(QString nameInput, int widthInput, GeoFieldType typeInput)
		:name(nameInput),width(widthInput),type(typeInput){}
	GeoFieldDefine(const GeoFieldDefine& rhs)
		:name(rhs.name), width(rhs.width), type(rhs.type) {}
	bool isSame(const GeoFieldDefine& rhs) { return name == rhs.name; }
	GeoFieldType getType()const { return type; }
	QString getName()const { return name; }
	int getWidth()const { return width; }
	GeoFieldType setType(GeoFieldType typeInput) {  type=typeInput; }
	QString setName(QString nameInput) { this->name=nameInput; }
	int setWidth(int widthInput){ this->width=widthInput; }

private:
	int width = 0;
	GeoFieldType type = FeildUnknown;
	QString name;
};

GeoFieldDefine::GeoFieldDefine()
{
}

GeoFieldDefine::~GeoFieldDefine()
{
}