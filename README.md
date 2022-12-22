# RECON tool

A `C++` based web endpoint enumerator.

## Compile
> Make sure to have libcurl installed
```
g++ -o recon recon.cpp -lcurl
```

## Usage
```
./recon -u <target-URL> -w <wordlist>
./recon -u <target-URL> -w <wordlist> -r <max requests per minute>
```

## TODO

* Add custom HTTP headers
