#include "UserManager.h"
#include "Properties.h"

UserManager::UserManager()
{
	Properties* properties = Properties::getInstance();
	properties->setConfigFilePath("userManager.properties");
	properties->load();

	this->mysqlConnect = mysql_init(NULL);
	//�����ַ�����
	mysql_options(this->mysqlConnect, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(this->mysqlConnect, properties->getValue("host").c_str(), properties->getValue("user").c_str(), properties->getValue("password").c_str(), properties->getValue("databaseName").c_str(), atoi(properties->getValue("port").c_str()), NULL, 0)) {
		fprintf(stderr, "mysql_use_result() failed. Error: %s\n", mysql_error(mysqlConnect));
		exit(1);
	}
}

UserManager::~UserManager()
{
	mysql_close(this->mysqlConnect);
}

bool UserManager::insert_user(User user)
{
	// ��users���в����������ݵ�sql���
	string sql = "INSERT INTO users(username,password,email) VALUES('" + user.getUsername() + "','"
		+ user.getPassword() + "','"
		+ user.getEmail() +"')";
	// ִ��SQL
	if (mysql_query(this->mysqlConnect, sql.c_str())) {
		cerr << "INSERT * error: " << mysql_error(mysqlConnect) << endl;
		return false;
	}return true;
}

bool UserManager::insert_users(vector<User> users)
{
	ostringstream os;
	string sql;
	os << "INSERT INTO users(username,password,email) VALUES ";
	int n = users.size();
	int cnt = 0;
	for (int i = 0; i < n; i++) {
		os << "('" <<users[i].getUsername()<<"','"<<users[i].getPassword()<<"','"<<users[i].getEmail()<<"')";
		if (((i+1) % 500)!=0 && i!=n-1) os << ",";
		else {
			// ִ��SQL
			sql = os.str();
			//cout << sql << endl;
			if (mysql_query(this->mysqlConnect, sql.c_str())) {
				cerr << "INSERT * error: " << mysql_error(mysqlConnect) << endl;
				return false;
			}
			mysql_commit(this->mysqlConnect);//�ύ����
			os.str("");//���֮ǰ��ostringstream������os�е��ַ������ݣ��Ӷ���ȥ��װ��һ��insert�������!
			os << "INSERT INTO users(username,password,email) VALUES ";
		}
	}
	return true;
	// ִ��SQL
	/*if (mysql_query(this->mysqlConnect, sql.c_str())) {
		cerr << "INSERT * error: " << mysql_error(mysqlConnect) << endl;
		return false;
	}return true;*/
}

