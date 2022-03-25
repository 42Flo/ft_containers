#!/bin/bash

CC="c++"
FLAGS="-Wall -Wextra"

containers=(vector)
path_containers="../containers"
path_tools="../tools"

path_bin="bin"
path_output="output"
path_errors="compile_errors"
path_diff="diff"

compareOutputs()
{
    echo
}

# $1=container $2=file_to_compile
executeTest()
{
    name="$1_${2%.*}"

    echo $name

    $CC $FLAGS -o $path_bin/ft_$name -I $path_containers -I $path_tools $1/$2 2> $path_errors/ft_$name
    [ -s $path_errors/ft_$name ] || rm $path_errors/ft_$name # delete error file if empty

    $CC $FLAGS -o $path_bin/std_$name -I $path_containers -I $path_tools -D STL $1/$2 2> $path_errors/std_$name
    [ -s $path_errors/std_$name ] || rm $path_errors/std_$name

    ./$path_bin/ft_$name > $path_output/ft_$name.txt 2> /dev/null
    ./$path_bin/std_$name > $path_output/std_$name.txt 2> /dev/null

    diff $path_output/ft_$name.txt $path_output/std_$name.txt > $path_diff/$name.txt
    [ -s $path_diff/$name.txt ] || rm $path_diff/$name.txt

    #TODO print diff etc..
}

# $1=container
initTests()
{
    files=$(ls $1 | grep .cpp | sort)

    for file in $files
    do
        executeTest $1 $file
    done
}

mkdir -p $path_bin
mkdir -p $path_output
mkdir -p $path_errors
mkdir -p $path_diff

for container in $containers
do
    initTests $container
done
