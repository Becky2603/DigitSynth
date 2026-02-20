# Raspberry Pi Application

## How to Build

Generate build files: 

```
cmake -B build
```

Build: 

```
cmake --build build/
```

### Release mode
Run the following to compile with max optimisations and no debug symbols: 

```
cmake -B build -DRELEASE=1
cmake --build build/
```

## How to Run

```
./build/digitsynth
```

## Dependencies
