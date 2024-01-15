#!/bin/bash

# Number of iterations
iterations=1000

# Commands to execute
commands=("./Binary" "./temperature" "./working" "./multiplication")
declare -A max_execution_times

# Loop to execute the specified commands
for command in "${commands[@]}"; do
    max_execution_time=0

    for ((i=0; i<$iterations; i++)); do
        # Execute the command here and measure the execution time with time -p
        output=$(/usr/bin/time -p $command 2>&1 | awk '/real/ {print $2}')
        # Retrieve the execution time if it is in the expected format
        if [[ $output =~ ^[0-9]+\.[0-9]+$ ]]; then
            # Convert the time to milliseconds without rounding
            time_in_milliseconds=$(bc <<< "$output * 1000")
            if (( $(echo "$time_in_milliseconds > $max_execution_time" | bc -l) )); then
                max_execution_time=$time_in_milliseconds
            fi
        fi
    done

    max_execution_times[$command]=$max_execution_time
done

# Print all the maximum execution times at the end
for command in "${!max_execution_times[@]}"; do
    echo "Max time for $command : ${max_execution_times[$command]} ms"
done