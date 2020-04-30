#!/bin/sh

#***************************************************************************************************************
#	Filename:		GenPlots_MGDY.sh (MadGraph Drell-Yan)
#	Author:		Victoria Trenton
#	Last Update:	Aug. 24, 2012
# 
# This script combines ROOT files of the same event and creates histograms for a particular
# charge in a new ROOT file. A call is made to RunMonoPlots.C to create the histograms.
# (Afterward, the user can run MakePlots.C to generate picture files.)
# 
# Arguments for this script are charge and mass. To run, type this on the command line:
# 		./GenPlots_MGDY.sh
#***************************************************************************************************************

#=====================================================================
# Add 1 or 2 zeroes to the beginning of mass integer to make it 3 digits
#=====================================================================
prefixZeroesToMass()
{
	# Local variables
	local massVal=$1				# Store function's mass argument in local variable
	local massStringNum

	# If 0 <= mass <= 9, prefix two zeroes to mass.
	if [ $massVal -ge 0 ] && [ $massVal -le 9 ]; then
		massStringNum="00$mass"					# e.g. 5 -> 005
	# If 10 <= mass <= 99, prefix one zero to mass.
	elif [ $massVal -ge 10 ] && [ $massVal -le 99 ]; then
		massStringNum="0$mass"						# e.g. 50 -> 050
	else
		massStringNum="$mass"						# e.g. 500
	fi
	
	return $massStringNum
}

#=====================================================================
# Call RunMonoPlots.C and pass charge argument
#=====================================================================
runScriptForRootFile()
{
	# Local variables
	local charge=$1
	local msVal=$2
	local mnVal=$3
	local mgVal=$4
	#local mass=$2
	#local massString=""

	echo "============================================================"
	echo "===== Generating Monopole distribution for mass point ====="
	echo ""
	echo "---------- Default coupling Ms"$msVal"_Mn"$mnVal"_Mg"$mgVal" GeV/c^2 ----------"
	
	# Prefix zeroes so that massString has 3 digits
	#prefixZeroesToMass $mass
	#massString=$?							# Store variable returned from function
	
	#----------------------------------------------------------------------------------------------------------------------------------
	# User can change the path to the directory containing same-Event ROOT files (which
	# does not include subfolders) and the path to move the generated ROOT file to.
	# e.g. inputRootPath = /work/trenton/MadGraph5_v1_4_5/QBProd6/QBProd6_m050/Events/
	# e.g. resultRootPath = Results/qball6_m050_histos.root
	#----------------------------------------------------------------------------------------------------------------------------------
	local mgPath="/work/trenton/MadGraph5_v1_4_5"
	
	#sampleIdentifierPath="QBProd"$charge"/QBProd"$charge"_m"$massString
	#sampleIdentifierPath="QBProd"$charge"/QBProd"$charge"_m"$massString"_b4000"
	local sampleIdentifierPath="LSProd_Ms"$msVal"_Mn"$mnVal"_Mg"$mgVal
	
	#inputRootPath=$mgPath"/"$sampleIdentifierPath"/Events"
	local inputRootPath=$mgPath"/LSProd_benitez/"$sampleIdentifierPath
	
	#resultRootPath="Results/qball"$charge"_m"$massString"_histos.root"
	#resultRootPath="Results/qball"$charge"_m"$massString"_b4000_histos.root"
	local resultRootPath="Results/Leptosusy_Ms"$msVal"_Mn"$mnVal"_Mg"$mgVal"_histos.root"
	#----------------------------------------------------------------------------------------------------------------------------------
	
	# Create a symbolic link (soft link) to directory containing ROOT files (for ReadLHE.h)
	echo "---------- Linking directory ----------"
	local tempLink="ntuplesdir"
	if [ -h $tempLink ]; then					# If previous soft link exists, remove it
		rm $tempLink
	fi
	ln -s	$inputRootPath $tempLink
	
	# Pass charge to RunMonoPlots.C (l: load fast, b: batch mode, q: quit afterward)
	# (To call a macro in ROOT, string arguments need explicit quotation marks.)
	echo "---------- Generating histograms ----------"
	root -l -b -q "RunMonoPlots.C("$charge")"		# Need quotes to indicate string
	rm $tempLink
	
	# Rename and move the generated ROOT file to the "Results" directory
	echo "---------- Moving generated root file ----------"
	if [ ! -d "Results" ] && [! -d "Plots" ]; then		# If directories don't exist, create them
		mkdir {"Results","Plots"}				# "Plots" directory for MakePlots.C
	fi
	mv "OUT_histos.root" $resultRootPath
}

#=====================================================================
# Main part of script (written for Leptosusy)
#=====================================================================
#------------------------------------------------------------------------------------------------------------------------------------------
# User can change the number of mass sets and their values {ms, mn, mg}
#------------------------------------------------------------------------------------------------------------------------------------------
# Global variable
numMasses=9

