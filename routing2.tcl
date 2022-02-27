set ns [new Simulator]
$ns rtproto LS

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
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
set n9 [$ns node]
set n10 [$ns node]
set n11 [$ns node]
$ns duplex-link $n0 $n8 1Mb 10ms DropTail
$ns duplex-link $n1 $n10 1Mb 10ms DropTail
$ns duplex-link $n0 $n9 1Mb 10ms DropTail
$ns duplex-link $n9 $n11 1Mb 10ms DropTail
$ns duplex-link $n10 $n11 1Mb 10ms DropTail
$ns duplex-link $n11 $n5 1Mb 10ms DropTail
$ns duplex-link $n10 $n5 1mMb 10ms DropTail
$ns duplex-link-op $n0 $n8 orient left-up
$ns duplex-link-op $n1 $n10 orient left
$ns duplex-link-op $n0 $n9 orient right-up
$ns duplex-link-op $n9 $n11 orient right
$ns duplex-link-op $n10 $n11 orient left-up
$ns duplex-link-op $n11 $n5 orient right-up
$ns duplex-link-op $n10 $n5 orient left-up
set udp1 [new Agent/UDP]
$ns attach-agent $n0 $udp1
set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns connect $udp1 $null
$udp1 set fid_ 1
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set type_ CBR
$cbr1 set packet_size_ 1000
$cbr1 set rate_ 1mb
$cbr1 set random_ false#Setup a UDP connections over 1 and 5
set udp2 [new Agent/UDP]
$ns attach-agent $n1 $udp2
set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns connect $udp2 $null
$udp2 set fid_ 2
set cbr2 [new Application/Traffic/CBR]
$cbr2 attach-agent $udp2
$cbr2 set type_ CBR
$cbr2 set packet_size_ 1000
$cbr2 set rate_ 1mb
$cbr2 set random_ false
$ns rtmodel-at 2.5 down $n11 $n5
$ns rtmodel-at 3.5 up $n11 $n5

$ns at 0.0 "$cbr1 start"
$ns at 0.0 "$cbr2 start"
$ns at 5.0 "$cbr2 stop"
$ns at 5.0 "$cbr2 stop"
$ns at 6.0 "finish"
$ns run