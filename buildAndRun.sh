dpkg-query -l freeglut3-dev > /dev/null || sudo apt-get install freeglut3-dev
for d in */ ; do
    cd $d
	make run
	cd ..
done
