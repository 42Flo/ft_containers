#!/bin/bash

RED="\e[91m"
GREEN="\e[92m"
BOLD="\e[1m"
ULINE="\e[4m"
RESET="\e[0m"

CC="c++"
FLAGS="-Wall -Wextra -g -fsanitize=address"

containers=(vector stack map)
path_containers="../containers"
path_tools="../tools"
path_it="../iterators"

path_bin="bin"
path_output="output"
path_errors="compile_errors"
path_diff="diff"

compile_error=0
diff_error=0

# $1=file_name
printLine()
{
    printf "$1\tCOMPILE$BOLD" | expand -t 25
    if [ $compile_error -eq 0 ]; then
        printf "$GREEN OK"
    else
        printf "$RED KO"
    fi
    printf "$RESET\tDIFF$BOLD" | expand -t 10
    if [ $diff_error -eq 0 ]; then
        printf "$GREEN OK\t"
    else
        printf "$RED KO\t"
    fi
    printf "$RESET\n"
}

# $1=file_name
getCompileErrors()
{
    [ ! -s $path_errors/ft_$1 ] || compile_error=1
}

# $1=file_name
getDiff()
{
    [ ! -s $path_diff/$1.txt ] || diff_error=1
}

# $1=container $2=file_to_compile
executeTest()
{
    name="$1_${2%.*}"
    pname="${2%.*}"
    include="-I$path_containers -I$path_tools -I$path_it"

    $CC $FLAGS -o $path_bin/ft_$name $include $1/$2 2> $path_errors/ft_$name
    [ -s $path_errors/ft_$name ] || rm $path_errors/ft_$name # delete error file if empty

    $CC $FLAGS -o $path_bin/std_$name $include -D STL $1/$2 2> $path_errors/std_$name
    [ -s $path_errors/std_$name ] || rm $path_errors/std_$name

    ./$path_bin/ft_$name > $path_output/ft_$name.txt 2> /dev/null
    ./$path_bin/std_$name > $path_output/std_$name.txt 2> /dev/null

    diff $path_output/ft_$name.txt $path_output/std_$name.txt > $path_diff/$name.txt
    [ -s $path_diff/$name.txt ] || rm $path_diff/$name.txt

    getCompileErrors $name
    getDiff $name
    printLine $pname

    #TODO time diff
}

# $1=container
initTests()
{
    files=$(ls $1 | grep .cpp | sort)

    printf "$ULINE$BOLD$1$RESET\n"

    for file in $files
    do
        compile_error=0
        diff_error=0
        executeTest $1 $file
    done
}

./clean.sh

mkdir -p $path_bin
mkdir -p $path_output
mkdir -p $path_errors
mkdir -p $path_diff

if [[ ! ${containers[*]} =~ $1 ]]
then
    printf "Bad arguments.\n"

    exit
fi

if [ $# -ne 0 ]
then
    containers=($1)
fi

for container in ${containers[@]}
do
    initTests $container
done
