# fpcomp

This program is a reimplementation of `acoustid_compare.c` from [AcoustID utility functions for PostgreSQL] to run as a command. It compares two Acoustid fingerprints and outputs a similarity percentage.


### Build
```shell
make
```


### Usage
```shell
fpcomp a.bin b.bin
```

`a.bin` and `b.bin` are Acoustid fingerprint binary files and should contain an array of raw signed 32 bit integers generated by [Chromaprint fpcalc].


[AcoustID utility functions for PostgreSQL]: https://github.com/acoustid/pg_acoustid
[Chromaprint fpcalc]: https://acoustid.org/chromaprint
