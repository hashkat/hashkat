/*
 * This file is part of the #KAT Social Network Simulator.
 *
 * The #KAT Social Network Simulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The #KAT Social Network Simulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the #KAT Social Network Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Addendum:
 *
 * Under this license, derivations of the #KAT Social Network Simulator typically must be provided in source
 * form. The #KAT Social Network Simulator and derivations thereof may be relicensed by decision of 
 * the original authors (Kevin Ryczko & Adam Domurad, Isaac Tamblyn), as well, in the case of a derivation,
 * subsequent authors. 
 */

#include <iostream>
#include <iomanip>
#include <fstream>

#include <map>
#include <memory>
#include "analyzer.h"
#include "serialization.h"
#include "io.h"

using namespace std;

typedef shared_ptr<fstream> fstream_ptr;

// Uses global variables for convenience, as there can only be one API-serving instance:
struct ApiState {
    map<string, fstream_ptr> tweet_pipes;
};

static ApiState api;

static void handle_api_request(const string& line) {
    stringstream str_stream {line};
    cereal::JSONInputArchive ar {str_stream};
    string type;
    ar(NVP(type));
    if (type == "add_tweet_stream") {
        string stream_path;
        ar(NVP(stream_path));
        printf("Adding tweet stream '%s'\n", stream_path.c_str());
        api.tweet_pipes[stream_path] = fstream_ptr {new fstream {stream_path, fstream::out}};
    } else if (type == "remove_tweet_stream") {
        string stream_path;
        ar(NVP(stream_path));
        api.tweet_pipes.erase(stream_path);
    }
}

// Write out tweets to the locations specified by the API: 
void analyzer_api_tweet(AnalysisState& state, Tweet& tweet) { 
    for (pair<string, fstream_ptr> entry : api.tweet_pipes) {
        stringstream str_stream;
        JsonWriter writer {state, str_stream};
        tweet.api_serialize(writer);
        string tweet = str_stream.str();
        tweet.erase(std::remove(tweet.begin(), tweet.end(), '\n'), tweet.end());
        // Output the tweet data on a single line:
        *entry.second << tweet << '\n';
        // Make sure the file stream is not buffered:
        flush(*entry.second);
    }
}

// Read an API request from a single line of standard input. 
void analyzer_handle_api_request(AnalysisState& state) {
    std::string line;
    std::getline(std::cin, line);
    std::cout << line << std::endl;
    handle_api_request(line);
}
