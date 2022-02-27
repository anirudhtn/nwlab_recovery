BEGIN {
	receive = 0;  # number of packets received
	startTime = 900 ; # start time of the transmission
	stopTime = 0;  # stop time of transmission
	recvdSize = 0; # packet sizes received ( sum of number of bytes in each packet = total number of bytes transmitted )
	sent = 0;
}
{
  event = $1
  time = $2
  src_node = $3
  protocol = $5
  dest_node = $4
  pkt_size = $8

  if (event == "+" && protocol == "tcp" && src_node == "0") {
    sent++
# Note the change in the next line. This initializes the startTime with the first encountered "time" value.
    if (!startTime || (time < startTime)) {
      startTime = time
    }
  }

  if (event == "r" && $5 == "tcp" && dest_node == "4") {
    receive++
    if (time > stopTime) {
      stopTime = time
    }
    recvdSize += pkt_size
  }
}

END {
  printf("sent_packets\t %d\n",sent)
  printf("received_packets %d\n",receive)
  printf("PDR %.2f%\n",(receive/sent)*100);
  printf("Average Throughput[kbps] = %.2f\tStartTime=%.2f\tStopTime = %.2f\n", (recvdSize/(stopTime-startTime))*(8/1000),startTime,stopTime);
}

