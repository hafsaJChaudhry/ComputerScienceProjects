HW5 , FFT 25 pts
 
Use your browser to listen to  .wav  files.

you can use your browser to listen to files and to
copy files into your directory.
You and copy my files on gl.umbc.edu using:
cp  /afs/umbc.edu/users/s/q/squire/pub/download/xxx.wav .
for xxx= cat, short_count, train  etc.

Take a .wav file. Listen to it.
Extract the amplitude data from the .wav file.
Use a FFT to get the frequency spectrum.
Modify the frequency spectrum
Use inverse FFT to get amplitude data.
Insert the modified amplitude data into the .wav file.
Listen to the new .wav file.

Just do one of, a) b) c)

a) delete high and low frequencies from the spectrum or
   perform some other modification. Comment on how the
   sound changed.

b) Move all of the spectrum from i to n down into 1 to n-i,
   zero spectrum from n to top. You should be able to
   understand the modified .wav yet it will be a much
   deeper voice. You have lowered the frequency.

c) Mess up the sound with some other change to the spectrum.
 
Look at the spectrum plots. It depends on what code you use:
Possibly leave the zeroth frequency alone or zero it.
For N=2^n, leave the N/2 as it is, this is the Nyquist frequency.
Moving the top half down and the bottom half up, raises the pitch.
It also introduces noise.


Sample code for FFT and IFFT provided.
copy whatever you need from Lecture 18, FFT

Sample code to read and write a .wav is provided for a few languages.

waveplay.m reads a .wav file, plays the file
at three speeds, then writes a .dat file with just the sound.
The .dat file can be read with %f format. soundplay.m
Will play a .dat file, that may be read, modified, and rewritten.
In Matlab, if it works for you, use fft_wav.m

In Python, hack sound.py and use Python's fft and ifft.
Python sound may only work on Microsoft Windows.
UMBC screwed up Python, use your own computer and use
pip install on packages such as  wave  scipy  etc.
fftwav.py3 for fft and ifft on .wav


In Java, hack ClipPlayerTest.java and use
my Cxfft.java for fft and ifft functions.
There is read_wav.java to read
and write  .wav  files.
Or, if it works for you, use fft_wav.java
With a plot, use fft_frame.java

In Ubuntu using plain "C", hack pcm_dat.c and use any of the FFT and
IFFT written in "C".

The program read_wav.c reads and writes a .wav file.
You can put a FFT in between the read and write for your homework.
Use common existing software to play the .wav file(s).

You may need to convert complex values to magnitude.
Given a+bi  amplitude = sqrt(a*a+b*b)

Turn in paper or submit your source code, 
submit input .wav files and output .wav files.

submit cs455 hw5 your-files

(option: use just ascii files of numbers, read and write with %f
 MatLab code  soundplay.m plays file.
 short_count.dat is one two three.)

See Lecture 18 for more information
