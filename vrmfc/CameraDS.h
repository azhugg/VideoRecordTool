//////////////////////////////////////////////////////////////////////
// Video Capture using DirectShow
// Author: Shiqi Yu (shiqi.yu@gmail.com)
// Thanks to:
//		HardyAI@OpenCV China
//		flymanbox@OpenCV China (for his contribution to function CameraName, and frame width/height setting)
// Last modification: April 9, 2009
//
// ʹ��˵����
//   1. ��CameraDS.h CameraDS.cpp�Լ�Ŀ¼DirectShow���Ƶ������Ŀ��
//   2. �˵� Project->Settings->Settings for:(All configurations)->C/C++->Category(Preprocessor)->Additional include directories
//      ����Ϊ DirectShow/Include
//   3. �˵� Project->Settings->Settings for:(All configurations)->Link->Category(Input)->Additional library directories
//      ����Ϊ DirectShow/Lib
//////////////////////////////////////////////////////////////////////

#ifndef CCAMERA_H
#define CCAMERA_H

#define WIN32_LEAN_AND_MEAN

#include <atlbase.h>

#include "qedit.h"
#include "dshow.h"

#include <windows.h>
#include <strmif.h>
//#include <opencv2/opencv.hpp>


#define MYFREEMEDIATYPE(mt)	{if ((mt).cbFormat != 0)		\
					{CoTaskMemFree((PVOID)(mt).pbFormat);	\
					(mt).cbFormat = 0;						\
					(mt).pbFormat = NULL;					\
				}											\
				if ((mt).pUnk != NULL)						\
				{											\
					(mt).pUnk->Release();					\
					(mt).pUnk = NULL;						\
				}}			

#include "config.h"

class CCameraDS
{
private:

	//mi mi_ = {};
	mi mi_ = {};
	procamp vamp_ = {};
	camera_set cam_ = {};

	bool m_bConnected, m_bLock, m_bChanged;

	int m_nWidth, m_nHeight;

	long m_nBufferSize;

	//IplImage *m_pFrame;
	cv::Mat frame;

	CComPtr<IGraphBuilder> m_pGraph;

	CComPtr<ISampleGrabber> m_pSampleGrabber;

	CComPtr<IMediaControl> m_pMediaControl;

	CComPtr<IMediaEvent> m_pMediaEvent;

	CComPtr<IBaseFilter> m_pSampleGrabberFilter;
	CComPtr<IBaseFilter> m_pDeviceFilter;
	CComPtr<IBaseFilter> m_pNullFilter;

	CComPtr<IPin> m_pGrabberInput;
	CComPtr<IPin> m_pGrabberOutput;
	CComPtr<IPin> m_pCameraOutput;
	CComPtr<IPin> m_pNullInputPin;

	//CComPtr<IAMCameraControl> pcam = {};
	IAMCameraControl* pcam = nullptr;
	IAMVideoProcAmp* pamp = nullptr;

	bool BindFilter(int nCamIDX, IBaseFilter **pFilter);

	void SetCrossBar();

public:

	CCameraDS();
	virtual ~CCameraDS();

	bool get_info(int nCamID, mi& mi, procamp& vamp, camera_set& cam);
	//bool set_info(procamp& vamp, camera_set& cam);
	bool update_video(VideoProcAmpProperty p, int value);
	bool reset_video();
	procamp get_video() const { return vamp_; }
	camera_set get_camera() const { return cam_; }

	//mi get_mi() const { return mi_; }

	//������ͷ��nCamIDָ�����ĸ�����ͷ��ȡֵ����Ϊ0,1,2,...
	//bDisplayPropertiesָʾ�Ƿ��Զ���������ͷ����ҳ
	//nWidth��nHeight���õ�����ͷ�Ŀ�͸ߣ��������ͷ��֧�����趨�Ŀ�Ⱥ͸߶ȣ��򷵻�false
	bool OpenCamera(int nCamID, bool bDisplayProperties = true, int nWidth = 320, int nHeight = 240, const char* mstype = MT_YUY2);

	bool isOpened() const { return m_bConnected; }

	//�ر�����ͷ�������������Զ������������
	void CloseCamera();

	//��������ͷ����Ŀ
	//���Բ��ô���CCameraDSʵ��������int c=CCameraDS::CameraCount();�õ������
	static int CameraCount();

	//��������ͷ�ı�ŷ�������ͷ������
	//nCamID: ����ͷ���
	//sName: ���ڴ������ͷ���ֵ�����
	//nBufferSize: sName�Ĵ�С
	//���Բ��ô���CCameraDSʵ��������CCameraDS::CameraName();�õ������
	static int CameraName(int nCamID, char* sName, int nBufferSize);

	//����ͼ����
	int GetWidth() { return m_nWidth; }

	//����ͼ��߶�
	int GetHeight() { return m_nHeight; }

	//ץȡһ֡�����ص�IplImage�����ֶ��ͷţ�
	//����ͼ�����ݵ�ΪRGBģʽ��Top-down(��һ���ֽ�Ϊ���Ͻ�����)����IplImage::origin=0(IPL_ORIGIN_TL)
	cv::Mat QueryFrame();
};

#endif 
