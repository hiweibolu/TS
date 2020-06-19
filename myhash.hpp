word myHash(const char *str) {
	unsigned short h1 = 89, h2 = 997;

	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		h1 = h1 * 83 + str[i];
		h2 = h2 * 263 + str[i];
	}
	return (((word)h1)<<16)+(word)h2;
}