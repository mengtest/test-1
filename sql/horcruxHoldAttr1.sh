


#!/bin/bash

F=horcruxHoldAttr1.txt
if [ "$1" != "" ]
then
    F=$1
fi

echo $F
function horcruxHoldAttr1()
{
# 11,"302,2,3|303,1",0,40,"3,4"
    f=$1
    d=horcruxHoldAttr1
    sed -i /ID/d $f
    sed -i /id/d $f
    sed -i /lv/d $f
    sed -i /^$/d $f
#    sed -i /^[[:space:]]*$/d $f   #lb
    sed -i /REF/d $f
    sed -i s/\"//g $f
    export lines=`wc -l $f | awk '{print $1}'`
    echo "Generating file $d, total lines $l"
    awk '
        BEGIN {
            print "INSERT INTO `horcruxHoldAttr1` VALUES";
        } {
            printf("(%u,%d)",$3,$2);
            if (NR <= ENVIRON["lines"])
                printf(",");
            else if (NR > ENVIRON["lines"])
                printf(";");
            printf("\n");
        }
        END {
        }
    ' $f > $d
    sed -i s/\\r//g $d
    if [ $? -eq 0 ]
    then
        iconv2utf8 $d
        echo "OK"
    else
        echo "ERROR"
    fi
}
function iconv2utf8()
{
    iconv -f cp936 -t utf8 $1 -o $1.tmp
    rm $1
    mv $1.tmp $1
}

if [ -f $F  ]
then
    horcruxHoldAttr1 $F
else
    echo "File $F is not exists"
fi
