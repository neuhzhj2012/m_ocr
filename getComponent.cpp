#include "ocr.h"
#include "opencv.h"
int main()
{
        const char* pchImgName = "/home/cv/work/deep_ocr/test_data_eng.png";
	Pix *image = pixRead(pchImgName);
        cv::Mat mtImg = cv::imread(pchImgName);
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	api->Init(NULL, "eng");
	api->SetImage(image);
	Boxa* boxes = api->GetComponentImages(tesseract::RIL_TEXTLINE, true, NULL, NULL);
	printf("Found %d textline image components.\n", boxes->n);
	for (int i = 0; i < boxes->n; i++) {
		BOX* box = boxaGetBox(boxes, i, L_CLONE);
		api->SetRectangle(box->x, box->y, box->w, box->h);
		char* ocrResult = api->GetUTF8Text();
		int conf = api->MeanTextConf();
                cv::Rect rct(box->x, box->y,box->w, box->h);
                rectangle(mtImg, rct, cv::Scalar(0, 0, 255), 2);
		fprintf(stdout, "Box[%d]: x=%d, y=%d, w=%d, h=%d, confidence: %d, text: %s",
			i, box->x, box->y, box->w, box->h, conf, ocrResult);
	}
        cv::imwrite("rct.jpg", mtImg);
}
