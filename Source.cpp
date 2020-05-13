#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include<time.h>
using namespace std;

/******************************************************************************
*							TradeItem Struct
*******************************************************************************/
struct TradeItem {
	string symbol;
	long long int timeStamp;
	int quantity;
	int price;

	TradeItem(string s, long long int ts, int qt, int p) {
		symbol = s;
		timeStamp = ts;
		quantity = qt;
		price = p;
	}
};


/******************************************************************************
*							TradeUnit Class Header
*******************************************************************************/
class TradeUnit {
private:
	string symbol;
	long long int maxTimeGap;
	long long int lastTimeStamp;
	int volumn;
	int maxPrice;
	int totalQuantity;
	int totalPrice;
	//vector<TradeItem> tradedList;
	void beforeAddingNewTradeItem(TradeItem Trade);
public:
	TradeUnit(string s);
	int addingTradeItem(TradeItem T);
	string getSymbol();
	int getWeight();
	long long int getMaxTimeGap();
	int getMaxPrice();
	int getVolumn();

	template <typename Type>
	Type findMax(Type a, Type b);
};

/******************************************************************************
*							TradeUnit Class's implementation
*******************************************************************************/
TradeUnit::TradeUnit(string s) {
	symbol = s;
	maxTimeGap = 0;
	volumn = 0;
	maxPrice = 0;
	totalQuantity = 0;
	totalPrice = 0;
	lastTimeStamp = 0;
}

void TradeUnit::beforeAddingNewTradeItem(TradeItem T) {
	if (totalQuantity > 0) {
		maxTimeGap = findMax(maxTimeGap, T.timeStamp - lastTimeStamp);
	}
	volumn += T.quantity;
	maxPrice = findMax(maxPrice, T.price);
	totalPrice += T.price*T.quantity;
	totalQuantity += T.quantity;
	lastTimeStamp = T.timeStamp;
}
template <typename Type>
Type TradeUnit::findMax(Type a, Type b) {
	return a >= b ? a : b;
}

int TradeUnit::getWeight() {
	return totalPrice / totalQuantity;
}
string TradeUnit::getSymbol() {
	return symbol;
}
long long int TradeUnit::getMaxTimeGap() {
	return maxTimeGap;
}
int TradeUnit::getMaxPrice() {
	return maxPrice;
}

int TradeUnit::getVolumn() {
	return volumn;
}

int TradeUnit::addingTradeItem(TradeItem T) {
	if (T.symbol == symbol) {
		beforeAddingNewTradeItem(T);
		//tradedList.push_back(T);
		return 1;
	}
	return 0;
}
/****************************END TradeUnit Class********************************/


string getInputFile();
bool readFileInput(const char* fileName, map<string, TradeUnit*> &symbols);
void writeToFile(const char * fileName, map<string, TradeUnit*> &TUs);
void mapTradeItem(TradeItem &T, map<string, TradeUnit*>&tradeUnitMap);

/******************************************************************************
*							Entry of program
*******************************************************************************/
int main(int argc, char *arvg[]) {

	map<string, TradeUnit*> tradeUnitMap;
	vector<TradeUnit> allTradeUnit;
	string inputFile = getInputFile();
	//clock_t t1;
	//t1 = clock();
	if (readFileInput(inputFile.c_str(), tradeUnitMap)) {
		writeToFile("output.csv", tradeUnitMap);
	}
	//printf("time= %d clicks (%f seconds).\n", t1, ((float)t1) / CLOCKS_PER_SEC);
	system("PAUSE");
	return 0;
}

string getInputFile() {
	string fileName;
	cout << "Please enter a input *.csv file:" << endl;
	cin >> fileName;
	return fileName;
}

bool readFileInput(const char* fileName, map<string, TradeUnit*> &tradeUnitMap) {
	ifstream ifs(fileName);
	string line;
	vector <TradeUnit> tradeUnits;
	getline(ifs, line);
	int numOfUnits = 0;
	if (line.empty() && ifs.eof()) {
		cout << "Cannot open input file" << endl;
		return false;

	}
	cout << "Processing File ..." << endl;
	do {
		stringstream ss(line);
		vector<string> tradeItemInfo;
		while (ss.good()) {
			string substr;
			getline(ss, substr, ',');
			tradeItemInfo.push_back(substr);
		}

		string sb = tradeItemInfo[1];
		long long int ts = stoll(tradeItemInfo[0]);
		int qt = stol(tradeItemInfo[2]);
		int p = stoi(tradeItemInfo[3]);
		TradeItem T(sb, ts, qt, p);//create an instance of TradeItem with provided info about traded item
		mapTradeItem(T, tradeUnitMap);

	} while (getline(ifs, line));
	return true;
}

void mapTradeItem(TradeItem &T, map<string, TradeUnit*>&tradeUnitMap) {
	map<string, TradeUnit*>::iterator itr;
	itr = tradeUnitMap.find(T.symbol);
	if (itr == tradeUnitMap.end()) {
		tradeUnitMap.insert(std::make_pair(T.symbol, new TradeUnit(T.symbol)));
		itr = tradeUnitMap.find(T.symbol);
	}
	itr->second->addingTradeItem(T);
}

void writeToFile(const char* outputFile, map<string, TradeUnit*> &tradeUnitMap) {
	ofstream ofs(outputFile);
	map<string, TradeUnit*>::iterator itr;
	for (itr = tradeUnitMap.begin();itr != tradeUnitMap.end(); ++itr) {
		string sb = itr->first;
		long long int maxTimeGap = itr->second->getMaxTimeGap();
		int volumn = itr->second->getVolumn();
		int weightedAveragePrice = itr->second->getWeight();
		int maxPrice = itr->second->getMaxPrice();
		ofs << sb << "," << maxTimeGap << "," << volumn << "," << weightedAveragePrice << "," << maxPrice << endl;
	}
	ofs.close();
	cout << "Completed! Results are written into output.csv file" << endl;

}

/*********************************END Program********************************/