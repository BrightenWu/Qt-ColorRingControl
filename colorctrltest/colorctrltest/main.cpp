#include "colorctrltest.h"
#include <QtWidgets/QApplication>
#include "colorctrl.h"
#include <fstream>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QColorCtrl w;

	std::ifstream m_fin;;
	m_fin.open("flower_yuyv422_352X288.yuv", std::ifstream::in | std::ifstream::binary);
	if (!m_fin.is_open())
	{
		return false;
	}

 	//m_fin.seekg(0, std::ifstream::end);
 	//int length = m_fin.tellg();
 	//m_fin.seekg(0, std::ifstream::beg);
	char buffer[202752] = { 0 };
	m_fin.read(buffer, 202752);

	m_fin.close();

	w.loadYuyvImage(buffer, QSize(352, 288),.5);
	//w.loadRGBImage("bmp1.bmp");
	w.showHue(-150.0);
	w.showAperture(120.0);
	w.showSaturationThreshold(0.3);
	w.showKeyColorSaturation(0.7);

	w.show();
	return a.exec();
}
