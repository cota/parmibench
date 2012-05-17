#!/bin/bash

OUTFILE=$1
PROCESSORS=$2

NAME=$(echo $1 | sed 's|\([^.]*\).*|\1|' | tr '[:lower:]' '[:upper:]')

echo "#ifndef _${NAME}_H_" > $OUTFILE
echo "#define _${NAME}_H_" >> $OUTFILE
echo "" >> $OUTFILE

echo "#define PROCESSORS $PROCESSORS" >> $OUTFILE

echo "" >> $OUTFILE
echo "#endif /* _${NAME}_H_ */" >> $OUTFILE
