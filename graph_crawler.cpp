#include <queue>
#include <string>
#include "rapidjson/document.h"
#include <iostream>
#include <curl/curl.h> 
#include <vector>
#include <string>
#include <chrono>
#include <unordered_set> 
using namespace std;
using namespace rapidjson;

const string BASE_URL =  "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";

//GET requests

//callback function by libcurl to write recieved data into a string when recieved

size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* output) {

    size_t totalSize = size * nmemb; //total size of data
    output->append((char*)contents, totalSize); //appending data to output string
    return totalSize; //return bytes processed
}
//fetching neighbors of a given node using libcurl
vector<string> fetch_neighbors(const string& node){
    //Pointer to curl handle
    CURL* curl;
    //variable to store result of curl operations
    CURLcode res = curl_easy_perform(curl);
if (res != CURLE_OK) {
    std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
}

    //construct full url by appending node to base URL

    string url = BASE_URL + node;
    //API's response
    string response;
    
    //initialize the curl session
    curl = curl_easy_init();
    if(curl == NULL){
        cerr <<"Curl Error" <<endl;
        return{}; //empty vector is returned
    }
    //URL for CURL request
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    //Specify callback function to write recieved data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    //provide string to store response data
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    //Allow request to foll http redirects
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    //get request
    curl_easy_perform(curl);

    //cleanup curl
    curl_easy_cleanup(curl);

    //Parsing JSON response using RAPIDJSON 
    Document doc;
    //checking for errors
    if(doc.Parse(response.c_str()).HasParseError()){

        cerr<<"Failed to parse JSON response"<<endl;
        return{};
    }
    vector<string> neighbors; //storing neighbors 
    if(doc.HasMember("neighbors")&& doc["neighbors"].IsArray()){
        for(const auto& neighbor: doc["neighbors"].GetArray()){
            neighbors.push_back(neighbor.GetString());
        }
    }
    return neighbors;
}
void bfs_traversal(const string& start_node, int depth){
    //adding time measurement
    auto start_time = chrono::high_resolution_clock::now(); 

    queue<pair<string, int>>q; //queue holds pairs of nodes + current depth
    unordered_set<string> visited;

    q.push({start_node, 0});
    visited.insert(start_node);
    cout << "BFS starting from: " << start_node << ", Depth limit: " << depth << endl;

    while (!q.empty()){
        auto [node, current_depth] = q.front();
        q.pop();
        cout << "Visited: " << node << " at depth " << current_depth << endl;
        if(current_depth < depth){
            vector<string> neighbors = fetch_neighbors(node);
        for(const string&neighbor:neighbors){
            if(visited.find(neighbor)==visited.end()){
                visited.insert(neighbor);
                q.push({neighbor, current_depth+1});
            }
            }
        }
}
        auto end_time = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end_time - start_time;
        cout << "Execution Time: " << elapsed.count() << " seconds" << endl;
}
int main(int argc, char*argv[]){
    if(argc !=3){
        cerr << "Usage: " << argv[0] << " <start_node> <depth>" << endl;
        return 1;
    }
    string start_node = argv[1];
    int depth = stoi(argv[2]);
    bfs_traversal(start_node,depth);
    return 0;
}
