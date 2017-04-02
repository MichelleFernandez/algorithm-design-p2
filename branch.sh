#!/bin/bash

for file in instanciasPRPP/*
do
	./branc_and_bound.out "$file" 1 >> output.txt
done