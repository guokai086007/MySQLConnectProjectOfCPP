#include "UserManager.h"
#include "Properties.h"

UserManager::UserManager()
{
	Properties* properties = Properties::getInstance();
	properties->setConfigFilePath("userManager.properties");
	properties->load();

	this->mysqlConnect = mysql_init(NULL);
	//设置字符编码
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
	// 向users表中插入语言数据的sql语句
	string sql = "INSERT INTO users(username,password,email) VALUES('" + user.getUsername() + "','"
		+ user.getPassword() + "','"
		+ user.getEmail() +"')";
	// 执行SQL
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
			// 执行SQL
			sql = os.str();
			//cout << sql << endl;
			if (mysql_query(this->mysqlConnect, sql.c_str())) {
				cerr << "INSERT * error: " << mysql_error(mysqlConnect) << endl;
				return false;
			}
			mysql_commit(this->mysqlConnect);//提交事务
			os.str("");//清空之前的ostringstream流对象os中的字符串内容，从而再去组装下一句insert插入语句!
			os << "INSERT INTO users(username,password,email) VALUES ";
		}
	}
	return true;
	// 执行SQL
	/*if (mysql_query(this->mysqlConnect, sql.c_str())) {
		cerr << "INSERT * error: " << mysql_error(mysqlConnect) << endl;
		return false;
	}return true;*/
}

//bool UserManager::insert_users(vector<User> users)
//{
//	/*
//		预处理语句(Prepared Statements)用于执行重复的 SQL 语句或提高安全性，避免 SQL 注入。
//		主要函数有 mysql_stmt_init、mysql_stmt_prepare、mysql_stmt_execute 等。
//	*/
//	MYSQL_STMT* stmt;
//	stmt = mysql_stmt_init(this->mysqlConnect);//初始化预处理语句,mysql_stmt_init函数会返回一个指向mysql预处理语句对象的指针
//	
//	//使用 mysql_stmt_prepare 来准备一个 SQL 语句。对于批量插入，可以使用参数化查询。
//	const char* sql = "INSERT INTO users (username, password,email) VALUES (?, ?, ?)";//?符号是占位符用于后面的参数绑定
//	if (mysql_stmt_prepare(stmt, sql, strlen(sql))) {//mysql_stmt_prepare函数成功返回 0，失败返回非 0 值！
//		fprintf(stderr, "Prepare failed: %s\n", mysql_stmt_error(stmt));
//		return false;
//	}
//	/*
//		下面是绑定参数并设置批量数据需要为每个参数绑定一个变量，并设置其类型。
//		对于批量插入，可以多次调用 mysql_stmt_bind_param 和 mysql_stmt_execute。
//		MYSQL_BIND结构体: MYSQL_BIND 用于定义输入或输出参数。
//		(就是用于绑定上面的初始化预处理语句stmt对象所对应sql语句中的参数，
//		 上面的sql中是用‘?’进行占位，现在就用MYSQL_BIND数组中的每一个MYSQL_BIND结构体对象进行绑定对应)
//		其成员包括：
//			buffer: 指向参数数据的指针。
//			buffer_type: 参数的数据类型，如 MYSQL_TYPE_LONG、MYSQL_TYPE_STRING。
//			is_null: 指向标识参数是否为 NULL 的指针。
//			length:  指向保存数据长度的指针。
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
//	bind[0].is_null = 0; // 非空值
//	bind[0].length = 0; // 不使用长度字段
//	bind[1].buffer_type = MYSQL_TYPE_VAR_STRING;
//	bind[1].buffer = (char*)"123456";
//	//bind[1].buffer_length = strlen(password);
//	bind[1].is_null = 0; // 非空值
//	bind[1].length = 0; // 不使用长度字段
//	bind[2].buffer_type = MYSQL_TYPE_VAR_STRING;
//	bind[2].buffer = (char*)"test@qq.com";
//	//bind[2].buffer_length = strlen(email);
//	bind[2].is_null = 0; // 非空值
//	bind[2].length = 0; // 不使用长度字段
//
//	if (mysql_stmt_bind_param(stmt, bind)) {
//		fprintf(stderr, "Bind failed: %s\n", mysql_stmt_error(stmt));
//		return false;
//	}
//	if (mysql_stmt_execute(stmt)) {//成功返回 0，失败返回非 0 值。
//		fprintf(stderr, "Execute failed: %s\n", mysql_stmt_error(stmt));
//		return false;
//	}
//
//	/*
//		执行语句并处理结果（对于批量插入）对于批量插入，
//		可以连续调用 mysql_stmt_execute 来插入多行数据。每次调用都会执行一次插入操作。
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
//	//	bind[0].is_null = 0; // 非空值
//	//	bind[0].length = 0; // 不使用长度字段
//	//	bind[1].buffer_type = MYSQL_TYPE_STRING;
//	//	bind[1].buffer = password;
//	//	//bind[1].buffer_length = strlen(password);
//	//	bind[1].is_null = 0; // 非空值
//	//	bind[1].length = 0; // 不使用长度字段
//	//	bind[2].buffer_type = MYSQL_TYPE_STRING;
//	//	bind[2].buffer = email;		
//	//	//bind[2].buffer_length = strlen(email);
//	//	bind[2].is_null = 0; // 非空值
//	//	bind[2].length = 0; // 不使用长度字段
//
//	//	//用bind数组中的每一个MYSQL_BIND结构体对象的buffer指针所指向的值，对sql中的参数进行绑定式的对应！
//	//	if (mysql_stmt_bind_param(stmt, bind)) {
//	//		fprintf(stderr, "Bind failed: %s\n", mysql_stmt_error(stmt));
//	//		return false;
//	//	}
//
//	//	if (mysql_stmt_execute(stmt)) {//成功返回 0，失败返回非 0 值。
//	//		fprintf(stderr, "Execute failed: %s\n", mysql_stmt_error(stmt));
//	//		return false;
//	//	}
//	//}
//	mysql_stmt_close(stmt);
//	return true;
//}

