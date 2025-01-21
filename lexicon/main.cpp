#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <thread>
#include <future>
#include <boost/algorithm/string.hpp>
#include <boost/locale.hpp>

using namespace std;
using namespace boost::locale;

namespace odds {
    const string STOP_WORDS_FILE = "./resources/stop_words.txt";

    class Lexicon {
        private:
            string s;
            int max_workers;
            vector<string> stop_words;
            
            void load_stop_words() {
                ifstream file(STOP_WORDS_FILE);
                string word;
                while(file >> word) {
                    stop_words.push_back(word);
                }
            }

            void process_part(const string &part) {
                generator gen;
                locale loc = gen("");
                locale::global(loc);
                cout.imbue(loc);

                // tokenize string
                vector<string> tokens;
                boost::split(tokens, part, boost::is_any_of(" \t\r\n"));

                // remove stop words
                tokens.erase(remove_if(tokens.begin(), tokens.end(), [this](const string &token) {
                    return find(stop_words.begin(), stop_words.end(), token) != stop_words.end();
                }), tokens.end());

                // process tokens
                for(const auto &token : tokens) {
                    cout << "token: " << endl;
                }
            }
        public:
            Lexicon(string s) : s(s), max_workers(3) {}
            
            void run() {
                vector<future<void>> futes;

                // Split the string into parts
                size_t part_size = s.size() / max_workers;

                for(int i=0; i<max_workers; i++) {
                    size_t start = i * part_size;
                    size_t end = (i == max_workers - 1) ? s.size() : (i + 1) * part_size;
                    string part = s.substr(start, end - start);

                    futes.push_back(async(launch::async, &Lexicon::process_part, this, part));
                }

                for(auto &f : futes) {
                    f.get();
                }
            }
    };
}

using namespace odds;

int main(int argc, char *argv[]) {
    Lexicon lx("testing things out for now...");
    lx.run();

    return 0;
}