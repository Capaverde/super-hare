com="$(printf %s "$3" | sed -e 's/\ /%20/g')"
printf %s "$com"
#curl https://dis.tinychan.net/$1/post --data "formtime=1589402929&board=$1&thread=$2&com=$com" -A Mozilla

