#include "ofMain.h"

#include "RFItem.h"
#include "ofxXmlSettings.h"
class RFID{
public:
	RFID();
	~RFID();
	
	void			listDevices();	
	void 			enumerateDevices();

	vector <ofSerialDeviceInfo> getDeviceList();

	void 			close();
	bool			setup();		
	bool			setup(string portName, int baudrate);
	bool			setup(int deviceNumber, int baudrate);

	int 			readBytes(unsigned char * buffer, int length);
	int 			writeBytes(unsigned char * buffer, int length);
	bool			writeByte(unsigned char singleByte);
	int             readByte();  // returns -1 on no read or error...
	void			flush(bool flushIn = true, bool flushOut = true);
	int				available();

	void            drain();
	void 			setVerbose(bool bLoudmouth) { serial.setVerbose(bLoudmouth); };

	void loadDB(string filename);   //if file doesn't exist, automatically make the file with 'filename'
	bool isTimeOut();
	void update(RFItem item);
	bool isItemIn(RFItem item);
	void setTOT(float time);//set timeout time
	//void dbReset();
	void addItem(RFItem item);
	void addItem(string tag,int item);
	int getNofItems();
	RFItem getItemFromIndex(int i);
	RFItem activeTag;
	int getIndexOfItems(RFItem item);

private :
	ofxXmlSettings XML;
	string xmlStructure;
	float lastTime;
	bool bTimeOut;
	float timeOutTime;
	string dbName;
	ofSerial serial;
	vector<RFItem> items;
	int nofItems;
	
};