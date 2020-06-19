class node {
public:
	dword key[M], num, isLeaf, next, son[M] ;

	node() {
		memset(son, 0, sizeof son);
		memset(key, 0, sizeof key);
		isLeaf = false;
		num = 0;
		next = 0;
	}

	int search(dword x) {
		int l = 0, r = num;
		while (l < r) {
			int mid = l + r >> 1;
			if (key[mid] >= x) r = mid;
			else l = mid + 1;
		}
		if (l == num) throw 1;
		return l;
	}

	int add(dword x, dword y = 0) {
		int res = -1;
		for (int i = 0; i < num + 1; i++) {
			if (key[i] == 0) {
				key[i] = x;
				son[i] = y;
				num++;
				if (res == -1) res = i;
				break;
			}
			if (key[i] > x) {
				swap(key[i], x);
				swap(son[i], y);
				if (res == -1) res = i;
			}
			if (key[i] == x) {
				son[i] = y;
				return i;
			}
		}
		return res;
	}

	dword top() {
		if (num==0) throw 1;
		return key[num - 1];
	}

	void print(int t = 0) {
		for (int i = 1; i <= t; i++) cout << "    ";
		if (isLeaf)
			for (int i = 0; i < num; i++) cout << key[i] << '_' << son[i] <<  ' ';
		else
			for (int i = 0; i < num; i++) cout << key[i] << ' ';
		cout << endl;
	}

};

class btree {
public:
	fstream fs;
	char filename[20];

	btree(const char * name) {
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
		if (fs.tellg() == 0){
			dword len = sizeof(dword);
			fs.write((char*)& len, sizeof len);

			node temp;
			temp.isLeaf = true;
			temp.num = 0;
			temp.add(INF);
			fs.write((char*)& temp, sizeof temp);

		}
	}

	~btree(){
		fs.close();
	}

	void clean() {
		fs.close();
		fs.open(filename, ios::out);
	}
	
	void getnode(node &now, dword p) {
		fs.seekg(p,ios::beg);
		fs.read((char*)& now, sizeof now);
	}
	void putnode(node& now, dword p) {
		fs.seekg(p, ios::beg);
		fs.write((char*)& now, sizeof now);
	}
	dword newnode(node& now) {
		fs.seekg(0, ios::end);
		dword pos = fs.tellg();
		fs.write((char*)& now, sizeof now);
		return pos;
	}
	dword getroot() {
		dword res;
		fs.seekg(0, ios::beg);
		fs.read((char*)& res, sizeof res);
		return res;
	}
	void setroot(dword pos) {
		fs.seekg(0, ios::beg);
		fs.write((char*)& pos, sizeof pos);
	}

	void query(dword x, dword* buffer, int &cnt) {
		query(getroot(), x, buffer, cnt);
	}
	void query(dword _now, dword x, dword* buffer, int& cnt) {
		node now;
		getnode(now, _now);

		int ind = now.search(x);
		if (now.isLeaf) {
			while (ind < now.num && now.key[ind] >= x && now.key[ind] < x + (1ull << 32)) {
				buffer[cnt++] = now.son[ind++];
			}
			if (ind == now.num && now.next) {
				query(now.next, x, buffer, cnt);
			}
		}
		else query(now.son[ind], x, buffer, cnt);
	}

	/*void modify(dword x, dword y) {
		modify(getroot(), x, y);
	}
	void modify(dword _now, dword x, dword y) {
		node now;
		getnode(now, _now);

		int ind = now.search(x);
		if (now.isLeaf) {
			if (now.key[ind] == x) {
				now.add(x, y);
			}
			else return;
		}
		else modify(now.son[ind], x, y);
	}*/
	dword search(dword x) {
		return search(getroot(), x);
	}
	dword search(dword _now, dword x) {
		node now;
		getnode(now, _now);

		int ind = now.search(x);
		if (now.isLeaf) {
			if (now.key[ind] == x) return now.son[ind];
			else return 0;
		}
		else {
			return search(now.son[ind], x);
		}
	}
	void insert(dword x, dword y) {
		insert(x, y, getroot(), 0);
	}
	void insert(dword x, dword y, dword _now, node *fa) {
		node now;
		getnode(now, _now);

		if (now.isLeaf) {
			now.add(x, y);
		}
		else {
			insert(x, y, now.son[now.search(x)], &now);
		}
		if (now.num > maxNum) {
			int mid = now.num / 2;

			node tmp;
			tmp.num = now.num - mid;
			for (int i = 0; i < now.num - mid; i++) {
				tmp.key[i] = now.key[i + mid];
				tmp.son[i] = now.son[i + mid];
			}
			tmp.isLeaf = now.isLeaf;
			tmp.next = now.next;
			dword _tmp = newnode(tmp);

			for (int i = mid; i < now.num; i++) {
				now.key[i] = 0;
				now.son[i] = 0;
			}
			now.num = mid;
			now.next = _tmp;
			putnode(now, _now);

			if (fa == 0) {
				node tmd;
				tmd.isLeaf = 0;
				tmd.add(now.top(), _now);
				tmd.add(tmp.top(), _tmp);

				dword _tmd = newnode(tmd);
				setroot(_tmd);
				fa = &tmd;
			}
			else {
				fa->add(now.top(), _now);
				fa->add(tmp.top(), _tmp);
			}
		}
		else {
			putnode(now, _now);
		}
	}
	
	void print(dword _now, int t) {
		node now;
		getnode(now, _now);
		now.print(t);
		if (now.isLeaf == 0)
			for (int i = 0; i < now.num; i++) if (now.son[i]) print(now.son[i], t + 1);
	}
	void print() {
		print(getroot(), 0);
	}
};