#include "LanguageManager.h"
#include "Properties.h"

LanguageManager::LanguageManager()
{
	Properties *properties = Properties::getInstance();
	properties->setConfigFilePath("languageManager.properties");
	properties->load();

	this->mysqlConnect = mysql_init(NULL);
	//�����ַ�����
	mysql_options(this->mysqlConnect, MYSQL_SET_CHARSET_NAME, "GBK");
	if (!mysql_real_connect(this->mysqlConnect, properties->getValue("host").c_str(), properties->getValue("user").c_str(), properties->getValue("password").c_str(), properties->getValue("databaseName").c_str(), atoi(properties->getValue("port").c_str()), NULL, 0)) {
		fprintf(stderr, "mysql_use_result() failed. Error: %s\n", mysql_error(mysqlConnect));
		exit(1);
	}
}

LanguageManager::~LanguageManager()
{
	mysql_close(this->mysqlConnect);
}

bool LanguageManager::insert_language(Language language)
{
	// ��language���в����������ݵ�sql���
	string sql = "INSERT INTO language(name) VALUES('" + language.getName() + "')";
	// ִ��SQL
	if (mysql_query(this->mysqlConnect, sql.c_str())) {
		cerr << "INSERT * error: " << mysql_error(mysqlConnect) << endl;
		return false;
	}return true;
}

bool LanguageManager::update_language(Language language)
{
	// ��language���в����������ݵ�sql���
	string sql = "UPDATE language SET name = '" + language.getName() + "'WHERE language_id = "+ to_string(language.getLanguage_id());
	// ִ��SQL
	if (mysql_query(this->mysqlConnect, sql.c_str())) {
		cerr << "UPDATE * error: " << mysql_error(mysqlConnect) << endl;
		return false;
	}return true;
}

bool LanguageManager::delete_languageById(int language_id)
{
	// ��language���в����������ݵ�sql���
	string sql = "DELETE FROM language WHERE language_id = " + to_string(language_id);
	// ִ��SQL
	if (mysql_query(this->mysqlConnect, sql.c_str())) {
		cerr << "DELETE * error: " << mysql_error(mysqlConnect) << endl;
		return false;
	}return true;
}

Language LanguageManager::query_languageById(int language_id)
{
	Language language = Language();
	string sql = "SELECT * FROM language WHERE language_id = '" + to_string(language_id) + "'";
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
		language.setLanguage_id(atoi(row[0]));
		language.setName(row[1]);
		language.setLast_update(row[2]);
	}
	else return {};
	return language;
}

Language LanguageManager::query_languageByName(string name)
{
	Language language = Language();
	string sql = "SELECT * FROM language WHERE name = '"+ name + "'";
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
		language.setLanguage_id(atoi(row[0]));
		language.setName(row[1]);
		language.setLast_update(row[2]);
	}
	else return {};
	return language;
}

vector<Language> LanguageManager::query_languageAll()
{
	vector<Language> languageList;
	MYSQL_RES* res;
	MYSQL_ROW row;
	// ִ��SQL��ѯ
	if (mysql_query(mysqlConnect, "SELECT * FROM language")) {
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
		Language language =  Language();
		language.setLanguage_id(atoi(row[0]));
		language.setName(row[1]);
		language.setLast_update(row[2]);
		languageList.push_back(language);
	}
	return languageList;
}

