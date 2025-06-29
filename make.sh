#!/usr/bin/bash

current_script_dir=$(realpath "$0" | sed 's|\(.*\)/.*|\1|')
xv6PkgName=$1

if [[ -z "$xv6PkgName" ]]; then
    xv6PkgName="xv6-labs-2020"
fi

cFiles=$(find ./lab_* -name '*.c') && cFiles=(${cFiles})

for file in ${cFiles[@]};
do
    echo -E "copying $file to $current_script_dir/$xv6PkgName/user/"
    cp $file $current_script_dir/$xv6PkgName/user/
done

filenames=()
for filename in ${cFiles[@]};
do
    filename=$(basename $filename)
    filenames+=($filename)
done

appendLineNum=$(grep -n "UPROGS=" $current_script_dir/$xv6PkgName/Makefile | awk -F":" '{print $1}')
for filename in ${filenames[@]};
do
    makeProgName=_${filename/.c/}
    # Example for appendLine 153s/$/\n\t\$U\/_sleep.c\\/
    appendLine=$appendLineNum"s"/\$/\\n\\t\\\$U\\\/$makeProgName\\\\\/
    sed -i $appendLine $current_script_dir/$xv6PkgName/Makefile

    appendLineNum=$((appendLineNum + 1))
done

cd $current_script_dir/$xv6PkgName/ && make qemu

for filename in ${filenames[@]};
do
    rm $current_script_dir/$xv6PkgName/user/$filename
done

cd $current_script_dir/$xv6PkgName/ && git restore $current_script_dir/$xv6PkgName/Makefile