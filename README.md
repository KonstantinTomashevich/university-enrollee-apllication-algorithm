# University Enrollee Application Algorithm
Framework for automatic enrollees application to university, where each enrollee can specify its priority and choice more than 1 specialty.

## Project parts
### UEAA
(University Enrollee Application Algorithm)

Framework, which contains algorithm classes (`Enrollee`, `EnrolleeChoice`, `University`, `Faculty`, `Specialty`) and some utils (hashing, reference counting). Any algorithm class can be serialized to XML. `University` contains all `Enrollee`s and all `Faculty`s, each `Faculty `contains its `Specialty`s. Also `University` can process `Enrollee`s application using `ProcessEnrolleesApplication`.

### UEADB
(University Enrollee Application Data Base)

Executable, uses UEAA as library to provide convenience work with UEAA classes. Implements all UEAA features as commands (`LoadUniversity`, `SaveUniversity`, `ProcessApplication`, `AddSpecialty`, etc), commands can share values using shared context, each command separated with ','. Example UEADB call:
```bash
./UEADB LoadUniversity "MyUniversity.xml" , ProcessApplication , SaveUniversity "MyProcessedUniversity.xml"
```

### EnrolleeRandomizer
Executable, primary used for testing and debugging UEAA and UEADB. Generates enrollees from given XML and updates their exams results and certificate marks using random.

### UEAA Tests
UEAA unit tests, located in `Tests` directory.

### Scripts
Small useful shell scripts.

#### PackUniversity
Packs unpacked university (see below) using UEADB. `UEADB` environment variable must be specified.

#### TestUEADB
Tests UEADB commands with some basic combinations. `UEADB` environment variable must be specified.

### Dependencies
They are located in `Dependencies` dir.

#### TinyXML2
[GitHub repository.](https://github.com/leethomason/tinyxml2)

Cool library for XML parsing.

#### CrossPlatformMain
Replaces `main` and provides `CrossPlatformMain` function, which is useful for cross platform command line arguments parsing. Used in UEADB and EnrolleeRandomizer.

## How to build
Build system is quite simple. `CMake` required!
```bash
mkdir build && cd build && cmake .. && make && make test
```
**Note:** `make test` runs UEAA tests. To test UEADB you should run `TestUEADB.sh` manually.

## About university format
## Internal structure

## File format
University XML can be packed or unpacked. Packed means that all university data (with enrollees) is packed into one XML file. It is normal behaviour of `University::SaveToXML`.

Unpacked means that university data was divided to special directories. Unpacked university can be converted to packed using `PackUniversity.sh`.

Unpacked university structure:

<root folder>
--- `.university.xml` --- *[optional]* University XML, will be loaded before any actions.

--- `Faculties` Folder with faculties.

--- --- `Faculty1` Folder with faculty, folder name = faculty name.


--- --- --- `Specialty1.xml` Specialty XML, will be loaded and added to this faculty.

--- `Enrollees` Folder with enrollees, enrollees must be saved in subdirectories (for example, but passport series).

--- --- `AA` Folder with enrollees with `AA` passport series.

--- --- --- `AA1234567.xml` Enrollee XML, will be loaded.
