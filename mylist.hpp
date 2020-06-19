class mylist{
public:
	fstream fs;
	char filename[20];

	mylist(const char* name) {
		strcpy(filename, name);

		#ifdef RESET_EVERYTIME
			clean();
			fs.close();
		#endif

		ifstream in(name, ios::in);
		if (in.is_open()) {
			in.close();
			fs.open(name, ios::ate | ios::in | ios::out | ios::binary);
		}
		else {
			in.close();
			ofstream out(name, ios::out);
			out.close();
			fs.open(name, ios::ate | ios::in | ios::out | ios::binary);
		}

		fs.seekg(0, ios::end);
		if (fs.tellg() == 0) {
			word len = sizeof (word);
			fs.write((char*)& len, sizeof len);
		}

	}

	void clean() {
		fs.close();
		fs.open(filename, ios::out);
	}

	~mylist() {
		fs.close();
	}

	template <class T> word put(const T &x) {
		fs.seekg(0, ios::end);
		word pos = fs.tellg();
		fs.write((char*)&x, sizeof x);
		return pos;
	}
	template <class T> void set(const T& x, word pos) {
		fs.seekg(pos, ios::beg);
		fs.write((char*)& x, sizeof x);
	}
	template <class T> void get(T& x, word pos) {
		fs.seekg(pos, ios::beg);
		fs.read((char*)&x, sizeof x);
	}
	
	bool empty() {
		fs.seekg(0, ios::end);
		return fs.tellg() == sizeof (word);
	}
	
};