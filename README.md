# ccze-cute

A [ccze](https://github.com/cornet/ccze) plugin for cute::log output (see [cutestation](https://github.com/ngc7293/cutestation), [rocketsd](https://github.com/ngc7293/rocketsd))

## Brief

A small ccze plugin for the log format I use in some of my projects. The expected format is:

```
[datetime] (level) <thread threadid> [component] message
```

The colors are currently hardcoded.

## Requirements

- CMake 3.13+
- ccze
- pcre

## Building

```bash
mkdir build; cd build;
cmake -G Ninja ..
ninja
sudo ninja install
```

## Running

```bash
./app | ccze -m ansi -p cute
```
