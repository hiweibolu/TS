class train {
public:
	char trainID[23], stations[100][43];
	word stationNum, seatNum[103], prices[103], startTime, travelTimes[103], stopoverTimes[103], saleDate[2];
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