#include "bridge.h"
using namespace std;

string pr(int br_id){
	return "B"+int_to_str(br_id);
}

void print_message(message m){
	/*
		method to print a configuration message
	*/
	cout<<m.t<<" "<<m.c<<" "<<pr(m.Bk)<<" ("<<pr(m.Bi)<<", "<<m.d<<", "<<pr(m.Bj)<<")"<<endl;
}

void print_learn_msg(learning_message m){
	cout << m.t << " " << m.c << " B" << m.br_id << " " << m.src << "-->" << m.dest << endl;
}

string int_to_str(int num){
    stringstream ss;
    ss << num; 
    return ss.str();
}

int str_to_int(string str){
    stringstream ss(str);
    int num;
    ss >> num;
    return num;
}

bool bridge::message_received(message m){
	/*
		action of bridge on receiving a message
		active is whether the bridge is root or not
	*/
	// print_message(m, 'r');
	if(this->active){
		if(m.Bi < this->root_bridge)//condition 1
		{
			this->active = false;
			this->lan_status[m.lan] = "RP"; // setting root port
			this->root_bridge = m.Bi; // setting root bridge
			this->lan_id = m.lan; // storing root lan port
			this->dist = m.d + 1; // storing distance to root
			this->next_bridge = m.Bj; // storing next hop bridge(parent bridge)
			// cout<<"Changed status"<<endl;
			return true;
		}
		else
			return false;
	}
	else{
		if((m.Bi < this->root_bridge)||// condition 1
		   (m.Bi==this->root_bridge && m.d+1 < this->dist)||//condition 2
		   (m.Bi==this->root_bridge && m.d+1==this->dist && m.Bj<this->next_bridge)||
		   (m.Bi==this->root_bridge && m.d+1==this->dist && m.Bj==this->next_bridge && m.lan < this->lan_id))//condition : ensure further determinism
		{
			// setting all other lan ports to DP since there was a change in network topology
			for(auto it=this->lan_status.begin();
				it!=this->lan_status.end();it++){
				it->second = "DP";
			}
			this->lan_status[m.lan] = "RP";// setting root port
			this->root_bridge = m.Bi;// setting root bridge
			this->lan_id = m.lan;// storing root lan port
			this->dist = m.d + 1;// storing distance to root
			this->next_bridge = m.Bj;// storing next hop bridge(parent bridge)
			// cout<<"Changed status1"<<endl;
			return true;
		}
		else if(((m.d < this->dist)||// condition 4
				(m.d == this->dist && m.Bj < this->bridge_id))&&//condition 5
				(m.Bi == this->root_bridge)&&(this->lan_status[m.lan]!="RP")&&(this->lan_status[m.lan]!="NP"))//necessary conditions for NP Port
		{
			// cout<<"Changed status2"<<endl;
			this->lan_status[m.lan] = "NP";
			return true;
		}
		else
			return false;
	}
}

void bridge::display_status(){
	/*
		Displays the status of all lan segments connected to this bridge
	*/
	cout<<pr(this->bridge_id)<<":";
	for(auto it=this->lan_status.begin();it!=this->lan_status.end();it++){
		string lan_name=it->first;//name of lan
		string status=it->second;//status of lan DP RP NP
		cout<<" "<<lan_name<<"-"<<status;
	}
	cout<<'\n';
}

string bridge::return_status(string lan_seg_id){
	/*
		returns status of lan_seg_id
	*/
	string value = this->lan_status[lan_seg_id]; 
	return value;
}

int bridge::is_root(){
	if(active)
		return 1;
	else
		return 0;
}

void bridge::src_present(int src, string port){
	if(this->forwarding_table.find(src)==this->forwarding_table.end())
		forwarding_table[src] = port;
}

bool bridge::dest_present(int dest){
	if(this->forwarding_table.find(dest)!=this->forwarding_table.end())
		return true;
	else
		return false;
}

void bridge::update_ports(){
	for(auto it=this->lan_status.begin();it!=this->lan_status.end();it++){
		if(it->second!="NP")
			this->active_ports.push_back(it->first);
	}
}

string bridge::returnft(int dest){
	if(this->forwarding_table.find(dest)==this->forwarding_table.end())
		cout<<"Error in code"<<endl;
	return this->forwarding_table[dest];
}

void bridge::display_ft(){
	cout<<pr(this->bridge_id)<<":"<<endl;
	cout<<"HOST ID | FORWARDING PORT"<<endl;
	for(auto it=this->forwarding_table.begin();it!=this->forwarding_table.end();it++){
		cout<<"H"<<it->first<<" | "<<it->second<<endl;
	}
}