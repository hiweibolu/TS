class order {
public:
	word state;
	char trainID[27], from[47], to[47];
	word leaving_time, arriving_time, price, num, d;
	word next, prev;
	order() {
		state = 0;
		next = 0;
		prev = 0;
	}
};