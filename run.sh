# create a build folder if it does not already exist
if [ ! -d "build" ]; then
	mkdir build
fi

cd build

# remove the res folder
if [ -d "res" ]; then
	rm -rf res
fi

# copy the new res folder inside the directory
cp -r ./../res/ ./

cmake ./..
make
echo "Output:"
echo ""
./WorldCreator
