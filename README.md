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

### Dependencies
They are located in `Dependencies` dir.

#### TinyXML2
[GitHub repository.](https://github.com/leethomason/tinyxml2)

Cool library for XML parsing.

#### CrossPlatformMain
Replaces `main` and provides `CrossPlatformMain` function, which is useful for cross platform command line arguments parsing. Used in UEADB and EnrolleeRandomizer.
