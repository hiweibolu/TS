class user {
public:
	char username[27], name[27], password[37], mailAddr[37];
	long privilege;
	word first, orderNum;

	user() {
		privilege = -1;
		username[0] = 0;
		name[0] = 0;
		password[0] = 0;
		mailAddr[0] = 0;
		first = orderNum= 0;
	}

	word getHash() {
		return myHash(username);
	}
};