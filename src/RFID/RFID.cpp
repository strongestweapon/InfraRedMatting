#include "RFID.h"
RFID::RFID(){}

void RFID::listDevices(){
	serial.listDevices();
}
bool RFID::setup(){
	return serial.setup();
}
bool RFID::setup(string portName, int baudrate){
	return serial.setup(portName,baudrate);
}
bool RFID::setup(int deviceNumber, int baudrate){
	return serial.setup(deviceNumber,baudrate);
}
void RFID::close(){
	serial.close();
}
void RFID::enumerateDevices(){
	serial.enumerateDevices();
}
vector <ofSerialDeviceInfo> RFID::getDeviceList(){
	return serial.getDeviceList();
}
int RFID::readBytes(unsigned char * buffer, int length){
	return serial.readBytes(buffer,length);
}
int RFID::writeBytes(unsigned char * buffer, int length){
	return serial.writeBytes(buffer,length);	
}
bool RFID::writeByte(unsigned char singleByte){
	return serial.writeByte(singleByte);
}
int RFID::readByte(){
	return serial.readByte();
}
void RFID::flush(bool flushIn, bool flushOut){
	serial.flush(flushIn,flushOut);
}
int RFID::available(){
	return serial.available();
}
void RFID::drain(){
	serial.drain();
}
RFItem RFID::getItemFromIndex(int i){
	return items[i];
}
void RFID::setTOT(float time){
	timeOutTime = time;
}
int RFID::getIndexOfItems(RFItem item){
	for(int i =0,n=items.size();i<n;i++){
		if(item.getRFID()==items[i].getRFID())
			return i;
	}
	return -1;
}

void RFID::addItem(RFItem item){

	if(isItemIn(item)){
		int index=getIndexOfItems(item);
		if(items[index].getItem()==item.getItem())
			return;
		else{
			items[index].setItem(item.getItem());

			XML.loadFile(dbName);
			/*XML.clear();
			XML.addTag("RFID");*/
			XML.pushTag("RFID",0);
			XML.setValue("ID:TAG",item.getRFID(),index);
			XML.setValue("ID:ITEM",item.getItem(),index);
			XML.popTag();
			XML.saveFile(dbName);
		}
	}else{
		items.push_back(item);

		XML.loadFile(dbName);
		/*XML.clear();
		XML.addTag("RFID");*/
		XML.pushTag("RFID",0);
		int tagNum = XML.addTag("ID");
		XML.setValue("ID:TAG",item.getRFID(),tagNum);
		XML.setValue("ID:ITEM",item.getItem(),tagNum);
		XML.popTag();
		XML.saveFile(dbName);
	}
}

void RFID::addItem(string tag,int item){
	RFItem temp(tag,item);
	addItem(temp);
}

bool RFID::isItemIn(RFItem item){
	for(int i=0,n=items.size();i<n;i++){
		if(items[i].getRFID()==item.getRFID())
			return true;
	}
	return false;
}

int RFID::getNofItems(){
	return items.size();
}

void RFID::update(RFItem item){
	lastTime = ofGetElapsedTimef();
	activeTag = items[getIndexOfItems(item)];
}

bool RFID::isTimeOut(){
	if(ofGetElapsedTimef()-lastTime > timeOutTime){
		return true;
		
	}else{
		return false;
	}

}

void RFID::loadDB(string filename){
	XML.loadFile(filename);
	dbName = filename;
	int nId=XML.getNumTags("RFID:ID");
	
	if(nId >0){
		XML.pushTag("RFID");
		int nIdTags=XML.getNumTags("ID");
		if(nIdTags>0){
			for(int i =0 ; i<nIdTags;i++){
				string tag = XML.getValue("ID:TAG","",i);
				int item = XML.getValue("ID:ITEM",-1,i);

				RFItem temp(tag,item);
				items.push_back(temp);
			}

		}
		XML.popTag();
	}else{
		XML.addTag("RFID");
		XML.saveFile();
	}
	

}

RFID::~RFID(){
}