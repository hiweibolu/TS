#define _CRT_SECURE_NO_WARNINGS
//#define RESET_EVERYTIME

const int M = 127 * 2, LEN = 10000;
const int minNum = M / 2, maxNum = M - 1;
const int maxTrainNumber = 10000;
const int maxStationNumber = maxTrainNumber * 10;
typedef unsigned long word;
typedef unsigned long long dword;
const dword INF = -1;
word DAYS[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };

int cnt = 0, ccnt = 0;

#include<cstring>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<cstdio>
using namespace std;

#include "btree.hpp"
#include "mylist.hpp"
#include "myhash.hpp"
#include "user.hpp"
#include "explainer.hpp"
#include "train.hpp"
#include "order.hpp"

const word minutesPerDay = 24 * 60;

word date2day(const char* s) {
	return DAYS[(s[0] - '0') * 10 + s[1] - '0' - 1] + (s[3] - '0') * 10 + s[4] - '0';
}

word date2minute(const char* s) {
	return date2day(s) * minutesPerDay;
}

void printTime(word x) {
	word d = x / (minutesPerDay);
	for (int i = 1; i <= 12; i++) {
		if (d <= DAYS[i]) {
			printf("%02d-%02u ", i, d - DAYS[i - 1]);
			break;
		}
	}
	/*if (d <= DAYS[1]) {
		printf("06-%02u ", d);
	}
	else if (d <= DAYS[2]) {
		printf("07-%02u ", d - DAYS[1]);
	}
	else {
		printf("08-%02u ", d - DAYS[2]);
	}*/
	x %= minutesPerDay;
	printf("%02u:%02u", x / 60, x % 60);
}

bool mystrcmp(const char *a, const char *b) {
	return strncmp(a, b, strlen(b)) == 0;
}

btree* userb, * loginb, * trainb, * stationb, *ttrainb;
mylist* users, * trains, *orders, *ttrains;
explainer epl;

void add_user(const char *ord) {
	bool ok = false, okk = users->empty();
	user now, cur;
	word nowh, curh;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-c") == 0) {
			if (okk) continue;
			curh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-u") == 0) {
			strcpy(now.username, epl.argv);
			nowh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-p") == 0) {
			strcpy(now.password, epl.argv);
		}
		else if (strcmp(epl.argc, "-n") == 0) {
			strcpy(now.name, epl.argv);
		}
		else if (strcmp(epl.argc, "-m") == 0) {
			strcpy(now.mailAddr, epl.argv);
		}
		else if (strcmp(epl.argc, "-g") == 0) {
			if (okk) {
				now.privilege = 10;
				continue;
			}
			now.privilege = atoi(epl.argv);
		}
	}

	if (!okk) {
		if (loginb->search(curh) > 0) {
			word _cur = userb->search(curh);
			users->get(cur, _cur);
			if (cur.privilege > now.privilege) {
				if (userb->search(nowh) == 0) {
					ok = true;
				}
			}
		}
	}

	if (okk || ok) {
		word _now = users->put(now);
		userb->insert(nowh, _now);
		printf("0\n");
	}
	else printf("-1\n");
}

void login(const char * ord) {
	bool ok = false;
	user now;
	word nowh;
	char pw[31];

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-u") == 0) {
			nowh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-p") == 0) {
			strcpy(pw, epl.argv);
		}
	}

	if (loginb->search(nowh) == 0) {
		word _now = userb->search(nowh);
		if (_now) {
			users->get(now, _now);
			if (strcmp(now.password, pw) == 0) {
				ok = true;
			}
		}
	}
	
	if (ok) {
		loginb->insert(nowh, 1);
		printf("0\n");
	}
	else printf("-1\n");

}

void logout(const char * ord) {
	bool ok = false;
	user now;
	word nowh;
	
	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-u") == 0) {
			nowh = myHash(epl.argv);
		}
	}

	if (loginb->search(nowh)) {
		ok = true;
	}

	if (ok) {
		loginb->insert(nowh, 0);
		printf("0\n");
	}
	else printf("-1\n");

}

