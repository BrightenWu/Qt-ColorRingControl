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

	void loadYuyvImage(void *pData, const QSize & size, float fRatio = 1.0);  //����ͼƬ��ͼƬ����pData(yuyv)��ͼƬ��Сsize��ͼƬ���ű���fRatio
	void loadRGBImage(const QString & fileName);  //����RGBͼƬ,test

	//��㻭����ʾ����
	void showHue(float fHue);  //ɫ�ȣ��Ƕ�ֵ����λ��
	void showAperture(float fAperture);  //Keying Region���ڽǣ�����0�㣬С��360��
	void showSaturationThreshold(float fSaturationThreshold);  //���Ͷ���ֵ��0~1
	void showKeyColorSaturation(float fKeyColorSaturation);  //KeyColor���Ͷȣ�0~1����С�ڱ��Ͷ���ֵ

protected:
	void paintEvent(QPaintEvent * event) Q_DECL_OVERRIDE;
	void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;

private:
	void drawColorRing(float fInCircleRadius, float fExCircleRadius);  //����ɫ��
	void drawImagePoint();  //����ͼ�����ݵ�
	void drawImageParameter();  //����ͼ�����
	void yCbCrToRGB(unsigned char byY, unsigned char byCb, unsigned char byCr, CustomRGB &structMyRgb);
	void rGBToYCbCr(unsigned char byR, unsigned char byG, unsigned char byB, CustomYCbCr &structMyYcbcr);

private:
	unsigned char *m_pImageData;  //ͼ������ָ��
	unsigned char *m_pYCbCrData;  //test
	int m_nLength;  //ͼƬ���ݳ���
	float m_fRatio;  //�����ؼ������ű���
	float m_fHue;
	float m_fAperture;
	float m_fSaturationThreshold;
	float m_fKeyColorSaturation;
	QImage m_scaledImage, m_ringImage, m_pointImage, m_paraImage;  //m_scaledImage:���ŵ�ͼ��,m_ringImage:ɫ��ͼ��,m_pointImage:���ص�ͼ��,m_paraImage:����ͼ��
};

#endif