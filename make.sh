current_script_dir=$(realpath "$0" | sed 's|\(.*\)/.*|\1|')
filenames=$(find ./lab_* -name '*.c' | xargs basename)

for file in $(find ./lab_* -name '*.c');
do
    cp $file $current_script_dir/xv6-labs-2020/user/
done

cd $current_script_dir/xv6-labs-2020/ && make qemu

for filename in $filenames;
do
    rm $current_script_dir/xv6-labs-2020/user/$filename
done