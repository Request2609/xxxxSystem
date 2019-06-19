#!/bin/bash
#!/bin/bash
#Filename: netscan.sh
#Functions: 局域网主机联通性的扫描

network=$1
time=$(date +%H%M%S)

for i in $(seq $2 $3)
do
    ping -c 1 -w 2 $network.$i > /dev/null
    if [ $? -eq 0 ]; then
          arp $network.$i | grep ":" | awk '{print $1,$3}' >> $time.log
          echo "host $network.$i is up"
   else
          echo "host $network.$i is down"
   fi
done
