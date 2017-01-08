#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

//Keywords
const string AUTH = "Authenticated";
const string CLOSED = "closed";

const int STARTPOS = 32;
//Finds username at input line.
void getName(string& str, string& result, int pos = STARTPOS);

//Reads file and updates userMap with usernames and corresponding truth value.
void readFile(map<string, bool>& userMap, string const& file);

int main() {
  const string rotatedLog = "/var/log/mumble-server/mumble-server.log.1";
  const string log = "/var/log/mumble-server/mumble-server.log";
  map<string, bool> users;
  
  readFile(users, rotatedLog); //Read old log first.
  readFile(users, log); //Read current log.

  map<string, bool>::iterator it;
  
  //Check if server is empty.
  bool hasUsers = false;
  if (!users.empty())
    for (it = users.begin(); it != users.end(); it++)
      hasUsers = hasUsers || it->second;;
  
  cout << "------------------------" << endl;

  if (!hasUsers) {
    cout << "No users online." << endl;
    cout << "------------------------" << endl;
    return 0;
  }

  //Print only online users.
  for (it = users.begin(); it != users.end(); it++)
    if (it->second)
      cout << it->first << endl;;
  cout << "------------------------" << endl;

  return 0;
}

void getName(string& str, string& result, int pos) {
  // Find starting position of username.
  if (pos == STARTPOS) {
    while (str.at(pos) != ':')
      ++pos;
    ++pos;
  }

  // parse line for username.
  if (str.at(pos) == '(') {
    return;
  } else {
    result += str.at(pos);
    pos++;
    getName(str, result, pos);
  }
}

void readFile(map<string, bool>& userMap,string const& file) {
  ifstream logFile(file.c_str());
  string line;
  
  while (getline(logFile, line)) {
    string userName = "";

    if (line.find(AUTH) != string::npos) { 
      getName(line, userName);
      userMap[userName] = true;
    } else if (line.find(CLOSED) != string::npos) {
      getName(line, userName);
      userMap[userName] = false;
    }
  }
  logFile.close();
}
