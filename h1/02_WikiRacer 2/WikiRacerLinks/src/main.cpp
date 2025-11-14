#include <algorithm>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include "console.h"
#include <istream>
#include <iostream>
#include <sstream>
#include <string>
#include <set>
using std::cout;            using std::endl;
using std::string;          using std::unordered_set;
using namespace std;


/*
 * Note that you have to pass in the file as a single string
 * into the findWikiLinks function!
 * So we have to turn the passed-in file into a single string...
 * does that sound familiar at all?
 */
unordered_set<string> findWikiLinks(const string& page_html) {
    unordered_set<string> s;
    auto it_start = page_html.begin();
    auto it_end = page_html.end();
    // string::iterator it;
    string error_str1 = "ams";
    string error_str2 = "special";
    auto it = it_start;
    std::string target = "<a href=\"/wiki/";
    while(true){
        it = std::search(it,it_end,target.begin(),target.end());
        if(it != it_end){
            it += target.length();
            string str;
            while(*it != '"' && *it != '/'){
                str += *it;
                ++it;
            }
            // 应该使用 std::search
            string temp = str;
            transform(temp.begin(),temp.end(),temp.begin(),::tolower);
            auto temp1 = std::search(temp.begin(), temp.end(), error_str1.begin(), error_str1.end());
            auto temp2 = std::search(temp.begin(), temp.end(), error_str2.begin(), error_str2.end());

            if(temp1 == temp.end() && temp2 == temp.end()){
                if(std::all_of(str.begin(),str.end(),[](char c){return c != ':' && c != '#';}))
                s.insert(str);
            }
        }
        else{
            break;
        }
    }
    return s;
}

int main() {
    /* Note if your file reading isn't working, please go to the
     * projects tab on the panel on the left, and in the run section,
     * uncheck the "Run in terminal" box and re-check it. This
     * should fix things.
     */
    string page_html;
    ifstream input;
    string filename;
    while(!input.is_open()){
        cout << "Enter a file name: ";
        string path = "res\\";
        getline(cin, filename);
        filename = path + filename;
        input.open(filename);
    }
    string line;
    while(getline(input,line)){
        std::istringstream iss(line);
        page_html += line;
        // line += '\n';
    }
    // cout << page_html << endl;
    // TODO: Create a filestream from the filename, and convert it into
    //       a single string of data called page_html (declared above)



    unordered_set<string> validLinks = findWikiLinks(page_html);
    set<string> ss;
    cout << "validLinks is : ";
    for(auto s: validLinks){
        ss.insert(s);
    }
    for(auto s: ss){
        cout << s << endl;
    }
    // TODO: Print out the validLinks set above.
    //       You can print it out in any format; this is just for you
    //       to be able to compare your results with the [output] results
    //       under the /res folder.

    // Write code here

    return 0;
}



