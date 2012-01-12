#pragma once

#include "ofMain.h"
#include "FlyCapture2.h"
#include "ofxOpenCv.h"
#include "RFID.h"
#include "ofxAlphaVideoPlayer.h"

using namespace FlyCapture2;
class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofImage mImg;
		ofImage cImg;
		ofImage mask;
		Image mRawImg;
		Image cRawImg;
		Image cConvertImg;

		int mouseX,mouseY;


		Camera camM;
		Camera camC;
		ofVideoPlayer bgMov;
		ofImage Top;
		ofImage BG;
		ofImage Mask;
		ofImage Movie;
		unsigned char *mpx;
		unsigned char *cpx;
		unsigned char *maskpx;
		bool bFrameRate;
		bool bMonitorImg;
		bool bInvertAlpha;
		bool bDivideKey;
		bool bGetBG;

		float thresD;
		float thresND;
		bool bThresUp;
		bool bThresDown;
		bool bVideoOn;

		unsigned int offW,offH,camW,camH,screenW,screenH;

		ofShader maskShader;

		ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

        //ofxCvContourFinder 	contourFinder;

		int 				threshold;
		bool				bLearnBakground;


		char uc2c(unsigned char uc);
		unsigned char c2uc(char c);



		RFID r;
		bool bGetByte;
		unsigned char buff[100];
		int tempCnt;
		string printStr;
		bool bTimeOut;
		bool bRFIn;
		bool bAdd;
		int stage;


		bool bIsPlay;
		ofxAlphaVideoPlayer avp;
};
