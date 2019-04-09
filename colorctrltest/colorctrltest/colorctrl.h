#ifndef COLORCTRL_HEADER
#define COLORCTRL_HEADER

#include <QWidget>

class QColorCtrl :public QWidget
{
public:
	QColorCtrl(QWidget *parent = 0);
	~QColorCtrl();

	struct CustomRGB
	{
		CustomRGB() :byR(0), byG(0), byB(0){}
		unsigned char byR;
		unsigned char byG;
		unsigned char byB;
	};

	struct CustomYCbCr
	{
		CustomYCbCr() :byY(0), byCb(0), byCr(0){}
		unsigned char byY;
		unsigned char byCb;
		unsigned char byCr;
	};

	void loadYuyvImage(void *pData, const QSize & size, float fRatio = 1.0);  //加载图片，图片数据pData(yuyv)，图片大小size，图片缩放比例fRatio
	void loadRGBImage(const QString & fileName);  //加载RGB图片,test

	//描点画线显示参数
	void showHue(float fHue);  //色度，角度值，单位°
	void showAperture(float fAperture);  //Keying Region开口角，大于0°，小于360°
	void showSaturationThreshold(float fSaturationThreshold);  //饱和度阈值，0~1
	void showKeyColorSaturation(float fKeyColorSaturation);  //KeyColor饱和度，0~1，不小于饱和度阈值

protected:
	void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;

private:
	void drawColorRing(float fInCircleRadius, float fExCircleRadius);  //绘制色环
	void drawImagePoint();  //绘制图像数据点
	void drawImageParameter();  //绘制图像参数
	void yCbCrToRGB(unsigned char byY, unsigned char byCb, unsigned char byCr, CustomRGB &structMyRgb);
	void rGBToYCbCr(unsigned char byR, unsigned char byG, unsigned char byB, CustomYCbCr &structMyYcbcr);

private:
	unsigned char *m_pImageData;  //图像数据指针
	unsigned char *m_pYCbCrData;  //test
	int m_nLength;  //图片数据长度
	float m_fRatio;  //整个控件的缩放比例
	float m_fHue;
	float m_fAperture;
	float m_fSaturationThreshold;
	float m_fKeyColorSaturation;
	QImage m_scaledImage, m_ringImage, m_pointImage, m_paraImage;  //m_scaledImage:缩放的图像,m_ringImage:色环图像,m_pointImage:像素点图像,m_paraImage:参数图像
};

#endif