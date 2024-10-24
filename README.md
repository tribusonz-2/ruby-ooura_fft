# OouraFFT
## Ooura FFT front-end for Ruby

This is a Ruby's front end that allows to use FFT software of Professor Ooura (Kyoto University).

After downloading, unzip it to any directory, go to the lib directory, and type the following in the terminal to import it.
```Bash
$ ruby extconf.rb
$ make
$ make install
$ make distclean
```

## Usage

Now, there are a total of 6 FFT routines, which are as follows:
```
    cdft: Complex Discrete Fourier Transform
    rdft: Real Discrete Fourier Transform
    ddct: Discrete Cosine Transform
    ddst: Discrete Sine Transform
    dfct: Cosine Transform of RDFT (Real Symmetric DFT)
    dfst: Sine Transform of RDFT (Real Anti-symmetric DFT)
```
These routines are owned by the OouraFFT module.  

Since the software's using is a discrete type, the ABI makes use of arrays.  
In Ruby, it is communicated with the Array class. The Array class is used as an argument, and the FFT computed Array class is the return value.  

The design is unified as follows:  
```Ruby
    OouraFFT.xxxx(*nusec, inversion: bool)
```
This `*nusec` is the Array class. Internally, it is treated as a numeric sequence.  
It also supports inverse FFT, and works by applying `true` to the keyword argument `inversion`. The default value is `false`.  

```Ruby
    def sinewave(amp, f0, fs, n)
      amp * Math.sin(2 * Math::PI * f0 * n / fs)
    end

    OouraFFT.ddct(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
    #=> [0.6284174365157309,
    #=> -0.6284174365157309,
    #=>  0.18707572033318604,
    #=> -0.18707572033318612,
    #=>  0.08352232973991239,
    #=> -0.08352232973991236,
    #=>  0.02486404592245728,
    #=> -0.024864045922457167]

    OouraFFT.ddst(Array.new(8){|n| sinewave(0.25, 250.0, 8000, n)})
    #=> [0.875,
    #=>  0.12500000000000003,
    #=> -0.12499999999999997,
    #=>  0.125,
    #=> -0.125,
    #=>  0.12500000000000006,
    #=> -0.12499999999999994,
    #=>  0.12499999999999994]
```

For more information on how to the fourier transform, please refer to the technical book.  
