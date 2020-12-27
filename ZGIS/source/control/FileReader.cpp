#include "FileReader.h"

FileReader::FileReader()
{
}

FileReader::~FileReader()
{
}

void FileReader::init(bool isUTF8=false)
{
	if (!isUTF8)
		CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	OGRRegisterAll();
}

OGRDataSource* FileReader::OpenshapeFile(std::string filePath)
{
	CPLSetConfigOption("SHAPE_ENCODING", "");
	OGRDataSource* dataSource = (OGRDataSource*)GDALOpenEx(filePath.c_str(), GDAL_OF_VECTOR, nullptr, nullptr, nullptr);
	if (dataSource == NULL)
	{
		printf("open shape file error");
		return NULL;
	}
	return dataSource;
}
/// <summary>
/// 函数暂时不可使用，没写完
/// </summary>
/// <param name="layer"></param>
/// <param name="coord"></param>
void FileReader::getPointCoord(OGRLayer* layer,std::vector<double>& coord)
{
	OGRFeature* feature =NULL;
	while ((feature=layer->GetNextFeature())!=NULL)
	{
		OGRFeatureDefn* fieldDefine = layer->GetLayerDefn();
		OGRGeometry* geometry = feature->GetGeometryRef();
		if (geometry->getGeometryType() == wkbPoint)
		{
			OGRPoint* point=geometry->toPoint();
			coord.push_back(point->getX());
			coord.push_back(point->getY());
			OGRLineString s;
		}
	}
}