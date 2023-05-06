if [ ! -d "build" ]; then
	mkdir build
fi

cd build
cmake ./..
make
echo "Output:"
echo ""
./WorldCreator
