#tahoe.tcl
set ns [new Simulator]
$ns color 1 Red
set nf [open out2.nam w]
$ns namtrace-all $nf
proc finish {} {
	global ns nf
	$ns flush-trace
	close $nf
	exec nam out2.nam &
	exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 10Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns queue-limit $n1 $n2 10
$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n2 orient right
set tcp [new Agent/TCP]
$tcp set class_ 1
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $tcp $sink
$tcp set fid_ 1
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP

proc plotWindow {tcpSource outfile} {
global ns
set now [$ns now]
set cwnd [$tcpSource set cwnd_]
puts $outfile "$now $cwnd"
$ns at [expr $now+0.1] "plotWindow $tcpSource $outfile"
}
                                                        
set outfile [open "congestion2.txt" w]
$ns at 0.0 "plotWindow $tcp $outfile"
$ns at 0.2 "$ftp start"
$ns at 1.0 "$ftp stop"
$ns at 1.0 "$ns detach-agent $n0 $tcp ; $ns detach-agent $n2 $sink"
$ns at 1.2 "finish"
#Run the simulation
$ns run







