#!/usr/bin/env bash

echo "<university></university>" > CorrectUniversity.xml
firstTestSaveLoadProcess="${UEADB} LoadUniversity CorrectUniversity.xml , ProcessApplication , SaveUniversity CorrectUniversity.xml"
echo "Test: ${firstTestSaveLoadProcess}"

${firstTestSaveLoadProcess}
if [ $? = 0 ]
then
    echo "Done!"
    rm CorrectUniversity.xml
else
    echo "Incorrect result: $?"
    rm CorrectUniversity.xml
    exit 1
fi
echo

echo "<something></something>" > IncorrectUniversity.xml
secondTestSaveLoadProcess="${UEADB} LoadUniversity IncorrectUniversity.xml"
echo "Test: ${secondTestSaveLoadProcess}"

${secondTestSaveLoadProcess}
if [ $? = 3 ]
then
    echo "Done!"
    rm IncorrectUniversity.xml
else
    echo "Incorrect result: $?"
    rm IncorrectUniversity.xml
    exit 1
fi
echo

firstTestNewRemoveFaculty="${UEADB} RemoveFaculty FacultyOfFaculties"
echo "Test: ${firstTestNewRemoveFaculty}"

${firstTestNewRemoveFaculty}
if [ $? = 5 ]
then
    echo "Done!"
else
    echo "Incorrect result: $?"
    exit 1
fi

secondTestNewRemoveFaculty="${UEADB} NewFaculty FacultyOfFaculties , NewFaculty FacultyOfFaculties"
echo "Test: ${secondTestNewRemoveFaculty}"

${secondTestNewRemoveFaculty}
if [ $? = 5 ]
then
    echo "Done!"
else
    echo "Incorrect result: $?"
    exit 1
fi

thirdTestNewRemoveFaculty="${UEADB} NewFaculty FacultyOfFaculties , RemoveFaculty FacultyOfFaculties"
echo "Test: ${thirdTestNewRemoveFaculty}"

${thirdTestNewRemoveFaculty}
if [ $? = 0 ]
then
    echo "Done!"
else
    echo "Incorrect result: $?"
    exit 1
fi
