echo "press any key to go to next model"
for d in */ ; do
    cd $d
	make run
	cd ..
done
