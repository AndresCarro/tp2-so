# How to compile

1. Download Docker image to compile

```
docker pull agodio/itba-so:1.0
```

2. Run a container in the folder of this project

```
docker run -v "${PWD}:/root" --privileged -ti agodio/itba-so:1.0 
```

3. Compile Toolchain

```
cd Toolchain
make all
cd ..
```

4. Compile Kernel

```
make all
```

# Run Kernel

We use QEMU to run the kernel.

```
./run.sh
```

### GDB

To run with GDB, run the following command in the terminal

```
./run.sh gdb
```

And in another terminal of the same docker container, run

```
gdb
```