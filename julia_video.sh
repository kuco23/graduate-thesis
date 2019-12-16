name="julia_series"
folder_name="images"
fps=40

mkdir -p $folder_name &&
rm -f $folder_name/*.ppm &&
echo "Compiling the code ..." &&
g++ julia.cpp lib/julia_polynomial_series.cpp lib/color_picker.cpp -o julia &&
echo "Finished Compiling" &&
echo "Creating ppm files ..." &&
./julia.exe &&
echo "Finished making ppm files" &&
rm -f julia.exe &&
echo "Making video ..." &&
python ppms_to_avi.py $name $fps&&
echo "Finished video"