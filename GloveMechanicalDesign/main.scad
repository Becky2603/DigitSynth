
include <config.scad>

use <proximalSection.scad>

middleAssembly();

translate([0,0,70]) 
rotate([0,180,0]) 
proximalAssembly();