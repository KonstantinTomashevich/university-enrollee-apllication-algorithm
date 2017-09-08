#!/usr/bin/env bash
echo "Starting PackUniversity..."
echo "UEADB executable: ${UEADB}"
if [ "${UEADB}" = "" ]
then
    echo "Please, set UEADB global variable, which points to UEADB executable."
    exit 1
fi
echo "WARNING! Path must not contain whitespaces!"

UEADBCommand="${UEADB}"
if [ -e .university.xml ] 
then
    UEADBCommand="${UEADBCommand} LoadUniversity .university.xml"
fi

for faculty in $(cd Faculties && ls -d */ && cd ..)
do
    facultyName=`expr "$faculty" : "^\(.[a-z]*\)"`
    UEADBCommand="${UEADBCommand} , NewFaculty ${facultyName}"
    
    for specialtyXML in Faculties/${facultyName}/*.xml
    do
        UEADBCommand="${UEADBCommand} , AddSpecialty ${facultyName} ${specialtyXML}"
    done
done

for enrolleeXML in ./Enrollees/*/*.xml
do
    UEADBCommand="${UEADBCommand} , AddEnrollee ${enrolleeXML}"
done

UEADBCommand="${UEADBCommand} , SaveUniversity PackedUniversity.xml"
echo "$(${UEADBCommand})"
exit 0
