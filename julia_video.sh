name="julia_polynomial_series"
folder_name="images"
fps=20

mkdir -p $folder_name &&
rm -f $folder_name/*.ppm &&
echo "Compiling the code ..." &&
g++ julia.cpp "lib/${name}.cpp" -o $name &&
echo "Finished Compiling" &&
echo "Creating ppm files ..." &&
./"${name}.exe" &&
echo "Finished making ppm files" &&
rm -f "${name}.exe" &&
echo "Making video ..." &&
python ppms_to_avi.py $name $fps&&
echo "Finished video"