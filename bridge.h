#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<sstream>
#include<cstring>
#include<queue>
#include<algorithm>
using namespace std;


struct message
{
  /*
    defines a struct for a configuration message
  */
  int t; //message generated at time t
  string lan; // lan on which it is being sent
  char c; // sender or receiver
  int Bk; // bridge at which event occurs
  int Bi; // Root bridge that sender thinks
  int d; // Its distance
  int Bj; // Sender of message

  message(int t, string lan, char c, int Bk, int Bi, int d, int Bj){
    this->t = t;
    this->lan = lan;
    this->c = c;
    this->Bk = Bk;
    this->Bi = Bi;
    this->d = d;
    this->Bj = Bj;
  }
};

struct learning_message
{
  /*
    Defines a struct for a configuration message
  */
  int t; // message generated at time t
  char c; // stores if sender or receiver
  int br_id; // bridge through which it travels
  string src; // source lan
  string dest; // destination lan

  learning_message(int t, char c, int br_id, string src, string dest){
    this->t = t;
    this->c = c;
    this->br_id = br_id;
    this->src = src;
    this->dest = dest;
  } 
};


struct learn_less_than_key
{
  /*
    sort learning_message in order
  */
    inline bool operator() (const learning_message& msg1, const learning_message& msg2)
    {
      if(msg1.t < msg2.t)
        return true;
      else if((msg1.t == msg2.t) && (msg1.c != msg2.c) && (msg1.c == 's'))
        return true;
      else if((msg1.t == msg2.t) && (msg1.c == msg2.c) && (msg1.br_id < msg2.br_id))
        return true;
      else if((msg1.t == msg2.t) && (msg1.c == msg2.c) && (msg1.br_id == msg2.br_id) &&
              (msg1.src < msg2.src))
        return true;
      else if((msg1.t == msg2.t) && (msg1.c == msg2.c) && (msg1.br_id == msg2.br_id) &&
              (msg1.src == msg2.src) && (msg1.dest < msg2.dest))
        return true;
      else
        return false;
    }
};

struct send_less_than_key
{
  /*
    sort message in order for sending
  */
    inline bool operator() (const message& msg1, const message& msg2)
    {
      if(msg1.Bj < msg2.Bj)
        return true;
      else if((msg1.Bj == msg2.Bj) && (msg1.Bi < msg2.Bi))
        return true;
      else if((msg1.Bj == msg2.Bj) && (msg1.Bi == msg2.Bi) && (msg1.d < msg2.d))
        return true;
      else
        return false;
    }
};

struct receive_less_than_key
{
  /*
    sort message in order for sending
  */
    inline bool operator() (const message& msg1, const message& msg2)
    {
      if(msg1.Bk < msg2.Bk)
        return true;
      else if((msg1.Bk == msg2.Bk) && (msg1.Bi < msg2.Bi))
        return true;
      else if((msg1.Bk == msg2.Bk) && (msg1.Bi == msg2.Bi) && (msg1.d < msg2.d))
        return true;
      else if((msg1.Bk == msg2.Bk) && (msg1.Bi == msg2.Bi) && (msg1.d == msg2.d) && (msg1.Bj < msg2.Bj))
        return true;
      else
        return false;
    }
};

struct bridge
{
      int bridge_id, root_bridge, next_bridge; // bridge name and root bridge
      string lan_id; // root lan
      bool active; // whether it is the root port
      int dist; // its distance from the root port
      map<string, string> lan_status; // status of the lan segment to which it is connected
      map<int, string> forwarding_table; // stores forwarding table once spanning tree protocol stablizes
      vector<string> active_ports; // list of ports on which bridge sends once protocol terminates

      bridge(){
        this->bridge_id = -1;
        this->next_bridge = -1;
        this->lan_id = "1"; // empty string constant 
        this->root_bridge = -1;
        this->active = true;
        this->dist = 0;
      }

      bridge(int name, vector<string> lans){
        /*
          Upon initialisation bridge_id is same as root port as name as every bridge thinks itself as root
          All lan segments think that it is the designated bridge for them
          Its distance from root is zero since it is the root
        */
        this->bridge_id = name;
        this->next_bridge = -1;
        this->lan_id = "1";// empty string constant 
        this->root_bridge = name;
        this->active = true;
        this->dist = 0;
        for(int i=0;i<lans.size();i++)
          this->lan_status[lans[i]] = "DP";
      }

      bool message_received(message);
      void display_status();
      string return_status(string);
      int is_root();
      void update_ports();
      void display_ft();
      void src_present(int src, string port);
      bool dest_present(int dest);
      string returnft(int dest); 
};

void print_message(message m);
void print_learn_msg(learning_message m);
string pr(int id);
string int_to_str(int num);
int str_to_int(string str);