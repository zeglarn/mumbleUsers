#include <iostream>
#include <fstream>
#include <map>
#include <stack>
using namespace std;

//Keywords
const string AUTH = "Authenticated";
const string CLOSED = "closed";

const int STARTPOS = 32;
//Finds username at input line.
void getName(string& str, string& result, int pos = STARTPOS);

//Reads file and updates userMap with usernames and corresponding truth value.
void readFile(string const& file, map<string, bool>& userMap);

//Different print options.
void printDecorated(stack<string>& onlineUsers);
void printCSV(stack<string>& onlineUsers);
void printHelp();

int main(int argc, char* argv[]) {

  const string rotatedLog = "/var/log/mumble-server/mumble-server.log.1";
  const string log = "/var/log/mumble-server/mumble-server.log";
  map<string, bool> users;
  
  readFile(rotatedLog, users); //Read old log first.
  readFile(log, users); //Read current log.

  map<string, bool>::iterator it;
  
  //Check if server is empty.
  bool hasUsers = false;
  if (!users.empty())
    for (it = users.begin(); it != users.end(); it++)
      hasUsers = hasUsers || it->second;;

  stack<string> onlineUsers;
  for (it = users.begin(); it != users.end(); it++)
    if (it->second)
      onlineUsers.push(it->first);

  if(argc < 2)
    printDecorated(onlineUsers);
  else {
    if (argv[1][0] == '-' && argv[1][2] == 0) {
      if (argv[1][1] == 'c') {
	printCSV(onlineUsers);
	return 0;
      } else if (argv[1][1] == 'd') {
	printDecorated(onlineUsers);
	return 0;
      } else if (argv[1][1] == 'h') {
	printHelp();
	return 0;
      }
      printHelp();
    } else
    printHelp();
  }
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

void readFile(string const& file, map<string, bool>& userMap) {
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

void printDecorated(stack<string>& users) {
  cout << "---------N30.se---------" << endl;
  if (users.empty()) {
    cout << "No Pokémon in the bag." << endl;
    cout << "------------------------" << endl;
    return;
  }

  //Print only online users.
  while (!users.empty()) {
    cout << users.top() << endl;
    users.pop();
  }
  cout << "------------------------" << endl;
}

void printCSV(stack<string>& users) {
  if (users.empty()) {
    cout << "NULL" << endl;
    return;
  }
  
  while (!users.empty()) {
    cout << users.top();
    if (users.size() != 1)
      cout << ',';
    users.pop();
  }
  cout << endl;
}

void printHelp() {
  cout << "Options:" << endl;
  cout << " -c  Prints users as comma separated list." << endl;
  cout << " -d  Prints decorated output." << endl;
  cout << " -h  Prints this." << endl;
}
