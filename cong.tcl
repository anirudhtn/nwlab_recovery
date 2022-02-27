set ns [new Simulator]

$ns color 1 Blue
$ns color 2 Red

set nf [open namfile.nam w]
set tf [open tracefile.tr w]

$ns namtrace-all $nf
$ns trace-all $tf

proc finish {} {
	global ns nf tf
	$ns flush-trace
	close $nf
	close $tf
	exec nam namfile.nam &
	exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]

$ns duplex-link $n0 $n1 10Mb 1ms DropTail
$ns duplex-link $n1 $n2 0.75Mb 100ms DropTail

$ns queue-limit $n1 $n2 5

$ns duplex-link-op $n0 $n1 orient right
$ns duplex-link-op $n1 $n2 orient right

$ns duplex-link-op $n1 $n2 queuePos 0.5

set taho [new Agent/TCP]
$taho set fid_ 1
$taho set window_ 40
$ns attach-agent $n0 $taho
set sink [new Agent/TCPSink]
$ns attach-agent $n2 $sink
$ns connect $taho $sink

$taho attach $tf
$taho tracevar cwnd_

set ftp [new Application/FTP]
$ftp attach-agent $taho
$ftp set type_ FTP

$ns at 1.0 "$ftp start"
$ns at 4.0 "$ftp stop"

$ns at 4.5 "$ns detach-agent $n0 $taho;$ns detach-agent $n2 $sink"
$ns at 5.0 "finish"
$ns run