bool UserManager::update_user(User user)
{
	// 向users表中插入语言数据的sql语句
	string sql = "UPDATE users SET username = '" + user.getUsername() + "',"
		+"password = '" + user.getPassword() + "',"
		+"email = '" + user.getEmail() + "'"
		+"WHERE id = " + to_string(user.getId());
	// 执行SQL
	if (mysql_query(this->mysqlConnect, sql.c_str())) {
		cerr << "UPDATE * error: " << mysql_error(mysqlConnect) << endl;
		return false;
	}return true;
}

bool UserManager::delete_userById(int user_id)
{
	// 向users表中插入语言数据的sql语句
	string sql = "DELETE FROM users WHERE id = " + to_string(user_id);
	// 执行SQL
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
	// 执行SQL查询
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
	// 执行SQL查询
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
	// 执行SQL查询
	if (mysql_query(mysqlConnect, "SELECT * FROM users")) {
		cerr << "SELECT * error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	res = mysql_store_result(mysqlConnect);
	if (res == NULL) {
		cerr << "mysql_store_result() failed. Error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	// 处理查询结果
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

//模糊查询方法：根据输入的用户信息进行模糊查询
vector<User> UserManager::query_userByLike(User user)
{
	vector<User> userList;
	bool flag = false;

	ostringstream oss;//字符串输出流对象，用于sql语句的字符串的拼接任务非常便利！
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
	// 执行SQL查询
	if (mysql_query(mysqlConnect, sql.c_str())) {
		cerr << "SELECT * error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	res = mysql_store_result(mysqlConnect);
	if (res == NULL) {
		cerr << "mysql_store_result() failed. Error: " << mysql_error(mysqlConnect) << endl;
		return {};
	}
	// 处理查询结果
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









