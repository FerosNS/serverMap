/* Plaintext Server Map Creator
 * Author: Allen Perry
 * This program creates and maintains a database of network settings and servers.
 *
 */

 /* TODO: Use find function to navigate to specific task
  *   Check for valid IP addresses (see TODO:1)
  *
  *   FIXME Check for invalid inputs
  *     - inputting a wrong value when asking for number of new servers results in
  *        infinite loop
  */

  /* Completed:
   *  -Read from file works to get number of Servers (streams are open)
   *  -Printing map works
   *
   *
   *
  */
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>

using namespace std;
int _serverIndex = 0;

class NAT{
public:
  void add_rule();
  string rulename;
  string public_port;
  string destination_port;
};

class Server {
public:
  void add_server();
  void get_server();
  string hostname;
  string ip;
  vector<NAT> rules;
  int rulecount;
};

class Map{
public:
  void build_map();
  void print_map();
  void save_map(ofstream &save_file);
  ofstream save_node;
  int numServers;
  vector<Server> servers;
};

void get_data(Map &server_map,ifstream &load_file);
void display_menu(Map &server_map, ifstream &load_file, ofstream &save_file);
//void add_server(Map &server_map);
//void save_map(Server &server_map, ifstream &load_file, ofstream &save_file);


int main(){
    ofstream save_file;
    ifstream load_file;
    Map server_map;
    //Check if map save exists else create
    display_menu(server_map, load_file, save_file);
    // get_data(load_file);
    // save_data(save_file);
    // display_menu();
    //save_file.close();
    return 0;
}
void get_data(Map &server_map,ifstream &load_file){
  string input;
  server_map.numServers = 0;
  load_file >> server_map.numServers;
  if (server_map.numServers == 0){
    cout << "You have no servers in the map. Would you like to add some?[Y/n]: ";
    if (input == "" || input == "Y" || input == "yes" || input == "y"){
      cin >> input;
      int newServers = 0;
      cout << "Input the amount of servers to add to the map: ";
      cin >> newServers;
      cout << endl;
       for (int i = 0; i<newServers;i++){
         server_map.servers.push_back(Server());
         server_map.servers[server_map.numServers].add_server();
         server_map.numServers = server_map.servers.size();
       }
    }
  } else {
    for (int i = 0; i < server_map.numServers; i++){
        Server server;
        server_map.servers.push_back(server);
        load_file >> server_map.servers[i].hostname;
        server_map.servers[i].get_server();
    }
   }
  //display_menu(load_file,1);
}
void display_menu(Map &server_map,ifstream &load_file, ofstream &save_file){
  int option = 0;
  bool quit = false;
  string input;
  cout << "\n\n\n\n";
  cout << "|--------Not Another IPAM--------|" << endl;
  cout << "|--  A Networking Tool For All --|" << endl;
  cout << "|-                              -|" << endl;
  cout << "|---- Created by Allen Perry ----|" << endl;
  cout << "\n\n";
  cout << "\tLoading..." << endl;
  cout << "\tChecking for existing data...\n" << endl;
  load_file.open("server-map.txt");
  get_data(server_map,load_file);
  cout << "\tLoaded \n";
  load_file.close();

  while (!quit){
    cout << "---Main Menu---" << endl;
    cout << "\n\t[1] Print Server Map" << endl;
    cout << "\t[2] Edit Server Map" << endl;
    cout << "\t[3] Save Server Map" << endl;
    cout << "\t[4] Exit"<< endl;
    cout << "\nEnter Option: ";
    cin >> option;
    switch (option){
      case 1:
        server_map.print_map();
        // cout << "Feature coming Soon(tm)" << endl;
        break;
      case 2:
        //edit_map();
        cout << "Feature coming Soon(tm)" << endl;
        break;
      case 3:
        server_map.save_map(save_file);
        //cout << "Feature coming Soon(tm)" << endl;
        break;
      case 4:
        cout << "Do you want to save? [y/n]: ";
        while (!quit){
          cin.clear();
          cin >> input;
          if (input == "Y" || input == "yes" || input == "y"){
          server_map.save_map(save_file);
          // cout << "Feature coming Soon(tm)" << endl;
          quit = true;
          }else if ((input == "N" || input == "no" || input == "n")){
            quit = true;
          }else{
            cout << "Invalid Input... Try again." << endl;
            cout << "Do you want to save? [y/n]: ";
            cin.clear();
            cin >> input;

          }
        }
        break;
      default:
        cout << "Not a valid choice... Try again." << endl;
    }
  }
}
// void NAT::get_rule(ifstream &load_node){
//   NAT rule;
//   load_node >> rule.rulename;
//   load_node >> rule.public_port;
//   load_node >> rule.destination_port;
// }
void Server::add_server(){
  string input;
  cout << "Input Host Name: ";
  cin >> hostname;
  //TODO:1
  cout << "Input "<< hostname <<"'s' IP address in xxx.xxx.xxx.xxx form " << endl;
  cout << "IP: ";
  cin >> ip;
  cout << endl;
  cout << "Would you like to add NAT rules? [Y/n]";
  cin.clear();
  cin >> input;
  if (input == "\n" || input == "Y" || input == "yes" || input == "y"){
    int i = 0;
    NAT rule;
    while (input != "#"){
      cout << "Enter Rule Name: ";
      cin >> rule.rulename;
      cout << endl << "Enter Public Port: ";
      cin >> rule.public_port;
      cout << endl << "Enter Destination Port: ";
      cin >> rule.destination_port;
      cout << endl;
      rules.push_back(rule);
      cout << "Add another rule?: (input # to stop)";
      cin.clear();
      cin >> input;
      i++;
    }
    rulecount = i;
  }
}

void Server::get_server(){
  ifstream load_node;
  load_node.open(("servers/server-map." + hostname).c_str());
  load_node >> rulecount;
  load_node >> ip;
  for (int i = 0; i < rulecount; i++){
    NAT rule;
    load_node >> rule.rulename;
    load_node >> rule.public_port;
    load_node >> rule.destination_port;
    rules.push_back(rule);
  }
  load_node.close();
}

void Map::print_map(){
  cout << "Server Map: ";
  cout << numServers << " {" << endl;
  for (int i = 0; i < numServers; i++){
    cout << "\tServer: " << servers[i].hostname << " " << servers[i].ip <<" {" << endl;
    cout << "\tRules: " << servers[i].rulecount << endl;
    for (int j = 0; j < servers[i].rulecount; j++){
      cout << "\t\t" << servers[i].rules[j].rulename;
      cout << "\t\t" << servers[i].rules[j].public_port;
      cout << "\t\t" << servers[i].rules[j].destination_port << endl;
      }
      cout << "\t}" << endl;
    }
  cout << "}" << endl;
}
void Map::save_map(ofstream &save_file){
  //TODO:2
  save_file.open("server-map.txt");
  save_file << numServers << endl;
  for (int i = 0; i < numServers; i++){
    save_file << servers[i].hostname << endl;
    save_node.open(("servers/server-map." + servers[i].hostname).c_str());
    save_node << servers[i].rulecount << endl;
    save_node << servers[i].ip << endl;
    for (int j = 0; j < servers[i].rulecount; j++){
      save_node << servers[i].rules[j].rulename << endl;
      save_node << servers[i].rules[j].public_port << endl;
      save_node << servers[i].rules[j].destination_port << endl;
      }
      save_node.close();
    }
  save_file.close();
}
