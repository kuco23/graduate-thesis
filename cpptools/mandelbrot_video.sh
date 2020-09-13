image_import_folder="temp_imgs"
video_export_folder="temp_vids"
fps=25

echo "Setting up the image folder ..."
mkdir -p $image_import_folder &&
rm -f $image_import_folder/*.ppm &&
echo "Compiling the code ..." &&
g++ mandelbrot.cpp lib/mandelbrot_zoom.cpp lib/color_mixer.cpp -o mandelbrot &&
echo "Creating ppm files ..." &&
./mandelbrot.exe &&
rm -f mandelbrot.exe &&
echo "Making video ..." &&
python ppms_to_avi.py $image_import_folder $video_export_folder"/mandelbrot_series.avi" $fps  &&
echo "Finished video"