void query_profile(const char * ord) {
	bool ok = false;
	user now, cur;
	word nowh, curh;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-c") == 0) {
			curh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-u") == 0) {
			nowh = myHash(epl.argv);
		}
	}

	if (loginb->search(curh) > 0) {
		word _cur = userb->search(curh);
		users->get(cur, _cur);
		word _now = userb->search(nowh);
		if (_now) {
			users->get(now, _now);
			if (cur.privilege > now.privilege || curh == nowh) {
				ok = true;
			}
		}
	}

	if (ok) printf("%s %s %s %u\n", now.username, now.name, now.mailAddr, now.privilege);
	else printf("-1\n");
}

void modify_profile(const char * ord) {
	bool ok = false;
	user now, cur, tmp;
	word nowh, curh, _now;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-c") == 0) {
			curh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-u") == 0) {
			nowh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-p") == 0) {
			strcpy(tmp.password, epl.argv);
		}
		else if (strcmp(epl.argc, "-n") == 0) {
			strcpy(tmp.name, epl.argv);
		}
		else if (strcmp(epl.argc, "-m") == 0) {
			strcpy(tmp.mailAddr, epl.argv);
		}
		else if (strcmp(epl.argc, "-g") == 0) {
			tmp.privilege = atoi(epl.argv);
		}
	}

	if (loginb->search(curh) > 0) {
		word _cur = userb->search(curh);
		users->get(cur, _cur);
		_now = userb->search(nowh);
		if (_now > 0) {
			users->get(now, _now);
			if (cur.privilege > now.privilege || curh == nowh) {
				if (tmp.privilege == -1 || cur.privilege > tmp.privilege || cur.privilege == tmp.privilege && curh == nowh) {
					if (tmp.password[0]) strcpy(now.password, tmp.password);
					if (tmp.name[0]) strcpy(now.name, tmp.name);
					if (tmp.mailAddr[0]) strcpy(now.mailAddr, tmp.mailAddr);
					if (tmp.privilege != -1) now.privilege = tmp.privilege;
					ok = true;
				}
			}
		}
	}

	if (ok) {
		users->set(now, _now);
		printf("%s %s %s %u\n", now.username, now.name, now.mailAddr, now.privilege);
	}
	else printf("-1\n");

}

void add_train(const char* ord) {
	bool ok = false;
	train now;
	word nowh;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-i") == 0) {
			strcpy(now.trainID, epl.argv);
		}
		else if (strcmp(epl.argc, "-n") == 0) {
			now.stationNum = atoi(epl.argv);
		}
		else if (strcmp(epl.argc, "-m") == 0) {
			now.seatNum[0] = atoi(epl.argv);
		}
		else if (strcmp(epl.argc, "-s") == 0) {
			int len = 0;
			char* p = epl.argv,* q = now.stations[len];
			while (*p) {
				if (*p == '|') {
					*q = 0;
					q = now.stations[++len];
				}
				else *(q++) = *p;
				p++;
			}
			*q = 0;
		}
		else if (strcmp(epl.argc, "-p") == 0) {
			word* q = now.prices + 1;
			char* p = epl.argv;
			while (*p) {
				if (*p == '|') q++;
				else *q = *q * 10 + *p - '0';
				p++;
			}
		}
		else if (strcmp(epl.argc, "-x") == 0) {
			now.startTime = ((epl.argv[0]- '0') * 10 + epl.argv[1] - '0') * 60 + ((epl.argv[3] - '0') * 10 + epl.argv[4] - '0');
		}
		else if (strcmp(epl.argc, "-t") == 0) {
			word* q = now.travelTimes;
			char* p = epl.argv;
			while (*p) {
				if (*p == '|') q++;
				else *q = *q * 10 + *p - '0';
				p++;
			}
		}
		else if (strcmp(epl.argc, "-o") == 0) {
			word* q = now.stopoverTimes + 1;
			char* p = epl.argv;
			while (*p) {
				if (*p == '|') q++;
				else *q = *q * 10 + *p - '0';
				p++;
			}
		}
		else if (strcmp(epl.argc, "-d") == 0) {
			now.saleDate[0] = date2day(epl.argv);
			now.saleDate[1] = date2day(epl.argv + 6);
		}
		else if (strcmp(epl.argc, "-y") == 0) {
			now.type = epl.argv[0];
		}
	}

	nowh = now.getHash();
	if (trainb->search(nowh) == 0) {
		ok = true;
	}
	
	if (ok) {
		now.state = 0;
		for (int i = 1; i < now.stationNum; i++) now.seatNum[i] = now.seatNum[0];
		now.prices[0] = 0;
		for (int i = 1; i < now.stationNum; i++) now.prices[i] += now.prices[i-1];

		word _now = trains->put(now);
		trainb->insert(nowh, _now);
		
		for (int i = now.saleDate[0]; i <= now.saleDate[1]; i++) {
			ttrainb->insert(nowh+((dword)i << 32), ttrains->put(now));
		}

		for (int i = 0; i < now.stationNum; i++) {
			word tmph = myHash(now.stations[i]);
			stationb->insert(((dword)tmph << 32) + cnt, ((dword)nowh << 32ull) + i);
		}

		printf("0\n");
	}
	else printf("-1\n");
}

