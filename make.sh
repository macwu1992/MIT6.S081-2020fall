#!/usr/bin/bash

current_script_dir=$(realpath "$0" | sed 's|\(.*\)/.*|\1|')
filenames=$(find ./lab_* -name '*.c' | xargs basename)
xv6PkgName=$1

appendLineNum=$(grep -n "UPROGS=" $current_script_dir/$xv6PkgName/Makefile | awk -F":" '{print $1}')
for file in $(find ./lab_* -name '*.c');
do
    cp $file $current_script_dir/$xv6PkgName/user/
done

for filename in $filenames;
do
    makeProgName=_${filename/.c/}
    # Example for appendLine 153s/$/\n\t\$U\/_sleep.c\\/
    appendLine=$appendLineNum"s"/\$/\\n\\t\\\$U\\\/$makeProgName\\\\\/
    sed -i $appendLine $current_script_dir/$xv6PkgName/Makefile

    appendLineNum=$((appendLineNum + 1))
done

cd $current_script_dir/$xv6PkgName/ && rm fs.img && make qemu

for filename in $filenames;
do
    rm $current_script_dir/$xv6PkgName/user/$filename
done

cd $current_script_dir/$xv6PkgName/ && git restore $current_script_dir/$xv6PkgName/Makefile