#include "LanguageManager.h"
#include "Properties.h"
#include "UserManager.h"
#include "UserJsonHandler.h"

#include "json.h" 
//const char* host = "127.0.0.1";
//const char* user = "root";
//const char* password = "086007gk";
//const char* databaseName = "sakila";
//const int port = 3306;

//#include <filesystem>
//#include <fstream>
//#include <iomanip>
//#include <sstream>
namespace fs = std::filesystem;

void fileStreamTest(fs::path p);
void test2();
void test3();
void test4();
void test1();
void userManagerTest();
void readStrJson();
void readStrProJson();
void readFileJson();
void writeFileJson();
void insert_usersTest();
int main()
{
	//test2();

	//test3();

	//test4();

	//test1(); 
	
	//userManagerTest();

	//readStrJson();

	//readStrProJson();

	//readFileJson();

	//writeFileJson();

	//insert_usersTest();

	User user = User();
	//cout << user.getUsername().size() <<endl;
	//user.setUsername("ka");
	//user.setEmail("659@qq");
	user.setCreatetime("2025-01-23");
	//cout << user.getUsername().size() << endl;
	UserManager* um = UserManager::getInstance();
	vector<User> users = um->query_userByLike(user);
	for (auto user : users) {
		cout << user.getId() << setw(12) << user.getUsername() << setw(15) << user.getPassword() << setw(20) << user.getEmail() << setw(25) << user.getCreatetime() << endl;
	}
	return 0;
}

