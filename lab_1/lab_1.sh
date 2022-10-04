echo "DATE:" $(date '+%d-%m-%Y')
echo "USER:" "$USER"
echo "HOSTNAME:" "$HOSTNAME"
cat /proc/cpuinfo | grep 'model name' | uniq
lscpu | grep 'Architecture:' | uniq
echo "CORES:" $(nproc)
echo $(lscpu | grep 'Thread(s) per core' | uniq)
echo "RAM:" $(free -m | awk 'NR==2{printf "%s\n", $2 }')
echo "RAM AVAILABLE:" $(free -m | awk 'NR==2{printf "%s\n", $2 - $3}')
echo "Hard disk space " $(sudo lshw -class disk | grep 'size:' | uniq)
echo "Available disk space:" $(free | awk 'NR==2{printf "%s\n", ($2 - $3) / (1024 * 8 * 1024)}')"GiB"
echo "Mounted: /dev/sda"
echo "SWAP:" $(free | awk 'NR==3{printf "%s\n", ($2) / (1024 * 8 * 1024)}')"GiB"
echo "Available SWAP:" $(free | awk 'NR==3{printf "%s\n", ($2 - $3) / (1024 * 8 * 1024)}')"GiB"
COUNTER=0
IP=-1
MASK=0
for var in $(ifconfig | awk '{printf "%s\n", $1}' | grep ':' | uniq)
do
COUNTER=$(($COUNTER+1))
IP=$(($IP+2))
MASK=$((MASK+1))
echo "$var" $(ifconfig | grep 'netmask' | awk 'NR=='"$MASK"'{printf $4}') "|" $(ifconfig | grep 'inet' | awk 'NR=='"$IP"'{printf $2}') "|" $(ifconfig | grep 'mtu' | awk 'NR=='"$MASK"'{printf $'"4"'}') "KBi/s"
done
