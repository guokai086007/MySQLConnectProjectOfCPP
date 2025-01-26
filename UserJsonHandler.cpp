#include "UserJsonHandler.h"

//ͨ��path·������ȡ�û�Json�ļ�������װ���û���̬���鷵��
vector<User> UserJsonHandler::readUserJsonFile(string path)
{
    Json::Reader reader;
    Json::Value root;
    vector<User> userList = {};

    //���ļ��ж�ȡ����֤��ǰ�ļ�������demo.json�ļ�  
    ifstream in(path, ios::binary);//���е�ios::binary��ָ�Զ����Ʒ�ʽ���ļ�
    if (!in.is_open())
    {
        cerr << "Error opening file in readUserJsonFile\n";
        return userList;
    }
    if (reader.parse(in, root)) {
        //��ȡ���ڵ���Ϣ  
        Json::Value usersJson = root["users"]; 
        //usersJson�Ǹ�Json��������ѭ�����������飬�Ѹ����������ÿһ��Json�û���Ϣ��װ���û�User����
        int n = usersJson.size();//��ȡ���Json����ĳ���
        User user;
        for (int i = 0; i < n; i++) {
            user = User();
            user.setUsername( usersJson[i]["username"].asString() );
            user.setPassword( usersJson[i]["password"].asString() );
            user.setEmail(usersJson[i]["email"].asString());
            userList.push_back(user);
        }
    }
    return userList;
}

//��ȡ�û�Json�ַ���������װ���û���̬���鷵��
vector<User> UserJsonHandler::readUserJsonString(string JsonString)
{
    Json::Reader reader;
    Json::Value root;
    vector<User> userList = {};

    if (reader.parse(JsonString, root)){
        //��ȡ���ڵ���Ϣ  
        Json::Value usersJson = root["users"];
        //usersJson�Ǹ�Json��������ѭ�����������飬�Ѹ����������ÿһ��Json�û���Ϣ��װ���û�User����
        int n = usersJson.size();//��ȡ���Json����ĳ���
        User user;
        for (int i = 0; i < n; i++) {
            user = User();
            user.setUsername(usersJson[i]["username"].asString());
            user.setPassword(usersJson[i]["password"].asString());
            user.setEmail(usersJson[i]["email"].asString());
            userList.push_back(user);
        }
    }
    return userList;
}
//����������·��path�Լ���̬����users����һ��json��¼�ļ�
bool UserJsonHandler::writeUsersToJsonFile(vector<User> users,string path)
{
    //���ڵ�  
    Json::Value root;
    //���ڵ��е��û�Json����
    Json::Value usersValue;

    int n = users.size();
    for (int i = 0; i < n; i++) {
        Json::Value user;
        user["id"] = users[i].getId();
        user["username"] = users[i].getUsername();
        user["password"] = users[i].getPassword();
        user["email"] = users[i].getEmail();
        user["createtime"] = users[i].getCreatetime();
        usersValue.append(user);
    }

    //���û�Json����ҵ�root��
    root["users"] = usersValue;

    //�������  
    //cout << "StyledWriter:" << endl;
    //StyledWriter���write�����ǽ�����ĸ��ڵ�root�����е�����Json�������ϳ�һ��Json�ַ������ػ�����
    Json::StyledWriter sw;
    string jsonString = sw.write(root);
    //cout << jsonString << endl;

    //��ȡ��ǰ·��
    filesystem::path currentPath = filesystem::current_path();
    //cout << currentPath << endl;
    path = currentPath.string() + "\\" + path;

    //������ļ�  
    ofstream os(path);
    if (!os.is_open()) {
        cerr << "error��can not find or create the file which named "+ path +"!" << endl;
        return false;
    }  
    os << jsonString;
    os.close();
    return true;
}
//����������·��path�Լ��û�����user����һ��json��¼�ļ�
bool UserJsonHandler::writeUserToJsonFile(User user, string path)
{
    //���ڵ�  
    Json::Value root;
    //���ڵ��е��û�Json����
    Json::Value userValue;
    userValue["id"] = user.getId();
    userValue["username"] = user.getUsername();
    userValue["password"] = user.getPassword();
    userValue["email"] = user.getEmail();
    userValue["createtime"] = user.getCreatetime();

    root["user"] = userValue;

    Json::StyledWriter sw;
    string jsonString = sw.write(root);
    //cout << jsonString << endl;

    //��ȡ��ǰ·��
    filesystem::path currentPath = filesystem::current_path();
    //cout << currentPath << endl;
    path = currentPath.string() + "\\" + path;

    //������ļ�  
    ofstream os(path);
    if (!os.is_open()) {
        cerr << "error��can not find or create the file which named " + path + "!" << endl;
        return false;
    }
    os << jsonString;
    os.close();

    return true;
}
