#ifndef DAHUA_H
#define DAHUA_H

#include <QWidget>
#include "GenICam/System.h"
#include "Media/VideoRender.h"
#include "Media/ImageConvert.h"
#include "MessageQue.h"
#include <QElapsedTimer>
#include <QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\dnn.hpp>
#include <string>
#include <time.h>
#include <Windows.h>
#include <ShellAPI.h>
#include <tchar.h>

using namespace cv;
using namespace dnn;
using namespace std;

class CFrameInfo : public Dahua::Memory::CBlock
{
public:
	CFrameInfo()
	{
		m_pImageBuf = NULL;
		m_nBufferSize = 0;
		m_nWidth = 0;
		m_nHeight = 0;
		m_PixelType = Dahua::GenICam::gvspPixelMono8;
		m_nPaddingX = 0;
		m_nPaddingY = 0;
		m_nTimeStamp = 0;
	}

	~CFrameInfo()
	{
	}

public:
	BYTE		*m_pImageBuf;
	int			m_nBufferSize;
	int			m_nWidth;
	int			m_nHeight;
	Dahua::GenICam::EPixelType	m_PixelType;
	int			m_nPaddingX;
	int			m_nPaddingY;
	uint64_t	m_nTimeStamp;
};

namespace Ui {
class CammerWidget;
}

class CammerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CammerWidget(QWidget *parent = 0);
    ~CammerWidget();
	bool Mode_of_trig = false;
	//枚举触发方式
	enum ETrigType
	{
		trigContinous = 0,	//连续拉流
		trigSoftware = 1,	//软件触发
		trigLine = 2,		//外部触发		
	};

	//回调函数
	void DahuaCallback(const Dahua::GenICam::CFrame & frame);
	//设置曝光
	bool SetExposeTime(double exposureTime);
	//设置增益
	bool SetAdjustPlus(double gainRaw);
	//打开相机
	bool CameraOpen(void);
	//关闭相机
	bool CameraClose(void);
	//开始采集
	bool CameraStart(void);
	//停止采集
	bool CameraStop(void);
	//切换采集方式、触发方式 （连续采集、外部触发、软件触发）
	void CameraChangeTrig(ETrigType trigType = trigContinous);
	//执行一次软触发（该接口为大华添加，用于执行软触发）            
	void ExecuteSoftTrig(void);
	//检测像机数、序列号
	void CameraCheck(void);
	/// \brief 显示一帧图像
	/// \param [in] pRgbFrameBuf 要显示的图像数据
	/// \param [in] nWidth 图像的宽
	/// \param [in] nHeight 图像的高
	/// \param [in] pixelFormat 图像的格式
	/// \retval true 显示失败
	/// \retval false  显示成功 
	bool ShowImage(uint8_t* pRgbFrameBuf, int pRgbFrameBufSize, int nWidth, int nHeight, uint64_t pixelFormat);
	//-------------------------------------------以下部分为新加内容---------------------------------------------

	//图书检测
	bool bookdetection(Mat image_in);
	void setLabelText(QString s);
	void testRun();

signals:    //添加自定义的信号，信号强制为公有类型，所以没有前缀
	void SendUpdateLCDMsg(int LCDNumBer);  //信号只需要声明，不要给信号写实体代码

	//-------------------------------------------以上部分为新加内容---------------------------------------------

private:
    Ui::CammerWidget *ui;
	
	Dahua::Infra::TVector<Dahua::GenICam::ICameraPtr> m_vCameraPtrList;	// 发现的相机列表
	Dahua::GenICam::ICameraPtr m_pCamera;								// 当前相机，默认为列表中的第一个相机	
	Dahua::GenICam::IStreamSourcePtr m_pStreamSource;					// 流对象

	//CRender                             _render;          // 显示对象	
	Dahua::Infra::CThreadLite           m_thdDisplayThread;   // 显示线程      
	TMessageQue<CFrameInfo>				m_qDisplayFrameQueue;// 显示队列

	Dahua::Infra::CMutex				m_mxTime;
	int									m_nDisplayInterval;         // 显示间隔
	uintmax_t							m_nFirstFrameTime;          // 第一帧的时间戳
	uintmax_t							m_nLastFrameTime;           // 上一帧的时间戳
	QElapsedTimer						m_elapsedTimer;				//用来计时，控制显示帧率

	VR_HANDLE          m_handler;           /* 绘图句柄 */
	VR_OPEN_PARAM_S    m_params;            /* 显示参数 */

											// Initialize the parameters
	float confThreshold = 0.5; // Confidence threshold
	float nmsThreshold = 0.4;  // Non-maximum suppression threshold
	int inpWidth = 416;  // Width of network's input image
	int inpHeight = 416; // Height of network's input image
	vector<string> classes;

	// 拉流线程
	void GrabFrameThreadProc(Dahua::Infra::CThreadLite& lite);

	// 显示线程
	void DisplayThreadProc(Dahua::Infra::CThreadLite& lite);

	// 设置显示频率，默认一秒钟显示30帧
	void setDisplayFPS(int nFPS);

	// 计算该帧是否显示
	bool isTimeToDisplay(uintmax_t nCurTime);

	/* 显示相关函数 */
	bool open(uint32_t width, uint32_t height);
	bool close();

	void closeEvent(QCloseEvent * event);

	/**
	* @brief 将 QImage 的类型图像转换为 cv::Mat 类型
	* @param image 待转换的图像，支持 Format_Indexed8/Format_Grayscale、24 位彩色、32 位彩色格式，
	* @param clone true 表示与 QImage 不共享内存，更改生成的 mat 不会影响原始图像，false 则会与 QImage 共享内存
	* @param rg_swap 只针对 RGB888 格式，如果 true 则会调换 R 与 B RGB->BGR，如果共享内存的话原始图像也会发生变化
	* @return 转换后的 cv::Mat 图像
	* https://blog.csdn.net/liyuanbhu/article/details/86307283
	*/
	Mat QImage2cvMat(QImage &image, bool clone = false, bool rb_swap = true);
	QImage cvMat2QImage(const cv::Mat& mat);

	/**
	* @name:
	* @test: test font
	* @msg: 线性拟合，根据传入参数点进行线性拟合判断相关系数跟斜率等
	* @param  const vector<Point> points 传入的点
	* @param slope 标准的斜率
	* @param intercept 标准的截距
	* @param r_square 标准的相关系数
	* @return: 0表示拟合的直线跟给定的直线不匹配，1表示拟合成功
	*/
	bool LinearFitting(const vector<Point> points, double slope, double intercept, double r_square);

	string get_datetime();

	vector<Rect> detect_image(Mat frame, string modelWeights, string modelConfiguration);
	vector<String> getOutputsNames(const Net& net);
	vector<Rect> postprocess_return(Mat& frame, const vector<Mat>& out);
	wstring string2tchar(const string& s);
	void run_exe(string path, string params);
	void run_database(string time, string result);

	VR_HWND		m_hWnd;
};
#endif // DAHUA_H

////B.h
//class B
//{
//	......
//		signals:
//	void toA([ParamList]);
//}
//
////B.cpp
//B::B()
//{
//	emit toA([ParamList]);
//}