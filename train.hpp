class train {
public:
	char trainID[21], stations[100][41];
	word stationNum, seatNum[100], prices[100], startTime, travelTimes[100], stopoverTimes[100], saleDate[2];
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