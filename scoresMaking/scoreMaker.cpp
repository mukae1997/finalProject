#include <iostream>
#include <fstream>

#include "json/json.h"

using namespace std;
using namespace Json;
int main() {
	// eotm:155
	// sos:160
	int BPM = 160;
	Json::Value score;
	Json::Value arrayOfUnits;


	Json::Value tap;
	tap["length"] = 1;
	tap["type"] = "tap";

	Json::Value none;
	none["length"] = 0;
	none["type"] = "none";

	Json::Value noodle;
	int noodleLength = 3;
	int noodleInterval = 500;
	noodle["length"] = noodleLength;
	noodle["type"] = "noodle";
	int lastNoodleEndTime = 0;
	for (int i = 0; i < 300; i++) {

		Json::Value unit;
		// 100是落下的时间
		int tempoLength = (60000 / BPM);
		int timeStamp = tempoLength  * i / 10;
		if (timeStamp < 0) continue;
		unit["time"] =  timeStamp;

		Json::Value track1Note;
		Json::Value track2Note;
		Json::Value track3Note;
		Json::Value track4Note;
		int k = 0;
		bool toggle = i % 2 ? true : false;

		track1Note = toggle ? tap : none;
		track1Note["num"] = k++;


		track2Note = toggle ? none : tap;
		track2Note["num"] = k++;
		
		track3Note = toggle ? tap : none;
		track3Note["num"] = k++;

		if (timeStamp > lastNoodleEndTime + noodleInterval) { 
			track4Note = noodle;
			track4Note["num"] = k++;
			lastNoodleEndTime = timeStamp + noodleLength * tempoLength;
		}
		else {
			track4Note = none;
			track4Note["num"] = k++;
		}

		unit["tracks"].append(track1Note);
		unit["tracks"].append(track2Note);
		unit["tracks"].append(track3Note);
		unit["tracks"].append(track4Note);

		arrayOfUnits.append(unit);
	}
	score["array"] = arrayOfUnits;
	string out = score.toStyledString();
	ofstream jsonoutput("../../sos.json");
	jsonoutput << out;
}