void release_train(const char* ord) {
	bool ok = false;
	train now;
	word nowh, _now;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-i") == 0) {
			nowh = myHash(epl.argv);
		}
	}

	_now = trainb->search(nowh);
	if (_now) {
		trains->get(now, _now);
		if (now.state == 0) {
			ok = true;
		}
	}

	if (ok) {
		now.state = 1;
		trains->set(now, _now);
		printf("0\n");
	}
	else printf("-1\n");

}

void query_train(const char* ord) {
	bool ok = false;
	train now;
	word nowh, _now, d;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-i") == 0) {
			nowh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-d") == 0) {
			d = date2day(epl.argv);
		}
	}

	_now = trainb->search(nowh);
	if (_now) {
		trains->get(now, _now);
		if (now.saleDate[0] <= d && d <= now.saleDate[1]) {
			_now = ttrainb->search(nowh + ((dword)d << 32));
			ttrains->get(now, _now);
			ok = true;
		}
	}

	if (ok) {
		printf("%s %c\n", now.trainID, now.type);
		word m = now.startTime;
		for (int i = 0; i < now.stationNum; i++) {
			printf("%s ", now.stations[i]);
			if (i == 0) printf("xx-xx xx:xx");
			else {
				m += now.travelTimes[i - 1];
				printTime(d * minutesPerDay + m);
			}
			printf(" -> ");
			if (i == now.stationNum - 1) printf("xx-xx xx:xx");
			else {
				if (i > 0) m += now.stopoverTimes[i];
				printTime(d * minutesPerDay + m);
			}
			printf(" %u ", now.prices[i]);
			if (i == now.stationNum - 1) printf("x\n");
			else printf("%u\n", now.seatNum[i]);
		}
		//printf("0\n");
	}
	else printf("-1\n");

}

void delete_train(const char* ord) {
	bool ok = false;
	train now;
	word nowh, _now;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-i") == 0) {
			nowh = myHash(epl.argv);
		}
	}

	_now = trainb->search(nowh);
	if (_now) {
		trains->get(now, _now);
		if (now.state == 0) {
			ok = true;
		}
	}

	if (ok) {
		trainb->insert(nowh, 0);
		printf("0\n");
	}
	else printf("-1\n");
}

struct trainInfo {
	word start, price, seat, time;
	char trainID[21];
}*tInfo;
bool cmp1(const trainInfo& a, const trainInfo& b) {
	return a.price < b.price || a.price == b.price && strcmp(a.trainID, b.trainID) < 0;
}
bool cmp2(const trainInfo& a, const trainInfo& b) {
	return a.time < b.time || a.time == b.time && strcmp(a.trainID, b.trainID) < 0;
}

