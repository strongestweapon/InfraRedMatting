#include "testApp.h"
#include <math.h>
void PrintCameraInfo( CameraInfo* pCamInfo )
{
    printf(
        "\n*** CAMERA INFORMATION ***\n"
        "Serial number - %u\n"
        "Camera model - %s\n"
        "Camera vendor - %s\n"
        "Sensor - %s\n"
        "Resolution - %s\n"
        "Firmware version - %s\n"
        "Firmware build time - %s\n\n",
        pCamInfo->serialNumber,
        pCamInfo->modelName,
        pCamInfo->vendorName,
        pCamInfo->sensorInfo,
        pCamInfo->sensorResolution,
        pCamInfo->firmwareVersion,
        pCamInfo->firmwareBuildTime );
}
//--------------------------------------------------------------
void testApp::setup(){
	


	offW =320;
	offH = 240;
	camW = 640;
	camH = 480;
	//offW =4;
	//offH = 4;
	//camW = 1280;
	//camH = 960;
	screenW = ofGetWidth()+200;
	screenH = ofGetHeight();

	
	grayImage.allocate(camW,camH);
	grayBg.allocate(camW,camH);
	grayDiff.allocate(camW,camH);

	//OF Settings
	ofEnableAlphaBlending();
	ofBackground(255,0,0);
	ofSetVerticalSync(true);
	//ofSetFrameRate(120);


	//Key Press Initialize

	bInvertAlpha= false;
	bMonitorImg = false;
	bFrameRate = false;
	bGetBG=false;
	bDivideKey=false;
	bThresUp=false;
	bThresDown=false;

	cpx=new unsigned char[camW*camH*3];
	mpx=new unsigned char[camW*camH];
	maskpx=new unsigned char[camW*camH];
	BusManager busMgr;
    unsigned int camNum;
   busMgr.GetNumOfCameras(&camNum);
    PGRGuid guid[2];
    printf( "Number of cameras detected: %u\n", camNum );
	
	for(int i = 0; i<camNum;i++){
		busMgr.GetCameraFromIndex(i,&guid[i]);
		busMgr.FireBusReset(&guid[i]);
		Camera cam;
		cam.Connect(&guid[i]);
		CameraInfo camInfo;
		cam.GetCameraInfo(&camInfo);
		printf("%d: %c\n",i,camInfo.modelName[15]);
		if(camInfo.modelName[15] == 'M'){
			cam.Disconnect();
			camM.Connect(&guid[i]);
			camM.GetCameraInfo(&camInfo);
			PrintCameraInfo(&camInfo);
		}else if(camInfo.modelName[15] == 'C'){
			cam.Disconnect();
			camC.Connect(&guid[i]);
			camC.GetCameraInfo(&camInfo);
			PrintCameraInfo(&camInfo);
		}else{
			cam.Disconnect();
		}
	}

	camC.Connect(&guid[1]);
	camM.Connect(&guid[0]);

	
	//Cam Setting
	
	//camM.SetVideoModeAndFrameRate(VIDEOMODE_1280x960Y8,FRAMERATE_30);
	//camC.SetVideoModeAndFrameRate(VIDEOMODE_1280x960RGB,FRAMERATE_30);

	camM.SetVideoModeAndFrameRate(VIDEOMODE_FORMAT7,FRAMERATE_FORMAT7);
	camC.SetVideoModeAndFrameRate(VIDEOMODE_FORMAT7,FRAMERATE_FORMAT7);


    bool valid;
    Format7PacketInfo fmt7PacketInfo;

	Format7ImageSettings f7m;
	f7m.mode=MODE_0;
	f7m.offsetX=offW;
	f7m.offsetY=offH;
	f7m.width=camW;
	f7m.height=camH;
	f7m.pixelFormat=PIXEL_FORMAT_MONO8;


	
    // Validate the settings to make sure that they are valid
	camM.ValidateFormat7Settings(
        &f7m,
        &valid,
        &fmt7PacketInfo );
  
    if ( !valid )
    {
        // Settings are not valid
		printf("Format7 settings for mono are not valid\n");
     
    }

	camM.SetFormat7Configuration(&f7m,fmt7PacketInfo.maxBytesPerPacket);

	Format7ImageSettings f7c;
	f7c.offsetX=offW;
	f7c.offsetY=offH;
	f7c.width=camW;
	f7c.height=camH;
	f7c.pixelFormat= PIXEL_FORMAT_RGB8;

	camC.ValidateFormat7Settings(
        &f7c,
        &valid,
        &fmt7PacketInfo );
  
    if ( !valid )
    {
        // Settings are not valid
		printf("Format7 settings for color are not valid\n");
     
    }
	
	camC.SetFormat7Configuration(&f7c,fmt7PacketInfo.maxBytesPerPacket);

	

	//Auto exposure;
	Property ex;
	ex.type=AUTO_EXPOSURE;
	ex.absControl=true;
	ex.autoManualMode=false;
	ex.onOff=false;
	ex.absValue=1.5;
	camC.SetProperty(&ex);
	camM.SetProperty(&ex);


	//Sharpness;
	Property sh;
	sh.type=SHARPNESS;
	sh.absControl=true;
	sh.autoManualMode=false;
	sh.onOff=true;
	sh.absValue=1500.0;
	camC.SetProperty(&sh);
	camM.SetProperty(&sh);

	//Shutter
	Property st;
	st.type=SHUTTER;
	st.absControl=true;
	st.autoManualMode=false;
	//st.onOff=true;
	st.absValue=80.0;
	camC.SetProperty(&st);
	camM.SetProperty(&st);

	
	//Gamma
	Property gm;
	gm.type=GAMMA;
	gm.absControl=true;
	gm.autoManualMode=false;
	gm.onOff=true;
	gm.absValue=1.0;
	camC.SetProperty(&gm);
	camM.SetProperty(&gm);

	//Saturation
	Property str;
	str.type=SATURATION;
	str.absControl=true;
	str.autoManualMode=false;
	str.onOff=true;
	str.absValue=130.0;
	camC.SetProperty(&str);
	camM.SetProperty(&str);

	//Hue
	Property hue;
	hue.type=HUE;
	hue.absControl=true;
	hue.autoManualMode=false;
	hue.onOff=true;
	hue.absValue=0.0;
	camC.SetProperty(&hue);
	camM.SetProperty(&hue);



	//White Balance
	Property wb;
	wb.type=WHITE_BALANCE;
	wb.absControl=true;
	wb.autoManualMode=false;
	wb.onOff=true;
	wb.absValue=550.0;
	camC.SetProperty(&wb);
	camM.SetProperty(&wb);

	
	//Gain
	Property gain;
	gain.type=GAIN;
	gain.absControl=true;
	gain.autoManualMode=false;
	//gain.onOff=true;
	gain.absValue=-6.0;
	//camC.SetProperty(&gain);
	camM.SetProperty(&gain);


	//Frame_rate;
	Property fr;
	fr.type=FRAME_RATE;
	fr.absControl=true;
	fr.autoManualMode=false;
	fr.onOff=true;
	fr.absValue=60.0;
	camC.SetProperty(&fr);
	camM.SetProperty(&fr);

	camC.StartCapture();
	camM.StartCapture();
	bLearnBakground = true;
	threshold = 80;


	bgMov.loadMovie("pth264.mov");
	//bgMov.loadMovie("fingers.mov");
	bgMov.play();
	
	maskShader.load("composite");

	//RFID
	r.setup("COM3",9600);
	r.loadDB("rfdb.xml");
	bGetByte=false;
	tempCnt=0;
	printStr="";
	r.setTOT(5);
	bTimeOut=false;
	bRFIn=false;
	bAdd=false;
	stage=-1;
	bIsPlay=false;

	//ofxAlphaVideoPlayer
	avp.loadMovie("Freeze.mov");
	avp.play();
	mouseX=screenW/2;
	mouseY=screenH/2;

}

