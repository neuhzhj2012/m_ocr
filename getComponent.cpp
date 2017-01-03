#include <iostream>
#include "ocr.h"
#include "opencv.h"

//usage   ./OCR /home/cv/work/deep_ocr/test_data_chi.jpg chi_sim 4

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("usage: OCR img lang(eng, chi_sim) segmode(1-5)");
		return -1;
	}

	const char* pchImgName = argv[1];
	const char* pchLang = argv[2];
	int nSegMode = atoi(argv[3]);

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
	std::cout << "img: " << pchImgName << "\tlang: " << pchLang << "\tmode: " << segMode << std::endl;
	Pix *image = pixRead(pchImgName);
	cv::Mat mtImg = cv::imread(pchImgName);
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	//api->Init(NULL, "eng");
	api->Init(NULL, pchLang);
	api->SetImage(image);
	//Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);// line seg
	Boxa* boxes = api->GetComponentImages(segMode, true, NULL, NULL);
	printf("Found %d textline image components.\n", boxes->n);
	for (int i = 0; i < boxes->n; i++) {
		BOX* box = boxaGetBox(boxes, i, L_CLONE);
		api->SetRectangle(box->x, box->y, box->w, box->h);
		char* ocrResult = api->GetUTF8Text();
		int conf = api->MeanTextConf();
		cv::Rect rct(box->x, box->y, box->w, box->h);
		rectangle(mtImg, rct, cv::Scalar(0, 0, 255), 2);

		fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
			i, box->x, box->y, box->w, box->h, conf, ocrResult);
	}
	cv::imwrite("rct.jpg", mtImg);
}
