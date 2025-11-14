#include "console.h"
#include <iostream>     // for cout, cin
#include <fstream>      // for ifstream
#include <sstream>      // for stringstream
#include <unordered_set>
#include <vector>
#include <queue>
#include <priorityqueue.h>
#include <unordered_map>
#include <istream>
#include "wikiscraper.h"

using std::cout;            using std::endl;
using std::ifstream;        using std::stringstream;
using std::string;          using std::vector;
using std::priority_queue;  using std::unordered_map;
using std::unordered_set;   using std::cin;

/*
 * 要找到从起始页到目标页的链接阶梯：将起始页设为当前处理页面。
 * 获取当前页的所有链接。
 * 若目标页恰为当前页的某个链接：则任务完成！返回至此所经过的链接路径。
 * 否则以智能方式访问当前页的每个链接，并以类似方法逐一搜索这些页面
 * This is the function you will be implementing. It takes
 * two string representing the names of a start_page and
 * end_page and is supposed to return a ladder, represented
 * as a vector<string>, of links that can be followed from
 * start_page to get to the end_page.
 *
 * For the purposes of this algorithm, the "name" of a Wikipedia
 * page is what shows at the end of the URL when you visit that page
 * in your web browser. For ex. the name of the Stanford University
 * Wikipedia page is "Stanford_University" since the URL that shows
 * in your browser when you visit this page is:
 *
 *       https://en.wikipedia.org/wiki/Stanford_University
 */


int set_intersection_size(unordered_set<string> a,unordered_set<string> b){
    int num = 0;
    for(auto s : a){
        if(b.find(s) != b.end()){
            num++;
        }
    }
    return num;
}

vector<string> findWikiLadder(const string& start_page, const string& end_page) {
    /*
    // TODO: 1. Once your file reading is working, replace the below
    //          return statement with "return {};".
    //       2. Make sure that you've copied over your findWikiLinks method
    //          from Part A into wikiscraper.cpp.
    //       3. Finally, implement this function per Part B in the handout!
    //
    //                Best of luck!
    // creates WikiScraper object
*/
    WikiScraper scraper;
    auto target_set = scraper.getLinkSet(end_page);
    unordered_map<string, unordered_set<string>> link_cache;
    auto get_cached_links = [&](const string& page) {
        if (link_cache.find(page) == link_cache.end()) {
            link_cache[page] = scraper.getLinkSet(page);
        }
        return link_cache[page];
    };
    auto compare = [&](const vector<string>& ladder1,const vector<string>& ladder2){
        string page1 = ladder1.back();
        string page2 = ladder2.back();
        auto page1_set = get_cached_links(page1);
        auto page2_set = get_cached_links(page2);
        int num1 = set_intersection_size(page1_set,target_set);
        int num2 = set_intersection_size(page2_set,target_set);
        return num1 > num2;
    };
    priority_queue<vector<string>,vector<vector<string>>,
                        decltype(compare)> ladderque(compare);
    vector<string> ladder;
    ladder.push_back(start_page);
    ladderque.push(ladder);
    unordered_set<string> visit{start_page};
    while(!ladderque.empty()){
        vector<string> ladd = ladderque.top();
        ladderque.pop();
        string ladd_end = ladd.back();
        // ladd.pop_back();
        auto end_set = get_cached_links(ladd_end);

        if(end_set.find(end_page) != end_set.end()){
            // ladd.push_back(ladd_end);
            ladd.push_back(end_page);
            return ladd;
        }

        for(const auto& page : end_set){
            if(page == end_page){
                ladd.push_back(page);
                return ladd;
            }

            if(visit.find(page) == visit.end()){
                vector<string> partial_ladder = ladd;
                partial_ladder.push_back(page);
                ladderque.push(partial_ladder);
                visit.insert(page);
            }
        }
    }
    return {};
    // return {"File reading works!", start_page, end_page};
}

int main() {
    /* Container to store the found ladders in */
    vector<vector<string>> outputLadders;
    cout << "hi homework1" << endl;
    ifstream input;
    string filename;
    while(!input.is_open()){
        cout << "Enter a file name: ";
        string path = "res\\";
        getline(cin, filename);
        filename = path + filename;
        input.open(filename);
    }
    cout << "ok,file " << filename << " is open" << endl;
    // TODO: Create a filestream from the filename.
    //       For each pair {start_page, end_page} in the input file,
    //       retrieve the result of findWikiLadder(start_page, end_page)
    //       and append that vector to outputLadders.
    string line;
    getline(input,line);
    int num = stoi(line);
    cout << "num is " << line << endl;
    // std::pair<string,string> pairs;
    while(num-- && getline(input,line)){
        // cout << "line is " << line << endl;
        string first;
        string second;
        std::istringstream iss(line);
        iss >> first;
        iss >> second;
        // cout << first << " " << second << endl;
        cout << "first is " << first << " second is " << second << endl;
        vector<string> ladder = findWikiLadder(first,second);
        if(!ladder.empty()){
            outputLadders.push_back(ladder);
            cout << "ladder size is " << ladder.size() << endl;
        }
    }



    /*
     * Print out all ladders in outputLadders.
     * We've already implemented this for you!
     */
    for (auto& ladder : outputLadders) {
        if(ladder.empty()) {
            cout << "No ladder found!" << endl;
        } else {
            cout << "Ladder found:" << endl;
            cout << "\t" << "{";

            std::copy(ladder.begin(), ladder.end() - 1,
                      std::ostream_iterator<string>(cout, ", "));
            /*
             * The above is an alternate way to print to cout using the
             * STL algorithms library and iterators. This is equivalent to:
             *    for (size_t i = 0; i < ladder.size() - 1; ++i) {
             *        cout << ladder[i] << ", ";
             *    }
             */
            // for(size_t i = 0;i < ladder.size();i++){
            //     cout << ladder[i] << ", ";
            // }
            cout << ladder.back() << "}" << endl;
        }
    }
    return 0;
}