dword tStart[maxTrainNumber], tEnd[maxTrainNumber];
void query_ticket(const char* ord) {
	bool ok = true;
	train now;
	word nowh, _now;
	word sth, edh, d, _st, _ed;
	bool sortByTime = true;
	char nameStart[41], nameEnd[41];
	tInfo = new trainInfo[maxTrainNumber];

	int cntInfo = 0, cntStart = 0, cntEnd = 0;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-s") == 0) {
			sth = myHash(epl.argv);
			strcpy(nameStart, epl.argv);
		}
		else if (strcmp(epl.argc, "-t") == 0) {
			edh = myHash(epl.argv);
			strcpy(nameEnd, epl.argv);
		}
		else if (strcmp(epl.argc, "-d") == 0) {
			d = date2day(epl.argv);
		}
		else if (strcmp(epl.argc, "-p") == 0) {
			if (epl.argv[0] == 'c') sortByTime = false;
		}
	}

	stationb->query(sth * (1ULL << 32), tStart, cntStart);
	stationb->query(edh * (1ULL << 32), tEnd, cntEnd);
	if (cntStart && cntEnd) {
		sort(tStart, tStart + cntStart);
		sort(tEnd, tEnd + cntEnd);
	}
	int j = 0;
	for (int i = 0; i < cntStart; i++) {
		while (j < cntEnd && (tEnd[j] >> 32) < (tStart[i] >> 32)) j++;
		if (j == cntEnd) break;
		if ((tEnd[j] >> 32) == (tStart[i] >> 32)) {
			nowh = tEnd[j] >> 32;
			_now = trainb->search(nowh);
			if (_now == 0) continue;
			trains->get(now, _now);
			int x = tStart[i], y = tEnd[j];
			if (x < y && now.state == 1) {
				word m = now.startTime;
				for (int k = 0; k < x; k++) {
					m += now.stopoverTimes[k + 1];
					m += now.travelTimes[k];
				}
				if (now.saleDate[0] <= d - m / minutesPerDay && now.saleDate[1] >= d - m / minutesPerDay) {
					ttrains->get(now, ttrainb->search(nowh + ((dword)d - m / minutesPerDay << 32)));
					strcpy(tInfo[cntInfo].trainID, now.trainID);
					m += (d - m / minutesPerDay) * minutesPerDay;
					tInfo[cntInfo].start = m;
					tInfo[cntInfo].price = now.prices[y] - now.prices[x];
					tInfo[cntInfo].seat = -1;
					tInfo[cntInfo].time = 0;
					for (int k = x; k < y; k++) {
						tInfo[cntInfo].seat = min(tInfo[cntInfo].seat, now.seatNum[k]);
						tInfo[cntInfo].time += now.travelTimes[k];
						if (k != x) tInfo[cntInfo].time += now.stopoverTimes[k];
					}
					cntInfo++;
				}
			}
		}
	}

	if (ok) {
		if (sortByTime) sort(tInfo, tInfo + cntInfo, cmp2);
		else sort(tInfo, tInfo + cntInfo, cmp1);
		printf("%d\n", cntInfo);
		for (int i = 0; i < cntInfo; i++) {
			printf("%s %s ", tInfo[i].trainID, nameStart);
			printTime(tInfo[i].start);
			printf(" -> %s ", nameEnd);
			printTime(tInfo[i].start + tInfo[i].time);
			printf(" %u %u\n", tInfo[i].price, tInfo[i].seat);
		}
	}
	else printf("-1\n");
	delete[] tInfo;
}

