#!/bin/bash

cd libs_dir
make clean
make
cd ..
cd exec_dir
make clean
make
