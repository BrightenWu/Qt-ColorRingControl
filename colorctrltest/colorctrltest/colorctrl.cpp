#include "colorctrl.h"
#include <QPainter>
#include <QResizeEvent>
#include <cmath>

#define WIDGET_LENGTH 310*m_fRatio
#define INNER_CIRCLE_RADIUS 135*m_fRatio
#define RADTODEG 57.29577952f
#define DEGTORAD 0.017453293f
#define RING_STARTANGLE 9.0f

QColorCtrl::QColorCtrl(QWidget *parent /* = 0 */)
	:m_pImageData(nullptr), m_pYCbCrData(nullptr),m_fHue(0.0), m_fAperture(30.0), m_fSaturationThreshold(0.1), m_fKeyColorSaturation(0.5), m_nLength(0), m_fRatio(1.0)
{
	
}

QColorCtrl::~QColorCtrl()
{
	if (m_pYCbCrData!=nullptr)
	{
		delete[] m_pYCbCrData;
		m_pYCbCrData = nullptr;
	}
}

void QColorCtrl::loadYuyvImage(void *pData, const QSize & size, float fRatio /* = 1.0 */)
{
	if (pData == nullptr)
	{
		return;
	}

	QImage iniImage(reinterpret_cast<uchar *>(pData), size.width() / sqrt(2) + 1, size.height() / sqrt(2) + 1, QImage::Format_ARGB32);//Format_RGB16?
 	m_scaledImage = iniImage.scaled(QSize(iniImage.width()*fRatio, iniImage.height()*fRatio), Qt::KeepAspectRatio);
 
 	m_pImageData = m_scaledImage.bits();

	if (fRatio==1.0)
	{
		m_nLength = size.width()*size.height() * 2 - 3;
	}
	else
	{
		m_nLength = m_scaledImage.byteCount() - 3;
	}

	//重绘色环内部区域
	drawImagePoint();
	this->update();
}

void QColorCtrl::loadRGBImage(const QString & fileName)
{
	QImage rgbImage(fileName);

	if (rgbImage.isNull())
	{
		return;
	}

	m_pYCbCrData = new unsigned char[rgbImage.width()*rgbImage.height() * 2];
	memset(m_pYCbCrData, 0, rgbImage.width()*rgbImage.height() * 2);

	uchar *pRgbImageData = rgbImage.bits();

	for (int i = 0,n=0; i < rgbImage.byteCount()-7;i+=8,n+=4)
	{
		CustomYCbCr structMyycbcr;

		rGBToYCbCr(*(pRgbImageData + i+2), *(pRgbImageData + i+1), *(pRgbImageData+i), structMyycbcr);
		m_pYCbCrData[n] = structMyycbcr.byY;
		m_pYCbCrData[n + 1] = structMyycbcr.byCb;
		m_pYCbCrData[n + 3] = structMyycbcr.byCr;

		rGBToYCbCr(*(pRgbImageData + i + 6), *(pRgbImageData + i + 5), *(pRgbImageData + i+4), structMyycbcr);
		m_pYCbCrData[n + 2] = structMyycbcr.byY;
	}

	m_nLength = rgbImage.width()*rgbImage.height() * 2 - 3;
	m_pImageData = m_pYCbCrData;

	//重绘色环内部区域
	drawImagePoint();
	this->update();
}

void QColorCtrl::showHue(float fHue)
{
	m_fHue = fHue;

	drawImageParameter();
	this->update();
}

void QColorCtrl::showAperture(float fAperture)
{
	m_fAperture = fAperture;

	drawImageParameter();
	this->update();
}

void QColorCtrl::showSaturationThreshold(float fSaturationThreshold)
{
	m_fSaturationThreshold = fSaturationThreshold;

	drawImageParameter();
	this->update();
}

void QColorCtrl::showKeyColorSaturation(float fKeyColorSaturation)
{
	m_fKeyColorSaturation = fKeyColorSaturation;

	drawImageParameter();
	this->update();
}

void QColorCtrl::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);

	painter.drawImage(QPoint(0, 0), m_ringImage);
	painter.drawImage(QPoint(0, 0), m_pointImage);
	painter.drawImage(QPoint(0, 0), m_paraImage);

	__super::paintEvent(event);
}

void QColorCtrl::resizeEvent(QResizeEvent * event)
{
	QSize currentSize = event->size();
	int nCurrentWidth = currentSize.width();

	m_fRatio = nCurrentWidth / 310.0;

	if (nCurrentWidth != currentSize.height())
	{
		this->setFixedHeight(nCurrentWidth);
	}
	
	m_ringImage = QImage(WIDGET_LENGTH, WIDGET_LENGTH, QImage::Format_ARGB32);
	m_pointImage = QImage(WIDGET_LENGTH, WIDGET_LENGTH, QImage::Format_ARGB32);
	m_paraImage = QImage(WIDGET_LENGTH, WIDGET_LENGTH, QImage::Format_ARGB32);

	m_pointImage.fill(Qt::transparent);
	m_paraImage.fill(Qt::transparent);

	drawColorRing(INNER_CIRCLE_RADIUS, WIDGET_LENGTH* 0.5);
	drawImagePoint();
	drawImageParameter();
	
	__super::resizeEvent(event);
}

