#!/usr/bin/env python 

from flask import Flask
from flask import Response
from flask import json
import json
import tempfile
import subprocess
import atexit
import os

app = Flask(__name__)

# Spawn using ./run.sh. 
# A single instance of hashkat that lasts as long as the server is up is assumed, for now.
# The INFILE used should have no limits on time, either simulated or real.

hashkat = subprocess.Popen(['./run.sh', '--input', 'API_INFILE.yaml', '--no-ctrlc'], stdin=subprocess.PIPE)
def close_hashkat():
    os.system('kill -9 ' + str(hashkat.pid))
atexit.register(close_hashkat)
def send_command(cmd_object):
    cmd_str = json.dumps(cmd_object)
    hashkat.stdin.write(cmd_str + '\n')
    hashkat.stdin.flush()
temp_dir = tempfile.mkdtemp()

n_pipes_made = 0

@app.route('/tweets.json', methods = ['GET'])
def make_stream_api():
    # Make sure we create a unique pipe for each request. 
    global n_pipes_made
    n_pipes_made += 1
    # Set up a tweet pipe to listen to.
    pipe_path = temp_dir + '/tweet_pipe_' + str(n_pipes_made) + '.fifo'
    os.mkfifo(pipe_path)
    # Inform the hashkat instance of the pipe:
    send_command({"type": "add_tweet_stream", "stream_path": pipe_path})
    # Open the pipe for reading our tweet stream from:
    pipe = open(pipe_path, 'r')
    # Cleanup method to be called upon disconnect: 
    def cleanup():
        send_command({"type": "remove_tweet_stream", "stream_path": pipe_path})
        pipe.close()
        os.remove(pipe_path)
    def generate():
        try:
            for line in iter(pipe.readline, ''):
                tweet = json.loads(line)
                # TODO Transform tweet to meet Twitter API
                yield json.dumps(tweet) + '\n'
        finally:
            cleanup()
    return Response(generate(), status=200, content_type='application/json')
if __name__== '__main__':
    app.run(threaded=True)
