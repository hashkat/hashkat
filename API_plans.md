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
See analyzer_api.cpp.
Queries are read from standard input, with a signal sent to indicate that standard input should be checked.
```

Current working features:
-------------------------
With enable_query_api enabled in INFILE, run:

echo -e '{"type":"add_tweet_stream","stream_path":"path"}\n' | ./run.sh
Press ctrl-c during execution to have tweets routed to 'path'.

