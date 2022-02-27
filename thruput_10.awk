BEGIN {
	received = 0;  # number of packets received
	startTime = 900 ; # start time of the transmission
	stopTime = 0;  # stop time of transmission
	recvdSize = 0; # packet sizes received ( sum of number of bytes in each packet = total number of bytes transmitted )
	sent = 0;
	dropped = 0;
}
{
  event = $1
  time = $2
  src_node = $3
  protocol = $5
  dest_node = $4
  pkt_size = $8
  
  	if( (src_node=="0" || src_node=="1") && event=="+" && protocol=="cbr") {
  		sent++;
  		if(!startTime || startTime > time) {
  			startTime=time;
  		}
  	}

	if( event == "d" ) {
		dropped++;
	}
	
	if( dest_node=="5" && protocol=="cbr" && event=="r" ) {
		received++;
		recvdSize+=pkt_size ;	
	    if (time > stopTime) {
	      stopTime = time
	    }
	 
	}
}

END {
 # printf("sent_packets\t %d\n",sent)
 # printf("received_packets %d\n",receive)
  printf("PDR %.2f%\n",(received/sent)*100);
  printf("Average Throughput[kbps] = %.2f\tStartTime=%.2f\tStopTime = %.2f\n", (recvdSize/(stopTime-startTime))*(8/1000),startTime,stopTime);
  printf("Sent = %d\n",sent);
  printf("Dropped packets = %d\n", dropped);
  printf("dropped percentage = %f%\n", (dropped/sent)*100);
  
}
