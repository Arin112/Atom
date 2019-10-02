dirs=( "./easy" "./easyAndBeautiful" "./medium" "./easy3D" "./hard3D" )
for d in "${dirs[@]}"
do
    cd $d
	make run
	cd ..
done
