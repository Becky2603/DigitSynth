include <config.scad>
use <hinge.scad>
include <BOSL2/std.scad>
include <BOSL2/rounding.scad>

module proximalSection() {
    difference() {
        cylinder(h = proximalLength, d = proximalWidth + 2*wallThickness);
        translate([0,0,-0.1]) 
            cylinder(h = proximalLength + 1, d = proximalWidth);
    }
}

module hinge_connector() {

    hull() {

        // Top profile
        translate([0, 0, 0])
        linear_extrude(height = connector_profile_height)
        square([
            jointThickness,
            jointHeight 
        ], center = true);

        // Bottom profile
        translate([0, 0, -connector_drop])
        linear_extrude(height = connector_profile_height)
        square([
            jointThickness,
            connector_bottom_width
        ], center = true);
    }
}


module proximalAssembly() {

    // True outer radius of proximal cylinder
    outerRadius = proximalWidth/2 + wallThickness;
    union() {

        // Proximal section
        proximalSection();

        // ================= LEFT HINGE =================
        translate([
            -(outerRadius - wallThickness/2),
            0,
            proximalLength + hinge_vertical_offset
        ]) {
            difference() {
                hinge_connector();
                rotate([0,90,0])
                    cylinder(h = male_width, r = jointHeight, center = true);
            }
            hingeAssembly_female();
        }


        // ================= RIGHT HINGE =================
        translate([
            (outerRadius - wallThickness/2),
            0,
            proximalLength + hinge_vertical_offset
        ])
        mirror([1,0,0]) {
            difference() {
                hinge_connector();
                rotate([0,90,0])
                    cylinder(h = male_width, r = jointHeight, center = true);
            }
            hingeAssembly_female();
        }
    }
}

module middleAssembly() {

    // True outer radius of proximal cylinder
    outerRadius = proximalWidth/2 + wallThickness;
    union() {

        // Proximal section
        proximalSection();

        // ================= LEFT HINGE =================
        translate([
            -(outerRadius - wallThickness/2),
            0,
            proximalLength + hinge_vertical_offset
        ]) {

            hinge_connector();
            translate([-(hinge_clearance+0.1),0,0]) 
            hingeAssembly_male();
        }


        // ================= RIGHT HINGE =================
        translate([
            (outerRadius - wallThickness/2  ),
            0,
            proximalLength + hinge_vertical_offset
        ]) {
            hinge_connector();

            translate([-(hinge_clearance+0.1),0,0]) 
            hingeAssembly_male();
        }
    }
}


// difference(){
//     proximalAssembly();

//     translate([0,-5,25]) 
//     sphere(r = 10);

// }


hinge_connector();
translate([-(hinge_clearance+0.1),0,0]) 
hingeAssembly_male();

translate([0,0,40]){ 

difference() {
    hinge_connector();
    rotate([0,90,0])
    cylinder(h = male_width, r = jointHeight, center = true);
    }
hingeAssembly_female();

}