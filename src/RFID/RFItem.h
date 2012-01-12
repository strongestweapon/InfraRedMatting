#include "ofMain.h"

class RFItem{
public:
	RFItem();
	~RFItem();
	RFItem(string _rfid, int _item);
	string getRFID();
	int getItem();
	void setRFID(string _rfid);
	void setItem(int _item);
	bool operator ==(RFItem i);
private:
	string rfid;
	int item;
};