#include <iostream>
#include "ocr.h"
#include "opencv.h"

//usage   ./OCR /home/cv/work/deep_ocr/test_data_chi.jpg chi_sim 4

int GetFileList(char *basePath, char *pchLang, int nSegMode)
{
	DIR *dir;
	struct dirent *ptr;
	char base[1000];
	std::vector<std::string> vImgPath;

	if ((dir = opendir(basePath)) == NULL)
	{
		perror("Open dir error...");
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
			continue;
		else if (ptr->d_type == 8)    ///file
		{
			printf("d_name:%s/%s\n", basePath, ptr->d_name);
			std::string sBasePath = basePath;
			std::string sImgName = ptr->d_name;
			std::string sAbsPath = sBasePath + "\/" + sImgName;
			vImgPath.push_back(sAbsPath);
		}
		else if (ptr->d_type == 10)    ///link file
			printf("d_name:%s/%s\n", basePath, ptr->d_name);
		else if (ptr->d_type == 4)    ///dir
		{
			memset(base, '\0', sizeof(base));
			strcpy(base, basePath);
			strcat(base, "/");
			strcat(base, ptr->d_name);
			GetFileList(base, pchLang, nSegMode);
		}
	}

	for (int i = 0; i < (int)vImgPath.size(); i++)
	{
		tesseract::PageIteratorLevel segMode;
		switch (nSegMode)
		{
		case 1:
			segMode = tesseract::RIL_BLOCK;
		case 2:
			segMode = tesseract::RIL_PARA;
		case 3:
			segMode = tesseract::RIL_TEXTLINE;
		case 4:
			segMode = tesseract::RIL_WORD;
		case 5:
			segMode = tesseract::RIL_SYMBOL;

		default:
			break;
		}
		//parse imgname
		std::string sImgName = vImgPath[i];
		std::string sImgPath;
		std::string sSaveName;
		std::string sLogName;
		const char *pchImgName = sImgName.c_str();
		int nSeg = sImgName.rfind('\/');
		if (nSeg == std::string::npos)
		{
			printf("##parse imgName error##\n");
			return -2;
		}
		sImgPath = sImgName.substr(0, nSeg);
		sImgName = sImgName.substr(nSeg + 1);//*.jpg
		sSaveName = sImgPath + "/rst/" + sImgName;
		std::cout << "sSaveName" << sSaveName << std::endl;
		nSeg = sImgName.find('\.');
		if (nSeg == std::string::npos)
		{
			printf("##parse imgName error##\n");
			return -2;
		}

		sImgName = sImgName.substr(0, nSeg);
		sLogName = sImgName + ".txt";
		std::cout << "img: " << pchImgName << "\tlang: " << pchLang << "\tmode: " << segMode << "\tlog:" << sLogName << std::endl;
		Pix *image = pixRead(pchImgName);
		cv::Mat mtImg = cv::imread(pchImgName);
		tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
		//api->Init(NULL, "eng");
		api->Init(NULL, pchLang);
		api->SetImage(image);
		//Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);// line seg
		Boxa* boxes = api->GetComponentImages(segMode, true, NULL, NULL);
		printf("Found %d textline image components.\n", boxes->n);

		std::string sRecogRst;


		for (int i = 0; i < boxes->n; i++) {
			BOX* box = boxaGetBox(boxes, i, L_CLONE);
			api->SetRectangle(box->x, box->y, box->w, box->h);
			char* ocrResult = api->GetUTF8Text();
			std::string sOcrResult = ocrResult;
			sRecogRst += sOcrResult;
			int conf = api->MeanTextConf();
			cv::Rect rct(box->x, box->y, box->w, box->h);
			rectangle(mtImg, rct, cv::Scalar(0, 0, 255), 2);

			fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
				i, box->x, box->y, box->w, box->h, conf, ocrResult);

			FILE* pf = fopen(sLogName.c_str(), "at");
			if (pf != NULL)
			{
				fprintf(pf, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s", i, box->x, box->y, box->w, box->h, conf, ocrResult);
			}
			fclose(pf);

		}
		std::string::iterator it;
		for (it = sRecogRst.begin(); it != sRecogRst.end(); ++it)
		{
			if (*it == '\n')
			{
				sRecogRst.erase(it);
			}
		}
		FILE* pf = fopen(sLogName.c_str(), "at");
		if (pf != NULL)
		{
			fprintf(pf, "recognition text: %s", sRecogRst.c_str());
		}
		fclose(pf);

		cv::imwrite(sSaveName.c_str(), mtImg);

	}

	closedir(dir);
	return 1;
}



int main(int argc, char* argv[])
{
	//command line
	if (argc != 4)
	{
		printf("usage: OCR img lang(eng, chi_sim) segmode(1-5)");
		return -1;
	}

	const char* pchImgName = argv[1];
	const char* pchLang = argv[2];
	int nSegMode = atoi(argv[3]);

	////config
	//char achExePath[512];
	//char achImgPath[512] = { 0 };
	//char achLang[512] = { 0 };
	//char *pchTmp = NULL;
	//int nSegMode = 0;

	//memset(achExePath, 0, sizeof(achExePath));
	//GetCurrentPath(achExePath, CONF_FILE_PATH);

	//nSegMode = GetIniKeyInt("INFO", "Mode", achExePath);
	//pchTmp = GetIniKeyString("INFO", "Path", achExePath);
	//strcpy(achImgPath, pchTmp);
	//achLang = GetIniKeyString("INFO", "Lang", achExePath);
	//printf("achImgPath %s, len %d\n", achImgPath, strlen(achImgPath));
	//char *pchDir = "/home/cv/work/ocr/m_ocr/data";
	//printf("pchDir %s, len %d\n", pchDir, strlen(pchDir));
	GetFileList((char *)pchImgName, (char *)pchLang, nSegMode);
	return 0;

	/*

	*/
}


