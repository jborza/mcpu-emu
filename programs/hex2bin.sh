#/!bin/bash
cat $1 | tr '\r\n' ' '  | sed -e 's/[[:blank:]]\+/ /g' | xxd -r -p > $2