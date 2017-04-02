#!/bin/bash

for file in /instanciasPRPP/*
do
	./branch_and_bound "$file" 1 >> output.txt
done