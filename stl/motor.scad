//
//  WifiRobot
//  motor holder
//
//  The Cave, 2019
//  https://thecave.cz
//
//  Licensed under MIT License (see LICENSE file for details)
//

$fn = 100;
$wall = 3;
$width = 20;
$width2 = 10;
$step = 6;
$stepHeight = 7;
$height = 25;
$len = 7;
$offset = $len/2;
$nut=6.5;

rotate([90,0,0])
difference() {
    union() {
        translate([($width-$width2)/2,0,0]) {
            cube([$width2,$wall,$height]);
            translate([0,0,$height-$nut*0.9]) cube([$width2,$wall+2,$nut*0.9]);
        }
        cube([$width,$len+$wall,$wall]);
    }

    translate([10,$wall+$len+1,4]) #rotate([90,0,0]) {
        cylinder(d=3.2,h=20);
        translate([0,1,0]) cylinder(d=3.2,h=20); // some motors have 17mm hole distance instead of 18
        cylinder(d=$nut,h=$len+1,$fn=6);
    }
    
    translate([10,$wall+$len+1,4+18]) #rotate([90,0,0]) {
        cylinder(d=3.2,h=20);
        cylinder(d=$nut,h=$len+1,$fn=6);
    }
    
    translate([($width-$step)/4,$wall+$offset,-16]) {
        #cylinder(d=3.2,h=20);
        translate([0,0,20]) #cylinder(d=6,h=3);
    }
    translate([($width-$step)/4*3+$step,$wall+$offset,-16]) {
        #cylinder(d=3.2,h=20);
        translate([0,0,20]) #cylinder(d=6,h=3);
    }
}