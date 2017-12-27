#!/bin/bash

cd build
cmake .. && make -j4 && mv dungeon ..
cd ..
