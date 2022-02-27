set ns [new Simulator]
$ns color 1 Green
$ns color 2 Blue
set nf [open outfile1.nam w]
$ns namtrace-all $nf
set tf [open tracefile1.tr w]
$ns trace-all $tf
proc finish {} {
	global ns nf tf
	$ns flush-trace
	close $nf
	close $tf
	exec nam outfile1.nam &
	exit 0
}

set n0 [$ns node] 
set n1 [$ns node] 
set n2 [$ns node] 
set n3 [$ns node] 
set n4 [$ns node] 
set n5 [$ns node]

$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns simplex-link $n2 $n3 0.3Mb 100ms DropTail
$ns simplex-link $n3 $n2 0.3Mb 100ms DropTail
$ns duplex-link $n3 $n4 0.5Mb 40ms DropTail
$ns duplex-link $n3 $n5 0.5Mb 40ms DropTail

$ns queue-limit $n2 $n3 10

$ns duplex-link-op $n0 $n2 orient left-up
$ns duplex-link-op $n1 $n2 orient left-down
$ns simplex-link-op $n2 $n3 orient right
$ns simplex-link-op $n3 $n2 orient left
$ns duplex-link-op $n3 $n4 orient right-down
$ns duplex-link-op $n3 $n5 orient right-up

set tcp [new Agent/TCP]
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n4 $sink
$ns connect $tcp $sink
$tcp set fid_ 1

set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ftp set type_ FTP


set udp [new Agent/UDP]
$ns attach-agent $n1 $udp set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns connect $udp $null
$udp set fid_ 2
	
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
$udp set type_ CBR


$ns at 1.0 "$ftp start"
$ns at 1.0 "$cbr start"

$ns at 5.0 "finish"

$ns run