//C++文件操作流的演示函数
void fileStreamTest(fs::path p) {

	//获取当前时间 并存入currentTime数组
	time_t t = time(0);
	struct tm nowTime;
	localtime_s(&nowTime, &t);
	char currentTime[80];
	strftime(currentTime, sizeof(currentTime), "%Y-%m-%d %H:%M:%S", &nowTime);
	
	bool bo = fs::exists(p);
	if (!bo) {
		//std::ofstream流是用于文件写入的，但是会先检查传入构造函数的路径中有没有文件，没有会自动创建
		ofstream outfile = ofstream(p);//如果在path类的p对象所表示的路径下没有example.txt文件，就会在构造outfile对象时先创建该文件
		//ofstream类在操作文件时：如果文件已经存在那么会将原有的文件覆盖！！！！
		if (!outfile) {
			std::cerr << "无法打开文件！" << std::endl;
		}
		else {
			//使用刚刚创建的ofstream类对象outfile以及插入操作符"<<"来将数据写入到文件example.txt中
			outfile << currentTime << ": 写入测试！！" << endl;
			outfile.close(); // 关闭文件
		}
	}
	else {
		/*
		* 1、由于ofstream类创建对象的时候会覆盖已有文件，所以一般只用ofstream类来创建文件
		* 2、所以如果要往已经存在的文件的后面写入数据的话，就要使用fstream类！
		* 3、std::fstream用于读写，可以先打开文件，然后在向文件中写入数据
		*/ 
		fstream f;
		f.open(p, ios::out | ios::app);//ios::out表示写操作，ios::app表示是添加的那种写操作而不是覆盖原有的写操作！
		f << currentTime << ": 写入测试！！" << endl;
		f.close();
	}

	//下面的代码用于读取文件的内容
	ifstream infile(p); //首先要打开文件
	// 检查文件是否成功打开
	if (!infile) {
		cerr << "无法打开文件！" << endl;
	}
	string line;
	// 使用getline函数，从而能够按行读取文件内容
	while (getline(infile, line)) {
		// 输出每一行的内容
		cout << line << endl;
	}
	infile.close(); // 关闭文件
}
void test2() {
	/*fs::path p1 = "C:/Users/";
	fs::path p2 = "John/Documents/";
	fs::path p3 = p1 / p2;
	cout << p3 << std::endl;*/
	fs::path p = "F:/gk/example.txt";
	//fs::path parent = p.parent_path();
	//cout << parent << endl;
	//for (auto& entry : fs::directory_iterator(p)) {
	//	//if (fs::is_regular_file(entry))
	//	if (fs::is_directory(entry))
	//		cout << entry.path() << endl;
	//}
	bool bo = fs::exists(p);
	if (!bo) {
		cout << "no!\n";
	}
	else {
		cout << "yes!\n";
		/*bo = fs::create_directory(p);
		if (bo) {
			cout << "Directory created successfully." << endl;
		}
		else {
			cout << "Failed to create directory." << endl;
		}*/
		fileStreamTest(p);
	}

	/*fs::path currentPath = fs::current_path();
	cout << "Current working directory is: " << currentPath << endl;*/

	/*fs::path filename = p.stem();
	cout << filename << endl;

	fs::path extension = p.extension();
	cout << extension << endl;*/

	/*for (auto& entry : fs::directory_iterator(p)) {
		std::cout << entry.path() << std::endl;
	}*/
}
void test3(){
	/*string s = "   abc_ cbdsa     ";
	cout << s.length() << endl;
	s.erase(0,s.find_first_not_of(" "));
	cout << s.length() << endl;
	s.erase(s.find_last_not_of(" ") + 1);
	cout << s.length() << endl;
	cout << s;*/
	vector<string> res;
	string str = "host = 127.0.0.1";
	stringstream input(str);
	string temp;
	char partten = '=';
	while (getline(input, temp, partten))
	{
		res.push_back(temp);
	}
	
	for (auto x : res) {
		cout << x << " " << x.length() <<endl;
	}
}
void test4()
{
	string configPaht = "languageManager.properties";
	Properties properties = Properties();
	properties.setConfigFilePath(configPaht);
	//判断配置文件是否存在
	if (properties.is_exists()) {
		//读取配置文件写入props，keys
		properties.load();
		//读取键值，如果不存在就返回默认值defaultValue
		string value = properties.getValue("databaseName", "defaultValue");
		cout << value << endl;
		//设置键值并写入文件
		//properties.setValue("host", "localhost");
		map<string, string>* map = properties.getProps();
		for (auto p : *(map)) {
			cout << p.first << "->" << p.second << endl;
		}
	}
	
}
void test1() {
	//Language language = Language("Chinese");
	//LanguageManager::getInstance()->insert_language(language);
	LanguageManager* m = LanguageManager::getInstance();
	Language language1 = Language("Portuguese");
	m->insert_language(language1);
	vector<Language> list = m->query_languageAll();
	for (auto language : list) {
		cout << language.getLanguage_id() << " " << language.getName() << "\t" << language.getLast_update() << "\n";
	}
	cout << "-------------------------------------------------------------\n";
	Language language = m->query_languageByName("Mandarin");
	cout << language.getLanguage_id() << " " << language.getName() << "\t" << language.getLast_update() << "\n";
	cout << "-------------------------------------------------------------\n";
	language = m->query_languageById(10);
	cout << language.getLanguage_id() << " " << language.getName() << "\t" << language.getLast_update() << "\n";
	cout << "-------------------------------------------------------------\n";
	/*language.setName("Arabic");
	m->update_language(language);*/
	//m->delete_languageById(8);
}
void userManagerTest()
{
	//cout << setiosflags(ios::fixed) << setprecision(6) << setiosflags(ios::right);
	UserManager* um = UserManager::getInstance();

	/*User user = User("李四", "910JQKA", "1076272@qq.com");
	um->insert_user(user);*/

	vector<User> list = um->query_userAll();
	for (auto user : list) {
		cout << user.getId() << setw(12) << user.getUsername() << setw(15) << user.getPassword() << setw(20) << user.getEmail() << setw(25) << user.getCreatetime() << endl;
	}
	cout << "-------------------------------------------------------------\n";

	/*User user = User("张三", "123456", "1423572@qq.com");
	um->insert_user(user);*/

	//um->delete_userById(4);

	/*User user = User("guokai", "ok123456", "1411261659@qq.com", 1, "");
	um->update_user(user);*/

	User user = um->query_userById(2);
	cout << user.getId() << setw(12) << user.getUsername() << setw(15) << user.getPassword() << setw(20) << user.getEmail() << setw(25) << user.getCreatetime() << endl;
	cout << "-------------------------------------------------------------\n";

	user = um->query_userByName("张三");
	cout << user.getId() << setw(12) << user.getUsername() << setw(15) << user.getPassword() << setw(20) << user.getEmail() << setw(25) << user.getCreatetime() << endl;
	cout << "-------------------------------------------------------------\n";
}

