import subprocess
import re

# Number of iterations
iterations = 10000

# Commands to execute
noms = ["Binary", "temperature", "working", "multiplication"]
commands = ["bash -c 'time ./Binary'", "bash -c 'time ./temperature'", "bash -c 'time ./working'", "bash -c 'time ./multiplication'"]
max_time = [0.0, 0.0, 0.0, 0.0] # in seconds

# Regex pattern to extract time
pattern = re.compile(r'real\s+(\d+)m(\d+\.\d+)s')

# Loop to execute the specified commands
for command in commands:
    for i in range(iterations):
        result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, shell=True)
        match = pattern.search(result.stderr)
        if match:
            minutes = float(match.group(1))
            seconds = float(match.group(2))
            time = minutes * 60 + seconds
            if time > max_time[commands.index(command)]:
                max_time[commands.index(command)] = time

# Print max time for each task
for i in range(len(max_time)):
    print("Max time for task " + str(i) + " : " + str(max_time[i]) + " s")