//bool UserManager::insert_users(vector<User> users)
//{
//	/*
//		Ԥ�������(Prepared Statements)����ִ���ظ��� SQL ������߰�ȫ�ԣ����� SQL ע�롣
//		��Ҫ������ mysql_stmt_init��mysql_stmt_prepare��mysql_stmt_execute �ȡ�
//	*/
//	MYSQL_STMT* stmt;
//	stmt = mysql_stmt_init(this->mysqlConnect);//��ʼ��Ԥ�������,mysql_stmt_init�����᷵��һ��ָ��mysqlԤ�����������ָ��
//	
//	//ʹ�� mysql_stmt_prepare ��׼��һ�� SQL ��䡣�����������룬����ʹ�ò�������ѯ��
//	const char* sql = "INSERT INTO users (username, password,email) VALUES (?, ?, ?)";//?������ռλ�����ں���Ĳ�����
//	if (mysql_stmt_prepare(stmt, sql, strlen(sql))) {//mysql_stmt_prepare�����ɹ����� 0��ʧ�ܷ��ط� 0 ֵ��
//		fprintf(stderr, "Prepare failed: %s\n", mysql_stmt_error(stmt));
//		return false;
//	}
//	/*
//		�����ǰ󶨲�������������������ҪΪÿ��������һ�������������������͡�
//		�����������룬���Զ�ε��� mysql_stmt_bind_param �� mysql_stmt_execute��
//		MYSQL_BIND�ṹ��: MYSQL_BIND ���ڶ�����������������
//		(�������ڰ�����ĳ�ʼ��Ԥ�������stmt��������Ӧsql����еĲ�����
//		 �����sql�����á�?������ռλ�����ھ���MYSQL_BIND�����е�ÿһ��MYSQL_BIND�ṹ�������а󶨶�Ӧ)
//		���Ա������
//			buffer: ָ��������ݵ�ָ�롣
//			buffer_type: �������������ͣ��� MYSQL_TYPE_LONG��MYSQL_TYPE_STRING��
//			is_null: ָ���ʶ�����Ƿ�Ϊ NULL ��ָ�롣
//			length:  ָ�򱣴����ݳ��ȵ�ָ�롣
//	*/
//	MYSQL_BIND bind[3];
//	memset(bind, 0, sizeof(bind));
//
//	/*char username[30]{};
//	char password[30]{};
//	char email[30]{};*/
//	
//	bind[0].buffer_type = MYSQL_TYPE_VAR_STRING;
//	bind[0].buffer = (char*)"test1";
//	bind[0].is_null = 0; // �ǿ�ֵ
//	bind[0].length = 0; // ��ʹ�ó����ֶ�
//	bind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
//	bind[1].buffer = (char*)"123456";
//	//bind[1].buffer_length = strlen(password);
//	bind[1].is_null = 0; // �ǿ�ֵ
//	bind[1].length = 0; // ��ʹ�ó����ֶ�
//	bind[2].buffer_type = MYSQL_TYPE_VAR_STRING;
//	bind[2].buffer = (char*)"test@qq.com";
//	//bind[2].buffer_length = strlen(email);
//	bind[2].is_null = 0; // �ǿ�ֵ
//	bind[2].length = 0; // ��ʹ�ó����ֶ�
//
//	if (mysql_stmt_bind_param(stmt, bind)) {
//		fprintf(stderr, "Bind failed: %s\n", mysql_stmt_error(stmt));
//		return false;
//	}
//	if (mysql_stmt_execute(stmt)) {//�ɹ����� 0��ʧ�ܷ��ط� 0 ֵ��
//		fprintf(stderr, "Execute failed: %s\n", mysql_stmt_error(stmt));
//		return false;
//	}
//
//	/*
//		ִ����䲢�������������������룩�����������룬
//		������������ mysql_stmt_execute ������������ݡ�ÿ�ε��ö���ִ��һ�β��������
//	*/
//	//int n = users.size();
//	//for (int i = 0; i < n; i++) {
//	//	strcpy_s(username,users[i].getUsername().c_str());
//	//	strcpy_s(password,users[i].getPassword().c_str());
//	//	strcpy_s(email, users[i].getEmail().c_str());
//	//	
//	//	memset(bind, 0, sizeof(bind));
//	//	bind[0].buffer_type = MYSQL_TYPE_STRING;
//	//	bind[0].buffer = username;
//	//	bind[0].is_null = 0; // �ǿ�ֵ
//	//	bind[0].length = 0; // ��ʹ�ó����ֶ�
//	//	bind[1].buffer_type = MYSQL_TYPE_STRING;
//	//	bind[1].buffer = password;
//	//	//bind[1].buffer_length = strlen(password);
//	//	bind[1].is_null = 0; // �ǿ�ֵ
//	//	bind[1].length = 0; // ��ʹ�ó����ֶ�
//	//	bind[2].buffer_type = MYSQL_TYPE_STRING;
//	//	bind[2].buffer = email;		
//	//	//bind[2].buffer_length = strlen(email);
//	//	bind[2].is_null = 0; // �ǿ�ֵ
//	//	bind[2].length = 0; // ��ʹ�ó����ֶ�
//
//	//	//��bind�����е�ÿһ��MYSQL_BIND�ṹ������bufferָ����ָ���ֵ����sql�еĲ������а�ʽ�Ķ�Ӧ��
//	//	if (mysql_stmt_bind_param(stmt, bind)) {
//	//		fprintf(stderr, "Bind failed: %s\n", mysql_stmt_error(stmt));
//	//		return false;
//	//	}
//
//	//	if (mysql_stmt_execute(stmt)) {//�ɹ����� 0��ʧ�ܷ��ط� 0 ֵ��
//	//		fprintf(stderr, "Execute failed: %s\n", mysql_stmt_error(stmt));
//	//		return false;
//	//	}
//	//}
//	mysql_stmt_close(stmt);
//	return true;
//}

bool UserManager::update_user(User user)
{
	// ��users���в����������ݵ�sql���
	string sql = "UPDATE users SET username = '" + user.getUsername() + "',"
		+"password = '" + user.getPassword() + "',"
		+"email = '" + user.getEmail() + "'"
		+"WHERE id = " + to_string(user.getId());
	// ִ��SQL
	if (mysql_query(this->mysqlConnect, sql.c_str())) {
		cerr << "UPDATE * error: " << mysql_error(mysqlConnect) << endl;
		return false;
	}return true;
}

