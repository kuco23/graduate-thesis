new-item -itemtype directory -f -path images;
remove-item images/*.ppm;
g++ julia_polynomial_series.cpp -o julia_polynomial_series;
start-process julia_polynomial_series.exe -nonewwindow -wait;
remove-item julia_polynomial_series.exe;
python toavi.py julia_polynomial_series;