void QColorCtrl::drawColorRing(float fInCircleRadius, float fExCircleRadius)
{
	if (fInCircleRadius > fExCircleRadius)
	{
		return;
	}

	m_ringImage.fill(Qt::transparent);

	QPainter painter(&m_ringImage);

	painter.setRenderHint(QPainter::Antialiasing, true);  //反锯齿
	painter.setViewport(QRect(WIDGET_LENGTH* 0.5, WIDGET_LENGTH* 0.5, painter.window().width(), -painter.window().height()));  //改变原点位置和坐标轴方向

	painter.setPen(Qt::darkGray);
	painter.drawLine(QPoint(-WIDGET_LENGTH* 0.5, 0), QPoint(WIDGET_LENGTH* 0.5, 0));
	painter.drawLine(QPoint(0, -WIDGET_LENGTH* 0.5), QPoint(0, WIDGET_LENGTH* 0.5));

	//6色渐变色环
	painter.setPen(Qt::white);
	QConicalGradient conicalColor(0, 0, RING_STARTANGLE);
	conicalColor.setColorAt(0, Qt::blue);
	conicalColor.setColorAt(1.0 / 6.0, Qt::cyan);
	conicalColor.setColorAt(2.0 / 6.0, Qt::green);
	conicalColor.setColorAt(3.0 / 6.0, Qt::yellow);
	conicalColor.setColorAt(4.0 / 6.0, Qt::red);
	conicalColor.setColorAt(5.0 / 6.0, Qt::magenta);
	conicalColor.setColorAt(1, Qt::blue);

	painter.setBrush(conicalColor);

	QPainterPath bigCircle;
	bigCircle.addEllipse(-fExCircleRadius, -fExCircleRadius, fExCircleRadius * 2, fExCircleRadius * 2);
	QPainterPath smallCircle;
	smallCircle.addEllipse(-fInCircleRadius, -fInCircleRadius, fInCircleRadius * 2, fInCircleRadius * 2);

	QPainterPath path = bigCircle - smallCircle;
	bigCircle.setFillRule(Qt::WindingFill);
	painter.drawPath(path);
}

void QColorCtrl::drawImagePoint()
{
	if (nullptr == m_pImageData)
	{
		return;
	}

	m_pointImage.fill(Qt::transparent);

	QPainter painter(&m_pointImage);

	painter.setRenderHint(QPainter::Antialiasing, true);  //反锯齿
	painter.setViewport(QRect(WIDGET_LENGTH* 0.5, WIDGET_LENGTH* 0.5, painter.window().width(), -painter.window().height()));  //改变原点位置和坐标轴方向
	painter.setPen(QColor(0, 180, 0));

	for (int i = 0; i < m_nLength; i += 4)
	{
		unsigned char byY0 = *(m_pImageData + i);
		unsigned char byU = *(m_pImageData + i + 1);
		unsigned char byY1 = *(m_pImageData + i + 2);
		unsigned char byV = *(m_pImageData + i + 3);
		CustomRGB structMyrgb;
		
		yCbCrToRGB(byY0, byU, byV, structMyrgb);

		//RGB to HSV
		QColor rgbToHsv = QColor::fromRgb(structMyrgb.byR, structMyrgb.byG, structMyrgb.byB);
		float fHue = rgbToHsv.hueF()*360.0 + 120.0 - RING_STARTANGLE;
		float fSaturation = rgbToHsv.saturationF()*INNER_CIRCLE_RADIUS;

		float fX = fSaturation*cos(fHue*DEGTORAD);
		float fY = fSaturation*sin(fHue*DEGTORAD);

		painter.drawPoint(QPointF(fX,fY));

		yCbCrToRGB(byY1, byU, byV, structMyrgb);

		rgbToHsv = QColor::fromRgb(structMyrgb.byR, structMyrgb.byG, structMyrgb.byB);
		fHue = rgbToHsv.hueF()*360.0 + 120.0 - RING_STARTANGLE;
		fSaturation = rgbToHsv.saturationF()*INNER_CIRCLE_RADIUS;

		fX = fSaturation*cos(fHue*DEGTORAD);
		fY = fSaturation*sin(fHue*DEGTORAD);

		painter.drawPoint(QPointF(fX, fY));
	}
}