void buy_ticket(const char* ord) {
	int ok = 0;
	user unow;
	train tnow;
	order now;
	word unowh, tnowh, d, buyNum, hfrom, hto;
	dword sumPrice;
	char from[41], to[41];
	bool orderred = false;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-u") == 0) {
			unowh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-i") == 0) {
			tnowh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-d") == 0) {
			d = date2day(epl.argv);
		}
		else if (strcmp(epl.argc, "-n") == 0) {
			buyNum = atoi(epl.argv);
		}
		else if (strcmp(epl.argc, "-f") == 0) {
			strcpy(from, epl.argv);
		}
		else if (strcmp(epl.argc, "-t") == 0) {
			strcpy(to, epl.argv);
		}
		else if (strcmp(epl.argc, "-q") == 0) {
			if (epl.argv[0] == 't') orderred = true;
		}
	}


	hfrom = myHash(from);
	hto = myHash(to);
	word _tnow;
	if (loginb->search(unowh)) {
		_tnow = trainb->search(tnowh);
		trains->get(tnow, _tnow);
		int x=-1, y=-1;
		word m = tnow.startTime;
		for (int k = 0; k < tnow.stationNum - 1; k++) {
			if (hfrom == myHash(tnow.stations[k])) {
				x = k;
				break;
			}
			m += tnow.stopoverTimes[k + 1];
			m += tnow.travelTimes[k];
		}
		if (x != -1) {
			if (tnow.saleDate[0] <= d - m / minutesPerDay && tnow.saleDate[1] >= d - m / minutesPerDay) {
				_tnow = ttrainb->search(tnowh + ((dword)d - m / minutesPerDay << 32));
				ttrains->get(tnow, _tnow);

				word seat = -1, time = 0;
				for (int k = x; k < tnow.stationNum; k++) {
					if (hto == myHash(tnow.stations[k])) {
						y = k;
						break;
					}
					seat = min(seat, tnow.seatNum[k]);
					time += tnow.travelTimes[k];
					if (k != x) time += tnow.stopoverTimes[k];
				}
				if (y != -1) {
					
					now.price = tnow.prices[y] - tnow.prices[x];

					if (buyNum <= seat) {
						for (int k = x; k < y; k++)tnow.seatNum[k] -= buyNum;
						ok = 1;
						sumPrice = 1ull * now.price * buyNum;
						now.state = 0;
					}
					else {
						if (orderred) {
							if (buyNum <= tnow.seatNum[tnow.stationNum - 1]) {
								now.state = 1;
								ok = 2;
							}
						}
					}

					if (ok) {
						word _unow = userb->search(unowh);
						users->get(unow, _unow);
						now.prev = unow.first;
						now.next = tnow.first;
						strcpy(now.trainID, tnow.trainID);
						strcpy(now.from, from);
						strcpy(now.to, to);
						now.d = d - m / minutesPerDay;
						m += (d - m / minutesPerDay) * minutesPerDay;
						now.leaving_time = m;
						now.arriving_time = m + time;
						now.num = buyNum;
						word _now = orders->put(now);
						unow.first = _now;
						unow.orderNum++;
						users->set(unow, _unow);
						if (ok == 2) {
							tnow.first = _now;
						}
						ttrains->set(tnow, _tnow);
					}
				}
			}
		}
	}

	if (ok == 0) printf("-1\n");
	else if (ok == 1) printf("%llu\n", sumPrice);
	else printf("queue\n");
}

void query_order(const char* ord) {
	bool ok = false;
	user now;
	word nowh;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-u") == 0) {
			nowh = myHash(epl.argv);
		}
	}

	if (loginb->search(nowh)) {
		ok = true;
	}

	if (ok) {
		users->get(now, userb->search(nowh));
		printf("%u\n", now.orderNum);
		order onow;
		for (word pos = now.first; pos; pos = onow.prev) {
			orders->get(onow, pos);
			if (onow.state == 0) printf("[success] ");
			else if (onow.state == 1) printf("[pending] ");
			else printf("[refunded] ");
			printf("%s %s ", onow.trainID, onow.from);
			printTime(onow.leaving_time);
			printf(" -> %s ", onow.to);
			printTime(onow.arriving_time);
			printf(" %u %u\n", onow.price, onow.num);
		}
	}
	else printf("-1\n");
}

word checkOrder(train& tnow, word _onow) {
	if (_onow == 0) return 0;
	order onow;
	orders->get(onow, _onow);
	onow.next = checkOrder(tnow, onow.next);
	if (onow.state == 2) {
		orders->set(onow, _onow);
		return onow.next;
	}
	word hfrom = myHash(onow.from), hto = myHash(onow.to), seat = -1;
	int x, y;
	for (int j = 0, k = 0; j < tnow.stationNum; j++) {
		if (k == 0) {
			if (hfrom == myHash(tnow.stations[j])) {
				x = j;
				k = 1;
				seat = min(seat, tnow.seatNum[j]);
			}
		}
		else if (k == 1) {
			if (hto == myHash(tnow.stations[j])) {
				y = j;
				break;
			}
			else seat = min(seat, tnow.seatNum[j]);
		}
	}
	if (seat >= onow.num) {
		for (int i = x; i < y; i++) tnow.seatNum[i] -= onow.num;
		onow.state = 0;
		orders->set(onow, _onow);
		return onow.next;
	}
	else {
		orders->set(onow, _onow);
		return _onow;
	}
}

