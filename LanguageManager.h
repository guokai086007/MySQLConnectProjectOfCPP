#pragma once
#include<iostream>
#include<mysql.h>
#include<vector>
#include<string>
using namespace std;
class Language {
	public:
		Language() {}
		Language(string name) {
			this->name = name;
		}
		Language(int language_id,string name,string last_update) {
			this->language_id = language_id;
			this->name = name;
			this->last_update = last_update;
		}
		int getLanguage_id() const{
			return this->language_id;
		}
		void setLanguage_id(int language_id) {
			this->language_id = language_id;
		}
		string getName() const{
			return this->name;
		}
		void setName(string name) {
			this->name = name;
		}
		string getLast_update() const{
			return this->last_update;
		}
		void setLast_update(string last_update) {
			this->last_update = last_update;
		}
	private:
		int language_id;
		string name;
		string last_update;
};
class LanguageManager
{
	public:
		static LanguageManager* getInstance() {//µ¥ÀýÄ£Ê½
			static LanguageManager languageManager;
			return &languageManager;
		}
	public:
		LanguageManager();
		~LanguageManager();
		bool insert_language(Language language);
		bool update_language(Language language);
		bool delete_languageById(int language_id);
		Language query_languageById(int language_id);
		Language query_languageByName(string name);
		vector<Language> query_languageAll();
	private:
		MYSQL* mysqlConnect;
		/*const char* host = "127.0.0.1";
		const char* user = "root";
		const char* password = "086007gk";
		const char* databaseName = "sakila";
		const int port = 3306;*/
};

