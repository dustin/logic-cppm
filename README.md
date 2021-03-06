# CPPM Analyzer for Saleae Logic

This plugin for [Saleae Logic][logic] allows you to analyze a CPPM
stream as commonly found in RC receivers.

![logic analyzer](https://github.com/dustin/logic-cppm/raw/master/docs/ppm-logic.png)

## Configuration

![config](https://github.com/dustin/logic-cppm/raw/master/docs/cfg.png)

Defaults should generally work, but there are a few parameters that
may occasionally come in handy:

### PPM Sync Time Minimum

This is the minimum amount of time signal will be held high (or low)
to be considered a frame boundary by the analyzer.  2500μS is good for
most streams.

### Sync Level

The sync signal may be high (normal) or low (inverted).  "Auto"
doesn't care whether it's high or low and just looks for anything
matching the min sync time.

### Maximum Channels

The number of channels within a frame is automatically determined in
most cases, however, there was a known bug in the
[FrSky D4R][frskybug] that led to channel values being truncated and
frames being smashed together making it impossible to infer a
boundary.

If you know your stream will only contain 8 channels, but are finding
things hinting at this bug, you can set the maximum channel count to 8
and instead of the severely limited pulse being considered channel 9,
it'll be marked as an error like this:

![frsky bug](https://github.com/dustin/logic-cppm/raw/master/docs/frsky_bug.png)

These errors are also flagged on export streams.  In wide form, the
row will be marked as an error when the channel count got reset
because of error.  In the long form, the actual erroring value will
appear on its own with its timestamp and width.

### Minimum Change

Minimum change is a sort of deadband across any changes that prevents
you from seeing too much noise.  If you set it to 0, there will be a
marker, label, and search result for each change.

The default of 3 means that a particular channel has to move at least
3μS before the value is considered significant.  Smaller changes will
still get a bubble label at the top, but they won't have a distinct
search result, nor will they get a change marker.

## Exporting

There are two export formats.

One is a wide format csv that is timestamped with the beginning of a
cppm frame and includes a column for every channel found in the frame.
e.g.:

```csv
Time [s],ch1,ch2,ch3,ch4,ch5,ch6,ch7,ch8,error
0.020300000000000,984,1502,1499,1504,1502,1501,1502,1932,
0.040300250000000,984,1502,1498,1504,1501,1502,1502,1932,
0.060300250000000,984,1502,1499,1504,1502,1501,1502,1932,
0.080300375000000,984,1503,1496,1505,1502,1501,1502,1932,
0.100300375000000,984,1503,1496,1504,1501,1502,1501,1932,
0.120300625000000,984,1504,1497,1504,1501,1502,1501,1940,
0.140300750000000,984,1505,1499,1504,1502,1501,1502,1940,
0.160300875000000,984,1502,1499,1505,1502,1502,1501,1940,
0.180300875000000,984,1502,1498,1505,1502,1501,1502,1940,
```

The second is a long form csv with a row for every channel picked up
at the timestamp where it was found.  e.g.:

```csv
Time [s],val,chan,error
0.000299875000000,984,1,
0.001284125000000,1502,2,
0.002786375000000,1499,3,
0.004285500000000,1504,4,
0.005789750000000,1502,5,
0.007291875000000,1501,6,
0.008793125000000,1502,7,
0.010295250000000,1932,8,
0.020300000000000,984,1,
```

In addition to giving you more exact timestamps per channel, this form
is handy for building long-format data frames in
[r](http://www.r-project.org/) for doing stuff like this in ggplot:

![4ch remix analysis](https://github.com/dustin/logic-cppm/raw/master/docs/4ch.png)

## Building

Download the [Logic SDK][sdk] and extract it somewhere on your
machine.  In my case, it's `~/stuff/SaleaeAnalyzerSdk-1.1.32`.

Clone the repo and cd into the top level of it:

    git clone https://github.com/dustin/logic-cppm
    cd logic-cppm

Symlink the `include` and `lib` directories into your tree:

    ln -s ~/stuff/SaleaeAnalyzerSdk-1.1.32/{include,lib} .

Run the build script:

    ./build_analyzer.py

## Installing

In the Develoepr tab in Logic preferences, specify the path for
loading new plugins, then copy the built plugin into that location:

    cp release/* /path/specified/in/Logic/preferences

[logic]: https://www.saleae.com/downloads
[sdk]: http://support.saleae.com/hc/en-us/articles/201104644-Analyzer-SDK
[frskybug]: http://diydrones.com/profiles/blogs/why-frsky-cppm-signal-is-so-disappointing