void refund_ticket(const char* ord) {
	bool ok = false;
	user now;
	word nowh, id = 1;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-u") == 0) {
			nowh = myHash(epl.argv);
		}
		else if (strcmp(epl.argc, "-n") == 0) {
			id = atoi(epl.argv);
		}
	}

	if (loginb->search(nowh)) {
		users->get(now, userb->search(nowh));
		if (now.orderNum >= id) {
			ok = true;
			order onow;
			int i = 0;
			for (word pos = now.first; pos; pos = onow.prev) {
				orders->get(onow, pos);
				if (++i == id) {
					if (onow.state == 0) {
						onow.state = 2;
						ok = true;
						train tnow;
						word tnowh = myHash(onow.trainID), hfrom = myHash(onow.from), hto=myHash(onow.to);
						word _tnow = ttrainb->search(tnowh + ((dword)onow.d << 32));
						ttrains->get(tnow, _tnow);
						for (int j = 0, k = 0; j < tnow.stationNum; j++) {
							if (k == 0) {
								if (hfrom == myHash(tnow.stations[j])) {
									k = 1;
									tnow.seatNum[j] += onow.num;
								}
							}
							else if (k == 1) {
								if (hto == myHash(tnow.stations[j])) {
									break;
								}
								else tnow.seatNum[j] += onow.num;
							}
						}
						tnow.first = checkOrder(tnow, tnow.first);
						ttrains->set(tnow, _tnow);
						orders->set(onow, pos);
					}
					else if (onow.state == 1) {
						onow.state = 2;
						orders->set(onow, pos);
						ok = true;
					}
					break;
				}
			}
		}
	}

	if (ok) {
		printf("0\n");
	}
	else printf("-1\n");
}

