#include "bridge.h"
using namespace std;

struct bridgesim{
	/*
		Defines a structure for network topology
		stores two maps one lan_to_bridge and another bridge_to_lan
		trace stores whether the trace needs to be printed
	*/
	map<string, vector<int> > lan_to_bridge; // map of lans to connected bridges
	map<int, vector<string> > bridge_to_lan; // map of bridges to connected lans
	map<string, vector<int> > lan_to_hosts; // map of lans to connected hosts
	map<int, string> host_to_lan; // map of individual host to lan  
	bool trace; // whether trace is on
	map<string, vector<int> > lan_to_rdb; // lan to bridge connected to active bridges
	map<int, bridge> bridgevec; // stores map of bridge_name to bridge object

	bridgesim(map<string, vector<int> > lan_to_bridge,
		map<int, vector<string> > bridge_to_lan,
		map<string, vector<int> > lan_to_hosts,
		map<int, string > host_to_lan,
		int trace){
		this->lan_to_bridge = lan_to_bridge;
		this->bridge_to_lan = bridge_to_lan;
		this->lan_to_hosts = lan_to_hosts;
		this->host_to_lan = host_to_lan;
		this->trace = (trace == 1);

		for(auto it = bridge_to_lan.begin(); it != bridge_to_lan.end();it++){
			this->bridgevec[it->first] = bridge(it->first, it->second);
		}
	}

	void simulate();
	void display_bridge();
	void update_tree();
	// void display_tree();
	void transfer(string src, string dest);
	void display_fts();
};