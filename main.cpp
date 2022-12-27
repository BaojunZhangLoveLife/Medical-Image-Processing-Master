#include "MainWindow.h"
#include <QApplication>
#include <ItkSegmentation.h>
#include <GraphCut.h>
//int main(int argc, char* argv[])
//{
//	QApplication a(argc, argv);
//
//	MainWindow w;
//	w.show();
//	return a.exec();
//
//
//}
int main()
{
	GraphCutAlgorithm* graphcut = new GraphCutAlgorithm();
	std::string srcString = "C:\\Users\\Baojun\\Downloads\\bone-segmentation-master\\sample-volumes\\001-CT.nii";
	std::string fgString = "C:\\Users\\Baojun\\Downloads\\bone-segmentation-master\\sample-volumes\\FG.nii";
	std::string bgString = "C:\\Users\\Baojun\\Downloads\\bone-segmentation-master\\sample-volumes\\BG.nii";
	std::string dstString = "C:\\Users\\Baojun\\Downloads\\bone-segmentation-master\\sample-volumes\\DST.nii";
	graphcut->GraphCut(srcString, fgString, bgString, dstString);
 }