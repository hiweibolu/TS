class user {
public:
	char username[21], name[21], password[31], mailAddr[31];
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