#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

const string AUTH = "Authenticated";
const string CLOSED = "closed";
const int STARTPOS = 32;

void getName(string& str, string& result, int pos = STARTPOS);
void readFile(map<string, bool>& userMap, string const& file);

int main() {
  const string rotatedLog = "/var/log/mumble-server/mumble-server.log.1";
  const string log = "/var/log/mumble-server/mumble-server.log";
  map<string, bool> users;

  readFile(users, rotatedLog);
  readFile(users, log);

  map<string, bool>::iterator it;
  bool hasUsers = false;
  if (!users.empty())
    for (it = users.begin(); it != users.end(); it++)
      hasUsers = hasUsers || it->second;;
  
  cout << "------------------------" << endl;

  if (!hasUsers) {
    cout << "No Pokémon in the bag" << endl;
    cout << "------------------------" << endl;
    return 0;
  }

  for (it = users.begin(); it != users.end(); it++)
    if (it->second == true)
      cout << it->first << endl;;
  cout << "------------------------" << endl;

  return 0;
}

void getName(string& str, string& result, int pos) {
  if (pos == 32) {
    while (str.at(pos) != ':')
      ++pos;
    ++pos;
  }

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