//--------------------------------------------------------------
void testApp::update(){
	bgMov.idleMovie();
	avp.idleMovie();
	if(r.available()>0){
		unsigned char tempByte = r.readByte();
		if(tempByte =='>'){
			bGetByte=true;
			for(int i = 0 ; i <tempCnt;i++){
				buff[i]='\0';
			}
			tempCnt=0;
		}
		if(bGetByte){
			buff[tempCnt]=tempByte;
			tempCnt++;
			if(tempByte=='\n'){
				printStr="";
				for(int i = 0;i<tempCnt;i++){
					printStr+=uc2c(buff[i]);
				}
				RFItem temp(printStr,stage);
				cout<<printStr<<endl<<endl;
				bRFIn=r.isItemIn(temp);
				if(bAdd){
					r.addItem(temp);
				}
				if(bRFIn){
					r.update(temp);
				}
				r.flush();
			}
		}
	}
	//if(bRFIn){
		if(r.isTimeOut()){
			bTimeOut=true;
			bgMov.stop();
			bIsPlay=false;
			bgMov.setFrame(0);
		}else{
			bTimeOut=false;
			if(!bIsPlay){
				bgMov.play();
				bIsPlay=true;
			}
		}
	//}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	unsigned int cols=cRawImg.GetCols();
	unsigned int rows=cRawImg.GetRows();
	
	camM.RetrieveBuffer(&mRawImg);
	camC.RetrieveBuffer(&cRawImg);
	cRawImg.Convert(PIXEL_FORMAT_RGB,&cConvertImg);
	
	mImg.setFromPixels(mRawImg.GetData(),cols,rows,OF_IMAGE_GRAYSCALE,false);
	cImg.setFromPixels(cRawImg.GetData(),cols,rows,OF_IMAGE_COLOR,true);
	cImg.setFromPixels(cConvertImg.GetData(),cols,rows,OF_IMAGE_COLOR,true);
	
	if(bTimeOut){
		cImg.draw(0,0,screenW,screenH);
	}else{
		

	grayImage.setFromPixels(mRawImg.GetData(),mRawImg.GetCols(),mRawImg.GetRows());
	if (bLearnBakground == true){
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
	grayDiff.absDiff(grayBg, grayImage);
	grayDiff.threshold(threshold);
	grayDiff.blur(3);
	mask.setFromPixels(grayDiff.getPixels(),grayDiff.getWidth(),grayDiff.getHeight(),OF_IMAGE_GRAYSCALE,false);


	ofSetColor(255);
	
	
	//Movie.setFromPixels(bgMov.getPixels(),bgMov.getWidth(),bgMov.getHeight(),OF_IMAGE_COLOR,true);
	
	maskShader.begin();
	maskShader.setUniformTexture("Tex0", cImg.getTextureReference(), 0);	//color
	maskShader.setUniformTexture("Tex1", mask.getTextureReference(), 1);	//mask
	maskShader.end();

	bgMov.draw(0,0,screenW,screenW*9/16.0);
	
	maskShader.begin();

	glActiveTexture(GL_TEXTURE0_ARB);
	cImg.getTextureReference().bind();

	glActiveTexture(GL_TEXTURE1_ARB);
	mask.getTextureReference().bind();

	glBegin(GL_QUADS);

	glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, 0);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, 0);		
	glVertex2f( 0, 0);

	glMultiTexCoord2d(GL_TEXTURE0_ARB, cImg.getWidth(), 0);
	glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth(), 0);		
	glVertex2f( screenW, 0);

	glMultiTexCoord2d(GL_TEXTURE0_ARB,  cImg.getWidth(), cImg.getHeight());
	glMultiTexCoord2d(GL_TEXTURE1_ARB, mask.getWidth(), mask.getHeight());
	glVertex2f( screenW, screenW*3/4.0);

	glMultiTexCoord2d(GL_TEXTURE0_ARB, 0, cImg.getHeight());
	glMultiTexCoord2d(GL_TEXTURE1_ARB, 0, mask.getHeight());		
	glVertex2f( 0,screenW*3/4.0 );
	
	glEnd();

	glActiveTexture(GL_TEXTURE1_ARB);
	mask.getTextureReference().unbind();
	
	glActiveTexture(GL_TEXTURE0_ARB);
	cImg.getTextureReference().unbind();
	
	maskShader.end();
	
	}
	if(bMonitorImg){

	mImg.draw(screenW-600,0,600,400);
	cImg.draw(screenW,400,-600,400);
	//BG.draw(3240+200-600,800,600,400);
	mask.draw(screenW-600,1200,600,400);
	bgMov.draw(screenW-600,1600,600,600*9/16.0);
	}

	
	
	
	if(bFrameRate){
		ofPushStyle();
		ofSetColor(255,0,0);
		ofDrawBitmapString("OF FrameRate: " + ofToString(ofGetFrameRate(),2),20,300);
		ofDrawBitmapString("Movie FrameRate: " + ofToString(bgMov.getSpeed(),2),20,420);
		Property frmRate;

		frmRate.type = FRAME_RATE;

		camC.GetProperty( &frmRate );

		char reportStr[1024];

		sprintf( reportStr,"Color Camera frame rate is %3.2f fps\n", frmRate.absValue );

		ofDrawBitmapString(reportStr , 20, 600);
		Property frmRateM;
		frmRateM.type = FRAME_RATE;

		camM.GetProperty( &frmRateM );

		sprintf( reportStr,"Mono Camera frame rate is %3.2f fps\n", frmRateM.absValue );
		ofDrawBitmapString(reportStr , 20, 800);
    
		if(bDivideKey){
			sprintf( reportStr,"Key with Divide, Threshold %f\n", thresD );
			ofDrawBitmapString(reportStr , 20, 900);
		}else{
			sprintf( reportStr,"Key with NoneDivide, Threshold %f\n", thresND );
			ofDrawBitmapString(reportStr , 20, 900);
		}


		ofPopStyle();
	}
	ofPushStyle();
	ofSetColor(255,255,255);
	avp.draw(mouseX-avp.getWidth()/2,mouseY-avp.getHeight()/2);
	ofPopStyle();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch(key){
	case 'f':
		bFrameRate = !bFrameRate;
		break;
	case 'i':
		bInvertAlpha = !bInvertAlpha;
		break;
	case 'm':
		bMonitorImg = !bMonitorImg;
		break;
	case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	case 'd':
		bDivideKey= !bDivideKey;
		break;
	case 'v':
		bVideoOn= !bVideoOn;
		break;
	case OF_KEY_DOWN:
		bThresDown=true;
		break;
	case OF_KEY_UP:
		bThresUp=true;
		break;
	
	case 'a':
		bAdd=true;
		break;
	case '0':
		stage=0;
		break;
	case '1':
		stage=1;
		break;
	case '2':
		stage=2;
		break;
	}
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	mouseX=x;
	mouseY=y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

char testApp::uc2c(unsigned char uc){

	if(uc <128){
		return uc;
	}else{
		return uc-256;
	}
}

unsigned char testApp::c2uc(char c){
	if(c <0){
		return c+256;
	}else{
		return c;
	}
	
}