dword sta[maxStationNumber], eda[maxStationNumber];
void query_transfer(const char* ord) {
	bool ok = false;
	train now;
	word nowh, _now;
	word sth, edh, d, _st, _ed;
	bool sortByTime = true;
	char nameStart[41], nameEnd[41], nameMid[41], namemid[41];
	tInfo = new trainInfo[4];

	int cntInfo = 0, cntStart = 0, cntEnd = 0, snStart = 0, snEnd = 0;

	epl.init(ord);
	while (epl.next()) {
		if (strcmp(epl.argc, "-s") == 0) {
			sth = myHash(epl.argv);
			strcpy(nameStart, epl.argv);
		}
		else if (strcmp(epl.argc, "-t") == 0) {
			edh = myHash(epl.argv);
			strcpy(nameEnd, epl.argv);
		}
		else if (strcmp(epl.argc, "-d") == 0) {
			d = date2day(epl.argv);
		}
		else if (strcmp(epl.argc, "-p") == 0) {
			if (epl.argv[0] == 'c') sortByTime = false;
		}
	}

	//stationb->query(sth * (1ULL << 32), tBegin, cntBegin);
	stationb->query(sth * (1ULL << 32), tStart, cntStart);
	stationb->query(edh * (1ULL << 32), tEnd, cntEnd);
	for (int i = 0; i < cntStart; i++) {
		nowh = tStart[i] >> 32;
		_now = trainb->search(nowh);
		if (_now == 0) continue;
		trains->get(now, _now);
		for (int j = tStart[i] + 1; j < now.stationNum; j++) {
			sta[snStart++] = ((dword)myHash(now.stations[j]) << 32) + nowh;
		}
	}
	for (int i = 0; i < cntEnd; i++) {
		nowh = tEnd[i] >> 32;
		_now = trainb->search(nowh);
		if (_now == 0) continue;
		trains->get(now, _now);
		for (int j = 0; j < (int)tEnd[i]; j++) {
			eda[snEnd++] = ((dword)myHash(now.stations[j]) << 32) + nowh;
		}
	}

	if (snStart && snEnd) {
		sort(sta, sta + snStart);
		sort(eda, eda + snEnd);
	}

	if (ccnt == 20389) {
		ccnt = ccnt;
	}
	int j = 0;
	for (int i = 0; i < snStart; i++) {
		while (j < snEnd && (eda[j] >> 32) < (sta[i] >> 32)) j++;
		if (j == snEnd) break;
		int _j = j;
		while ((eda[j] >> 32) == (sta[i] >> 32)) {
			cntInfo = 0;
			nowh = sta[i];
			_now = trainb->search(nowh);
			if (_now == 0 || ((word)sta[i]) == ((word)eda[j])) {
				j++;
				continue;
			}
			trains->get(now, _now);
			if (now.state == 1) {
				int x = 0, y = 0;
				word m = now.startTime;
				for (int k = 0; k < now.stationNum - 1; k++) {
					if (sth == myHash(now.stations[k])) {
						x = k;
						break;
					}
					m += now.stopoverTimes[k + 1];
					m += now.travelTimes[k];
				}

				if (now.saleDate[0] <= d - m / minutesPerDay && now.saleDate[1] >= d - m / minutesPerDay) {
					ttrains->get(now, ttrainb->search(nowh + ((dword)d - m / minutesPerDay << 32)));
					strcpy(tInfo[cntInfo].trainID, now.trainID);
					m += (d - m / minutesPerDay) * minutesPerDay;
					tInfo[cntInfo].start = m;
					tInfo[cntInfo].seat = -1;
					tInfo[cntInfo].time = 0;
					for (int k = x; k < now.stationNum; k++) {
						if ((eda[j] >> 32) == myHash(now.stations[k])) {
							y = k;
							strcpy(namemid, now.stations[k]);
							break;
						}
						tInfo[cntInfo].seat = min(tInfo[cntInfo].seat, now.seatNum[k]);
						tInfo[cntInfo].time += now.travelTimes[k];
						if (k != x) tInfo[cntInfo].time += now.stopoverTimes[k];
					}
					tInfo[cntInfo].price = now.prices[y] - now.prices[x];
					word mm = m + tInfo[cntInfo].time;
					cntInfo++;
					//==============================================================================================
					{
						nowh = eda[j];
						_now = trainb->search(nowh);
						if (_now == 0) continue;
						trains->get(now, _now);
						if (now.state == 1) {
							int x = 0, y = 0;
							word m = now.startTime;
							for (int k = 0; k < now.stationNum - 1; k++) {
								if ((eda[j] >> 32) == myHash(now.stations[k])) {
									x = k;
									break;
								}
								m += now.stopoverTimes[k + 1];
								m += now.travelTimes[k];
							}

							if (now.saleDate[1] * minutesPerDay + m >= mm) {
								mm = max(mm, now.saleDate[0] * minutesPerDay + m);
								if (mm % minutesPerDay > m % minutesPerDay) mm += minutesPerDay + (m % minutesPerDay) - (mm % minutesPerDay);
								else mm += (m % minutesPerDay) - (mm % minutesPerDay);
								mm -= m;
								ttrains->get(now, ttrainb->search(nowh + ((dword)mm / minutesPerDay << 32)));
								strcpy(tInfo[cntInfo].trainID, now.trainID);
								m += (mm / minutesPerDay) * minutesPerDay;
								//if (m < mm) m += minutesPerDay;
								tInfo[cntInfo].start = m;
								tInfo[cntInfo].seat = -1;
								tInfo[cntInfo].time = 0;
								for (int k = x; k < now.stationNum; k++) {
									if (edh == myHash(now.stations[k])) {
										y = k;
										break;
									}
									tInfo[cntInfo].seat = min(tInfo[cntInfo].seat, now.seatNum[k]);
									tInfo[cntInfo].time += now.travelTimes[k];
									if (k != x) tInfo[cntInfo].time += now.stopoverTimes[k];
								}
								tInfo[cntInfo].price = now.prices[y] - now.prices[x];
								cntInfo++;
								{
									if (ok) {
										if (sortByTime) {
											if (tInfo[1].start + tInfo[1].time - tInfo[0].start < tInfo[3].start + tInfo[3].time - tInfo[2].start
												|| tInfo[1].start + tInfo[1].time - tInfo[0].start == tInfo[3].start + tInfo[3].time - tInfo[2].start
												&& tInfo[0].time < tInfo[2].time) {
												tInfo[2] = tInfo[0];
												tInfo[3] = tInfo[1]; 
												strcpy(nameMid, namemid);
											}
										}
										else {
											if (tInfo[0].price + tInfo[1].price < tInfo[2].price + tInfo[3].price
												|| tInfo[0].price + tInfo[1].price == tInfo[2].price + tInfo[3].price
												&& tInfo[0].time < tInfo[2].time) {
												tInfo[2] = tInfo[0];
												tInfo[3] = tInfo[1];
												strcpy(nameMid, namemid);
											}
										}
									}
									else {
										tInfo[2] = tInfo[0];
										tInfo[3] = tInfo[1];
										strcpy(nameMid, namemid);
									}
									ok = true;
								}
							}
						}
					}
				}
			}
			j++;
		}
		j = _j;
	}

	//printf("%d ", ccnt);
	if (ok) {
		printf("%s %s ", tInfo[2].trainID, nameStart);
		printTime(tInfo[2].start);
		printf(" -> %s ", nameMid);
		printTime(tInfo[2].start + tInfo[2].time);
		printf(" %u %u\n", tInfo[2].price, tInfo[2].seat);
		printf("%s %s ", tInfo[3].trainID, nameMid);
		printTime(tInfo[3].start);
		printf(" -> %s ", nameEnd);
		printTime(tInfo[3].start + tInfo[3].time);
		printf(" %u %u\n", tInfo[3].price, tInfo[3].seat);
	}
	else printf("0\n");
	delete[] tInfo;
}

