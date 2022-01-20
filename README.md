# C Structures sample

### Requirements
* cmake 3.21.1
* ninja 1.10.2

### Build
```shell
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -Bbin -H. -G Ninja .
cmake --Bbin --target run.me
```

### Run
```shell
./bin/run.me
```