# Initialize arrays
charge=("1")
ms=("1500" "1000" "10000"   "500"    "500"    "500"   "1000"   "1000"   "1000")
mn=("600"     "600"   "600"    "200"    "600"   "1000"    "200"     "600"   "1000")
mg=("1200" "2000" "1000" "10000" "10000" "10000" "10000" "10000" "10000")
#------------------------------------------------------------------------------------------------------------------------------------------

# Cycle over charges
for i in ${charge[@]}; do						# Can use @ or *
	#Cycle over masses
	j=0
	while [[ j -lt $numMasses ]]; do

		runScriptForRootFile $i ${ms[$j]} ${mn[$j]} ${mg[$j]}
		(( j++ ))
#		let j=j+1							# Alternative
	
	done				# Masses
done					# Charges







# Code below is commented out
: << COMMENT

echo 'Default coupling Mass 50 GeV/c^2:'
echo '  Linking M50 directory'
rm ntuplesdir 
ln -s	/work/trenton/MadGraph5_v1_4_5/QBProd6/QBProd6_m050/Events/ ntuplesdir
echo '  Generating histograms'
root -l RunMonoPlots.C
echo '  Moving root & text files'
mv OUT_histos.root Results/qball6_m050_histos.root

echo 'Default coupling Mass 100 GeV/c^2:'
echo '  Linking M100 directory'
rm ntuplesdir 
ln -s	/work/trenton/MadGraph5_v1_4_5/QBProd6/QBProd6_m100/Events/ ntuplesdir
echo '  Generating histograms'
root -l RunMonoPlots.C
echo '  Moving root & text files'
mv OUT_histos.root Results/qball6_m100_histos.root

echo 'Default coupling Mass 200 GeV/c^2:'
echo '  Linking M200 directory'
rm ntuplesdir 
#ln -s  /media/ed8e9260-8e40-4dd1-924b-b75fb2a61b92/taylorw/ATLAS/HIPs/datafiles/ntuples/LHE/magcoupl/m200/ ntuplesdir
#ln -s  /home/taylorw/ATLAS/HIPs/datafiles/ntuples/LHE/magcoupl/m200/ ntuplesdir
ln -s	/work/trenton/MadGraph5_v1_4_5/QBProd6/QBProd6_m200/Events/ ntuplesdir
echo '  Generating histograms'
root -l RunMonoPlots.C
echo '  Moving root & text files'
#mv OUT_histos.root Results/magcoupl_m200_histos.root
mv OUT_histos.root Results/qball6_m200_histos.root

echo 'Default coupling Mass 300 GeV/c^2:'
echo '  Linking M300 directory'
rm ntuplesdir 
ln -s	/work/trenton/MadGraph5_v1_4_5/QBProd6/QBProd6_m300/Events/ ntuplesdir
echo '  Generating histograms'
root -l RunMonoPlots.C
echo '  Moving root & text files'
mv OUT_histos.root Results/qball6_m300_histos.root

echo 'Default coupling Mass 400 GeV/c^2:'
echo '  Linking M400 directory'
rm ntuplesdir 
ln -s	/work/trenton/MadGraph5_v1_4_5/QBProd6/QBProd6_m400/Events/ ntuplesdir
echo '  Generating histograms'
root -l RunMonoPlots.C
echo '  Moving root & text files'
mv OUT_histos.root Results/qball6_m400_histos.root

echo 'Default coupling Mass 500 GeV/c^2:'
echo '  Linking M500 directory'
rm ntuplesdir 
#ln -s  /media/ed8e9260-8e40-4dd1-924b-b75fb2a61b92/taylorw/ATLAS/HIPs/datafiles/ntuples/LHE/magcoupl/m500/ ntuplesdir
#ln -s  /home/taylorw/ATLAS/HIPs/datafiles/ntuples/LHE/magcoupl/m500/ ntuplesdir
ln -s	/work/trenton/MadGraph5_v1_4_5/QBProd6/QBProd6_m500/Events/ ntuplesdir
echo '  Generating histograms'
root -l RunMonoPlots.C
echo '  Moving root & text files'
#mv OUT_histos.root Results/magcoupl_m500_histos.root
mv OUT_histos.root Results/qball6_m500_histos.root

echo 'Default coupling Mass 600 GeV/c^2:'
echo '  Linking M600 directory'
rm ntuplesdir 
ln -s	/work/trenton/MadGraph5_v1_4_5/QBProd6/QBProd6_m600/Events/ ntuplesdir
echo '  Generating histograms'
root -l RunMonoPlots.C
echo '  Moving root & text files'
mv OUT_histos.root Results/qball6_m600_histos.root

COMMENT