void explain(const char* ord) {
	if (mystrcmp(ord, "add_user")) add_user(ord + strlen("add_user"));
	else if (mystrcmp(ord, "login")) login(ord + strlen("login"));
	else if (mystrcmp(ord, "logout")) logout(ord + strlen("logout"));
	else if (mystrcmp(ord, "query_profile")) query_profile(ord + strlen("query_profile"));
	else if (mystrcmp(ord, "modify_profile")) modify_profile(ord + strlen("modify_profile"));
	else if (mystrcmp(ord, "add_train")) add_train(ord + strlen("add_train"));
	else if (mystrcmp(ord, "release_train")) release_train(ord + strlen("release_train"));
	else if (mystrcmp(ord, "query_train")) query_train(ord + strlen("query_train"));
	else if (mystrcmp(ord, "delete_train")) delete_train(ord + strlen("delete_train"));
	else if (mystrcmp(ord, "query_ticket")) query_ticket(ord + strlen("query_ticket"));
	else if (mystrcmp(ord, "query_transfer")) query_transfer(ord + strlen("query_transfer"));
	else if (mystrcmp(ord, "buy_ticket")) buy_ticket(ord + strlen("buy_ticket"));
	else if (mystrcmp(ord, "query_order")) query_order(ord + strlen("query_order"));
	else if (mystrcmp(ord, "refund_ticket")) refund_ticket(ord + strlen("refund_ticket"));
	else if (mystrcmp(ord, "clean")) {
		loginb->clean();
		userb->clean();
		users->clean();
		trainb->clean();
		trains->clean();
		stationb->clean();
		orders->clean();
		ttrains->clean();
		ttrainb->clean();
		delete loginb;
		delete userb;
		delete users;
		delete trainb;
		delete trains;
		delete stationb;
		delete orders;
		delete ttrains;
		delete ttrainb;
		loginb = new btree("loginb.txt");
		userb = new btree("userb.txt");
		users = new mylist("users.txt");
		trainb = new btree("trainb.txt");
		trains = new mylist("trains.txt");
		stationb = new btree("stationb.txt");
		orders = new mylist("orders.txt");
		ttrains = new mylist("ttrains.txt");
		ttrainb = new btree("ttrainb.txt");
	}

}

char ord[LEN];

int main() {
	fstream fs("cnt.txt", ios::in);
	if (fs.is_open()) fs >> cnt;
	else cnt = 0;

	/*freopen("in.txt", "r", stdin);
	freopen("data.out", "w", stdout);*/
	/*btree t("test.txt");
	cout << sizeof node << endl;
	for (int i = 1; i < 1000; i++) {
		t.insert(i, i * 3);
	}
	for (int i = 500; i < 700; i++) {
		cout << t.search(i) << ' ';
	}
	cout << endl;
	t.print();*/

	loginb = new btree("loginb.txt");
	userb = new btree("userb.txt");
	users = new mylist("users.txt");
	trainb = new btree("trainb.txt");
	trains = new mylist("trains.txt");
	stationb = new btree("stationb.txt");
	orders = new mylist("orders.txt");
	ttrains = new mylist("ttrains.txt");
	ttrainb = new btree("ttrainb.txt");

	for (int i = 1; i <= 12; i++) DAYS[i] += DAYS[i - 1];

	while (cin.getline(ord, LEN)) {
		if (strcmp(ord, "exit") == 0) {
			cout << "bye" << endl;
			loginb->clean();
			delete loginb;
			loginb = new btree("loginb.txt");
			break;
		}
		cnt++;
		ccnt++;
		explain(ord);
	}
	
	loginb->clean();

	fs.close();
	fs.open("cnt.txt", ios::out);
	fs << cnt;

	return 0;
}