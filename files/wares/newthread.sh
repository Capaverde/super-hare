subj="$(printf %s "$2" | sed 's/\ /%20/g')"
com="$(printf %s "$3" | sed 's/\ /%20/g')"
#echo $subj; echo $com;
curl https://dis.tinychan.net/$1/post --data "formtime=1589402929&board=$1&subject=$subj&com=$com" -A Mozilla


