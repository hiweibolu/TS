class order {
public:
	word state;
	char trainID[21], from[41], to[41];
	word leaving_time, arriving_time, price, num, d;
	word next, prev;
	order() {
		state = 0;
		next = 0;
		prev = 0;
	}
};