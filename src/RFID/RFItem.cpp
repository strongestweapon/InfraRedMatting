#include "RFItem.h"

RFItem::RFItem(){};

RFItem::RFItem(string _rfid, int _item){
	rfid=_rfid;
	item=_item;
}

string RFItem::getRFID(){
	return rfid;
}
int RFItem::getItem(){
	return item;
}

void RFItem::setRFID(string _rfid){
	rfid=_rfid;
}
void RFItem::setItem(int _item){
	item=_item;
}

bool RFItem::operator ==(RFItem i){
	if(rfid == i.getRFID())
		return true;
	return false;
}

RFItem::~RFItem(){
}