//通过JsonCPP库的类来处理Json字符串
void readStrJson()
{
	//字符串  
	const char* str =
		"{\"name\":\"guokai\",\"age\":26,\"sex\":\"男\"}";
	
	Json::Reader reader;
	Json::Value root;

	//从字符串中读取数据  
	if (reader.parse(str, root))
	{
		string name = root["name"].asString();
		int age = root["age"].asInt();
		string sex = root["sex"].asString();
		cout << name + " " << age << " " << sex << endl;
	}
}
//通过JsonCPP库的类来处理较为复杂的带有数组形式的Json字符串
void readStrProJson()
{
	string strValue = "{\"name\":\"shuiyixin\",\"major\":[{\"AI\":\"MachineLearning\"},{\"AI\":\"DeepLearning\"},{\"AI\":\"ComputerVision\"}]}";
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(strValue, value))
	{
		string out = value["name"].asString();
		cout << out << endl;
		const Json::Value arrayObj = value["major"];
		for (unsigned int i = 0; i < arrayObj.size(); i++)
		{
			out = arrayObj[i]["AI"].asString();
			cout << out << endl;
		}
	}
}

//通过JsonCPP库的类来读取Json文件
void readFileJson()
{
	Json::Reader reader;
	Json::Value root;

	//从文件中读取，保证当前文件夹中有demo.json文件  
	ifstream in("demo.json", ios::binary);//其中的ios::binary是指以二进制方式打开文件

	if (!in.is_open())
	{
		cout << "Error opening file\n";
		return;
	}

	if (reader.parse(in, root))
	{
		//读取根节点信息  
		string name = root["name"].asString();
		int age = root["age"].asInt();
		string sex = root["sex"].asString();

		cout << "My name is " << name << endl;
		cout << "I'm " << age << " years old" << endl;
		cout << "I'm a " << sex << endl;

		//读取子节点信息  
		string friend_name = root["friends"]["friend_name"].asString();
		int friend_age = root["friends"]["friend_age"].asInt();
		string friend_sex = root["friends"]["friend_sex"].asString();

		cout << "My friend's name is " << friend_name << endl;
		cout << "My friend's sex is " << friend_sex << endl;
		cout << "My friend is " << friend_age << " years old" << endl;

		//读取数组信息  
		cout << "Here's my hobby:" << endl;
		for (unsigned int i = 0; i < root["hobby"].size(); i++)
		{
			string ach = root["hobby"][i].asString();
			cout << ach << '\t';
		}
		cout << endl;

		cout << "Reading Complete!" << endl;
	}
	else
	{
		cout << "parse error\n" << endl;
	}

	in.close();
}

//通过JsonCPP库的类来向Json文件中写入Json数据
void writeFileJson()
{
	//根节点  
	Json::Value root;

	//根节点属性  
	root["name"] = Json::Value("shuiyixin");
	root["age"] = Json::Value(21);
	root["sex"] = Json::Value("man");

	//子节点  
	Json::Value friends;

	//子节点属性  
	friends["friend_name"] = Json::Value("ZhaoWuxian");
	friends["friend_age"] = Json::Value(21);
	friends["friend_sex"] = Json::Value("man");

	//子节点挂到根节点上  
	root["friends"] = Json::Value(friends);

	//数组形式  
	root["hobby"].append("sing");
	root["hobby"].append("run");
	root["hobby"].append("Tai Chi");

	//直接输出  
	//cout << "FastWriter:" << endl;
	//Json::FastWriter fw;
	//cout << fw.write(root) << endl << endl;

	//缩进输出  
	cout << "StyledWriter:" << endl;
	//StyledWriter类的write方法是将上面的根节点root所具有的所有Json内容整合成一个Json字符串返回回来！
	Json::StyledWriter sw;
	string jsonString = sw.write(root);
	cout << jsonString << endl;

	//输出到文件  
	/*ofstream os;
	os.open("demo.json", ios::out | ios::app);
	if (!os.is_open())
		cout << "error：can not find or create the file which named \" demo.json\"." << endl;
	os << jsonString;
	os.close();*/
}

