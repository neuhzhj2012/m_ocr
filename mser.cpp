#include "mser.h"

std::vector<cv::Rect> mserGetPlate(cv::Mat srcImage)
{
	// HSV�ռ�ת��  
	cv::Mat gray, gray_neg;
	cv::Mat hsi;
	cv::cvtColor(srcImage, hsi, CV_BGR2HSV);
	// ͨ������  
	std::vector<cv::Mat> channels;
	cv::split(hsi, channels);
	// ��ȡhͨ��  
	gray = channels[1];
	// �Ҷ�ת��   
	cv::cvtColor(srcImage, gray, CV_BGR2GRAY);
	// ȡ��ֵ�Ҷ�  
	gray_neg = 255 - gray;
	std::vector<std::vector<cv::Point> > regContours;
	std::vector<std::vector<cv::Point> > charContours;

	// ����MSER����  
	//cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(5, 15, 80, 0.25, 0.2, 200, 1.01, .003, 5);  
	//cv::Ptr<cv::MSER> mesr2 = cv::MSER::create(5, 15, 80, 0.25, 0.2, 200, 1.01, .003, 5);
	cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(5, 15, 100, 0.25, 0.2, 200, 1.01, .003,5);
	cv::Ptr<cv::MSER> mesr2 = cv::MSER::create(5, 15, 100, 0.25, 0.2, 200, 1.01, .003, 5);


	std::vector<cv::Rect> bboxes1;
	std::vector<cv::Rect> bboxes2;
	// MSER+ ���  
	mesr1->detectRegions(gray, regContours, bboxes1);
	// MSER-����  
	mesr2->detectRegions(gray_neg, charContours, bboxes2);

	cv::Mat mserMapMat = cv::Mat::zeros(srcImage.size(), CV_8UC1);
	cv::Mat mserNegMapMat = cv::Mat::zeros(srcImage.size(), CV_8UC1);

	for (int i = (int)regContours.size() - 1; i >= 0; i--)
	{
		// ���ݼ�����������mser+���  
		const std::vector<cv::Point>& r = regContours[i];
		for (int j = 0; j < (int)r.size(); j++)
		{
			cv::Point pt = r[j];
			mserMapMat.at<unsigned char>(pt) = 255;
		}
	}
	// MSER- ���  
	for (int i = (int)charContours.size() - 1; i >= 0; i--)
	{
		// ���ݼ�����������mser-���  
		const std::vector<cv::Point>& r = charContours[i];
		for (int j = 0; j < (int)r.size(); j++)
		{
			cv::Point pt = r[j];
			mserNegMapMat.at<unsigned char>(pt) = 255;
		}
	}
	// mser������  
	cv::Mat mserResMat;
	// mser+��mser-λ�����  
	mserResMat = mserMapMat & mserNegMapMat;
	// �ղ������ӷ�϶  
	cv::Mat mserClosedMat;
	cv::morphologyEx(mserResMat, mserClosedMat,
		cv::MORPH_CLOSE, cv::Mat::ones(1, 20, CV_8UC1));
#ifdef SHOWMIDRST
	cv::imshow("mserMapMat", mserMapMat);
	cv::imshow("mserNegMapMat", mserNegMapMat);
	cv::imshow("mserResMat", mserResMat);
	cv::imshow("mserClosedMat", mserClosedMat);
	cv::waitKey(0);
#endif // SHOWMIDRST

	//waitKey(0);
	// Ѱ���ⲿ����  
	std::vector<std::vector<cv::Point> > plate_contours;
	cv::findContours(mserClosedMat, plate_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	// ��ѡ���������ж����  
	std::vector<cv::Rect> candidates;
	std::vector<cv::Rect> rstRct;
	cv::Rect rctMaxArea;
	for (size_t i = 0; i != plate_contours.size(); ++i)
	{
		// �����С������  
		cv::Rect rect = cv::boundingRect(plate_contours[i]);
		// ��߱���  
		double wh_ratio = rect.width / double(rect.height);
		int i_area = rect.width * rect.height;
		// �����ϳߴ������ж�  
		//if (rect.height > 20 && wh_ratio > 4 && wh_ratio < 7)
		if (i_area > 500 && wh_ratio > 3)
			candidates.push_back(rect);
	}

	//�ϲ�����
	rstRct = combineRegion(candidates);

	for (size_t i = 0; i < rstRct.size(); i++)
	{
		if (rstRct[i].width * rstRct[i].height > rctMaxArea.width * rctMaxArea.height)
		{
			rctMaxArea = rstRct[i];
		}
	}
	rstRct.clear();
	rstRct.push_back(rctMaxArea);

	return  rstRct;
}


std::vector<cv::Rect> combineRegion(std::vector<cv::Rect> vRctSrc)
{
	////num of combine
	//int nHMaxNum = 0;
	//int nVMaxNum = 0;

	//for (size_t i = 0; i < vRctSrc.size(); i++)
	//	for (size_t j = 0; j < vRctSrc.size(); j++)
	//	{


	//	}
	
	int nItrNum = 1;
	std::vector<cv::Rect> rstRct;
	for (int k = 0; k < nItrNum; k++)
	{
		for (size_t i = 0; i < vRctSrc.size(); i++)
		{
			for (size_t j = 0; j < vRctSrc.size(); j++)
			{
				if (i == j)
				{
					continue;
				}
				bool bCombine = checkCombine(vRctSrc[i], vRctSrc[j], 3);

				if (bCombine)
				{
					//std::cout<<vRctSrc[i]<<std::endl;
					//std::cout << vRctSrc[j] << std::endl;
					//bool bCombine = checkCombine(vRctSrc[i], vRctSrc[j], 3);
					combine(vRctSrc[i], vRctSrc[j]);
				}
			}
		}
	}
	
	for (size_t i = 0; i < vRctSrc.size(); i++)
	{
		if (vRctSrc[i].width > 0)
		{
			rstRct.push_back(vRctSrc[i]);
		}
	}

	return rstRct;
}

bool checkCombine(cv::Rect rct1, cv::Rect rct2, int nFlag)
{
	//nFlag :1 ˮƽ���� ��2:��ֱ���� ��3����������
	bool bHSim = false;
	bool bVSim = false;
	bool bRtn = false;

	int nYDownU = std::max(rct1.y, rct2.y);
	int nYUpD = std::min(rct1.y + rct1.height, rct2.y + rct2.height);
	int nWmin = std::min(rct1.width , rct2.width );
	int nHmin = std::min(rct1.height, rct2.height);
	int nSubY = std::abs(rct1.y - rct2.y);
	int nDisY = std::abs(nYDownU - nYUpD);

	int nXmin = std::min(rct1.x, rct2.x);

	if (nSubY < nHmin / 4)
	{
		bHSim = true;
	}

	if (nDisY < nHmin * 1.2 )
	{
		bool b1In2 = ((rct1.x > rct2.x && rct1.x < rct2.x + rct2.width) || (rct1.x + rct1.width > rct2.x && rct1.x + rct1.width < rct2.x + rct2.width));
		bool b2In1 = ((rct2.x > rct1.x && rct2.x < rct1.x + rct1.width) || (rct2.x + rct2.width > rct1.x && rct2.x + rct2.width < rct1.x + rct1.width));
		if ( b1In2 || b2In1 || rct1.x == rct2.x || rct1.x + rct1.width == rct2.x + rct2.width)
		{
			bVSim = true;
		}
	}

	switch (nFlag)
	{
	case 1: bRtn = bHSim;
		break;
	case 2: bRtn = bVSim;
		break;
	case 3: bRtn = bHSim || bVSim;
		break;
	default:
		break;
	}

	return bRtn;
}

void combine(cv::Rect &rct1, cv::Rect &rct2)
{
	int nXmin = std::min(rct1.x, rct2.x);
	int nYmin = std::min(rct1.y, rct2.y);
	int nXmax = std::max(rct1.x + rct1.width, rct2.x + rct2.width);
	int nYmax = std::max(rct1.y + rct1.height, rct2.y + rct2.height);

	rct1.x = nXmin;
	rct1.y = nYmin;
	rct1.width = nXmax - nXmin;
	rct1.height = nYmax - nYmin;
	rct2 = cv::Rect(0, 0, 0, 0);
}
