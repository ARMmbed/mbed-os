# mbed-os 5 example application using mbed-trace library

## define mbed-os.lib

```
echo https://github.com/armmbed/mbed-os/#6a0a86538c0b9b2bfcc4583b1e2b7fea8f4e71e9 > mbed-os.lib
```

## build

```
mbed deploy
mbed compile -t GCC_ARM -m K64F
```


## Usage

When you flash a target with this application and open a terminal you should see the following traces:

```
[INFO][main] Hello tracers
[DBG ][main] Infinite loop..
[DBG ][main] Infinite loop..
[DBG ][main] Infinite loop..
...
```
