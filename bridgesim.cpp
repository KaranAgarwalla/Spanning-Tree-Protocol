#include "bridgesim.h"
using namespace std;

void bridgesim::simulate(){
	int t=0;
	bool isChanged = true;
	while(isChanged){

		isChanged = false;
		vector<message> messages_sent;
		vector<message> messages_revd;
		for(auto br=this->bridgevec.begin();br!=this->bridgevec.end();br++){

			vector<string> lanlst = bridge_to_lan[br->first];
			bridge curr = br->second;
			for(int i=0;i<lanlst.size();i++){
				if(curr.return_status(lanlst[i])=="DP"){
						message to_be_sent = {t, lanlst[i], 's', curr.bridge_id, curr.root_bridge, curr.dist, curr.bridge_id};
						messages_sent.push_back(to_be_sent);
					for(int j=0;j<this->lan_to_bridge[lanlst[i]].size();j++){
						bridge* dest = &this->bridgevec[lan_to_bridge[lanlst[i]][j]];
						if(curr.bridge_id!=dest->bridge_id){
						message to_be_received = {t+1, lanlst[i], 'r', dest->bridge_id, curr.root_bridge, curr.dist, curr.bridge_id};
						messages_revd.push_back(to_be_received);
						}						
					}						
				}
			}
		}

		if(this->trace){
			sort(messages_sent.begin(), messages_sent.end(), send_less_than_key());
			for(int i=0;i<messages_sent.size();i++)
				print_message(messages_sent[i]);

			sort(messages_revd.begin(), messages_revd.end(), receive_less_than_key());
			for(int i=0;i<messages_revd.size();i++)
				print_message(messages_revd[i]);
		}

		for(int i=0;i<messages_revd.size();i++){
			message currmsg = messages_revd[i];
			bridge* dest = &(this->bridgevec[currmsg.Bk]);
			bridge* src = &(this->bridgevec[currmsg.Bi]);
			bool isChangedDest = (dest->message_received(currmsg));
			isChanged = isChanged || isChangedDest;
			// if(isChangedDest){
			// cout<<"---------------"<<endl;
			// cout<<src->bridge_id<<" "<<dest->bridge_id<<endl;
			// src->display_status();
			// dest->display_status();
			// cout<<"Dest information"<<" "<<dest->root_bridge<<" "<<dest->dist<<endl;
			// print_message(currmsg, "s");
			// cout<<(src->is_root())<<endl;
			// cout<<(dest->is_root())<<endl;
			// cout<<"---------------"<<endl;
			// }
		}
		t++;
	}
}


void bridgesim::update_tree(){
	/*
		This function updates lan_to_rdb after spanning tree protocol has finished
		lan_to_rdb maps each lan port to list of bridges for which it is connected in spanning tree
	*/
	for(auto br=this->bridgevec.begin();br!=this->bridgevec.end();br++){
		/*
			br: pointer to bridge under consideration
		*/
		(br->second).update_ports();
		for(auto prt=(br->second).active_ports.begin();prt!=(br->second).active_ports.end();prt++)
			/*
				prt: pointer to lan to which it is connected
			*/
			this->lan_to_rdb[*prt].push_back(br->first);
	}
}

void bridgesim::display_bridge(){
	for(auto br=this->bridgevec.begin();br!=this->bridgevec.end();br++){
		(br->second).display_status();
	}
}

// void bridgesim::display_tree(){
// 	for(auto br=this->lan_to_rdb.begin();br!=this->lan_to_rdb.end();br++){
// 		cout<<br->first<<": ";
// 		for(int i=0;i<br->second.size();i++)
// 			cout<<br->second[i]<<" ";
// 		cout<<endl;
// 	}
// }

void bridgesim::transfer(string src, string dest){
	int src_number = str_to_int(src.substr(1, src.size()-1));
	int dest_number = str_to_int(dest.substr(1, dest.size()-1));
	vector<learning_message> messages;
	queue<pair<string, int> > lans_to_checked;// pair of lans and bridge sent from
	string src_lan = this->host_to_lan[src_number];
	lans_to_checked.push(make_pair(src_lan, -1));
	map<string, int> lans_reach_time; // When does message arrive at lan?
	// cout<<src_lan<<endl;
	lans_reach_time[src_lan] = 0;

	while(!lans_to_checked.empty()){
		
		string curr_lan = lans_to_checked.front().first;
		int curr_time = lans_reach_time[curr_lan];
		int incoming_bridge = lans_to_checked.front().second; 
		for(int i=0; i<this->lan_to_rdb[curr_lan].size(); i++){

			int outgoing_bridge = this->lan_to_rdb[curr_lan][i];
			if(outgoing_bridge!=incoming_bridge){
				bridge* bout = &this->bridgevec[outgoing_bridge];
				// cout<<bout->bridge_id<<endl;
				bout->src_present(src_number, curr_lan);
				bool pres = bout->dest_present(dest_number);
				if(pres){
					string lanout = bout->returnft(dest_number);
					if(lanout!=curr_lan){
						if(this->trace){
						lans_reach_time[lanout] = curr_time + 1;
						learning_message to_be_sent = {curr_time, 's', outgoing_bridge, curr_lan, lanout};
						learning_message to_be_revd = {curr_time + 1, 'r', outgoing_bridge, curr_lan, lanout};
						messages.push_back(to_be_sent);
						messages.push_back(to_be_revd);
						}
						lans_to_checked.push(make_pair(lanout, outgoing_bridge));
					}
				}
				else{
					for(auto it=bout->active_ports.begin();it!=bout->active_ports.end();it++){
						string lanout = *it;
						if(lanout!=curr_lan){
							if(this->trace){
							lans_reach_time[lanout] = curr_time + 1;
							learning_message to_be_sent = {curr_time, 's', outgoing_bridge, curr_lan, lanout};
							learning_message to_be_revd = {curr_time + 1, 'r', outgoing_bridge, curr_lan, lanout};
							messages.push_back(to_be_sent);
							messages.push_back(to_be_revd);
							}
							lans_to_checked.push(make_pair(lanout, outgoing_bridge));
						}
					}
				}	
			}
		}
		lans_to_checked.pop();
	}
	sort(messages.begin(), messages.end(), learn_less_than_key());
	for(int i=0;i<messages.size();i++)
		print_learn_msg(messages[i]);
}		

void bridgesim::display_fts(){
	for(auto br=this->bridgevec.begin();br!=this->bridgevec.end();br++){
		(br->second).display_ft();
	}
}