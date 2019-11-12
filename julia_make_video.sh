name="julia_polynomial_series"
folder_name="images"

mkdir -p $folder_name &&
rm $folder_name/*.ppm &&
echo "Compiling the code ..." &&
g++ julia_polynomial_series.cpp -o $name &&
echo "Finished Compiling" &&
echo "Creating ppm files ..." &&
./julia_polynomial_series.exe &&
echo "Finished making ppm files" &&
rm julia_polynomial_series.exe &&
echo "Making video ..." &&
python ppms_to_avi.py $name &&
echo "Finished video"