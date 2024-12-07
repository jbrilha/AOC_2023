#!/bin/zsh

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 <day_number> <part_number>"
    exit 1
fi

day_number=$1
part_number=$2

day_number=$(printf "%02d" $day_number)
day_folder=$(printf "day%02d" $day_number)

if [[ ! -d "$day_folder" ]]; then
    echo "Error: Day folder '$day_folder' not found."
    exit 1
fi

executable="d${day_number}p${part_number}"

if [[ ! -x "$day_folder/$executable" ]]; then
    echo "Error: Executable '$executable' not found in '$day_folder'."
    exit 1
fi

"$day_folder/$executable" "$day_folder/in.txt"