bool UserManager::delete_userById(int user_id)
{
	// ��users���в����������ݵ�sql���
	string sql = "DELETE FROM users WHERE id = " + to_string(user_id);
	// ִ��SQL
	if (mysql_query(this->mysqlConnect, sql.c_str())) {
		cerr << "DELETE * error: " << mysql_error(mysqlConnect) << endl;
		return false;
	}return true;
}

User UserManager::query_userById(int user_id)
{
	User user = User();
	string sql = "SELECT * FROM users WHERE id = '" + to_string(user_id) + "'";
	MYSQL_RES* res;
	MYSQL_ROW row;
	// ִ��SQL��ѯ
	if (mysql_query(mysqlConnect, sql.c_str())) {
		cerr << "SELECT * error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	res = mysql_store_result(mysqlConnect);
	row = mysql_fetch_row(res);
	if (row) {
		user.setId(atoi(row[0]));
		user.setUsername(row[1]);
		user.setPassword(row[2]);
		user.setEmail(row[3]);
		user.setCreatetime(row[4]);
	}
	else return {};
	return user;
}

User UserManager::query_userByName(string name)
{
	User user = User();
	string sql = "SELECT * FROM users WHERE username = '" + name + "'";
	MYSQL_RES* res;
	MYSQL_ROW row;
	// ִ��SQL��ѯ
	if (mysql_query(mysqlConnect, sql.c_str())) {
		cerr << "SELECT * error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	res = mysql_store_result(mysqlConnect);
	row = mysql_fetch_row(res);
	if (row) {
		user.setId(atoi(row[0]));
		user.setUsername(row[1]);
		user.setPassword(row[2]);
		user.setEmail(row[3]);
		user.setCreatetime(row[4]);
	}
	else return {};
	return user;
}

vector<User> UserManager::query_userAll()
{
	vector<User> userList;
	MYSQL_RES* res;
	MYSQL_ROW row;
	// ִ��SQL��ѯ
	if (mysql_query(mysqlConnect, "SELECT * FROM users")) {
		cerr << "SELECT * error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	res = mysql_store_result(mysqlConnect);
	if (res == NULL) {
		cerr << "mysql_store_result() failed. Error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	// �����ѯ���
	//int num_fields = mysql_num_fields(res);
	while ((row = mysql_fetch_row(res))) {
		User user = User();
		user.setId(atoi(row[0]));
		user.setUsername(row[1]);
		user.setPassword(row[2]);
		user.setEmail(row[3]);
		user.setCreatetime(row[4]);
		userList.push_back(user);
	}
	return userList;
}

//ģ����ѯ����������������û���Ϣ����ģ����ѯ
vector<User> UserManager::query_userByLike(User user)
{
	vector<User> userList;
	bool flag = false;

	ostringstream oss;//�ַ����������������sql�����ַ�����ƴ������ǳ�������
	oss << "SELECT * FROM users WHERE";
	if (user.getUsername().size() > 0) {
		oss << " username like '%" << user.getUsername() << "%'";
		flag = true;
	}
	if (user.getEmail().length() > 0) {
		if (flag) oss << " AND";
		oss << " email like '%" << user.getEmail() << "%'";
		flag = true;
	}
	if (user.getPassword().size() > 0) {
		if (flag) oss << " AND";
		oss << " password like '%" << user.getPassword() << "%'";
		flag = true;
	}
	if (user.getCreatetime().length() > 0) {
		if (flag) oss << " AND";
		oss << " createtime like '%" << user.getCreatetime() << "%'";
		flag = true;
	}

	if (!flag) {
		oss.str("");
		oss << "SELECT * FROM users";
	}
	string sql = oss.str();

	MYSQL_RES* res;
	MYSQL_ROW row;
	// ִ��SQL��ѯ
	if (mysql_query(mysqlConnect, sql.c_str())) {
		cerr << "SELECT * error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	res = mysql_store_result(mysqlConnect);
	if (res == NULL) {
		cerr << "mysql_store_result() failed. Error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	// �����ѯ���
	while ((row = mysql_fetch_row(res))) {
		User user = User();
		user.setId(atoi(row[0]));
		user.setUsername(row[1]);
		user.setPassword(row[2]);
		user.setEmail(row[3]);
		user.setCreatetime(row[4]);
		userList.push_back(user);
	}
	return userList;
}









