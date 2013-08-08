LiveTuiles
==========

LiveTuiles is an interactive looper/sequencer based on libTuiles.

ANR INEDIT Project
SCRIME, LaBRI, Université de Bordeaux, France
Florent Berthaut - David Janin

#Requires:
* FLTK 1.X.X (http://www.fltk.org)
* libJack (http://jackaudio.org/)
* LLVM (http://llvm.org/)
* FAUST (http://faust.grame.fr/)
* libsndfile (http://www.mega-nerd.com/libsndfile/)
* libsamplerate (http://www.mega-nerd.com/SRC/)
* libTuiles (https://github.com/scrime/libTuiles)

#Compilation if libTuiles is not already installed:
mkdir tuiles
cd tuiles
git clone https://github.com/scrime/libTuiles.git libTuiles
git clone https://github.com/scrime/LiveTuiles.git LiveTuiles
cd libTuiles
./waf configure
./waf
cd ../LiveTuiles
./waf configure
./waf 

#Compilation if libTuiles has already been installed:
git clone https://github.com/scrime/LiveTuiles.git LiveTuiles
cd ../LiveTuiles
./waf configure
./waf 

#Installation: 
sudo ./waf install

#Execution:
./build/livetuiles  (or livetuiles after installation)

#Update the sources:
cd tuiles/libTuiles
git pull
./waf clean
./waf configure
./waf 
cd ../LiveTuiles
git pull
./waf clean
./waf configure
./waf


