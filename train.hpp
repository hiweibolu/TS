const int maxStation = 50;
class train {
public:
	char trainID[23], stations[maxStation][43];
	word stationNum, seatNum[maxStation], prices[maxStation], startTime, travelTimes[maxStation], stopoverTimes[maxStation], saleDate[2];
	char type;
	word state,first;

	train() {
		memset(seatNum, 0, sizeof seatNum);
		memset(prices, 0, sizeof prices);
		memset(travelTimes, 0, sizeof travelTimes);
		memset(stopoverTimes, 0, sizeof stopoverTimes);
		first = 0;
	}

	word getHash() {
		return myHash(trainID);
	}
};