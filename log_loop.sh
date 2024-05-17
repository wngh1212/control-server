#!bin/bash

#!/bin/bash

while true; do
    logparser
   
    rm /var/log/syslog

    touch /var/log/syslog
    
    systemctl restart syslog
    
    sleep 300
done
