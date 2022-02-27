# ZGIS
本项目是在基于 C++语言开发的，通过借助 GDAL 读取 Shapefile
文件，使用 Qt 设计窗口界面，并调用原生 OpenGL 绘图。在绘图的基
础上，读取 SHP 文件，并应用到所读取的相应图层。在实现对地图的
基本操作功能，如放大、缩小、平移的同时创建空间索引，优化查询
效率。最后，在此基础上，实现点的核密度分析、要素编辑等一系列
功能的一个 GIS 系统内核原型。