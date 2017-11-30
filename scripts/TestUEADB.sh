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

firstTestAddRemoveEnrollee="${UEADB} RemoveEnrollee XX0000000"
echo "Test: ${firstTestAddRemoveEnrollee}"

${firstTestAddRemoveEnrollee}
if [ $? = 5 ]
then
    echo "Done!"
else
    echo "Incorrect result: $?"
    exit 1
fi

secondTestAddRemoveEnrollee="${UEADB} AddEnrollee FileIsNotExists.xml"
echo "Test: ${secondTestAddRemoveEnrollee}"

${secondTestAddRemoveEnrollee}
if [ $? = 3 ]
then
    echo "Done!"
else
    echo "Incorrect result: $?"
    exit 1
fi

echo "<enrollee passport=\"AB1234567\" rodSubject=\"\" rodType=\"4\"></enrollee>" > CorrectEnrollee.xml
thirdTestAddRemoveEnrollee="${UEADB} AddEnrollee CorrectEnrollee.xml , RemoveEnrollee AB1234567"
echo "Test: ${thirdTestAddRemoveEnrollee}"

${thirdTestAddRemoveEnrollee}
if [ $? = 0 ]
then
    echo "Done!"
    rm CorrectEnrollee.xml
else
    echo "Incorrect result: $?"
    rm CorrectEnrollee.xml
    exit 1
fi

firstTestAddRemoveSpecialty="${UEADB} RemoveSpecialty SomeFaculty SomeSpecialty"
echo "Test: ${firstTestAddRemoveSpecialty}"

${firstTestAddRemoveSpecialty}
if [ $? = 5 ]
then
    echo "Done!"
else
    echo "Incorrect result: $?"
    exit 1
fi

secondTestAddRemoveSpecialty="${UEADB} NewFaculty SomeFaculty , AddSpecialty SomeFaculty FileIsNotExists.xml"
echo "Test: ${secondTestAddRemoveSpecialty}"

${secondTestAddRemoveSpecialty}
if [ $? = 3 ]
then
    echo "Done!"
else
    echo "Incorrect result: $?"
    exit 1
fi

echo "<specialty name=\"SomeSpecialty\" maxEnrollees=\"50\" isPedagogical=\"false\"></specialty>" > CorrectSpecialty.xml
thirdTestAddRemoveSpecialty="${UEADB} NewFaculty SomeFaculty , AddSpecialty SomeFaculty CorrectSpecialty.xml , RemoveSpecialty SomeFaculty SomeSpecialty"
echo "Test: ${thirdTestAddRemoveSpecialty}"

${thirdTestAddRemoveSpecialty}
if [ $? = 0 ]
then
    echo "Done!"
    rm CorrectSpecialty.xml
else
    echo "Incorrect result: $?"
    rm CorrectSpecialty.xml
    exit 1
fi
