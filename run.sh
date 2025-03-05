#!/bin/bash

stty rows 30 cols 80

echo -ne "\033]0;Sneak\007"

docker build -t snake-game-dev .
docker run --rm -it --name snake_game snake-game-dev
