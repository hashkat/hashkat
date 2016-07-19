Plans to develeop the *REST API*.....


###On the python side:
```
import os
import time
import json

path = "input/queries.json"
os.mkfifo(path)

# TODO Create another pipe this way for hashkat to write results into

fifo = open(path, "w")
while True:
	time.sleep(1)
	# Write one query into the pipe
	# TODO send a signal to hashkat
	query = {"query": {"query_type": "agent_info", "agent_id": -1, "tweet_id": -1}}
    	query_string = json.dumps(query)
	fifo.write(query_string + '\n')
	fifo.flush()
fifo.close()
```

###On the C++ side 
```
void handle_query() {
	// TODO have incoming_queries ifstream
	// TODO have query_results ofstream
	string line;
	getline(incoming_queries, line);
	cout << "Line: " << line << endl;
	stringstream line_stream(line);
	JsonReader reader {state, line_stream};
	Query query; // Some Query object with every field of every query
	reader(NVP(query));
	// TODO write line of query result 
	cout << "Query: " << query.query_type << endl;

}
```