void insert_usersTest()
{
	//vector<User> users = UserJsonHandler::readUserJsonFile("users.json");
	vector<User> users;
	User user;
	for (int i = 0; i < 1253; i++) {
		user = User();
		user.setUsername("insertTest"+to_string(i));
		user.setPassword("passwordTest" + to_string(i));
		user.setEmail("emailTest" + to_string(i)+"@qq.com");
		users.push_back(user);
	}

	UserManager* um = UserManager::getInstance();
	um->insert_users(users);
}

void testUserJsonHandler() {
	vector<User> users = UserJsonHandler::readUserJsonFile("users.json");
	for (auto user : users) {
		cout << setw(12) << user.getUsername() << setw(15) << user.getPassword() << setw(20) << user.getEmail() << setw(25) << endl;
	}

	cout << "-----------------------------------------------------------\n";

	string JsonString = "{\"users\": [{\"username\": \"JsonTest1\",\"password\": \"Json12345\",\"email\" :  \"Json123@qq.com\"},{\"username\": \"JsonTest2\",\"password\": \"Json897AQ\",\"email\": \"Json123@139.com\"} ]}";
	users = UserJsonHandler::readUserJsonString(JsonString);
	for (auto user : users) {
		cout << setw(12) << user.getUsername() << setw(15) << user.getPassword() << setw(20) << user.getEmail() << endl;
	}

	//测试写用户Json文件
	//获取当前时间 并存入currentTime数组
	time_t t = time(0);
	struct tm nowTime;
	localtime_s(&nowTime, &t);
	char currentTime[80];
	strftime(currentTime, sizeof(currentTime), "%Y_%m_%d_%H_%M_%S", &nowTime);

	string path = "usersJson_" + string(currentTime) + ".json";
	for (int i = 0; i < users.size(); i++) {
		users[i].setId(i + 1);
		users[i].setCreatetime(string(currentTime));
	}
	/*bool bo = UserJsonHandler::writeUsersToJsonFile(users, path);
	if (bo)
		cout << "Success!!\n";
	else
		cout << "Failed!!\n";*/

	bool bo = UserJsonHandler::writeUserToJsonFile(users[1], path);
	if (bo)
		cout << "Success!!\n";
	else
		cout << "Failed!!\n";
}
/*
MYSQL* mysqlConnect = mysql_init(NULL);
	MYSQL_RES* res;
	MYSQL_ROW row;
	//设置字符编码
	mysql_options(mysqlConnect, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(mysqlConnect, host, user, password, databaseName, port, NULL, 0)) {
		fprintf(stderr, "mysql_use_result() failed. Error: %s\n", mysql_error(mysqlConnect));
		return -1;
	}
	// 执行SQL查询
	if (mysql_query(mysqlConnect, "SELECT * FROM language")) {
		cerr << "SELECT * error: " << mysql_error(mysqlConnect) << endl;
		mysql_close(mysqlConnect);
		exit(1);
	}
	else {
		res = mysql_store_result(mysqlConnect);
		if (res == NULL) {
			cerr << "mysql_store_result() failed. Error: " << mysql_error(mysqlConnect) << endl;
			mysql_close(mysqlConnect);
			exit(1);
		}
		// 处理查询结果
		int num_fields = mysql_num_fields(res);
		while ((row = mysql_fetch_row(res))) {
			for (int i = 0; i < num_fields; i++) {
				cout << (row[i] ? row[i] : "NULL") << " ";
			}
			cout << endl;
		}
		// 释放查询结果
		mysql_free_result(res);

		// 关闭数据库连接
		mysql_close(mysqlConnect);
	}

	insert into users(id,username,password,email) values(3,'test','1234','test@163.com');
*/