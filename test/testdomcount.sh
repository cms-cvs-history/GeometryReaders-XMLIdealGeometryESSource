#!/bin/tcsh 
# Michael Case 2009-09-17:  This is meant to validate that the geometry going into the db is
# indeed the same as the geometry that comes from the many xml files which created the db.
# It does NOT check SpecPars (yet).
# It checks the position of all parts in the hierarchy of the graph of parts positioned
# in the detector and is currently (in the file testCompareDumpFiles.py) set to look
# for differences exceeting .0004 mm in x, y and z and .0004 in the elements of the
# rotation matrix.
#
# To run this file, ./runXMLBigFileToDBAndBackValidation.sh in 
# GeometryReaders/XMLIdealGeometryESSource/test
# To RERUN the test, rm -rf workarea.

# What I want to know is 1 where the config file is located (for domcount and dddreport)
# and 2 what the sub-string corresponding to that is in the Configuration/StandardSequences.
cmsenv
if ($#argv == 0) then
    setenv geomxml "${CMSSW_RELEASE_BASE}/src/Geometry/CMSCommonData/python/cmsIdealGeometryXML_cfi.py"
else
    if ($#argv == 1) then
	setenv geomxml `echo ${1}`
    endif
endif

echo "START - All messages in this script pertain to geometry data described in xml files in: ${geomxml}" 

# validate current set of xmlfiles in IdealGeometry is correct.
#domcount.sh >& dcorig.out
#ASSUMPTIONS:  1.  relative path between documents (xml) and schema (DetectorDescription/Schema/DDLSchema.xsd)
#                  are no more than 4 away, i.e. ../../../../ MAX (for normal/cmsextent.xml files)
#grep ".xml" $geomxml | sed "{s/'//g}" | sed '{s/,//g}' | sed '{s/ //g}' | sed '{s/\t//g}' | sed '{s/geomXMLFiles=cms.vstring(//g}' | sed '{s/)//g}' | grep -v "#" >! /tmp/tmpcmsswdddxmlfileslist
set whst=`(grep ".xml" $geomxml | sed "{s/'//g}" | sed '{s/,//g}' | sed '{s/ //g}' | sed '{s/\t//g}' | sed '{s/geomXMLFiles=cms.vstring(//g}' | sed '{s/)//g}' | grep -v "#" )`
#echo $whst
rm dcorig.out
touch dcorig.out
if ( -e "../../../DetectorDescription/Schema/DDLSchema.xsd" ) then
//    echo "got here"
    foreach l ( $whst )
	if ( -e $CMSSW_BASE/src/$l ) then
	    set dp = `(echo "${l}" | awk -F\/ '{print NF}')`
	    set fn = `(echo "${l}" | awk -F\/ '{print $NF}')`
	    if ( $dp == 5 ) then
		cd workarea
	    endif
	    cp $CMSSW_BASE/src/$l .
	    DOMCount -v=always -n -s -f $fn >>& dcorig.out
	    rm $fn
	    if ( $dp == 5 ) then
		cd ..
	    endif
	else
	    if ( -e $CMSSW_RELEASE_BASE/src/$l ) then
		set dp = `(echo "${l}" | awk -F\/ '{print NF}')`
		set fn = `(echo "${l}" | awk -F\/ '{print $NF}')`
		if ( $dp == 5 ) then
		    cd workarea
		endif
		cp $CMSSW_RELEASE_BASE/src/$l .
		DOMCount -v=always -n -s -f $fn >>& dcorig.out
		rm $fn
		if ( $dp == 5 ) then
		    cd ..
		endif
	    else
		echo "Error file " $l " not found in " $CMSSW_RELEASE_BASE "/src or " $CMSSW_BASE "/src" >>& dcorig.out
	    endif
	endif
    end
else
    echo "Missing ../../../DetectorDescrition/Schema/DDLSchema.xsd"
endif

set errcnt = `(grep --count "Error" dcorig.out)`
set warcnt = `(grep --count "Error" dcorig.out)`
if ($errcnt == 0 && $warcnt == 0) then
    echo "No XML Schema violations in original xml files."
else
    echo "XML Schema violations can be seen in dcorig.out."
endif

echo "ALL DONE!"


