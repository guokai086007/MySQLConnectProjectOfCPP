#pragma once
#include<iostream>
#include<mysql.h>
#include<vector>
#include<string>
using namespace std;

class User {
	private:
		int id;
		string username;
		string password;
		string email;
		string createtime;
	public:
		User() {}
		User(string username, string password, string email) {
			this->username = username;
			this->password = password;
			this->email = email;
		}
		User(string username, string password, string email,int id,string createtime) {
			this->username = username;
			this->password = password;
			this->email = email;
			this->id = id;
			this->createtime = createtime;
		}
		void setId(int id) {
			this->id = id;
		}
		int getId() const {
			return this->id;
		}
		void setUsername(string username){
			this->username = username;
		}
		string getUsername() const {
			return this->username;
		}
		void setPassword(string password) {
			this->password = password;
		}
		string getPassword() const {
			return this->password;
		}
		void setEmail(string email) {
			this->email = email;
		}
		string getEmail() const {
			return this->email;
		}
		void setCreatetime(string createtime) {
			this->createtime = createtime;
		}
		string getCreatetime() const {
			return this->createtime;
		}
};

class UserManager
{
	private:
		MYSQL* mysqlConnect;
	public:
		static UserManager* getInstance() {//µ¥ÀýÄ£Ê½
			static UserManager userManager;
			return &userManager;
		}
	public:
		UserManager();
		~UserManager();
		bool insert_user(User user);
		bool insert_users(vector<User> users);
		bool update_user(User user);
		bool delete_userById(int user_id);
		User query_userById(int user_id);
		User query_userByName(string name);
		vector<User> query_userAll();
		vector<User> query_userByLike(User user);
};

