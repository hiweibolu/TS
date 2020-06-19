class order {
public:
	word state;
	char trainID[23], from[43], to[43];
	word leaving_time, arriving_time, price, num, d;
	word next, prev;
	order() {
		state = 0;
		next = 0;
		prev = 0;
	}
};