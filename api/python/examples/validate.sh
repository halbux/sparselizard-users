#!/bin/bash


# Make the .so available to python:
cp ../build/spylizard.so ~/.local/lib/python3.9/site-packages;

# LOOP ON ALL EXAMPLES:

echo ''; 
for i in $(ls .); 
do 

# Skip this script:
if [ $i == "validate.sh" ]
then
continue
fi

cd $i;

# Run the current example:
out=$(python3.9 main.py);

# Get the last 4 characters in the output:
out="${out:$((${#out}-4)):4}"

if [ $out == "True" ]
then
echo 'SUCCESS AT' $i;
else
echo 'FAILED AT' $i;
echo '';
exit 1;
fi

# CLEAN:
rm -f *.pos;
rm -f *.vtk;
rm -f *.vtu;
rm -f *.pvd;
rm -f *.csv;
rm -f *.txt;
rm -f *.slz;

cd ..;

done

echo ''; 
echo '';
echo 'ALL OK!';

