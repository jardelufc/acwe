# acwe
Active Contours Without Edges

# use

mkdir build
cd build
cmake ..
make
./main todo.mhd saida_.raw original.raw 200
python raw2npy
python mostrarnpy
