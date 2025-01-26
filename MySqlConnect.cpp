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

//C++�ļ�����������ʾ����
void fileStreamTest(fs::path p) {

	//��ȡ��ǰʱ�� ������currentTime����
	time_t t = time(0);
	struct tm nowTime;
	localtime_s(&nowTime, &t);
	char currentTime[80];
	strftime(currentTime, sizeof(currentTime), "%Y-%m-%d %H:%M:%S", &nowTime);
	
	bool bo = fs::exists(p);
	if (!bo) {
		//std::ofstream���������ļ�д��ģ����ǻ��ȼ�鴫�빹�캯����·������û���ļ���û�л��Զ�����
		ofstream outfile = ofstream(p);//�����path���p��������ʾ��·����û��example.txt�ļ����ͻ��ڹ���outfile����ʱ�ȴ������ļ�
		//ofstream���ڲ����ļ�ʱ������ļ��Ѿ�������ô�Ὣԭ�е��ļ����ǣ�������
		if (!outfile) {
			std::cerr << "�޷����ļ���" << std::endl;
		}
		else {
			//ʹ�øոմ�����ofstream�����outfile�Լ����������"<<"��������д�뵽�ļ�example.txt��
			outfile << currentTime << ": д����ԣ���" << endl;
			outfile.close(); // �ر��ļ�
		}
	}
	else {
		/*
		* 1������ofstream�ഴ�������ʱ��Ḳ�������ļ�������һ��ֻ��ofstream���������ļ�
		* 2���������Ҫ���Ѿ����ڵ��ļ��ĺ���д�����ݵĻ�����Ҫʹ��fstream�࣡
		* 3��std::fstream���ڶ�д�������ȴ��ļ���Ȼ�������ļ���д������
		*/ 
		fstream f;
		f.open(p, ios::out | ios::app);//ios::out��ʾд������ios::app��ʾ����ӵ�����д���������Ǹ���ԭ�е�д������
		f << currentTime << ": д����ԣ���" << endl;
		f.close();
	}

	//����Ĵ������ڶ�ȡ�ļ�������
	ifstream infile(p); //����Ҫ���ļ�
	// ����ļ��Ƿ�ɹ���
	if (!infile) {
		cerr << "�޷����ļ���" << endl;
	}
	string line;
	// ʹ��getline�������Ӷ��ܹ����ж�ȡ�ļ�����
	while (getline(infile, line)) {
		// ���ÿһ�е�����
		cout << line << endl;
	}
	infile.close(); // �ر��ļ�
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
	//�ж������ļ��Ƿ����
	if (properties.is_exists()) {
		//��ȡ�����ļ�д��props��keys
		properties.load();
		//��ȡ��ֵ����������ھͷ���Ĭ��ֵdefaultValue
		string value = properties.getValue("databaseName", "defaultValue");
		cout << value << endl;
		//���ü�ֵ��д���ļ�
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

	/*User user = User("����", "910JQKA", "1076272@qq.com");
	um->insert_user(user);*/

	vector<User> list = um->query_userAll();
	for (auto user : list) {
		cout << user.getId() << setw(12) << user.getUsername() << setw(15) << user.getPassword() << setw(20) << user.getEmail() << setw(25) << user.getCreatetime() << endl;
	}
	cout << "-------------------------------------------------------------\n";

	/*User user = User("����", "123456", "1423572@qq.com");
	um->insert_user(user);*/

	//um->delete_userById(4);

	/*User user = User("guokai", "ok123456", "1411261659@qq.com", 1, "");
	um->update_user(user);*/

	User user = um->query_userById(2);
	cout << user.getId() << setw(12) << user.getUsername() << setw(15) << user.getPassword() << setw(20) << user.getEmail() << setw(25) << user.getCreatetime() << endl;
	cout << "-------------------------------------------------------------\n";

	user = um->query_userByName("����");
	cout << user.getId() << setw(12) << user.getUsername() << setw(15) << user.getPassword() << setw(20) << user.getEmail() << setw(25) << user.getCreatetime() << endl;
	cout << "-------------------------------------------------------------\n";
}

//ͨ��JsonCPP�����������Json�ַ���
void readStrJson()
{
	//�ַ���  
	const char* str =
		"{\"name\":\"guokai\",\"age\":26,\"sex\":\"��\"}";
	
	Json::Reader reader;
	Json::Value root;

	//���ַ����ж�ȡ����  
	if (reader.parse(str, root))
	{
		string name = root["name"].asString();
		int age = root["age"].asInt();
		string sex = root["sex"].asString();
		cout << name + " " << age << " " << sex << endl;
	}
}
//ͨ��JsonCPP������������Ϊ���ӵĴ���������ʽ��Json�ַ���
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

//ͨ��JsonCPP���������ȡJson�ļ�
void readFileJson()
{
	Json::Reader reader;
	Json::Value root;

	//���ļ��ж�ȡ����֤��ǰ�ļ�������demo.json�ļ�  
	ifstream in("demo.json", ios::binary);//���е�ios::binary��ָ�Զ����Ʒ�ʽ���ļ�

	if (!in.is_open())
	{
		cout << "Error opening file\n";
		return;
	}

	if (reader.parse(in, root))
	{
		//��ȡ���ڵ���Ϣ  
		string name = root["name"].asString();
		int age = root["age"].asInt();
		string sex = root["sex"].asString();

		cout << "My name is " << name << endl;
		cout << "I'm " << age << " years old" << endl;
		cout << "I'm a " << sex << endl;

		//��ȡ�ӽڵ���Ϣ  
		string friend_name = root["friends"]["friend_name"].asString();
		int friend_age = root["friends"]["friend_age"].asInt();
		string friend_sex = root["friends"]["friend_sex"].asString();

		cout << "My friend's name is " << friend_name << endl;
		cout << "My friend's sex is " << friend_sex << endl;
		cout << "My friend is " << friend_age << " years old" << endl;

		//��ȡ������Ϣ  
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

//ͨ��JsonCPP���������Json�ļ���д��Json����
void writeFileJson()
{
	//���ڵ�  
	Json::Value root;

	//���ڵ�����  
	root["name"] = Json::Value("shuiyixin");
	root["age"] = Json::Value(21);
	root["sex"] = Json::Value("man");

	//�ӽڵ�  
	Json::Value friends;

	//�ӽڵ�����  
	friends["friend_name"] = Json::Value("ZhaoWuxian");
	friends["friend_age"] = Json::Value(21);
	friends["friend_sex"] = Json::Value("man");

	//�ӽڵ�ҵ����ڵ���  
	root["friends"] = Json::Value(friends);

	//������ʽ  
	root["hobby"].append("sing");
	root["hobby"].append("run");
	root["hobby"].append("Tai Chi");

	//ֱ�����  
	//cout << "FastWriter:" << endl;
	//Json::FastWriter fw;
	//cout << fw.write(root) << endl << endl;

	//�������  
	cout << "StyledWriter:" << endl;
	//StyledWriter���write�����ǽ�����ĸ��ڵ�root�����е�����Json�������ϳ�һ��Json�ַ������ػ�����
	Json::StyledWriter sw;
	string jsonString = sw.write(root);
	cout << jsonString << endl;

	//������ļ�  
	/*ofstream os;
	os.open("demo.json", ios::out | ios::app);
	if (!os.is_open())
		cout << "error��can not find or create the file which named \" demo.json\"." << endl;
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

	//����д�û�Json�ļ�
	//��ȡ��ǰʱ�� ������currentTime����
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
	//�����ַ�����
	mysql_options(mysqlConnect, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(mysqlConnect, host, user, password, databaseName, port, NULL, 0)) {
		fprintf(stderr, "mysql_use_result() failed. Error: %s\n", mysql_error(mysqlConnect));
		return -1;
	}
	// ִ��SQL��ѯ
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
		// �����ѯ���
		int num_fields = mysql_num_fields(res);
		while ((row = mysql_fetch_row(res))) {
			for (int i = 0; i < num_fields; i++) {
				cout << (row[i] ? row[i] : "NULL") << " ";
			}
			cout << endl;
		}
		// �ͷŲ�ѯ���
		mysql_free_result(res);

		// �ر����ݿ�����
		mysql_close(mysqlConnect);
	}

	insert into users(id,username,password,email) values(3,'test','1234','test@163.com');
*/