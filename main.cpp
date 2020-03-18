#include "bridgesim.h"
using namespace std;

int main(){
	string tr, bct;
	getline(cin, tr);
	getline(cin, bct);
	int trace = str_to_int(tr);
	int bridgect = str_to_int(bct);
	map<int, vector<string> > bridge_to_lan;
	map<string, vector<int> > lan_to_bridge;
	
	for(int i=0; i<bridgect; i++){
		string line;
		getline(cin, line);
		vector<string> bridge_lan;
	    istringstream iss(line);
    	for(std::string line; iss >> line; )
        	bridge_lan.push_back(line);
        // bridge_lan[0]=bridge_lan[0].substr(0,bridge_lan[0].size()-1);
        // cout<<int(bridge_lan[0][1])<<endl;
        // cout<<bridge_lan[0]<<" "<<bridge_lan[0].size()<<endl;
        // cout<<bridge_lan[0].substr(1,bridge_lan.size()-2)<<endl;
        int curr_bridge_inp = str_to_int(bridge_lan[0].substr(1,bridge_lan[0].size()-2));
        // cout<<curr_bridge_inp<<endl;
   		// vector<string> bridge_lan = splitspaces(line, true);
		for(int pos = 1 ; pos < bridge_lan.size() ; pos++){
			lan_to_bridge[bridge_lan[pos]].push_back(curr_bridge_inp);
			bridge_to_lan[curr_bridge_inp].push_back(bridge_lan[pos]);
		}
	}

	int len = lan_to_bridge.size();
	map<string, vector<int> > lan_to_hosts;
	map<int, string> host_to_lan;

	for(int i=0; i<len; i++){
		string line;
		getline(cin, line);
		vector<string> lan_hosts;
	    istringstream iss(line);
    	for(std::string line; iss >> line; )
        	lan_hosts.push_back(line);
        lan_hosts[0]=lan_hosts[0].substr(0,lan_hosts[0].size()-1);
        // vector<string> lan_hosts = splitspaces(line, true);
		for(int pos = 1 ; pos < lan_hosts.size() ; pos++){
			int host_number = str_to_int(lan_hosts[pos].substr(1, lan_hosts[pos].size()-1));
			lan_to_hosts[lan_hosts[0]].push_back(host_number);
			host_to_lan[host_number] = lan_hosts[0];
		}
	}

	for(auto it=lan_to_bridge.begin();it!=lan_to_bridge.end();it++)
		sort(it->second.begin(), it->second.end());

	for(auto it=bridge_to_lan.begin();it!=bridge_to_lan.end();it++)
		sort(it->second.begin(), it->second.end());

	for(auto it=lan_to_hosts.begin();it!=lan_to_hosts.end();it++)
		sort(it->second.begin(), it->second.end());
	
	string queries;
	getline(cin, queries);
	int count = str_to_int(queries);
	vector<pair<string, string> > transfer_queries(count);
	for(int i = 0; i < count ; i++){
		string line;
		getline(cin, line);
		vector<string> transfers;
	    istringstream iss(line);
    	for(std::string line; iss >> line; )
        	transfers.push_back(line);
		// vector<string> transfers = splitspaces(line, false);
		string src, dest;
		src = transfers[0];
		dest = transfers[1];
		transfer_queries[i] = make_pair(src, dest);
	}

	struct bridgesim simulation = {lan_to_bridge, bridge_to_lan, lan_to_hosts, host_to_lan, trace};
	// cout<<"Starting Minimum Spanning Tree"<<endl;
	simulation.simulate();
	simulation.display_bridge();
	simulation.update_tree();
	
	for(int i = 0; i < count ; i++){
		string src, dest;
		src = transfer_queries[i].first;
		dest = transfer_queries[i].second;
		simulation.transfer(src, dest);
		simulation.display_fts();
		cout<<endl;
	}
}