void QColorCtrl::drawImageParameter()
{
	if (nullptr == m_pImageData)
	{
		return;
	}

	if (m_fKeyColorSaturation<m_fSaturationThreshold)
	{
		return;
	}

	m_paraImage.fill(Qt::transparent);

	QPainter painter(&m_paraImage);

	painter.setRenderHint(QPainter::Antialiasing, true);  //反锯齿
	painter.setViewport(QRect(WIDGET_LENGTH* 0.5, WIDGET_LENGTH* 0.5, painter.window().width(), -painter.window().height()));  //改变原点位置和坐标轴方向

	float fSaturationThreshold_r = INNER_CIRCLE_RADIUS*m_fSaturationThreshold;
	float fSaturationThreshold_x = fSaturationThreshold_r*cos(m_fHue*DEGTORAD);
	float fSaturationThreshold_y = fSaturationThreshold_r*sin(m_fHue*DEGTORAD);

	float fKeyColorSaturation_r = INNER_CIRCLE_RADIUS*m_fKeyColorSaturation;
	float fKeyColorSaturation_x = fKeyColorSaturation_r*cos(m_fHue*DEGTORAD);
	float fKeyColorSaturation_y = fKeyColorSaturation_r*sin(m_fHue*DEGTORAD);

	painter.setPen(Qt::cyan);
	painter.drawLine(QPointF(fSaturationThreshold_x, fSaturationThreshold_y), QPointF(fKeyColorSaturation_x, fKeyColorSaturation_y));

	if (m_fAperture > 360.0 || m_fAperture < 0.0)
	{
		return;
	}

	//通过正弦定理计算出Aperture两边与内圆相交的点
	float fAlpha = m_fAperture* 0.5 + m_fHue - asin(m_fSaturationThreshold*sin((180.0 - m_fAperture* 0.5)*DEGTORAD))*RADTODEG;
	float fBeta = m_fHue - m_fAperture* 0.5 + asin(m_fSaturationThreshold*sin((180.0 - m_fAperture* 0.5)*DEGTORAD))*RADTODEG;

	float fApertureAlpha_x = INNER_CIRCLE_RADIUS*cos(fAlpha*DEGTORAD);
	float fApertureAlpha_y = INNER_CIRCLE_RADIUS*sin(fAlpha*DEGTORAD);

	float fApertureBeta_x = INNER_CIRCLE_RADIUS*cos(fBeta*DEGTORAD);
	float fApertureBeta_y = INNER_CIRCLE_RADIUS*sin(fBeta*DEGTORAD);

	painter.drawLine(QPointF(fSaturationThreshold_x, fSaturationThreshold_y), QPointF(fApertureAlpha_x, fApertureAlpha_y));
	painter.drawLine(QPointF(fSaturationThreshold_x, fSaturationThreshold_y), QPointF(fApertureBeta_x, fApertureBeta_y));

	painter.setPen(Qt::red);
	painter.setBrush(Qt::red);
	painter.drawEllipse(QPointF(fSaturationThreshold_x, fSaturationThreshold_y), 1, 1);
	painter.drawEllipse(QPointF(fKeyColorSaturation_x, fKeyColorSaturation_y), 1, 1);
}

void QColorCtrl::yCbCrToRGB(unsigned char byY, unsigned char byCb, unsigned char byCr, CustomRGB &structMyRgb)
{
	int nR, nG, nB;

	//YCbCr转RGB
	nR = round(byY + 1.402*(byCr - 128));
	nG = round(byY - 0.34414*(byCb - 128) - 0.71414*(byCr - 128));
	nB = round(byY + 1.772*(byCb - 128));

	nR = nR < 0 ? 0 : (nR>255 ? 255 : nR);
	nG = nG < 0 ? 0 : (nG>255 ? 255 : nG);
	nB = nB < 0 ? 0 : (nB>255 ? 255 : nB);

	structMyRgb.byR = nR;
	structMyRgb.byG = nG;
	structMyRgb.byB = nB;
}

void QColorCtrl::rGBToYCbCr(unsigned char byR, unsigned char byG, unsigned char byB, CustomYCbCr &structMyYcbcr)
{
	int nY, nCb, nCr;

	//RGB转YCbCr
	nY = round(0.299*byR + 0.587*byG + 0.114*byB);
	nCb = round(-0.16874*byR - 0.33126*byG + 0.5*byB + 128);
	nCr = round(0.5*byR - 0.41869*byG - 0.08131*byB + 128);

	nY = nY < 0 ? 0 : (nY>255 ? 255 : nY);
	nCb = nCb < 0 ? 0 : (nCb>255 ? 255 : nCb);
	nCr = nCr < 0 ? 0 : (nCr>255 ? 255 : nCr);

	structMyYcbcr.byY = nY;
	structMyYcbcr.byCb = nCb;
	structMyYcbcr.byCr = nCr;
}