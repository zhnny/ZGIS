#pragma once
#include "gdal/ogrsf_frmts.h"

class FileReader
{
public:
	FileReader();
	~FileReader();
	void init(bool isUTF8);
	OGRDataSource* OpenshapeFile(std::string filePath);
	void getPointCoord(OGRLayer* layer ,std::vector<double>& coord);

private:

};

