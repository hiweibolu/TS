class explainer{
public:
	const char* ptr;
	char argc[5], argv[LEN];

	void init(const char * _ptr){
		ptr = _ptr;
		argc[0] = 0;
		argv[0] = 0;
	}

	int next() {
		while (*ptr && *ptr == ' ') ptr++;
		if (*ptr == 0) return 0;

		int len = 0;
 		while (*ptr && *ptr != ' ') argc[len++] = *(ptr++);
		argc[len] = 0;

		while (*ptr && *ptr == ' ') ptr++;
		if (*ptr == 0) return 0;
		len = 0;
		while (*ptr && *ptr != ' ') argv[len++] = *(ptr++);
		argv[len] = 0;
		
		return 1;
	}

};