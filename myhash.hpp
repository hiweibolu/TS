word myHash(const char *str) {
	unsigned short h1 = 7, h2 = 41;

	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		h1 = h1 * 97 + str[i];
		h2 = h2 * 43 + str[i];
	}
	return (((word)h1)<<16)+(word)h2;
}