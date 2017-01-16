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
                break;
	case 2:
		segMode = tesseract::RIL_PARA;
                break;
	case 3:
		segMode = tesseract::RIL_TEXTLINE;
                break;
	case 4:
		segMode = tesseract::RIL_WORD;
                break;
	case 5:
		segMode = tesseract::RIL_SYMBOL;
               break;

	default:
		break;
	}
	//parse imgname
	std::string sImgName = pchImgName;
	std::string sLogName;
	int nSeg = sImgName.rfind('\/');
	if (nSeg == std::string::npos)
	{
		printf("##parse imgName error##\n");
		return -2;
	}
	sImgName = sImgName.substr(nSeg + 1);//*.jpg
	nSeg = sImgName.find('\.');
	if (nSeg == std::string::npos)
	{
		printf("##parse imgName error##\n");
		return -2;
	}
	sImgName = sImgName.substr(0, nSeg);
	sLogName = sImgName + ".txt";
	std::cout << "img: " << pchImgName << "\tlang: " << pchLang << "\tmode: " << segMode << "\tlog:"<< sLogName<< std::endl;
	//Pix *image = pixRead(pchImgName);
	cv::Mat mtImg = cv::imread(pchImgName);
	cv::Mat mtImgGray = cv::imread(pchImgName, 0);

	std::vector<cv::Rect> candidates;
	cv::Rect rctRoi;
	candidates = mserGetPlate(mtImg);	//mser
	rctRoi = candidates[0];
        //extend u/d edge
        rctRoi.y = rctRoi.y - 5 > 0 ? (rctRoi.y - 5):0;
        rctRoi.height = mtImgGray.rows - 1 - (rctRoi.y + rctRoi.height + 10) > 0 ? rctRoi.height + 10: mtImgGray.rows - 1 - rctRoi.y; 
	cv::Mat mtImgRoi = mtImgGray(rctRoi).clone(); //need data copy,not just matrix head
	cv::rectangle(mtImg, rctRoi, cv::Scalar(0, 0, 255), 2);
	cv::imwrite("imgcrop.jpg", mtImgRoi);
	cv::imwrite("imgrct.jpg",mtImg);
	/*
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	api->Init(NULL, pchLang);
 
	api->SetVariable("chop_enable","T");  
	api->SetVariable("use_new_state_cost","F");  
	api->SetVariable("segment_segcost_rating","F");  
	api->SetVariable("enable_new_segsearch","0");  
	api->SetVariable("language_model_ngram_on","0");  
	api->SetVariable("textord_force_make_prop_words","F");   
	*/

	tesseract::TessBaseAPI tess;
	tess.Init(NULL, pchLang, tesseract::OEM_DEFAULT);
	tess.SetImage((uchar*)mtImgRoi.data, mtImgRoi.cols, mtImgRoi.rows, 1, mtImgRoi.cols);
	char *out = tess.GetUTF8Text();
	std::cout<<"rst: "<<out<<std::endl;
	// release Memory
	//api->End();
    delete [] out;

	/*
	api->SetImage(image); //Pix img format
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
        for (it =sRecogRst.begin(); it != sRecogRst.end(); ++it)
        {
           if ( *it == '\n')
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

	cv::imwrite("rct.jpg", mtImg);
	*/
}
