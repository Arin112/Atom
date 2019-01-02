dirs=( "./easy" "./easyAndBeautiful" "./medium" "./hard" )
for d in "${dirs[@]}"
do
    cd $d
	make run
	cd ..
done
