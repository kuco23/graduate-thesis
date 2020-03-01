folder_name="images"
video_name="julia_series"
fps=25

echo "Setting up the image folder ..."
mkdir -p $folder_name &&
rm -f $folder_name/*.ppm &&
echo "Compiling the code ..." &&
g++ julia.cpp lib/julia_polynomial_series.cpp lib/colors.cpp -o julia &&
echo "Finished compiling" &&
echo "Creating ppm files ..." &&
./julia.exe &&
echo "Finished making ppm files" &&
rm -f julia.exe &&
echo "Making video ..." &&
python ppms_to_avi.py $video_name $fps&&
echo "Finished video"