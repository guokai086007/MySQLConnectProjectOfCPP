#include "UserJsonHandler.h"

//通过path路径来读取用户Json文件，并封装成用户动态数组返回
vector<User> UserJsonHandler::readUserJsonFile(string path)
{
    Json::Reader reader;
    Json::Value root;
    vector<User> userList = {};

    //从文件中读取，保证当前文件夹中有demo.json文件  
    ifstream in(path, ios::binary);//其中的ios::binary是指以二进制方式打开文件
    if (!in.is_open())
    {
        cerr << "Error opening file in readUserJsonFile\n";
        return userList;
    }
    if (reader.parse(in, root)) {
        //读取根节点信息  
        Json::Value usersJson = root["users"]; 
        //usersJson是个Json数组所以循环遍历该数组，把该数组里面的每一个Json用户信息封装成用户User对象
        int n = usersJson.size();//获取这个Json数组的长度
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

//读取用户Json字符串，并封装成用户动态数组返回
vector<User> UserJsonHandler::readUserJsonString(string JsonString)
{
    Json::Reader reader;
    Json::Value root;
    vector<User> userList = {};

    if (reader.parse(JsonString, root)){
        //读取根节点信息  
        Json::Value usersJson = root["users"];
        //usersJson是个Json数组所以循环遍历该数组，把该数组里面的每一个Json用户信息封装成用户User对象
        int n = usersJson.size();//获取这个Json数组的长度
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
//根据所给的路径path以及动态数组users创建一个json记录文件
bool UserJsonHandler::writeUsersToJsonFile(vector<User> users,string path)
{
    //根节点  
    Json::Value root;
    //根节点中的用户Json数组
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

    //把用户Json数组挂到root上
    root["users"] = usersValue;

    //缩进输出  
    //cout << "StyledWriter:" << endl;
    //StyledWriter类的write方法是将上面的根节点root所具有的所有Json内容整合成一个Json字符串返回回来！
    Json::StyledWriter sw;
    string jsonString = sw.write(root);
    //cout << jsonString << endl;

    //获取当前路径
    filesystem::path currentPath = filesystem::current_path();
    //cout << currentPath << endl;
    path = currentPath.string() + "\\" + path;

    //输出到文件  
    ofstream os(path);
    if (!os.is_open()) {
        cerr << "error：can not find or create the file which named "+ path +"!" << endl;
        return false;
    }  
    os << jsonString;
    os.close();
    return true;
}
//根据所给的路径path以及用户对象user创建一个json记录文件
bool UserJsonHandler::writeUserToJsonFile(User user, string path)
{
    //根节点  
    Json::Value root;
    //根节点中的用户Json数组
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

    //获取当前路径
    filesystem::path currentPath = filesystem::current_path();
    //cout << currentPath << endl;
    path = currentPath.string() + "\\" + path;

    //输出到文件  
    ofstream os(path);
    if (!os.is_open()) {
        cerr << "error：can not find or create the file which named " + path + "!" << endl;
        return false;
    }
    os << jsonString;
    os.close();

    return true;
}
